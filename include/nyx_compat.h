/*
 * Nyx Compatibility Subsystem & Sandboxed Prefix Capability Engine (nyx.sys.compat)
 * 
 * Provides:
 * 1. Sandboxed micro-region capability prefixes (Bottles/Soda architecture)
 * 2. Zero-GC isolation for legacy Win32 / POSIX binary execution
 * 3. Lock-free Atomic AddressMapTable (Blackburn ISMM 2016 multi-threaded byte maps)
 * 4. Capability permission tokens and deterministic frame quotas (< 500MB)
 */

#ifndef NYX_COMPAT_H
#define NYX_COMPAT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NYX_COMPAT_MAX_PREFIXES 64
#define NYX_COMPAT_DEFAULT_QUOTA_BYTES (512 * 1024 * 1024) // 512MB max sandbox footprint

/* Capability Permission Flags */
typedef enum {
    NYX_CAP_FS_READ     = (1 << 0),
    NYX_CAP_FS_WRITE    = (1 << 1),
    NYX_CAP_NET_CLIENT  = (1 << 2),
    NYX_CAP_NET_SERVER  = (1 << 3),
    NYX_CAP_DISPLAY_X11 = (1 << 4),
    NYX_CAP_DISPLAY_WAYLAND = (1 << 5),
    NYX_CAP_AUTH_2FA    = (1 << 6),
    NYX_CAP_ALL         = 0xFF
} NyxCapabilityFlags;

/* Prefix Execution Environment Types */
typedef enum {
    NYX_RUNNER_WIN32_SODA  = 0, // Win32 / MS 365 / DirectX translated runner
    NYX_RUNNER_POSIX_MUSL  = 1, // POSIX / Linux ABI compatibility runner
    NYX_RUNNER_WASM_SANDBOX = 2, // WebAssembly capability container
    NYX_RUNNER_UNIKERNEL   = 3  // Nyx native zero-GC isolated unikernel
} NyxRunnerType;

/* Atomic Address Map Table (from Blackburn ISMM 2016) */
typedef struct {
    uint8_t *map_data;
    size_t num_bytes;
    uintptr_t base_address;
    size_t page_size;
} NyxAddressMapTable;

/* Sandboxed Capability Prefix Instance (A "Bottle" in NyxOS) */
typedef struct {
    uint32_t prefix_id;
    char prefix_name[64];
    NyxRunnerType runner_type;
    uint32_t capabilities;
    size_t memory_quota_bytes;
    size_t memory_used_bytes;
    uint8_t *region_heap_base;
    bool is_active;
    bool is_authenticated;
    uint64_t syscall_counter;
    double execution_start_time;
} NyxCompatPrefix;

/* Global Compatibility Runtime State */
typedef struct {
    NyxCompatPrefix prefixes[NYX_COMPAT_MAX_PREFIXES];
    uint32_t active_prefix_count;
    NyxAddressMapTable global_address_map;
    bool is_initialized;
} NyxCompatRuntime;

/* API Methods */
bool nyx_compat_init(size_t address_space_size);
void nyx_compat_shutdown(void);

NyxCompatPrefix* nyx_compat_prefix_create(const char *name, NyxRunnerType runner, uint32_t capabilities, size_t quota_bytes);
bool nyx_compat_prefix_authenticate(NyxCompatPrefix *prefix, const char *auth_token);
void* nyx_compat_prefix_alloc(NyxCompatPrefix *prefix, size_t size);
bool nyx_compat_prefix_check_capability(const NyxCompatPrefix *prefix, NyxCapabilityFlags cap);
bool nyx_compat_prefix_exec_syscall(NyxCompatPrefix *prefix, uint32_t syscall_nr, void *args);
void nyx_compat_prefix_reset_memory(NyxCompatPrefix *prefix);
void nyx_compat_prefix_destroy(NyxCompatPrefix *prefix);

/* Address Map Table Methods */
void nyx_address_map_set_byte(NyxAddressMapTable *table, uintptr_t address, uint8_t mark);
uint8_t nyx_address_map_get_byte(const NyxAddressMapTable *table, uintptr_t address);

#ifdef __cplusplus
}
#endif

#endif /* NYX_COMPAT_H */
