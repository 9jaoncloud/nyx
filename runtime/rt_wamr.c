/*
 * rt_wamr.c — Nyx Embedded WebAssembly Micro Runtime (WAMR) Region Sandboxing Engine
 */

#include "rt_wamr.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bool nyx_wamr_init(void) {
    /* Initialize global WAMR runtime parameters if needed */
    return true;
}

NyxWasmSandbox* nyx_wamr_create_sandbox(Arena* arena, const uint8_t* wasm_bytes, size_t wasm_size, size_t pool_size) {
    if (!arena || !wasm_bytes || wasm_size < 8) {
        return NULL;
    }

    /* Verify WebAssembly Binary Magic & Version */
    uint32_t magic = *(const uint32_t*)wasm_bytes;
    uint32_t version = *(const uint32_t*)(wasm_bytes + 4);
    if (magic != NYX_WASM_MAGIC || version != NYX_WASM_VERSION) {
        return NULL;
    }

    /* Record checkpoint before allocating sandbox structures and pool */
    ArenaCheckpoint cp = arena_checkpoint(arena);

    /* Allocate sandbox descriptor in the region arena */
    NyxWasmSandbox* sandbox = (NyxWasmSandbox*)arena_alloc(arena, sizeof(NyxWasmSandbox));
    if (!sandbox) return NULL;

    sandbox->arena = arena;
    sandbox->checkpoint = cp;
    sandbox->wasm_bytecode = wasm_bytes;
    sandbox->wasm_size = wasm_size;
    sandbox->instruction_count = 0;
    sandbox->is_isolated = true;

    /* Minimum pool size is 64KB (1 Wasm page) + 4KB metadata */
    if (pool_size < (NYX_WAMR_PAGE_SIZE + 4096)) {
        pool_size = NYX_WAMR_PAGE_SIZE + 4096;
    }
    sandbox->pool_size = pool_size;
    sandbox->pool_used = 0;

    /* Carve out the WAMR host-managed memory pool directly from the Nyx Region Arena */
    sandbox->memory_pool = (uint8_t*)arena_alloc_aligned(arena, pool_size, 16);
    if (!sandbox->memory_pool) {
        arena_rewind(arena, cp);
        return NULL;
    }

    /* Initialize linear memory inside the pool */
    sandbox->linear_mem_size = NYX_WAMR_PAGE_SIZE;
    sandbox->linear_mem_max = pool_size - 4096;
    sandbox->linear_memory = sandbox->memory_pool;
    sandbox->pool_used += sandbox->linear_mem_size;
    memset(sandbox->linear_memory, 0, sandbox->linear_mem_size);

    sandbox->num_functions = 0;
    sandbox->num_exports = 0;

    return sandbox;
}

void nyx_wamr_rewind_and_free(NyxWasmSandbox* sandbox) {
    if (!sandbox || !sandbox->arena) return;
    Arena* arena = sandbox->arena;
    ArenaCheckpoint cp = sandbox->checkpoint;
    /* Instantaneous rewind with 0.00 ms GC latency */
    arena_rewind(arena, cp);
}

void nyx_wamr_destroy(NyxWasmSandbox* sandbox) {
    nyx_wamr_rewind_and_free(sandbox);
}

uint32_t nyx_wamr_alloc_linear(NyxWasmSandbox* sandbox, size_t size) {
    if (!sandbox) return 0;
    /* Align to 8 bytes */
    size = (size + 7) & ~7;
    if (sandbox->pool_used + size > sandbox->linear_mem_max) {
        return 0; /* Out of sandbox linear memory */
    }
    uint32_t offset = (uint32_t)sandbox->pool_used;
    sandbox->pool_used += size;
    if (sandbox->pool_used > sandbox->linear_mem_size) {
        sandbox->linear_mem_size = sandbox->pool_used;
    }
    return offset;
}

void* nyx_wamr_get_host_ptr(NyxWasmSandbox* sandbox, uint32_t wasm_offset, size_t size) {
    if (!sandbox || !sandbox->linear_memory) return NULL;
    /* Hardware boundary check to prevent sandbox breakout */
    if ((uint64_t)wasm_offset + (uint64_t)size > (uint64_t)sandbox->linear_mem_size) {
        return NULL;
    }
    return (void*)(sandbox->linear_memory + wasm_offset);
}

