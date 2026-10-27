/**
 * Nyx Programming Language — Zero-GC Hypervisor & iOS Virtualization Subsystem
 * Header: include/nyx_vz.h
 * 
 * Provides native bindings for Apple Virtualization.framework, Hypervisor.framework,
 * Mach-O kernelcache patching, TrustCache generation, and VirtIO hardware emulation.
 * 
 * Author: Simeon Bala (9jaoncloud@gmail.com)
 * Copyright (c) 2026 9jaonCloud Infrastructure
 */

#ifndef NYX_VZ_H
#define NYX_VZ_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Configuration constants */
#define NYX_VZ_MAX_CPUS           16
#define NYX_VZ_MAX_DISKS          8
#define NYX_VZ_MAX_NETWORKS       4
#define NYX_VZ_FRAMEBUFFER_MAX_W  1920
#define NYX_VZ_FRAMEBUFFER_MAX_H  1080

/* Virtual Machine State */
typedef enum {
    NYX_VZ_STATE_STOPPED = 0,
    NYX_VZ_STATE_CONFIGURED,
    NYX_VZ_STATE_BOOTING,
    NYX_VZ_STATE_RUNNING,
    NYX_VZ_STATE_PAUSED,
    NYX_VZ_STATE_ERROR
} nyx_vz_state_t;

/* Architecture Target */
typedef enum {
    NYX_VZ_ARCH_ARM64 = 0,
    NYX_VZ_ARCH_X86_64
} nyx_vz_arch_t;

/* iOS VM Configuration Descriptor */
typedef struct {
    uint32_t cpu_count;               /* Number of vCPUs (e.g. 4) */
    uint64_t memory_bytes;            /* RAM in bytes (e.g. 4ULL * 1024 * 1024 * 1024) */
    char ipsw_path[512];              /* Path to IPSW firmware archive */
    char disk_image_path[512];        /* Path to APFS raw disk image */
    char kernelcache_path[512];       /* Path to extracted kernelcache */
    char devicetree_path[512];        /* Path to synthetic DeviceTree blob */
    char trustcache_path[512];        /* Path to injected TrustCache */
    bool enable_root_ssh;             /* Enable root dropbear/OpenSSH daemon */
    bool enable_vnc_display;          /* Enable VirtIO GPU display output */
    uint16_t ssh_port;                /* Host mapped SSH port (e.g. 2222) */
    uint16_t vnc_port;                /* Host mapped VNC port (e.g. 5900) */
} nyx_vz_config_t;

/* Live Virtual Machine Instance */
typedef struct {
    nyx_vz_config_t config;
    nyx_vz_state_t state;
    uint64_t vm_handle;               /* Internal VZVirtualMachine pointer */
    uint64_t uptime_ms;
    uint64_t instructions_executed;
    uint32_t active_vcpus;
    float cpu_usage_pct;
    
    /* Framebuffer Memory Tap */
    uint32_t fb_width;
    uint32_t fb_height;
    uint32_t fb_stride;
    uint8_t* fb_pixels;               /* Zero-copy RGBX buffer */
    
    /* TrustCache Telemetry */
    uint32_t trustcache_entry_count;
    char ios_build_version[64];
    char kernel_version[128];
} nyx_vz_vm_t;

/* Core Lifecycle API */
int nyx_vz_init_default_config(nyx_vz_config_t* config);
int nyx_vz_create_vm(const nyx_vz_config_t* config, nyx_vz_vm_t** out_vm);
int nyx_vz_patch_ipsw_bootchain(const char* ipsw_in, const char* out_dir, char* out_kernel, char* out_dt);
int nyx_vz_generate_trustcache(const char* daemon_dir, const char* out_trustcache, uint32_t* out_entries);
int nyx_vz_start_vm(nyx_vz_vm_t* vm);
int nyx_vz_pause_vm(nyx_vz_vm_t* vm);
int nyx_vz_resume_vm(nyx_vz_vm_t* vm);
int nyx_vz_stop_vm(nyx_vz_vm_t* vm);
void nyx_vz_destroy_vm(nyx_vz_vm_t* vm);

/* Framebuffer & Event Interception (Zero-GC) */
int nyx_vz_capture_framebuffer(nyx_vz_vm_t* vm, uint8_t* dest_buffer, size_t dest_size, uint32_t* out_w, uint32_t* out_h);
int nyx_vz_send_touch_event(nyx_vz_vm_t* vm, uint32_t x, uint32_t y, uint8_t touch_phase);
int nyx_vz_send_key_event(nyx_vz_vm_t* vm, uint16_t keycode, bool is_down);

/* Guest Command Execution via SSH Bridge */
int nyx_vz_exec_guest_command(nyx_vz_vm_t* vm, const char* cmd, char* out_stdout, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif /* NYX_VZ_H */
