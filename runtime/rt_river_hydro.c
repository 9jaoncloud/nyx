/*
 * rt_river_hydro.c — Nyx Hydrodynamic River Network & Fluid Simulation Implementation
 */

#include "runtime/rt_river_hydro.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

NyxHydroNetwork* nyx_hydro_network_create(void) {
    NyxHydroNetwork* net = (NyxHydroNetwork*)calloc(1, sizeof(NyxHydroNetwork));
    return net;
}

void nyx_hydro_network_destroy(NyxHydroNetwork* net) {
    if (net) {
        free(net);
    }
}

/* ── Uphill River Propagation Generator (Red Blob Games algorithm) ───────── */
void nyx_hydro_generate_rivers(NyxHydroNetwork* net, const NyxWorldConfig* cfg, size_t num_deltas) {
    if (!net || !cfg) return;

    net->river_count = 0;
    size_t target_rivers = num_deltas > MAX_RIVER_SYSTEMS ? MAX_RIVER_SYSTEMS : num_deltas;

    const char* river_names[] = {
        "Amazonas Great Arterial", "Nile Oasis Highway", "Yukon Glacial Run", 
        "Congo Basin Delta", "Danube Trade Stream", "Mississippi Basin"
    };

    for (size_t r = 0; r < target_rivers; r++) {
        NyxRiverSystem* river = &net->rivers[net->river_count++];
        river->id = (int)(r + 1);
        snprintf(river->name, sizeof(river->name), "%s #%zu", 
                 river_names[r % (sizeof(river_names)/sizeof(river_names[0]))], r + 1);
        river->node_count = 0;
        river->total_length_km = 0.0f;

        // 1. Seed Delta at coastal interface (low elevation, high ocean proximity)
        float angle = ((float)r / (float)target_rivers) * 2.0f * (float)M_PI;
        float center_x = cfg->continent_width_km * 0.5f;
        float center_y = cfg->continent_height_km * 0.5f;
        float coast_radius = cfg->continent_width_km * 0.38f;

        float cur_x = center_x + cosf(angle) * coast_radius;
        float cur_y = center_y + sinf(angle) * coast_radius;

        NyxTerrainCell start_cell = nyx_sample_world_cell(cfg, cur_x, cur_y);
        
        // Add Delta Root Node (Order 4 Highway River)
        NyxRiverNode delta_node;
        delta_node.x_km = cur_x;
        delta_node.y_km = cur_y;
        delta_node.elevation = start_cell.elevation;
        delta_node.flow_volume = 1200.0f; // High flow at ocean delta
        delta_node.width_meters = 180.0f;
        delta_node.depth_meters = 14.0f;
        delta_node.stream_order = 4;
        delta_node.parent_index = -1;

        river->nodes[river->node_count++] = delta_node;

        // 2. Propagate Uphill towards interior mountain highlands
        int max_steps = 180;
        float step_len = 0.45f; // 450 meters per segment

        for (int step = 0; step < max_steps; step++) {
            if (river->node_count >= MAX_RIVER_NODES - 1) break;

            int prev_idx = (int)river->node_count - 1;
            NyxRiverNode prev = river->nodes[prev_idx];

            // Search for highest elevation and precipitation in upstream direction
            // Triangular / Meandering Angular offset
            float uphill_dir_x = center_x - prev.x_km;
            float uphill_dir_y = center_y - prev.y_km;
            float len = sqrtf(uphill_dir_x * uphill_dir_x + uphill_dir_y * uphill_dir_y);
            if (len > 0.001f) {
                uphill_dir_x /= len;
                uphill_dir_y /= len;
            }

            // Natural Meandering Oscillation using sine wave + noise
            float meander = sinf((float)step * 0.35f + (float)r) * 0.65f;
            float normal_x = -uphill_dir_y;
            float normal_y = uphill_dir_x;

            float next_x = prev.x_km + (uphill_dir_x + normal_x * meander) * step_len;
            float next_y = prev.y_km + (uphill_dir_y + normal_y * meander) * step_len;

            // Bounds check
            if (next_x < 2.0f || next_x > cfg->continent_width_km - 2.0f ||
                next_y < 2.0f || next_y > cfg->continent_height_km - 2.0f) {
                break;
            }

            NyxTerrainCell cell = nyx_sample_world_cell(cfg, next_x, next_y);

            // Flow volume tapers upstream as tributaries separate
            float flow_ratio = 1.0f - ((float)step / (float)max_steps);
            float flow = 1200.0f * (flow_ratio * flow_ratio) + 20.0f;

            NyxRiverNode node;
            node.x_km = next_x;
            node.y_km = next_y;
            node.elevation = cell.elevation;
            node.flow_volume = flow;
            node.width_meters = fmaxf(8.0f, 180.0f * sqrtf(flow / 1200.0f));
            node.depth_meters = fmaxf(1.5f, 14.0f * powf(flow / 1200.0f, 0.4f));
            node.stream_order = flow > 600.0f ? 4 : (flow > 200.0f ? 3 : (flow > 50.0f ? 2 : 1));
            node.parent_index = prev_idx;

            river->nodes[river->node_count++] = node;
            river->total_length_km += step_len;

            // Terminate if reached mountain peak ridge (> 0.75 elevation)
            if (cell.elevation > 0.75f) {
                break;
            }
        }
    }
}

