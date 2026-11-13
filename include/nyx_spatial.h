/**
 * @file nyx_spatial.h
 * @brief Nyx Spatial UI, Raymarched SDFs & GPU Compute Particle Subsystem
 * @author Simeon Bala (9jaoncloud@gmail.com)
 * @copyright (c) 2026 9jaonCloud Engineering. All Rights Reserved.
 *
 * Provides C99-native zero-allocation structures and algorithms for:
 * 1. Raymarched Signed Distance Fields (SDFs): Spheres, gyroids, plasma toruses.
 * 2. 50,000+ Audio-Reactive Particle Compute Swarms.
 * 3. Cybernetic Tactical Telemetry Reticles & Radar Scopes.
 * 4. 3D Neural Synapse Thought Graph Layout.
 */

#ifndef NYX_SPATIAL_H
#define NYX_SPATIAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum particles per compute frame */
#define NYX_MAX_PARTICLES 65536
/* Maximum neural thought nodes */
#define NYX_MAX_SYNAPSE_NODES 256
/* Maximum tactical telemetry widgets */
#define NYX_MAX_HUD_WIDGETS 32

/* Vector 3D float */
typedef struct {
    float x;
    float y;
    float z;
} nyx_vec3_t;

/* Vector 4D float (Color / Homogeneous) */
typedef struct {
    float r;
    float g;
    float b;
    float a;
} nyx_color_t;

/* Single Particle Structure (16-byte aligned for SIMD) */
typedef struct {
    nyx_vec3_t position;
    float life;
    nyx_vec3_t velocity;
    float size;
    nyx_color_t color;
    float energy;
    float padding[3];
} nyx_particle_t;

/* Particle Swarm Simulation Buffer */
typedef struct {
    nyx_particle_t particles[NYX_MAX_PARTICLES];
    size_t active_count;
    float vortex_strength;
    float audio_reactivity;
    float expansion_rate;
} nyx_particle_swarm_t;

/* SDF Primitive Types */
typedef enum {
    NYX_SDF_SPHERE   = 0,
    NYX_SDF_TORUS    = 1,
    NYX_SDF_GYROID   = 2,
    NYX_SDF_PULSAR   = 3
} nyx_sdf_type_t;

/* SDF Volume State */
typedef struct {
    nyx_sdf_type_t type;
    nyx_vec3_t center;
    float radius;
    float morph_factor;
    float pulse_frequency;
    float glow_intensity;
    nyx_color_t primary_color;
    nyx_color_t secondary_color;
} nyx_sdf_volume_t;

/* Neural Synapse Node State */
typedef struct {
    uint32_t node_id;
    char label[64];
    nyx_vec3_t position;
    float activation_level;
    float pulse_phase;
    uint32_t connected_nodes[8];
    size_t connection_count;
} nyx_synapse_node_t;

/* Neural Synapse Thought Graph */
typedef struct {
    nyx_synapse_node_t nodes[NYX_MAX_SYNAPSE_NODES];
    size_t node_count;
    float global_pulse;
    bool is_reasoning;
} nyx_synapse_graph_t;

/* Cybernetic HUD Widget */
typedef struct {
    uint32_t widget_id;
    char title[32];
    float value;
    float target_value;
    float min_val;
    float max_val;
    char unit[16];
    bool alert_status;
} nyx_hud_widget_t;

/* Master Spatial UI Context */
typedef struct {
    nyx_particle_swarm_t swarm;
    nyx_sdf_volume_t plasma_orb;
    nyx_synapse_graph_t synapse_graph;
    nyx_hud_widget_t widgets[NYX_MAX_HUD_WIDGETS];
    size_t widget_count;
    float frame_time_ms;
    float target_fps;
    bool is_initialized;
} nyx_spatial_context_t;

/* Spatial Subsystem API */
int nyx_spatial_init(nyx_spatial_context_t* ctx, size_t initial_particles);
int nyx_spatial_update_particles(nyx_spatial_context_t* ctx, float delta_time, float audio_energy);
int nyx_spatial_update_sdf(nyx_spatial_context_t* ctx, float delta_time, float speech_amplitude);
int nyx_spatial_add_synapse_node(nyx_spatial_context_t* ctx, const char* label, float x, float y, float z);
int nyx_spatial_connect_synapses(nyx_spatial_context_t* ctx, uint32_t src_id, uint32_t dst_id);
int nyx_spatial_update_widget(nyx_spatial_context_t* ctx, uint32_t widget_id, const char* title, float val, const char* unit);
float nyx_spatial_evaluate_sdf_distance(const nyx_sdf_volume_t* sdf, nyx_vec3_t point);

#ifdef __cplusplus
}
#endif

#endif /* NYX_SPATIAL_H */
