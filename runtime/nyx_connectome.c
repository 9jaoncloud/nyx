/*
 * Nyx Connectome & Spiking Neuromorphic Subsystem Runtime Implementation
 * Bio-inspired Whole-Brain Connectome Engine (Drosophila / Fly Brain 140K Architecture)
 */

#include "nyx_connectome.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

nyx_connectome_t* nyx_connectome_create(uint32_t num_neurons, uint32_t max_synapses) {
    if (num_neurons == 0) return NULL;

    nyx_connectome_t* brain = (nyx_connectome_t*)calloc(1, sizeof(nyx_connectome_t));
    if (!brain) return NULL;

    brain->num_neurons = num_neurons;
    brain->neurons = (nyx_lif_neuron_t*)calloc(num_neurons, sizeof(nyx_lif_neuron_t));
    brain->synapse_offsets = (uint32_t*)calloc(num_neurons + 1, sizeof(uint32_t));
    brain->synapses = (nyx_synapse_t*)calloc(max_synapses > 0 ? max_synapses : 1024, sizeof(nyx_synapse_t));

    for (uint32_t i = 0; i < num_neurons; i++) {
        brain->neurons[i].resting_v = -70.0f;
        brain->neurons[i].threshold_v = -50.0f;
        brain->neurons[i].voltage = -70.0f;
        brain->neurons[i].leak_factor = 0.92f;
        brain->neurons[i].refractory_timer = 0.0f;
        brain->neurons[i].spiked = false;
        brain->neurons[i].type = (uint8_t)(i % 6);
    }

    return brain;
}

void nyx_connectome_destroy(nyx_connectome_t* brain) {
    if (!brain) return;
    if (brain->neurons) free(brain->neurons);
    if (brain->synapse_offsets) free(brain->synapse_offsets);
    if (brain->synapses) free(brain->synapses);
    free(brain);
}

void nyx_connectome_add_synapse(nyx_connectome_t* brain, uint32_t pre_idx, uint32_t post_idx, float weight) {
    if (!brain || pre_idx >= brain->num_neurons || post_idx >= brain->num_neurons) return;

    uint32_t idx = brain->num_synapses;
    brain->synapses[idx].target_idx = post_idx;
    brain->synapses[idx].weight = weight;
    brain->synapses[idx].delay_ms = 1.0f;
    brain->num_synapses++;

    // Increment subsequent offsets in CSR format
    for (uint32_t i = pre_idx + 1; i <= brain->num_neurons; i++) {
        brain->synapse_offsets[i]++;
    }
}

void nyx_connectome_step(nyx_connectome_t* brain, const float* input_currents, float dt_ms) {
    if (!brain) return;
    brain->last_dt_ms = dt_ms;

    // Phase 1: Reset spikes & update membrane potentials with leak and external input
    for (uint32_t i = 0; i < brain->num_neurons; i++) {
        nyx_lif_neuron_t* n = &brain->neurons[i];
        n->spiked = false;

        if (n->refractory_timer > 0.0f) {
            n->refractory_timer -= dt_ms;
            n->voltage = n->resting_v;
            continue;
        }

        // Leaky integration
        float input_i = input_currents ? input_currents[i] : 0.0f;
        n->voltage = n->resting_v + (n->voltage - n->resting_v) * n->leak_factor + input_i;

        // Check threshold
        if (n->voltage >= n->threshold_v) {
            n->spiked = true;
            n->voltage = 20.0f; // Spike peak
            n->refractory_timer = 2.0f; // 2ms refractory period
            brain->total_spikes_fired++;
        }
    }

    // Phase 2: Propagate synaptic conductances from spiking pre-synaptic neurons
    for (uint32_t i = 0; i < brain->num_neurons; i++) {
        if (brain->neurons[i].spiked) {
            uint32_t start = brain->synapse_offsets[i];
            uint32_t end = brain->synapse_offsets[i + 1];

            for (uint32_t s = start; s < end; s++) {
                uint32_t target = brain->synapses[s].target_idx;
                float weight = brain->synapses[s].weight;
                if (target < brain->num_neurons) {
                    brain->neurons[target].voltage += weight;
                }
            }
        }
    }
}

uint32_t nyx_connectome_get_spikes(const nyx_connectome_t* brain, uint32_t* out_spiking_indices, uint32_t max_out) {
    if (!brain || !out_spiking_indices || max_out == 0) return 0;

    uint32_t count = 0;
    for (uint32_t i = 0; i < brain->num_neurons && count < max_out; i++) {
        if (brain->neurons[i].spiked) {
            out_spiking_indices[count++] = i;
        }
    }
    return count;
}

