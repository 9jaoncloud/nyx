/**
 * Nyx Sovereign Bio-Neuromorphic Cognitive Architecture Runtime
 * Mathematical Implementation of the 6 Biological Brain Faculties in Zero-GC Region Memory.
 * 
 * Author: Simeon Bala (9jaoncloud@gmail.com)
 */

#include "../include/nyx_bio_cortex.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* =========================================================================
   1. 6-LAYER NEOCORTICAL COLUMN (HTM / PREDICTIVE CODING)
   ========================================================================= */
NyxNeocortexColumn* nyx_neocortex_create(uint32_t num_columns) {
    NyxNeocortexColumn* col = (NyxNeocortexColumn*)malloc(sizeof(NyxNeocortexColumn));
    if (!col) return NULL;
    memset(col, 0, sizeof(NyxNeocortexColumn));
    col->columns_count = num_columns > 0 ? num_columns : NYX_SDR_SIZE;
    col->anomaly_score = 0.0f;
    col->free_energy_error = 0.0f;
    return col;
}

void nyx_neocortex_step(NyxNeocortexColumn* col, const float* feedforward_input, size_t input_len) {
    if (!col || !feedforward_input || input_len == 0) return;

    // 1. Layer IV Feedforward Ingestion: Map continuous input into 2048-bit SDR
    memset(&col->active_sdr, 0, sizeof(NyxSDR));
    uint32_t active_count = 0;

    for (size_t i = 0; i < input_len && active_count < NYX_SDR_ACTIVE_BITS; i++) {
        uint32_t idx = ((uint32_t)(fabsf(feedforward_input[i]) * 100000.0f) + (uint32_t)i * 37) % NYX_SDR_SIZE;
        uint32_t word = idx / 64;
        uint64_t bit = (1ULL << (idx % 64));

        if (!(col->active_sdr.bitset[word] & bit)) {
            col->active_sdr.bitset[word] |= bit;
            col->active_sdr.active_indices[active_count++] = idx;
        }
    }
    col->active_sdr.active_count = active_count;

    // 2. Compute overlap between predicted SDR (from Layer VI) and actual active SDR (Layer IV)
    uint32_t overlap = 0;
    for (uint32_t w = 0; w < (NYX_SDR_SIZE / 64); w++) {
        uint64_t match = col->active_sdr.bitset[w] & col->predicted_sdr.bitset[w];
        while (match) {
            overlap += (match & 1);
            match >>= 1;
        }
    }

    // 3. Friston Free Energy Anomaly Score
    if (active_count > 0) {
        col->anomaly_score = 1.0f - ((float)overlap / (float)active_count);
    } else {
        col->anomaly_score = 0.0f;
    }
    col->free_energy_error = col->anomaly_score * 4.2f;

    // 4. Layer VI Generative Prediction for Next Step (temporal transition)
    memset(&col->predicted_sdr, 0, sizeof(NyxSDR));
    for (uint32_t k = 0; k < active_count; k++) {
        uint32_t next_idx = (col->active_sdr.active_indices[k] + 7) % NYX_SDR_SIZE;
        col->predicted_sdr.bitset[next_idx / 64] |= (1ULL << (next_idx % 64));
    }
}

float nyx_neocortex_compute_anomaly(const NyxNeocortexColumn* col) {
    return col ? col->anomaly_score : 0.0f;
}

void nyx_neocortex_destroy(NyxNeocortexColumn* col) {
    if (col) free(col);
}

/* =========================================================================
   2. HIPPOCAMPUS & ENTORHINAL GRID-CELL BIO-SLAM
   ========================================================================= */
NyxHippoNavState* nyx_hipponav_create(void) {
    NyxHippoNavState* nav = (NyxHippoNavState*)malloc(sizeof(NyxHippoNavState));
    if (!nav) return NULL;
    memset(nav, 0, sizeof(NyxHippoNavState));
    nav->confidence = 1.0f;
    return nav;
}

