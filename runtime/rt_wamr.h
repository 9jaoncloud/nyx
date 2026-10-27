/*
 * rt_wamr.h — Nyx Embedded WebAssembly Micro Runtime (WAMR) Region Sandboxing Engine
 *
 * Implements:
 *  1. Host-Managed Heap Sandboxing via WAMR `Alloc_With_Pool` mode
 *  2. Pre-allocated Zero-GC Nyx Region Arena slice (`FrameArena`) as VM memory pool
 *  3. Instantaneous arena rewinding on call conclusion (0.00 ms GC pause latency)
 *  4. Microcontroller & IoT hardware boundary protection (zero buffer escapes)
 */

#ifndef RT_WAMR_H
#define RT_WAMR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "rt_arena.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NYX_WASM_MAGIC 0x6d736100  /* \0asm in little-endian */
#define NYX_WASM_VERSION 0x00000001
#define NYX_WAMR_PAGE_SIZE 65536   /* 64 KB Wasm Page */
#define NYX_WAMR_MAX_STACK 1024

/* Sandbox Execution Status */
typedef enum {
    NYX_WAMR_OK = 0,
    NYX_WAMR_ERR_INVALID_HEADER = 1,
    NYX_WAMR_ERR_OUT_OF_MEMORY = 2,
    NYX_WAMR_ERR_BOUNDS_VIOLATION = 3,
    NYX_WAMR_ERR_FUNCTION_NOT_FOUND = 4,
    NYX_WAMR_ERR_TRAP = 5
} NyxWamrStatus;

/* WebAssembly Micro Runtime Sandbox Context */
typedef struct NyxWasmSandbox {
    Arena* arena;                    /* Parent Nyx Region Arena */
    ArenaCheckpoint checkpoint;       /* Checkpoint before sandbox instantiation */
    uint8_t* memory_pool;            /* Pre-allocated slice from arena (Alloc_With_Pool) */
    size_t pool_size;                /* Total memory pool bytes */
    size_t pool_used;                /* Bytes used inside pool */
    
    /* Linear Memory */
    uint8_t* linear_memory;          /* Sandboxed linear memory buffer */
    size_t linear_mem_size;          /* Current linear memory size (bytes) */
    size_t linear_mem_max;           /* Maximum allowed memory size */

    /* Module and Function Descriptors */
    const uint8_t* wasm_bytecode;
    size_t wasm_size;
    uint32_t num_functions;
    uint32_t num_exports;
    
    /* Fast Execution Metrics */
    uint64_t instruction_count;
    bool is_isolated;
} NyxWasmSandbox;

/* Initialization and Lifecycle */
bool nyx_wamr_init(void);
NyxWasmSandbox* nyx_wamr_create_sandbox(Arena* arena, const uint8_t* wasm_bytes, size_t wasm_size, size_t pool_size);
void nyx_wamr_rewind_and_free(NyxWasmSandbox* sandbox);
void nyx_wamr_destroy(NyxWasmSandbox* sandbox);

/* Memory Operations within Sandbox Boundary */
uint32_t nyx_wamr_alloc_linear(NyxWasmSandbox* sandbox, size_t size);
bool nyx_wamr_copy_to_wasm(NyxWasmSandbox* sandbox, uint32_t wasm_dst_offset, const void* src, size_t size);
bool nyx_wamr_copy_from_wasm(NyxWasmSandbox* sandbox, void* dst, uint32_t wasm_src_offset, size_t size);
void* nyx_wamr_get_host_ptr(NyxWasmSandbox* sandbox, uint32_t wasm_offset, size_t size);

/* Function Invocation */
NyxWamrStatus nyx_wamr_execute(NyxWasmSandbox* sandbox, const char* func_name, const uint32_t* args, size_t argc, uint32_t* out_result);

/* Diagnostics */
size_t nyx_wamr_get_pool_used(const NyxWasmSandbox* sandbox);
uint64_t nyx_wamr_get_instruction_count(const NyxWasmSandbox* sandbox);

#ifdef __cplusplus
}
#endif

#endif /* RT_WAMR_H */
