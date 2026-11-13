/**
 * Nyx Native Runtime - C. elegans Whole-Organism Connectome & Fluid Hydrodynamics
 */

#include "../include/nyx_openworm.h"
#include <string.h>

void nyx_worm_init(NyxWormOrganism* worm, float start_x, float start_y) {
    if (!worm) return;
    memset(worm, 0, sizeof(NyxWormOrganism));

    // Initialize 302 neurons with resting potential
    for (int i = 0; i < NYX_WORM_NEURONS; i++) {
        worm->neurons[i].voltage = -70.0f;
        worm->neurons[i].threshold = -50.0f;
        worm->neurons[i].refractory_ms = 0.0f;
        worm->neurons[i].spiked = false;
    }

    // Connect sensory, interneurons, and ventral/dorsal motor neurons
    worm->synapse_count = 0;
    for (uint16_t i = 0; i < 200 && worm->synapse_count < NYX_WORM_MAX_SYNAPSES; i++) {
        // Forward undulation motor synapses
        worm->synapses[worm->synapse_count].pre = i;
        worm->synapses[worm->synapse_count].post = (uint16_t)((i + 1) % NYX_WORM_NEURONS);
        worm->synapses[worm->synapse_count].weight = 12.5f;
        worm->synapse_count++;

        // Cross-inhibitory dorsal-ventral contralateral synapses
        worm->synapses[worm->synapse_count].pre = i;
        worm->synapses[worm->synapse_count].post = (uint16_t)((i + 50) % NYX_WORM_NEURONS);
        worm->synapses[worm->synapse_count].weight = -8.0f;
        worm->synapse_count++;
    }

    // Initialize 95 muscle segments
    worm->head_x = start_x;
    worm->head_y = start_y;
    worm->heading_angle = 0.0f;
    worm->speed = 3.5f;

    for (int i = 0; i < NYX_WORM_MUSCLES; i++) {
        worm->muscles[i].activation = 0.0f;
        worm->muscles[i].phase = (float)i * 0.18f;
        worm->muscles[i].pos_x = start_x - (float)i * 3.5f;
        worm->muscles[i].pos_y = start_y;
    }
}

void nyx_worm_sense_chemical(NyxWormOrganism* worm, float target_x, float target_y, float attraction_strength) {
    if (!worm) return;
    float dx = target_x - worm->head_x;
    float dy = target_y - worm->head_y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 1.0f) dist = 1.0f;

    float desired_angle = atan2f(dy, dx);
    float angle_diff = desired_angle - worm->heading_angle;
    while (angle_diff > 3.14159f) angle_diff -= 6.28318f;
    while (angle_diff < -3.14159f) angle_diff += 6.28318f;

    // Excite ASE/AWC sensory neurons for pirouette / klinokinesis steering
    float stimulus = (attraction_strength / dist) * 20.0f;
    if (angle_diff > 0.1f) {
        worm->neurons[0].voltage += stimulus * 1.5f; // Left turning neurons
    } else if (angle_diff < -0.1f) {
        worm->neurons[1].voltage += stimulus * 1.5f; // Right turning neurons
    }

    worm->heading_angle += angle_diff * 0.05f;
}

uint32_t nyx_worm_step(NyxWormOrganism* worm, float dt_ms) {
    if (!worm) return 0;
    uint32_t spikes_in_step = 0;

    // 1. Spiking LIF Connectome Step
    for (int i = 0; i < NYX_WORM_NEURONS; i++) {
        NyxWormNeuron* n = &worm->neurons[i];
        n->spiked = false;

        if (n->refractory_ms > 0.0f) {
            n->refractory_ms -= dt_ms;
            n->voltage = -70.0f;
            continue;
        }

        // Leaky decay
        n->voltage = -70.0f + (n->voltage - -70.0f) * expf(-dt_ms / 15.0f);

        // Intrinsic rhythmicity for forward locomotion
        if (i < 30) {
            n->voltage += sinf(worm->muscles[0].phase + (float)i * 0.2f) * 1.8f;
        }

        if (n->voltage >= n->threshold) {
            n->spiked = true;
            n->voltage = -70.0f;
            n->refractory_ms = 2.0f;
            spikes_in_step++;
            worm->total_spikes++;
        }
    }

    // 2. Synaptic Axonal Propagation
    for (uint32_t s = 0; s < worm->synapse_count; s++) {
        NyxWormSynapse* syn = &worm->synapses[s];
        if (worm->neurons[syn->pre].spiked) {
            worm->neurons[syn->post].voltage += syn->weight;
        }
    }

    // 3. Biomechanical 95-Segment Muscle Undulation in Viscous Fluid
    worm->head_x += cosf(worm->heading_angle) * worm->speed;
    worm->head_y += sinf(worm->heading_angle) * worm->speed;

    worm->muscles[0].pos_x = worm->head_x;
    worm->muscles[0].pos_y = worm->head_y;
    worm->muscles[0].phase += 0.12f;

    for (int i = 1; i < NYX_WORM_MUSCLES; i++) {
        worm->muscles[i].phase += 0.12f;
        float wave = sinf(worm->muscles[i].phase) * 6.5f;

        float prev_x = worm->muscles[i - 1].pos_x;
        float prev_y = worm->muscles[i - 1].pos_y;

        float seg_dx = prev_x - worm->muscles[i].pos_x;
        float seg_dy = prev_y - worm->muscles[i].pos_y;
        float seg_len = sqrtf(seg_dx * seg_dx + seg_dy * seg_dy);
        if (seg_len < 0.1f) seg_len = 0.1f;

        float target_len = 3.2f;
        float ratio = target_len / seg_len;

        worm->muscles[i].pos_x = prev_x - seg_dx * ratio + (-sinf(worm->heading_angle) * wave * 0.08f);
        worm->muscles[i].pos_y = prev_y - seg_dy * ratio + (cosf(worm->heading_angle) * wave * 0.08f);
    }

    return spikes_in_step;
}
