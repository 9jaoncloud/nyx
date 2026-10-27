/**
 * @file nyx_spatial.c
 * @brief Implementation of Nyx Spatial UI, Raymarched SDFs & Particle Subsystem
 * @author Simeon Bala (9jaoncloud@gmail.com)
 * @copyright (c) 2026 9jaonCloud Engineering. All Rights Reserved.
 */

#include "nyx_spatial.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int nyx_spatial_init(nyx_spatial_context_t* ctx, size_t initial_particles) {
    if (!ctx) return -1;
    memset(ctx, 0, sizeof(nyx_spatial_context_t));

    if (initial_particles > NYX_MAX_PARTICLES) {
        initial_particles = NYX_MAX_PARTICLES;
    }

    ctx->swarm.active_count = initial_particles;
    ctx->swarm.vortex_strength = 1.25f;
    ctx->swarm.audio_reactivity = 2.0f;
    ctx->swarm.expansion_rate = 0.5f;

    /* Initialize particle swarm in a spherical Fibonacci spiral */
    for (size_t i = 0; i < initial_particles; i++) {
        float phi = acosf(1.0f - 2.0f * ((float)i + 0.5f) / (float)initial_particles);
        float theta = (float)M_PI * (1.0f + sqrtf(5.0f)) * (float)i;
        float r = 1.5f + ((float)(rand() % 100) / 100.0f) * 0.5f;

        ctx->swarm.particles[i].position.x = r * sinf(phi) * cosf(theta);
        ctx->swarm.particles[i].position.y = r * sinf(phi) * sinf(theta);
        ctx->swarm.particles[i].position.z = r * cosf(phi);

        ctx->swarm.particles[i].velocity.x = -sinf(theta) * 0.4f;
        ctx->swarm.particles[i].velocity.y = cosf(theta) * 0.4f;
        ctx->swarm.particles[i].velocity.z = 0.1f * sinf(phi);

        ctx->swarm.particles[i].life = 1.0f;
        ctx->swarm.particles[i].size = 2.5f;
        ctx->swarm.particles[i].color.r = 0.22f; // Cyan/Purple primary
        ctx->swarm.particles[i].color.g = 0.74f;
        ctx->swarm.particles[i].color.b = 0.97f;
        ctx->swarm.particles[i].color.a = 0.85f;
        ctx->swarm.particles[i].energy = 1.0f;
    }

    /* Initialize central plasma SDF volume */
    ctx->plasma_orb.type = NYX_SDF_PULSAR;
    ctx->plasma_orb.center.x = 0.0f;
    ctx->plasma_orb.center.y = 0.0f;
    ctx->plasma_orb.center.z = 0.0f;
    ctx->plasma_orb.radius = 1.2f;
    ctx->plasma_orb.morph_factor = 0.15f;
    ctx->plasma_orb.pulse_frequency = 1.8f;
    ctx->plasma_orb.glow_intensity = 2.4f;
    ctx->plasma_orb.primary_color = (nyx_color_t){0.02f, 0.71f, 0.98f, 1.0f}; // Neon Cyan
    ctx->plasma_orb.secondary_color = (nyx_color_t){0.55f, 0.36f, 0.96f, 1.0f}; // Violet

    ctx->target_fps = 120.0f;
    ctx->frame_time_ms = 8.33f;
    ctx->is_initialized = true;

    return 0;
}

int nyx_spatial_update_particles(nyx_spatial_context_t* ctx, float delta_time, float audio_energy) {
    if (!ctx || !ctx->is_initialized) return -1;

    float speed_scale = 1.0f + audio_energy * ctx->swarm.audio_reactivity;

    for (size_t i = 0; i < ctx->swarm.active_count; i++) {
        nyx_particle_t* p = &ctx->swarm.particles[i];

        /* Tangential vortex acceleration */
        float dist_sq = p->position.x * p->position.x + p->position.y * p->position.y + p->position.z * p->position.z;
        float dist = sqrtf(dist_sq + 0.001f);

        /* Orbital velocity */
        p->position.x += (p->velocity.x * speed_scale - p->position.y * ctx->swarm.vortex_strength) * delta_time;
        p->position.y += (p->velocity.y * speed_scale + p->position.x * ctx->swarm.vortex_strength) * delta_time;
        p->position.z += p->velocity.z * speed_scale * delta_time;

        /* Inward pull toward equilibrium radius (r = 1.8) */
        float target_r = 1.8f + audio_energy * 0.8f;
        float pull = (target_r - dist) * 0.8f;
        p->position.x += (p->position.x / dist) * pull * delta_time;
        p->position.y += (p->position.y / dist) * pull * delta_time;
        p->position.z += (p->position.z / dist) * pull * delta_time;

        /* Particle energy modulated by audio */
        p->energy = 0.5f + audio_energy * 2.0f;
    }

    return 0;
}