void nyx_hipponav_update_odometry(NyxHippoNavState* nav, float vx, float vy, float vz, float v_theta, float dt) {
    if (!nav) return;

    // 1. Update Spatial Coordinates
    nav->heading_rad += v_theta * dt;
    nav->x += (vx * cosf(nav->heading_rad) - vy * sinf(nav->heading_rad)) * dt;
    nav->y += (vx * sinf(nav->heading_rad) + vy * cosf(nav->heading_rad)) * dt;
    nav->z += vz * dt;

    // 2. Theta Phase Precession (8Hz oscillatory phase)
    nav->theta_phase_rad = fmodf(nav->theta_phase_rad + 2.0f * (float)M_PI * 8.0f * dt, 2.0f * (float)M_PI);

    // 3. Medial Entorhinal Hexagonal Grid Cell Firing
    // g_i(x,y) = (2/3) * ( 1/3 * sum(cos(k_j . x)) + 1/2 )
    for (int mod = 0; mod < NYX_GRID_MODULES; mod++) {
        float scale = 0.5f * (float)(mod + 1); // Multiple spatial wavelength scales
        for (int c = 0; c < NYX_GRID_CELLS_PER_MODULE; c++) {
            float phase_offset = (float)c * (2.0f * (float)M_PI / (float)NYX_GRID_CELLS_PER_MODULE);
            float sum_cos = 0.0f;
            for (int j = 0; j < 3; j++) {
                float theta_j = (float)j * (float)M_PI / 3.0f; // 60-degree symmetry
                float kx = cosf(theta_j) / scale;
                float ky = sinf(theta_j) / scale;
                sum_cos += cosf(kx * nav->x + ky * nav->y + phase_offset);
            }
            float grid_val = (2.0f / 3.0f) * ((sum_cos / 3.0f) + 0.5f);
            if (grid_val < 0.0f) grid_val = 0.0f;
            nav->grid_activations[mod * NYX_GRID_CELLS_PER_MODULE + c] = grid_val;
        }
    }

    // 4. CA1/CA3 Gaussian Place Field Population
    float max_place_val = 0.0f;
    uint32_t best_id = 0;
    for (int p = 0; p < NYX_PLACE_CELLS; p++) {
        float cx = ((float)(p % 16) - 8.0f) * 2.0f;
        float cy = ((float)(p / 16) - 8.0f) * 2.0f;
        float dist_sq = (nav->x - cx) * (nav->x - cx) + (nav->y - cy) * (nav->y - cy);
        float p_act = expf(-dist_sq / 4.0f);
        nav->place_activations[p] = p_act;
        if (p_act > max_place_val) {
            max_place_val = p_act;
            best_id = (uint32_t)p;
        }
    }
    nav->best_place_id = best_id;
    nav->confidence = max_place_val > 0.1f ? max_place_val : 0.1f;
}

void nyx_hipponav_trigger_swr_replay(NyxHippoNavState* nav, float* out_trajectory, size_t max_steps) {
    if (!nav || !out_trajectory || max_steps == 0) return;
    nav->is_sharp_wave_ripple = true;
    for (size_t i = 0; i < max_steps; i++) {
        float step_ratio = (float)i / (float)max_steps;
        out_trajectory[i * 2 + 0] = nav->x * (1.0f - step_ratio);
        out_trajectory[i * 2 + 1] = nav->y * (1.0f - step_ratio);
    }
}

void nyx_hipponav_destroy(NyxHippoNavState* nav) {
    if (nav) free(nav);
}

/* =========================================================================
   3. 5-LAYER NEUROMORPHIC SILICON RETINA (EVENT-STREAM DVS)
   ========================================================================= */
NyxNeuromorphicRetina* nyx_retina_create(float threshold) {
    NyxNeuromorphicRetina* ret = (NyxNeuromorphicRetina*)malloc(sizeof(NyxNeuromorphicRetina));
    if (!ret) return NULL;
    memset(ret, 0, sizeof(NyxNeuromorphicRetina));
    ret->threshold = threshold > 0.01f ? threshold : 0.15f;
    return ret;
}

uint32_t nyx_retina_ingest_photons(NyxNeuromorphicRetina* ret, const float* light_frame, NyxDVSEvent* out_events, uint32_t max_events) {
    if (!ret || !light_frame || !out_events || max_events == 0) return 0;

    uint32_t event_count = 0;
    ret->current_time_us += 100; // Step time

    for (uint16_t y = 0; y < NYX_RETINA_HEIGHT; y++) {
        for (uint16_t x = 0; x < NYX_RETINA_WIDTH; x++) {
            uint32_t idx = y * NYX_RETINA_WIDTH + x;
            float raw_val = light_frame[idx] > 0.0001f ? light_frame[idx] : 0.0001f;
            float new_log_i = logf(raw_val);
            float diff = new_log_i - ret->log_intensity[idx];

            if (fabsf(diff) >= ret->threshold && event_count < max_events) {
                out_events[event_count].x = x;
                out_events[event_count].y = y;
                out_events[event_count].polarity = (diff > 0.0f) ? 1 : -1;
                out_events[event_count].timestamp_us = ret->current_time_us;
                event_count++;
                ret->log_intensity[idx] = new_log_i;
            }
        }
    }
    ret->total_events_generated += event_count;
    return event_count;
}

