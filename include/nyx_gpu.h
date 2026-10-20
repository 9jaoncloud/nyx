#ifndef NYX_GPU_H
#define NYX_GPU_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Nyx Unified Heterogeneous GPU & Accelerator Subsystem
 * Architecture for Zero-GC, Zero-Tax GPU Systems & Compilers
 * -------------------------------------------------------------------------- */

typedef enum {
    NYX_GPU_BACKEND_PTX = 0,     /* NVIDIA CUDA PTX ISA */
    NYX_GPU_BACKEND_ROCM = 1,    /* AMD ROCm / AMDGPU ISA */
    NYX_GPU_BACKEND_SPIRV = 2,   /* Vulkan / OpenCL SPIR-V Compute */
    NYX_GPU_BACKEND_METAL = 3,   /* Apple Metal Shading Language */
    NYX_GPU_BACKEND_CPU_SIMD = 4 /* AVX-512 / ARM Neon Fallback */
} NyxGpuBackendType;

typedef enum {
    NYX_GPU_MEM_GLOBAL = 0,  /* High-Bandwidth Device VRAM */
    NYX_GPU_MEM_SHARED = 1,  /* On-Chip Fast Scratchpad SRAM (Sub-ns) */
    NYX_GPU_MEM_WARP_REG = 2 /* SIMD Vector Register Ring */
} NyxGpuMemoryTier;

/* Hardware Capability Flags for GPU Sandboxing */
#define NYX_GPU_CAP_COMPUTE     (1 << 0)
#define NYX_GPU_CAP_DMA_P2P     (1 << 1)
#define NYX_GPU_CAP_SHARED_SRAM (1 << 2)
#define NYX_GPU_CAP_ATOMIC_FP32 (1 << 3)

/* GPU Execution Context and Region Envelopes */
typedef struct {
    uint32_t device_id;
    NyxGpuBackendType backend;
    uint32_t compute_units;
    size_t total_vram_bytes;
    size_t shared_sram_per_cu_bytes;
    uint32_t capability_flags;
} NyxGpuDevice;

typedef struct {
    uint8_t* base_ptr;
    uint8_t* cursor_ptr;
    size_t capacity_bytes;
    size_t used_bytes;
    NyxGpuMemoryTier tier;
    bool is_coherent;
} NyxGpuRegionFrame;

typedef struct {
    uint32_t grid_dim_x;
    uint32_t grid_dim_y;
    uint32_t block_dim_x;
    uint32_t block_dim_y;
    size_t dynamic_shared_bytes;
    void (*kernel_func)(void* args, uint32_t thread_idx, uint32_t block_idx);
} NyxGpuKernelDescriptor;

/* --------------------------------------------------------------------------
 * Public API
 * -------------------------------------------------------------------------- */

NyxGpuDevice* nyx_gpu_init_device(uint32_t device_id, NyxGpuBackendType backend);
void nyx_gpu_shutdown_device(NyxGpuDevice* dev);

NyxGpuRegionFrame* nyx_gpu_alloc_region(NyxGpuDevice* dev, NyxGpuMemoryTier tier, size_t capacity_bytes);
void* nyx_gpu_region_bump_alloc(NyxGpuRegionFrame* frame, size_t size, size_t align);
void nyx_gpu_region_reset(NyxGpuRegionFrame* frame);
void nyx_gpu_region_free(NyxGpuRegionFrame* frame);

/* Zero-Tax Affine Buffer Handoff (Host <-> Device) */
bool nyx_gpu_affine_dma_transfer(NyxGpuRegionFrame* dst, const void* src_host, size_t size);

/* Kernel Dispatch and Warp Execution */
bool nyx_gpu_dispatch_kernel(NyxGpuDevice* dev, const NyxGpuKernelDescriptor* desc, void* args);

#ifdef __cplusplus
}
#endif

#endif /* NYX_GPU_H */