/* Optic Flow EMD Implementation */
nyx_optic_flow_emd_t* nyx_optic_flow_create(uint32_t width, uint32_t height) {
    if (width == 0 || height == 0) return NULL;
    nyx_optic_flow_emd_t* emd = (nyx_optic_flow_emd_t*)calloc(1, sizeof(nyx_optic_flow_emd_t));
    if (!emd) return NULL;

    emd->width = width;
    emd->height = height;
    uint32_t size = width * height;
    emd->prev_luminance = (float*)calloc(size, sizeof(float));
    emd->delayed_luminance = (float*)calloc(size, sizeof(float));
    return emd;
}

void nyx_optic_flow_destroy(nyx_optic_flow_emd_t* emd) {
    if (!emd) return;
    if (emd->prev_luminance) free(emd->prev_luminance);
    if (emd->delayed_luminance) free(emd->delayed_luminance);
    free(emd);
}

void nyx_optic_flow_process_frame(nyx_optic_flow_emd_t* emd, const float* current_frame, float dt_ms) {
    if (!emd || !current_frame) return;

    float vx_sum = 0.0f;
    float vy_sum = 0.0f;
    uint32_t w = emd->width;
    uint32_t h = emd->height;

    // Hassenstein-Reichardt Correlator across adjacent photoreceptor pixels
    for (uint32_t y = 1; y < h - 1; y++) {
        for (uint32_t x = 1; x < w - 1; x++) {
            uint32_t idx = y * w + x;
            uint32_t left_idx = y * w + (x - 1);
            uint32_t right_idx = y * w + (x + 1);
            uint32_t up_idx = (y - 1) * w + x;
            uint32_t down_idx = (y + 1) * w + x;

            // Horizontal motion: right-current * left-delayed - left-current * right-delayed
            float h_motion = (current_frame[right_idx] * emd->delayed_luminance[left_idx]) - 
                             (current_frame[left_idx] * emd->delayed_luminance[right_idx]);

            // Vertical motion
            float v_motion = (current_frame[down_idx] * emd->delayed_luminance[up_idx]) - 
                             (current_frame[up_idx] * emd->delayed_luminance[down_idx]);

            vx_sum += h_motion;
            vy_sum += v_motion;

            emd->delayed_luminance[idx] = emd->prev_luminance[idx];
            emd->prev_luminance[idx] = current_frame[idx];
        }
    }

    uint32_t count = (w - 2) * (h - 2);
    emd->horizontal_velocity = count > 0 ? (vx_sum / count) : 0.0f;
    emd->vertical_velocity = count > 0 ? (vy_sum / count) : 0.0f;
    emd->expansion_rate = fabsf(emd->horizontal_velocity) + fabsf(emd->vertical_velocity);
}

/* Mushroom Body Associative Learning */
nyx_mushroom_body_t* nyx_mushroom_body_create(uint32_t n_pn, uint32_t n_kc, uint32_t n_mbon) {
    nyx_mushroom_body_t* mb = (nyx_mushroom_body_t*)calloc(1, sizeof(nyx_mushroom_body_t));
    if (!mb) return NULL;

    mb->num_projection_neurons = n_pn;
    mb->num_kenyon_cells = n_kc;
    mb->num_output_neurons = n_mbon;

    mb->projection_weights = (float*)calloc(n_pn * n_kc, sizeof(float));
    mb->output_weights = (float*)calloc(n_kc * n_mbon, sizeof(float));

    // Initialize sparse random binary projections (each KC connects to ~6 PN inputs)
    for (uint32_t k = 0; k < n_kc; k++) {
        for (uint32_t c = 0; c < 6; c++) {
            uint32_t p = (uint32_t)(rand() % n_pn);
            mb->projection_weights[k * n_pn + p] = 1.0f;
        }
    }

    return mb;
}

void nyx_mushroom_body_destroy(nyx_mushroom_body_t* mb) {
    if (!mb) return;
    if (mb->projection_weights) free(mb->projection_weights);
    if (mb->output_weights) free(mb->output_weights);
    free(mb);
}

