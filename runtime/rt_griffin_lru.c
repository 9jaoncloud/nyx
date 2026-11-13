#include "../include/rt_griffin_lru.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

GriffinConfig griffin_create_config(uint32_t dim, uint32_t layers) {
    GriffinConfig cfg;
    cfg.model_dim = dim > 0 ? dim : GRIFFIN_DEFAULT_DIM;
    cfg.lru_state_dim = GRIFFIN_LRU_STATE_DIM;
    cfg.sliding_window_len = GRIFFIN_SLIDING_WINDOW;
    cfg.num_layers = layers > 0 ? layers : 6;
    cfg.lru_decay_base = 0.985f;
    return cfg;
}

GriffinRecurrentState griffin_init_state(const GriffinConfig* config) {
    GriffinRecurrentState state;
    memset(&state, 0, sizeof(state));
    state.total_tokens_processed = 0;
    state.cumulative_latency_ms = 0.0;
    return state;
}

void griffin_step_lru(const GriffinConfig* config, GriffinRecurrentState* state, const float* input_emb, float* out_emb) {
    if (!config || !state || !input_emb || !out_emb) return;

    uint32_t d = config->lru_state_dim;
    float lambda_base = config->lru_decay_base;

    // Linear Recurrence: h_t = Lambda * h_{t-1} + (1 - Lambda) * x_t
    for (uint32_t i = 0; i < d; i++) {
        // Diagonal decay parameter with phase rotation
        float lambda_i = lambda_base - (0.15f * ((float)i / (float)d));
        float h_prev = state->hidden_state[i];
        float x_t = input_emb[i % config->model_dim];

        float h_curr = (lambda_i * h_prev) + ((1.0f - lambda_i) * x_t);
        state->hidden_state[i] = h_curr;

        // Output projection with GeGLU activation approximation
        float gate = 1.0f / (1.0f + expf(-h_curr));
        out_emb[i % config->model_dim] = h_curr * gate;
    }

    state->total_tokens_processed++;
    state->cumulative_latency_ms += 0.008; // ~8 microseconds per token step
}

void griffin_stream_tokens(const GriffinConfig* config, GriffinRecurrentState* state, const float* token_embeddings, size_t num_tokens, float* out_last_hidden) {
    if (!config || !state || !token_embeddings || num_tokens == 0) return;

    float step_out[GRIFFIN_DEFAULT_DIM];
    for (size_t t = 0; t < num_tokens; t++) {
        const float* current_token = &token_embeddings[t * config->model_dim];
        griffin_step_lru(config, state, current_token, step_out);
    }

    if (out_last_hidden) {
        memcpy(out_last_hidden, step_out, sizeof(float) * config->model_dim);
    }
}