bool nyx_wamr_copy_to_wasm(NyxWasmSandbox* sandbox, uint32_t wasm_dst_offset, const void* src, size_t size) {
    void* host_ptr = nyx_wamr_get_host_ptr(sandbox, wasm_dst_offset, size);
    if (!host_ptr || !src) return false;
    memcpy(host_ptr, src, size);
    return true;
}

bool nyx_wamr_copy_from_wasm(NyxWasmSandbox* sandbox, void* dst, uint32_t wasm_src_offset, size_t size) {
    void* host_ptr = nyx_wamr_get_host_ptr(sandbox, wasm_src_offset, size);
    if (!host_ptr || !dst) return false;
    memcpy(dst, host_ptr, size);
    return true;
}

size_t nyx_wamr_get_pool_used(const NyxWasmSandbox* sandbox) {
    return sandbox ? sandbox->pool_used : 0;
}

uint64_t nyx_wamr_get_instruction_count(const NyxWasmSandbox* sandbox) {
    return sandbox ? sandbox->instruction_count : 0;
}

/* LEB128 Reader Helpers */
static uint32_t read_u32_leb128(const uint8_t** p, const uint8_t* end) {
    uint32_t result = 0;
    uint32_t shift = 0;
    while (*p < end) {
        uint8_t byte = **p;
        (*p)++;
        result |= (uint32_t)(byte & 0x7F) << shift;
        if ((byte & 0x80) == 0) break;
        shift += 7;
    }
    return result;
}