void nyx_retina_compute_optical_flow(NyxNeuromorphicRetina* ret, const NyxDVSEvent* events, uint32_t num_events) {
    if (!ret || !events || num_events == 0) return;

    float sum_vx = 0.0f, sum_vy = 0.0f;
    for (uint32_t i = 0; i < num_events; i++) {
        sum_vx += (float)events[i].polarity * ((float)events[i].x - (float)NYX_RETINA_WIDTH / 2.0f);
        sum_vy += (float)events[i].polarity * ((float)events[i].y - (float)NYX_RETINA_HEIGHT / 2.0f);
    }
    ret->optical_flow_vx = sum_vx / (float)num_events * 0.05f;
    ret->optical_flow_vy = sum_vy / (float)num_events * 0.05f;
}

void nyx_retina_destroy(NyxNeuromorphicRetina* ret) {
    if (ret) free(ret);
}

/* =========================================================================
   4. TONOTOPIC SPIKING COCHLEA & AUDITORY CORTEX
   ========================================================================= */
NyxSpikingCochlea* nyx_cochlea_create(void) {
    NyxSpikingCochlea* coch = (NyxSpikingCochlea*)malloc(sizeof(NyxSpikingCochlea));
    if (!coch) return NULL;
    memset(coch, 0, sizeof(NyxSpikingCochlea));

    // Logarithmic frequency distribution from 100 Hz to 16 kHz
    for (int i = 0; i < NYX_COCHLEA_CHANNELS; i++) {
        float f_min = 100.0f;
        float f_max = 16000.0f;
        coch->center_freqs[i] = f_min * powf(f_max / f_min, (float)i / (float)(NYX_COCHLEA_CHANNELS - 1));
    }
    coch->speech_isolation_ratio = 0.85f;
    return coch;
}

void nyx_cochlea_process_binaural(NyxSpikingCochlea* coch, const float* left_audio, const float* right_audio, size_t num_samples, float sample_rate) {
    if (!coch || !left_audio || !right_audio || num_samples == 0 || sample_rate <= 0.0f) return;

    float left_power = 0.0f, right_power = 0.0f;
    float cross_corr_max = 0.0f;
    int best_lag = 0;
    int max_lag = (int)(sample_rate * 0.0007f); // max ITD ~700 microseconds for human head

    for (size_t i = 0; i < num_samples; i++) {
        left_power += left_audio[i] * left_audio[i];
        right_power += right_audio[i] * right_audio[i];
    }

    // Jeffress MSO Interaural Time Difference (ITD) Coincidence Cross-Correlation
    for (int lag = -max_lag; lag <= max_lag; lag++) {
        float corr = 0.0f;
        for (size_t i = 0; i < num_samples; i++) {
            int right_idx = (int)i + lag;
            if (right_idx >= 0 && right_idx < (int)num_samples) {
                corr += left_audio[i] * right_audio[right_idx];
            }
        }
        if (corr > cross_corr_max) {
            cross_corr_max = corr;
            best_lag = lag;
        }
    }

    // Map lag to azimuth angle (-90 deg to +90 deg)
    coch->estimated_azimuth_deg = ((float)best_lag / (float)max_lag) * 90.0f;
    if (coch->estimated_azimuth_deg > 90.0f) coch->estimated_azimuth_deg = 90.0f;
    if (coch->estimated_azimuth_deg < -90.0f) coch->estimated_azimuth_deg = -90.0f;

    // Update Tonotopic Spiking Activity
    for (int ch = 0; ch < NYX_COCHLEA_CHANNELS; ch++) {
        coch->spikes_generated[ch] = (uint32_t)((left_power + right_power) * 100.0f * (float)(ch + 1));
    }
}

void nyx_cochlea_destroy(NyxSpikingCochlea* coch) {
    if (coch) free(coch);
}

/* =========================================================================
   5. ROBOTIC SPIKING CEREBELLUM
   ========================================================================= */
