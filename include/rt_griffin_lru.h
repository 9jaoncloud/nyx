/*
 * rt_griffin_lru.h — Google DeepMind Griffin / RecurrentGemma Architecture in Nyx Runtime
 *
 * Implements:
 *  1. Linear Recurrent Units (LRU) with Diagonal Recurrence ($h_t = \text{diag}(\Lambda) h_{t-1} + B x_t$)
 *  2. Local Sliding-Window Multi-Query Attention (Sliding Window size W=512)
 *  3. Constant O(1) Memory State Footprint for Infinite Context Streaming on Edge/Mobile
 *  4. Zero-GC Region Bump Frame Allocation ($O(1)$ memory reset)
 */

#ifndef RT_GRIFFIN_LRU_H
#define RT_GRIFFIN_LRU_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GRIFFIN_DEFAULT_DIM 256
#define GRIFFIN_LRU_STATE_DIM 256
#define GRIFFIN_SLIDING_WINDOW 512

typedef struct {
    uint32_t model_dim;
    uint32_t lru_state_dim;
    uint32_t sliding_window_len;
    uint32_t num_layers;
    float lru_decay_base; // e.g. 0.95f
} GriffinConfig;

typedef struct {
    float hidden_state[GRIFFIN_LRU_STATE_DIM];
    uint64_t total_tokens_processed;
    double cumulative_latency_ms;
} GriffinRecurrentState;

GriffinConfig griffin_create_config(uint32_t dim, uint32_t layers);
GriffinRecurrentState griffin_init_state(const GriffinConfig* config);

// Step token through LRU state without growing memory (O(1) footprint)
void griffin_step_lru(const GriffinConfig* config, GriffinRecurrentState* state, const float* input_emb, float* out_emb);

// Process full token stream in constant memory
void griffin_stream_tokens(const GriffinConfig* config, GriffinRecurrentState* state, const float* token_embeddings, size_t num_tokens, float* out_last_hidden);

#ifdef __cplusplus
}
#endif

#endif /* RT_GRIFFIN_LRU_H */
