#ifndef RT_NEURAL_STREAM_H
#define RT_NEURAL_STREAM_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── Raw Electrode Spike Frame ── */
typedef struct NyxNeuralSpike {
    uint16_t channel_id;     /* Electrode channel index (0 to 65535) */
    uint16_t unit_id;        /* Clustered neuronal unit */
    float voltage_uv;        /* Microvolts potential amplitude */
    uint32_t timestamp_us;   /* Microsecond timestamp from hardware clock */
    float waveform[8];       /* 8-sample extracellular action potential waveform */
} NyxNeuralSpike;

/* ── Linear Capability Token (Cap<NeuralSpikeStream>) ── */
typedef struct NyxNeuralCap {
    uint64_t token_id;           /* Unforgeable linear capability token */
    bool is_consumed;            /* Linear flag: MUST be consumed exactly once */
    uint32_t channel_count;
    uint32_t spike_count;
    NyxNeuralSpike* spikes;      /* Dedicated zero-copy DMA electrode buffer */
} NyxNeuralCap;

/* Stream Ring Buffer */
typedef struct NyxNeuralStream {
    uint32_t channel_capacity;
    uint32_t buffer_depth;
    NyxNeuralSpike* dma_buffer;
    uint64_t next_token_id;
} NyxNeuralStream;

/* Lifecycle */
NyxNeuralStream* nyx_neural_stream_create(uint32_t channel_capacity, uint32_t buffer_depth);
void nyx_neural_stream_destroy(NyxNeuralStream* stream);

/* Acquire Linear Capability Token (Single Ownership) */
NyxNeuralCap nyx_neural_acquire_cap(NyxNeuralStream* stream, uint32_t spike_count);

/* Read Spike Through Capability (Ensures linear unconsumed state) */
bool nyx_neural_read_spike(const NyxNeuralCap* cap, uint32_t index, NyxNeuralSpike* out_spike);

/* Mandatory Linear Consumption (Havilah conclude() semantic) */
bool nyx_neural_conclude(NyxNeuralCap* cap);

#ifdef __cplusplus
}
#endif

#endif /* RT_NEURAL_STREAM_H */
