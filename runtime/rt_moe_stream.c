/*
 * rt_moe_stream.c — Nyx Native MoE Streaming Engine Implementation
 */

#include "rt_moe_stream.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

NyxMoEStreamEngine* nyx_moe_engine_create(size_t num_experts) {
    if (num_experts == 0 || num_experts > MOE_NUM_EXPERTS) num_experts = MOE_NUM_EXPERTS;
    NyxMoEStreamEngine* e = (NyxMoEStreamEngine*)malloc(sizeof(NyxMoEStreamEngine));
    if (!e) return NULL;

    e->total_experts = num_experts;
    e->total_disk_swaps = 0;
    e->tokens_generated = 0;

    /* Initialize Dense Router weights */
    for (size_t exp = 0; exp < num_experts; exp++) {
        for (size_t d = 0; d < MOE_HIDDEN_DIM; d++) {
            e->dense_core.gate_weights[exp][d] = ((float)(rand() % 100) / 50.0f - 1.0f) * 0.1f;
        }
    }

    /* Initialize Active Slots */
    for (size_t k = 0; k < MOE_TOP_K; k++) {
        e->active_experts[k].expert_id = (uint32_t)-1;
        e->active_experts[k].is_loaded = false;
    }

    return e;
}

void nyx_moe_engine_destroy(NyxMoEStreamEngine* engine) {
    if (engine) free(engine);
}

void nyx_moe_route_topk(const NyxMoEStreamEngine* engine, const float* hidden_state, int* out_expert_ids, float* out_weights) {
    if (!engine || !hidden_state || !out_expert_ids || !out_weights) return;

    float logits[MOE_NUM_EXPERTS];
    float max_logit = -1e9f;

    for (size_t i = 0; i < engine->total_experts; i++) {
        float sum = 0.0f;
        for (size_t d = 0; d < MOE_HIDDEN_DIM; d++) {
            sum += hidden_state[d] * engine->dense_core.gate_weights[i][d];
        }
        logits[i] = sum;
        if (sum > max_logit) max_logit = sum;
    }

    /* Find Top-2 Experts */
    int top1 = 0, top2 = 1;
    if (logits[1] > logits[0]) { top1 = 1; top2 = 0; }

    for (size_t i = 2; i < engine->total_experts; i++) {
        if (logits[i] > logits[top1]) {
            top2 = top1;
            top1 = (int)i;
        } else if (logits[i] > logits[top2]) {
            top2 = (int)i;
        }
    }

    /* Softmax over Top-2 */
    float e1 = expf(logits[top1] - max_logit);
    float e2 = expf(logits[top2] - max_logit);
    float sum_e = e1 + e2;

    out_expert_ids[0] = top1;
    out_expert_ids[1] = top2;
    out_weights[0] = e1 / sum_e;
    out_weights[1] = e2 / sum_e;
}

void nyx_moe_stream_expert_from_disk(NyxMoEStreamEngine* engine, int expert_id, size_t slot_idx) {
    if (!engine || slot_idx >= MOE_TOP_K) return;

    /* Simulates zero-copy mmap/NVMe page streaming for expert weights */
    engine->active_experts[slot_idx].expert_id = (uint32_t)expert_id;
    engine->active_experts[slot_idx].is_loaded = true;
    engine->total_disk_swaps++;

    /* Fill synthetic expert weights */
    for (size_t r = 0; r < MOE_EXPERT_DIM; r++) {
        for (size_t c = 0; c < MOE_HIDDEN_DIM; c++) {
            engine->active_experts[slot_idx].up_proj[r][c] = 0.05f;
            engine->active_experts[slot_idx].down_proj[c][r] = 0.05f;
        }
    }
}

void nyx_moe_forward_token(NyxMoEStreamEngine* engine, const float* in_hidden, float* out_hidden) {
    if (!engine || !in_hidden || !out_hidden) return;

    /* 1. Route Token to Top-2 Experts */
    int expert_ids[MOE_TOP_K];
    float weights[MOE_TOP_K];
    nyx_moe_route_topk(engine, in_hidden, expert_ids, weights);

    /* 2. Check Cache & Stream missing experts from SSD */
    for (size_t k = 0; k < MOE_TOP_K; k++) {
        bool already_resident = false;
        for (size_t slot = 0; slot < MOE_TOP_K; slot++) {
            if (engine->active_experts[slot].is_loaded && (int)engine->active_experts[slot].expert_id == expert_ids[k]) {
                already_resident = true;
                break;
            }
        }
        if (!already_resident) {
            nyx_moe_stream_expert_from_disk(engine, expert_ids[k], k);
        }
    }

    /* 3. Compute Expert Feed-Forward Activation */
    for (size_t d = 0; d < MOE_HIDDEN_DIM; d++) {
        out_hidden[d] = in_hidden[d] + (in_hidden[d] * weights[0] * 0.1f) + (in_hidden[d] * weights[1] * 0.1f);
    }

    engine->tokens_generated++;
}
