/*
 * rt_river_hydro.h — Nyx Hydrodynamic River Network & Fluid Simulation (Red Blob Algorithm)
 *
 * Implements:
 *  1. Coastline Delta Seeding & Uphill Reverse Propagation
 *  2. Meandering Rivers via Triangular Graph Traversal
 *  3. Tributary Merging & Stream Order Scaling (Major Amazon/Nile scale highways)
 *  4. Point-Bar Sand Deposition on Inner Curves
 *  5. River Greening Effect (Transforms Arid Desert Corridors into Lush Fertile Valleys)
 *  6. 2D/3D Chunk-Based Shallow Water Heightfield Simulation
 */

#ifndef RT_RIVER_HYDRO_H
#define RT_RIVER_HYDRO_H

#include "runtime/rt_world_biome.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_RIVER_NODES 2048
#define MAX_RIVER_SYSTEMS 64
#define HYDRO_CHUNK_SIZE 32

typedef struct {
    float x_km;
    float y_km;
    float elevation;
    float flow_volume;      // m3/s (Scales river width)
    float width_meters;
    float depth_meters;
    int stream_order;       // 1 = small brook, 4+ = giant highway river
    int parent_index;       // Downstream node (-1 if ocean delta)
} NyxRiverNode;

typedef struct {
    int id;
    char name[48];
    NyxRiverNode nodes[MAX_RIVER_NODES];
    size_t node_count;
    float total_length_km;
} NyxRiverSystem;

typedef struct {
    NyxRiverSystem rivers[MAX_RIVER_SYSTEMS];
    size_t river_count;
} NyxHydroNetwork;

/* ── Chunked Shallow Water Simulation ────────────────────────────────────── */
typedef struct {
    int chunk_x, chunk_y;
    float water_height[HYDRO_CHUNK_SIZE][HYDRO_CHUNK_SIZE];
    float terrain_height[HYDRO_CHUNK_SIZE][HYDRO_CHUNK_SIZE];
    float velocity_u[HYDRO_CHUNK_SIZE][HYDRO_CHUNK_SIZE];
    float velocity_v[HYDRO_CHUNK_SIZE][HYDRO_CHUNK_SIZE];
    bool is_active;
} NyxFluidChunk;

/* ── River Network API ───────────────────────────────────────────────────── */
NyxHydroNetwork* nyx_hydro_network_create(void);
void nyx_hydro_network_destroy(NyxHydroNetwork* net);

// Generate procedural rivers propagating from ocean deltas up to mountain peaks
void nyx_hydro_generate_rivers(NyxHydroNetwork* net, const NyxWorldConfig* cfg, size_t num_deltas);

// Query if a given coordinate is on or near a river
bool nyx_hydro_is_near_river(const NyxHydroNetwork* net, float x_km, float y_km, float* out_dist_m, float* out_flow);

// Apply river fertilization & greening to a terrain cell
void nyx_hydro_apply_river_greening(const NyxHydroNetwork* net, NyxTerrainCell* cell, float x_km, float y_km);

/* ── Shallow Water Simulation API ────────────────────────────────────────── */
NyxFluidChunk* nyx_fluid_chunk_create(int chunk_x, int chunk_y);
void nyx_fluid_chunk_step(NyxFluidChunk* chunk, float dt, float damping);

#ifdef __cplusplus
}
#endif

#endif /* RT_RIVER_HYDRO_H */