int nyx_spatial_update_sdf(nyx_spatial_context_t* ctx, float delta_time, float speech_amplitude) {
    if (!ctx || !ctx->is_initialized) return -1;

    ctx->plasma_orb.morph_factor = 0.15f + speech_amplitude * 0.45f;
    ctx->plasma_orb.glow_intensity = 2.4f + speech_amplitude * 3.5f;

    return 0;
}

int nyx_spatial_add_synapse_node(nyx_spatial_context_t* ctx, const char* label, float x, float y, float z) {
    if (!ctx || !ctx->is_initialized) return -1;
    if (ctx->synapse_graph.node_count >= NYX_MAX_SYNAPSE_NODES) return -2;

    uint32_t id = (uint32_t)ctx->synapse_graph.node_count;
    nyx_synapse_node_t* node = &ctx->synapse_graph.nodes[id];
    node->node_id = id;
    if (label) {
        strncpy(node->label, label, sizeof(node->label) - 1);
        node->label[sizeof(node->label) - 1] = '\0';
    }
    node->position.x = x;
    node->position.y = y;
    node->position.z = z;
    node->activation_level = 1.0f;
    node->pulse_phase = 0.0f;
    node->connection_count = 0;

    ctx->synapse_graph.node_count++;
    return (int)id;
}

int nyx_spatial_connect_synapses(nyx_spatial_context_t* ctx, uint32_t src_id, uint32_t dst_id) {
    if (!ctx || !ctx->is_initialized) return -1;
    if (src_id >= ctx->synapse_graph.node_count || dst_id >= ctx->synapse_graph.node_count) return -2;

    nyx_synapse_node_t* src = &ctx->synapse_graph.nodes[src_id];
    if (src->connection_count < 8) {
        src->connected_nodes[src->connection_count++] = dst_id;
        return 0;
    }
    return -3;
}

int nyx_spatial_update_widget(nyx_spatial_context_t* ctx, uint32_t widget_id, const char* title, float val, const char* unit) {
    if (!ctx || !ctx->is_initialized) return -1;
    if (widget_id >= NYX_MAX_HUD_WIDGETS) return -2;

    nyx_hud_widget_t* w = &ctx->widgets[widget_id];
    w->widget_id = widget_id;
    if (title) {
        strncpy(w->title, title, sizeof(w->title) - 1);
        w->title[sizeof(w->title) - 1] = '\0';
    }
    w->value = val;
    if (unit) {
        strncpy(w->unit, unit, sizeof(w->unit) - 1);
        w->unit[sizeof(w->unit) - 1] = '\0';
    }
    if (widget_id >= ctx->widget_count) {
        ctx->widget_count = widget_id + 1;
    }
    return 0;
}

float nyx_spatial_evaluate_sdf_distance(const nyx_sdf_volume_t* sdf, nyx_vec3_t point) {
    if (!sdf) return 1000.0f;

    float dx = point.x - sdf->center.x;
    float dy = point.y - sdf->center.y;
    float dz = point.z - sdf->center.z;
    float len = sqrtf(dx * dx + dy * dy + dz * dz);

    switch (sdf->type) {
        case NYX_SDF_SPHERE:
            return len - sdf->radius;
        case NYX_SDF_PULSAR:
        case NYX_SDF_GYROID:
        default: {
            /* Gyroid volumetric displacement */
            float displacement = sinf(dx * 3.0f) * cosf(dy * 3.0f) * sinf(dz * 3.0f) * sdf->morph_factor;
            return (len - sdf->radius) + displacement;
        }
    }
}