NyxCerebellarController* nyx_cerebellum_create(void) {
    NyxCerebellarController* cer = (NyxCerebellarController*)malloc(sizeof(NyxCerebellarController));
    if (!cer) return NULL;
    memset(cer, 0, sizeof(NyxCerebellarController));

    for (int p = 0; p < NYX_PURKINJE_CELLS; p++) {
        for (int g = 0; g < NYX_GRANULE_CELLS; g++) {
            cer->purkinje_weights[p][g] = 0.5f;
        }
    }
    cer->damping_adaptation = 0.95f;
    return cer;
}

void nyx_cerebellum_step(NyxCerebellarController* cer, const float* desired_pos, const float* actual_pos, const float* actual_vel, float dt) {
    if (!cer || !desired_pos || !actual_pos || !actual_vel) return;

    // 1. Mossy Fiber -> Granule Layer Expansion
    for (int g = 0; g < NYX_GRANULE_CELLS; g++) {
        int joint = g % 6;
        float state_proj = (desired_pos[joint] - actual_pos[joint]) * 2.0f - actual_vel[joint] * 0.1f;
        cer->granule_potentials[g] = (state_proj > 0.0f) ? state_proj : 0.0f;
    }

    // 2. Climbing Fiber Error & Purkinje Plasticity (LTD on error)
    for (int p = 0; p < NYX_PURKINJE_CELLS; p++) {
        int joint = p % 6;
        float error = desired_pos[joint] - actual_pos[joint];
        cer->climbing_fiber_error[p] = error;

        // Purkinje Output
        float purk_out = 0.0f;
        for (int g = 0; g < 16; g++) {
            int g_idx = p * 16 + g;
            if (g_idx < NYX_GRANULE_CELLS) {
                purk_out += cer->granule_potentials[g_idx] * cer->purkinje_weights[p][g_idx];
                // Climbing Fiber LTD plasticity update
                cer->purkinje_weights[p][g_idx] -= 0.01f * error * cer->granule_potentials[g_idx];
                if (cer->purkinje_weights[p][g_idx] < 0.0f) cer->purkinje_weights[p][g_idx] = 0.0f;
                if (cer->purkinje_weights[p][g_idx] > 2.0f) cer->purkinje_weights[p][g_idx] = 2.0f;
            }
        }

        // 3. Deep Cerebellar Nuclei (DCN) Corrective Torque
        cer->torque_output[joint] = (error * 15.0f) - (actual_vel[joint] * 2.5f) + (purk_out * 0.5f);
    }
}

void nyx_cerebellum_destroy(NyxCerebellarController* cer) {
    if (cer) free(cer);
}

/* =========================================================================
   6. MOTOR CORTEX INTRACORTICAL SPIKING BCI
   ========================================================================= */
NyxBCIDecoder* nyx_bci_create(void) {
    NyxBCIDecoder* bci = (NyxBCIDecoder*)malloc(sizeof(NyxBCIDecoder));
    if (!bci) return NULL;
    memset(bci, 0, sizeof(NyxBCIDecoder));
    bci->intent_confidence = 0.94f;
    return bci;
}

void nyx_bci_decode_spikes(NyxBCIDecoder* bci, const uint32_t* spike_counts, float dt) {
    if (!bci || !spike_counts || dt <= 0.0f) return;

    float sum_vx = 0.0f, sum_vy = 0.0f, sum_vz = 0.0f, sum_grip = 0.0f;

    // Population Vector Algorithm (Georgopoulos Model)
    for (int ch = 0; ch < NYX_BCI_CHANNELS; ch++) {
        float rate = (float)spike_counts[ch] / dt;
        bci->channel_spike_rates[ch] = rate;

        // Preferred directional tuning
        float theta = (float)ch * (2.0f * (float)M_PI / (float)NYX_BCI_CHANNELS);
        sum_vx += rate * cosf(theta);
        sum_vy += rate * sinf(theta);
        sum_vz += rate * cosf(theta * 2.0f);
        if (ch >= 48) sum_grip += rate;
    }

    bci->decoded_velocity_x = sum_vx * 0.002f;
    bci->decoded_velocity_y = sum_vy * 0.002f;
    bci->decoded_velocity_z = sum_vz * 0.0015f;
    bci->decoded_grip_force = (sum_grip > 0.0f) ? (sum_grip * 0.005f) : 0.0f;
    bci->intent_confidence = 0.88f + (fabsf(bci->decoded_velocity_x) + fabsf(bci->decoded_velocity_y)) * 0.05f;
    if (bci->intent_confidence > 0.99f) bci->intent_confidence = 0.99f;
}

void nyx_bci_destroy(NyxBCIDecoder* bci) {
    if (bci) free(bci);
}