void nyx_mushroom_body_train_oneshot(nyx_mushroom_body_t* mb, const float* odor_vec, uint32_t reward_action, float lr) {
    if (!mb || !odor_vec || reward_action >= mb->num_output_neurons) return;

    // Compute sparse Kenyon Cell activations
    for (uint32_t k = 0; k < mb->num_kenyon_cells; k++) {
        float sum = 0.0f;
        for (uint32_t p = 0; p < mb->num_projection_neurons; p++) {
            sum += odor_vec[p] * mb->projection_weights[k * mb->num_projection_neurons + p];
        }

        // Top-k winner-take-all sparsity (Kenyon cells only fire if threshold exceeded)
        if (sum > 2.5f) {
            // Dopamine synaptic depression/potentiation rule
            mb->output_weights[k * mb->num_output_neurons + reward_action] += lr;
        }
    }
}

uint32_t nyx_mushroom_body_predict(const nyx_mushroom_body_t* mb, const float* odor_vec) {
    if (!mb || !odor_vec) return 0;

    float* action_scores = (float*)calloc(mb->num_output_neurons, sizeof(float));
    for (uint32_t k = 0; k < mb->num_kenyon_cells; k++) {
        float sum = 0.0f;
        for (uint32_t p = 0; p < mb->num_projection_neurons; p++) {
            sum += odor_vec[p] * mb->projection_weights[k * mb->num_projection_neurons + p];
        }

        if (sum > 2.5f) {
            for (uint32_t a = 0; a < mb->num_output_neurons; a++) {
                action_scores[a] += mb->output_weights[k * mb->num_output_neurons + a];
            }
        }
    }

    uint32_t best_action = 0;
    float best_score = -1e9f;
    for (uint32_t a = 0; a < mb->num_output_neurons; a++) {
        if (action_scores[a] > best_score) {
            best_score = action_scores[a];
            best_action = a;
        }
    }

    free(action_scores);
    return best_action;
}

/* Pong / Game Agent using Fly Connectome */
nyx_flybrain_game_agent_t* nyx_flybrain_agent_create(void) {
    nyx_flybrain_game_agent_t* agent = (nyx_flybrain_game_agent_t*)calloc(1, sizeof(nyx_flybrain_game_agent_t));
    if (!agent) return NULL;

    agent->brain = nyx_connectome_create(128, 512);
    agent->vision = nyx_optic_flow_create(16, 16);
    agent->paddle_y = 200.0f;
    agent->paddle_velocity = 0.0f;
    agent->target_confidence = 1.0f;

    // Connect sensory visual neurons to motor neurons
    for (uint32_t i = 0; i < 32; i++) {
        nyx_connectome_add_synapse(agent->brain, i, 100, 15.0f); // Up motor neuron
        nyx_connectome_add_synapse(agent->brain, 32 + i, 101, 15.0f); // Down motor neuron
    }

    return agent;
}

void nyx_flybrain_agent_destroy(nyx_flybrain_game_agent_t* agent) {
    if (!agent) return;
    if (agent->brain) nyx_connectome_destroy(agent->brain);
    if (agent->vision) nyx_optic_flow_destroy(agent->vision);
    free(agent);
}

float nyx_flybrain_agent_update(nyx_flybrain_game_agent_t* agent, float ball_x, float ball_y, float ball_vx, float ball_vy, float dt_ms) {
    if (!agent) return 0.0f;

    float inputs[128] = {0};
    
    // Optic flow tracking: if ball is higher than paddle, fire upper sensory bank
    if (ball_y < agent->paddle_y - 10.0f) {
        for (int i = 0; i < 32; i++) inputs[i] = 25.0f; // Excite UP
    } else if (ball_y > agent->paddle_y + 10.0f) {
        for (int i = 32; i < 64; i++) inputs[i] = 25.0f; // Excite DOWN
    }

    // Step spiking connectome
    nyx_connectome_step(agent->brain, inputs, dt_ms);

    // Read motor neurons 100 (UP) and 101 (DOWN)
    if (agent->brain->neurons[100].spiked) {
        agent->paddle_velocity = -350.0f; // Move Up
    } else if (agent->brain->neurons[101].spiked) {
        agent->paddle_velocity = 350.0f; // Move Down
    } else {
        agent->paddle_velocity *= 0.85f; // Friction damping
    }

    agent->paddle_y += agent->paddle_velocity * (dt_ms / 1000.0f);
    if (agent->paddle_y < 40.0f) agent->paddle_y = 40.0f;
    if (agent->paddle_y > 440.0f) agent->paddle_y = 440.0f;

    return agent->paddle_y;
}
