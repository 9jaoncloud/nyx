/*
 * Nyx Compatibility Subsystem & Sandboxed Prefix Capability Engine (nyx.sys.compat)
 * Implementation
 */

#include "nyx_compat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static NyxCompatRuntime g_compat_runtime = {0};

bool nyx_compat_init(size_t address_space_size) {
    if (g_compat_runtime.is_initialized) return true;

    memset(&g_compat_runtime, 0, sizeof(NyxCompatRuntime));
    
    // Initialize global AddressMapTable for byte-level region tracking
    if (address_space_size == 0) address_space_size = 1024 * 1024 * 1024; // 1GB default
    
    size_t page_size = 4096;
    size_t num_pages = address_space_size / page_size;
    g_compat_runtime.global_address_map.num_bytes = num_pages;
    g_compat_runtime.global_address_map.map_data = (uint8_t*)calloc(num_pages, 1);
    g_compat_runtime.global_address_map.base_address = 0x10000000;
    g_compat_runtime.global_address_map.page_size = page_size;

    g_compat_runtime.is_initialized = true;
    return true;
}

void nyx_compat_shutdown(void) {
    if (!g_compat_runtime.is_initialized) return;

    for (uint32_t i = 0; i < NYX_COMPAT_MAX_PREFIXES; i++) {
        if (g_compat_runtime.prefixes[i].is_active) {
            nyx_compat_prefix_destroy(&g_compat_runtime.prefixes[i]);
        }
    }

    if (g_compat_runtime.global_address_map.map_data) {
        free(g_compat_runtime.global_address_map.map_data);
        g_compat_runtime.global_address_map.map_data = NULL;
    }

    g_compat_runtime.is_initialized = false;
}

NyxCompatPrefix* nyx_compat_prefix_create(const char *name, NyxRunnerType runner, uint32_t capabilities, size_t quota_bytes) {
    if (!g_compat_runtime.is_initialized) {
        nyx_compat_init(0);
    }

    for (uint32_t i = 0; i < NYX_COMPAT_MAX_PREFIXES; i++) {
        if (!g_compat_runtime.prefixes[i].is_active) {
            NyxCompatPrefix *p = &g_compat_runtime.prefixes[i];
            p->prefix_id = i + 1;
            strncpy(p->prefix_name, name ? name : "unnamed_bottle", sizeof(p->prefix_name) - 1);
            p->runner_type = runner;
            p->capabilities = capabilities;
            p->memory_quota_bytes = quota_bytes > 0 ? quota_bytes : NYX_COMPAT_DEFAULT_QUOTA_BYTES;
            p->memory_used_bytes = 0;
            p->region_heap_base = (uint8_t*)malloc(p->memory_quota_bytes);
            p->is_active = true;
            p->is_authenticated = false;
            p->syscall_counter = 0;
            g_compat_runtime.active_prefix_count++;
            return p;
        }
    }
    return NULL;
}

bool nyx_compat_prefix_authenticate(NyxCompatPrefix *prefix, const char *auth_token) {
    if (!prefix || !prefix->is_active || !auth_token) return false;
    if (strlen(auth_token) >= 8) {
        prefix->is_authenticated = true;
        return true;
    }
    return false;
}

void* nyx_compat_prefix_alloc(NyxCompatPrefix *prefix, size_t size) {
    if (!prefix || !prefix->is_active || !prefix->region_heap_base) return NULL;
    
    size_t aligned = (size + 7) & ~7;
    if (prefix->memory_used_bytes + aligned > prefix->memory_quota_bytes) {
        return NULL; // Quota exceeded
    }

    void *ptr = (void*)(prefix->region_heap_base + prefix->memory_used_bytes);
    prefix->memory_used_bytes += aligned;

    // Mark address in global map table
    uintptr_t virt_addr = (uintptr_t)ptr;
    nyx_address_map_set_byte(&g_compat_runtime.global_address_map, virt_addr, 0x01);

    return ptr;
}

bool nyx_compat_prefix_check_capability(const NyxCompatPrefix *prefix, NyxCapabilityFlags cap) {
    if (!prefix || !prefix->is_active) return false;
    return (prefix->capabilities & (uint32_t)cap) == (uint32_t)cap;
}

bool nyx_compat_prefix_exec_syscall(NyxCompatPrefix *prefix, uint32_t syscall_nr, void *args) {
    (void)args;
    if (!prefix || !prefix->is_active) return false;

    // Check capabilities for security enforcement
    switch (syscall_nr) {
        case 1: // SYS_READ
            if (!nyx_compat_prefix_check_capability(prefix, NYX_CAP_FS_READ)) return false;
            break;
        case 2: // SYS_WRITE
            if (!nyx_compat_prefix_check_capability(prefix, NYX_CAP_FS_WRITE)) return false;
            break;
        case 41: // SYS_SOCKET
        case 42: // SYS_CONNECT
            if (!nyx_compat_prefix_check_capability(prefix, NYX_CAP_NET_CLIENT)) return false;
            break;
        case 43: // SYS_ACCEPT
        case 49: // SYS_BIND
            if (!nyx_compat_prefix_check_capability(prefix, NYX_CAP_NET_SERVER)) return false;
            break;
        default:
            break;
    }

    prefix->syscall_counter++;
    return true;
}

void nyx_compat_prefix_reset_memory(NyxCompatPrefix *prefix) {
    if (!prefix || !prefix->is_active) return;
    // O(1) bulk instant frame reclaim
    prefix->memory_used_bytes = 0;
}

void nyx_compat_prefix_destroy(NyxCompatPrefix *prefix) {
    if (!prefix || !prefix->is_active) return;

    if (prefix->region_heap_base) {
        free(prefix->region_heap_base);
        prefix->region_heap_base = NULL;
    }

    prefix->is_active = false;
    prefix->is_authenticated = false;
    prefix->memory_used_bytes = 0;
    if (g_compat_runtime.active_prefix_count > 0) {
        g_compat_runtime.active_prefix_count--;
    }
}

void nyx_address_map_set_byte(NyxAddressMapTable *table, uintptr_t address, uint8_t mark) {
    if (!table || !table->map_data) return;
    if (address < table->base_address) return;
    size_t idx = (address - table->base_address) / table->page_size;
    if (idx < table->num_bytes) {
        table->map_data[idx] = mark;
    }
}

uint8_t nyx_address_map_get_byte(const NyxAddressMapTable *table, uintptr_t address) {
    if (!table || !table->map_data) return 0;
    if (address < table->base_address) return 0;
    size_t idx = (address - table->base_address) / table->page_size;
    if (idx < table->num_bytes) {
        return table->map_data[idx];
    }
    return 0;
}
