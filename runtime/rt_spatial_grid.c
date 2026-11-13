#include "rt_spatial_grid.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

NyxSpatialGrid3D* nyx_spatial_grid_create(uint32_t dim_x, uint32_t dim_y, uint32_t dim_z) {
    if (dim_x == 0 || dim_y == 0 || dim_z == 0) return NULL;
    
    NyxSpatialGrid3D* grid = (NyxSpatialGrid3D*)calloc(1, sizeof(NyxSpatialGrid3D));
    if (!grid) return NULL;
    
    grid->dim_x = dim_x;
    grid->dim_y = dim_y;
    grid->dim_z = dim_z;
    grid->node_size = sizeof(NyxSpatialNode);
    grid->total_cells = (uint64_t)dim_x * dim_y * dim_z;
    
    /* Allocate Morton buffer with 64-byte alignment */
    size_t total_bytes = grid->total_cells * grid->node_size;
#if defined(_WIN32)
    grid->buffer = _aligned_malloc(total_bytes, NYX_SPATIAL_CACHE_LINE);
#else
    if (posix_memalign(&grid->buffer, NYX_SPATIAL_CACHE_LINE, total_bytes) != 0) {
        grid->buffer = NULL;
    }
#endif

    if (!grid->buffer) {
        free(grid);
        return NULL;
    }

    memset(grid->buffer, 0, total_bytes);
    return grid;
}

void nyx_spatial_grid_destroy(NyxSpatialGrid3D* grid) {
    if (!grid) return;
    if (grid->buffer) {
#if defined(_WIN32)
        _aligned_free(grid->buffer);
#else
        free(grid->buffer);
#endif
    }
    free(grid);
}

NyxSpatialNode* nyx_spatial_get(NyxSpatialGrid3D* grid, uint32_t x, uint32_t y, uint32_t z) {
    if (!grid || !grid->buffer) return NULL;
    if (x >= grid->dim_x || y >= grid->dim_y || z >= grid->dim_z) return NULL;
    
    uint64_t morton = nyx_morton_encode_3d(x, y, z);
    if (morton >= grid->total_cells) {
        /* Fallback linear index for non-power-of-two dimensions */
        morton = (uint64_t)z * grid->dim_x * grid->dim_y + (uint64_t)y * grid->dim_x + x;
    }
    
    NyxSpatialNode* base = (NyxSpatialNode*)grid->buffer;
    return &base[morton];
}

NyxSpatialNode* nyx_spatial_get_morton(NyxSpatialGrid3D* grid, uint64_t morton_code) {
    if (!grid || !grid->buffer || morton_code >= grid->total_cells) return NULL;
    NyxSpatialNode* base = (NyxSpatialNode*)grid->buffer;
    return &base[morton_code];
}

bool nyx_spatial_relocate(NyxSpatialGrid3D* grid, 
                          uint32_t src_x, uint32_t src_y, uint32_t src_z,
                          uint32_t dst_x, uint32_t dst_y, uint32_t dst_z) {
    NyxSpatialNode* src = nyx_spatial_get(grid, src_x, src_y, src_z);
    NyxSpatialNode* dst = nyx_spatial_get(grid, dst_x, dst_y, dst_z);
    if (!src || !dst) return false;
    
    /* O(1) swap of node payloads inside aligned Morton space */
    NyxSpatialNode temp = *src;
    *src = *dst;
    *dst = temp;
    return true;
}

void nyx_spatial_apply_stencil(NyxSpatialGrid3D* grid, NyxStencilCallback cb, void* userdata) {
    if (!grid || !grid->buffer || !cb) return;

    for (uint32_t z = 1; z + 1 < grid->dim_z; ++z) {
        for (uint32_t y = 1; y + 1 < grid->dim_y; ++y) {
            for (uint32_t x = 1; x + 1 < grid->dim_x; ++x) {
                NyxSpatialNode* center = nyx_spatial_get(grid, x, y, z);
                NyxSpatialNode* neighbors[6] = {
                    nyx_spatial_get(grid, x - 1, y, z),     /* -X */
                    nyx_spatial_get(grid, x + 1, y, z),     /* +X */
                    nyx_spatial_get(grid, x, y - 1, z),     /* -Y */
                    nyx_spatial_get(grid, x, y + 1, z),     /* +Y */
                    nyx_spatial_get(grid, x, y, z - 1),     /* -Z */
                    nyx_spatial_get(grid, x, y, z + 1)      /* +Z */
                };
                cb(center, neighbors, userdata);
            }
        }
    }
}
