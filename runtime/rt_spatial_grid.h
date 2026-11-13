#ifndef RT_SPATIAL_GRID_H
#define RT_SPATIAL_GRID_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define NYX_SPATIAL_CACHE_LINE 64

#ifdef __cplusplus
extern "C" {
#endif

/* ── 3D Morton Z-Order Bit Interleaving ── */
/* Expands a 21-bit integer into 63 bits by inserting two zeros after each bit */
static inline uint64_t nyx_morton_part1by2(uint64_t n) {
    n &= 0x1fffff;
    n = (n | (n << 32)) & 0x1f00000000ffff;
    n = (n | (n << 16)) & 0x1f0000ff0000ff;
    n = (n | (n << 8))  & 0x100f00f00f00f00f;
    n = (n | (n << 4))  & 0x10c30c30c30c30c3;
    n = (n | (n << 2))  & 0x1249249249249249;
    return n;
}

/* Encodes 3D coordinates (X, Y, Z) into a single 64-bit Morton Z-order curve index */
static inline uint64_t nyx_morton_encode_3d(uint32_t x, uint32_t y, uint32_t z) {
    return (nyx_morton_part1by2(x)) |
           (nyx_morton_part1by2(y) << 1) |
           (nyx_morton_part1by2(z) << 2);
}

/* Decodes one coordinate from Morton code */
static inline uint32_t nyx_morton_compact1by2(uint64_t n) {
    n &= 0x1249249249249249;
    n = (n ^ (n >> 2))  & 0x10c30c30c30c30c3;
    n = (n ^ (n >> 4))  & 0x100f00f00f00f00f;
    n = (n ^ (n >> 8))  & 0x1f0000ff0000ff;
    n = (n ^ (n >> 16)) & 0x1f00000000ffff;
    n = (n ^ (n >> 32)) & 0x1fffff;
    return (uint32_t)n;
}

static inline void nyx_morton_decode_3d(uint64_t code, uint32_t* x, uint32_t* y, uint32_t* z) {
    if (x) *x = nyx_morton_compact1by2(code);
    if (y) *y = nyx_morton_compact1by2(code >> 1);
    if (z) *z = nyx_morton_compact1by2(code >> 2);
}

/* ── Spatial Grid Node for Programmable Matter & Nanotech ── */
typedef struct NyxSpatialNode {
    float potential;       /* Electric/magnetic potential */
    uint32_t phase;        /* Quantum / atomic phase state */
    uint32_t element_id;   /* Nanite/atom identity */
    uint32_t flags;        /* Entanglement / lock flags */
    float velocity[3];     /* Drift vector (vx, vy, vz) */
    uint8_t reserved[16];  /* 64-byte cache line alignment padding */
} __attribute__((aligned(16))) NyxSpatialNode;

/* ── 3D Spatial Grid Descriptor ── */
typedef struct NyxSpatialGrid3D {
    uint32_t dim_x;
    uint32_t dim_y;
    uint32_t dim_z;
    uint64_t total_cells;
    size_t node_size;
    void* buffer;          /* Morton-ordered cache-aligned buffer */
} NyxSpatialGrid3D;

/* Lifecycle and Operations */
NyxSpatialGrid3D* nyx_spatial_grid_create(uint32_t dim_x, uint32_t dim_y, uint32_t dim_z);
void nyx_spatial_grid_destroy(NyxSpatialGrid3D* grid);

/* O(1) Cache-Aligned Relocation */
bool nyx_spatial_relocate(NyxSpatialGrid3D* grid, 
                          uint32_t src_x, uint32_t src_y, uint32_t src_z,
                          uint32_t dst_x, uint32_t dst_y, uint32_t dst_z);

/* Direct Morton Access */
NyxSpatialNode* nyx_spatial_get(NyxSpatialGrid3D* grid, uint32_t x, uint32_t y, uint32_t z);
NyxSpatialNode* nyx_spatial_get_morton(NyxSpatialGrid3D* grid, uint64_t morton_code);

/* 6-Neighbor Stencil Evaluation */
typedef void (*NyxStencilCallback)(NyxSpatialNode* center, NyxSpatialNode* neighbors[6], void* userdata);
void nyx_spatial_apply_stencil(NyxSpatialGrid3D* grid, NyxStencilCallback cb, void* userdata);

#ifdef __cplusplus
}
#endif

#endif /* RT_SPATIAL_GRID_H */