/* Fast Execution Engine for Sandboxed WebAssembly Functions */
NyxWamrStatus nyx_wamr_execute(NyxWasmSandbox* sandbox, const char* func_name, const uint32_t* args, size_t argc, uint32_t* out_result) {
    if (!sandbox || !func_name || !out_result) {
        return NYX_WAMR_ERR_FUNCTION_NOT_FOUND;
    }

    const uint8_t* p = sandbox->wasm_bytecode + 8;
    const uint8_t* end = sandbox->wasm_bytecode + sandbox->wasm_size;

    uint32_t target_func_idx = 0xFFFFFFFF;
    bool found_export = false;

    /* Scan sections */
    const uint8_t* code_section_start = NULL;
    size_t code_section_len = 0;

    while (p < end) {
        uint8_t section_id = *p++;
        uint32_t section_len = read_u32_leb128(&p, end);
        const uint8_t* section_end = p + section_len;
        if (section_end > end) return NYX_WAMR_ERR_INVALID_HEADER;

        if (section_id == 7) { /* Export Section */
            const uint8_t* sp = p;
            uint32_t export_count = read_u32_leb128(&sp, section_end);
            for (uint32_t i = 0; i < export_count; i++) {
                uint32_t name_len = read_u32_leb128(&sp, section_end);
                const char* exp_name = (const char*)sp;
                sp += name_len;
                uint8_t kind = *sp++;
                uint32_t func_idx = read_u32_leb128(&sp, section_end);

                if (kind == 0 && name_len == strlen(func_name) && strncmp(exp_name, func_name, name_len) == 0) {
                    target_func_idx = func_idx;
                    found_export = true;
                }
            }
        } else if (section_id == 10) { /* Code Section */
            code_section_start = p;
            code_section_len = section_len;
        }

        p = section_end;
    }

    if (!found_export || !code_section_start) {
        return NYX_WAMR_ERR_FUNCTION_NOT_FOUND;
    }

    /* Locate function body in Code Section */
    const uint8_t* cp = code_section_start;
    const uint8_t* cend = cp + code_section_len;
    uint32_t func_count = read_u32_leb128(&cp, cend);

    if (target_func_idx >= func_count) {
        return NYX_WAMR_ERR_FUNCTION_NOT_FOUND;
    }

    for (uint32_t i = 0; i < target_func_idx; i++) {
        uint32_t body_size = read_u32_leb128(&cp, cend);
        cp += body_size;
    }

    uint32_t func_body_size = read_u32_leb128(&cp, cend);
    const uint8_t* body_end = cp + func_body_size;

    /* Read local declarations */
    uint32_t local_decls = read_u32_leb128(&cp, body_end);
    uint32_t total_locals = (uint32_t)argc;
    for (uint32_t i = 0; i < local_decls; i++) {
        uint32_t count = read_u32_leb128(&cp, body_end);
        uint8_t type = *cp++;
        (void)type;
        total_locals += count;
    }

    /* Stack and Locals */
    uint32_t locals[64];
    for (size_t i = 0; i < argc && i < 64; i++) {
        locals[i] = args[i];
    }
    for (size_t i = argc; i < 64; i++) {
        locals[i] = 0;
    }

    uint32_t stack[NYX_WAMR_MAX_STACK];
    int sp_idx = 0;

    /* Bytecode Execution Loop */
    while (cp < body_end) {
        uint8_t op = *cp++;
        sandbox->instruction_count++;

        if (op == 0x01) { /* nop */
            continue;
        } else if (op == 0x0F) { /* return */
            break;
        } else if (op == 0x0B) { /* end */
            break;
        } else if (op == 0x20) { /* local.get */
            uint32_t idx = read_u32_leb128(&cp, body_end);
            if (idx < 64 && sp_idx < NYX_WAMR_MAX_STACK) {
                stack[sp_idx++] = locals[idx];
            }
        } else if (op == 0x21) { /* local.set */
            uint32_t idx = read_u32_leb128(&cp, body_end);
            if (idx < 64 && sp_idx > 0) {
                locals[idx] = stack[--sp_idx];
            }
        } else if (op == 0x22) { /* local.tee */
            uint32_t idx = read_u32_leb128(&cp, body_end);
            if (idx < 64 && sp_idx > 0) {
                locals[idx] = stack[sp_idx - 1];
            }
        } else if (op == 0x41) { /* i32.const */
            uint32_t val = read_u32_leb128(&cp, body_end);
            if (sp_idx < NYX_WAMR_MAX_STACK) {
                stack[sp_idx++] = val;
            }
        } else if (op == 0x6A) { /* i32.add */
            if (sp_idx >= 2) {
                uint32_t b = stack[--sp_idx];
                uint32_t a = stack[--sp_idx];
                stack[sp_idx++] = a + b;
            }
        } else if (op == 0x6B) { /* i32.sub */
            if (sp_idx >= 2) {
                uint32_t b = stack[--sp_idx];
                uint32_t a = stack[--sp_idx];
                stack[sp_idx++] = a - b;
            }
        } else if (op == 0x6C) { /* i32.mul */
            if (sp_idx >= 2) {
                uint32_t b = stack[--sp_idx];
                uint32_t a = stack[--sp_idx];
                stack[sp_idx++] = a * b;
            }
        } else if (op == 0x6D) { /* i32.div_s */
            if (sp_idx >= 2) {
                int32_t b = (int32_t)stack[--sp_idx];
                int32_t a = (int32_t)stack[--sp_idx];
                if (b == 0) return NYX_WAMR_ERR_TRAP;
                stack[sp_idx++] = (uint32_t)(a / b);
            }
        } else if (op == 0x28) { /* i32.load */
            uint32_t align = read_u32_leb128(&cp, body_end);
            uint32_t offset = read_u32_leb128(&cp, body_end);
            (void)align;
            if (sp_idx >= 1) {
                uint32_t base = stack[--sp_idx];
                uint32_t addr = base + offset;
                void* ptr = nyx_wamr_get_host_ptr(sandbox, addr, 4);
                if (!ptr) return NYX_WAMR_ERR_BOUNDS_VIOLATION;
                stack[sp_idx++] = *(uint32_t*)ptr;
            }
        } else if (op == 0x36) { /* i32.store */
            uint32_t align = read_u32_leb128(&cp, body_end);
            uint32_t offset = read_u32_leb128(&cp, body_end);
            (void)align;
            if (sp_idx >= 2) {
                uint32_t val = stack[--sp_idx];
                uint32_t base = stack[--sp_idx];
                uint32_t addr = base + offset;
                void* ptr = nyx_wamr_get_host_ptr(sandbox, addr, 4);
                if (!ptr) return NYX_WAMR_ERR_BOUNDS_VIOLATION;
                *(uint32_t*)ptr = val;
            }
        }
    }

    if (sp_idx > 0) {
        *out_result = stack[--sp_idx];
    } else {
        *out_result = 0;
    }

    return NYX_WAMR_OK;
}