bool nyx_hydro_is_near_river(const NyxHydroNetwork* net, float x_km, float y_km, float* out_dist_m, float* out_flow) {
    if (!net) return false;

    float min_dist_sq = 1e12f;
    float best_flow = 0.0f;

    for (size_t r = 0; r < net->river_count; r++) {
        const NyxRiverSystem* river = &net->rivers[r];
        for (size_t i = 0; i < river->node_count; i++) {
            float dx = (river->nodes[i].x_km - x_km) * 1000.0f; // km to meters
            float dy = (river->nodes[i].y_km - y_km) * 1000.0f;
            float dist_sq = dx * dx + dy * dy;

            if (dist_sq < min_dist_sq) {
                min_dist_sq = dist_sq;
                best_flow = river->nodes[i].flow_volume;
            }
        }
    }

    float dist_m = sqrtf(min_dist_sq);
    if (out_dist_m) *out_dist_m = dist_m;
    if (out_flow) *out_flow = best_flow;

    return (dist_m <= 1500.0f); // 1.5 km corridor
}

void nyx_hydro_apply_river_greening(const NyxHydroNetwork* net, NyxTerrainCell* cell, float x_km, float y_km) {
    if (!net || !cell) return;

    float dist_m = 0.0f;
    float flow = 0.0f;
    if (nyx_hydro_is_near_river(net, x_km, y_km, &dist_m, &flow)) {
        if (dist_m < 80.0f) {
            cell->is_river = true;
            cell->is_water = true;
        }

        // River corridor irrigation & greening
        // Points within 400m of a river receive heavy water fertility boost
        if (dist_m < 400.0f) {
            float green_factor = 1.0f - (dist_m / 400.0f);
            
            // Greening transforms desert banks into lush vegetation
            if (cell->biome == BIOME_ARID_DESERT) {
                cell->fertility = fmaxf(cell->fertility, 0.75f * green_factor);
                cell->humidity = fmaxf(cell->humidity, 0.60f * green_factor);
            } else {
                cell->fertility = fminf(1.0f, cell->fertility + 0.35f * green_factor);
            }

            // Sand Point Bar Deposition on inner curves
            if (dist_m >= 40.0f && dist_m <= 120.0f) {
                cell->sand_point_bar = green_factor * 0.9f;
            }
        }
    }
}

/* ── Chunked Shallow Water Simulation ────────────────────────────────────── */
NyxFluidChunk* nyx_fluid_chunk_create(int chunk_x, int chunk_y) {
    NyxFluidChunk* chunk = (NyxFluidChunk*)calloc(1, sizeof(NyxFluidChunk));
    if (chunk) {
        chunk->chunk_x = chunk_x;
        chunk->chunk_y = chunk_y;
        chunk->is_active = true;
    }
    return chunk;
}

void nyx_fluid_chunk_step(NyxFluidChunk* chunk, float dt, float damping) {
    if (!chunk || !chunk->is_active) return;

    // Fast 2D Shallow Water Heightfield Wave Equation Solver
    float c_sq = 9.81f * dt * dt; // Gravity wave propagation

    for (int y = 1; y < HYDRO_CHUNK_SIZE - 1; y++) {
        for (int x = 1; x < HYDRO_CHUNK_SIZE - 1; x++) {
            float h_center = chunk->water_height[y][x];
            float h_left   = chunk->water_height[y][x - 1];
            float h_right  = chunk->water_height[y][x + 1];
            float h_up     = chunk->water_height[y - 1][x];
            float h_down   = chunk->water_height[y + 1][x];

            // Laplacian of water height
            float laplacian = (h_left + h_right + h_up + h_down) - 4.0f * h_center;

            // Velocity update
            chunk->velocity_u[y][x] += laplacian * c_sq * 0.25f;
            chunk->velocity_u[y][x] *= damping;

            // Height update
            chunk->water_height[y][x] += chunk->velocity_u[y][x];
            if (chunk->water_height[y][x] < 0.0f) {
                chunk->water_height[y][x] = 0.0f;
            }
        }
    }
}
