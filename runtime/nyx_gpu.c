#include "nyx_gpu.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

NyxGpuDevice* nyx_gpu_init_device(uint32_t device_id, NyxGpuBackendType backend) {
    NyxGpuDevice* dev = (NyxGpuDevice*)malloc(sizeof(NyxGpuDevice));
    if (!dev) return NULL;

    dev->device_id = device_id;
    dev->backend = backend;
    dev->compute_units = 64; // e.g. 64 Compute Units / Streaming Multiprocessors
    dev->total_vram_bytes = (size_t)16 * 1024 * 1024 * 1024; // 16 GB VRAM
    dev->shared_sram_per_cu_bytes = 64 * 1024; // 64 KB per CU
    dev->capability_flags = NYX_GPU_CAP_COMPUTE | NYX_GPU_CAP_DMA_P2P | NYX_GPU_CAP_SHARED_SRAM | NYX_GPU_CAP_ATOMIC_FP32;

    return dev;
}

void nyx_gpu_shutdown_device(NyxGpuDevice* dev) {
    if (dev) {
        free(dev);
    }
}

NyxGpuRegionFrame* nyx_gpu_alloc_region(NyxGpuDevice* dev, NyxGpuMemoryTier tier, size_t capacity_bytes) {
    if (!dev || capacity_bytes == 0) return NULL;

    NyxGpuRegionFrame* frame = (NyxGpuRegionFrame*)malloc(sizeof(NyxGpuRegionFrame));
    if (!frame) return NULL;

    // Simulate cache-aligned VRAM / SRAM allocation
    frame->base_ptr = (uint8_t*)malloc(capacity_bytes);
    if (!frame->base_ptr) {
        free(frame);
        return NULL;
    }

    frame->cursor_ptr = frame->base_ptr;
    frame->capacity_bytes = capacity_bytes;
    frame->used_bytes = 0;
    frame->tier = tier;
    frame->is_coherent = true;

    return frame;
}

void* nyx_gpu_region_bump_alloc(NyxGpuRegionFrame* frame, size_t size, size_t align) {
    if (!frame || size == 0) return NULL;

    if (align == 0) align = 16;
    uintptr_t current = (uintptr_t)frame->cursor_ptr;
    uintptr_t aligned = (current + (align - 1)) & ~(align - 1);
    size_t pad = aligned - current;

    if (frame->used_bytes + pad + size > frame->capacity_bytes) {
        return NULL; // Exceeds region quota
    }

    frame->cursor_ptr = (uint8_t*)(aligned + size);
    frame->used_bytes += (pad + size);

    return (void*)aligned;
}

void nyx_gpu_region_reset(NyxGpuRegionFrame* frame) {
    if (frame) {
        // Single-cycle O(1) bulk frame reclamation
        frame->cursor_ptr = frame->base_ptr;
        frame->used_bytes = 0;
    }
}

void nyx_gpu_region_free(NyxGpuRegionFrame* frame) {
    if (frame) {
        if (frame->base_ptr) {
            free(frame->base_ptr);
        }
        free(frame);
    }
}

bool nyx_gpu_affine_dma_transfer(NyxGpuRegionFrame* dst, const void* src_host, size_t size) {
    if (!dst || !src_host || size == 0) return false;

    void* dest_mem = nyx_gpu_region_bump_alloc(dst, size, 64);
    if (!dest_mem) return false;

    // Direct memory copy (simulates DMA bar transfer)
    memcpy(dest_mem, src_host, size);
    return true;
}

bool nyx_gpu_dispatch_kernel(NyxGpuDevice* dev, const NyxGpuKernelDescriptor* desc, void* args) {
    if (!dev || !desc || !desc->kernel_func) return false;

    uint32_t total_blocks = desc->grid_dim_x * desc->grid_dim_y;
    uint32_t threads_per_block = desc->block_dim_x * desc->block_dim_y;

    for (uint32_t b = 0; b < total_blocks; ++b) {
        for (uint32_t t = 0; t < threads_per_block; ++t) {
            desc->kernel_func(args, t, b);
        }
    }

    return true;
}
