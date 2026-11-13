#include "rt_neural_stream.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

NyxNeuralStream* nyx_neural_stream_create(uint32_t channel_capacity, uint32_t buffer_depth) {
    if (channel_capacity == 0 || buffer_depth == 0) return NULL;
    
    NyxNeuralStream* stream = (NyxNeuralStream*)calloc(1, sizeof(NyxNeuralStream));
    if (!stream) return NULL;

    stream->channel_capacity = channel_capacity;
    stream->buffer_depth = buffer_depth;
    stream->dma_buffer = (NyxNeuralSpike*)calloc(buffer_depth, sizeof(NyxNeuralSpike));
    stream->next_token_id = 0xBC10000000000001ULL;

    if (!stream->dma_buffer) {
        free(stream);
        return NULL;
    }
    return stream;
}

void nyx_neural_stream_destroy(NyxNeuralStream* stream) {
    if (!stream) return;
    if (stream->dma_buffer) {
        free(stream->dma_buffer);
    }
    free(stream);
}

NyxNeuralCap nyx_neural_acquire_cap(NyxNeuralStream* stream, uint32_t spike_count) {
    NyxNeuralCap cap;
    memset(&cap, 0, sizeof(cap));
    if (!stream) return cap;

    if (spike_count > stream->buffer_depth) {
        spike_count = stream->buffer_depth;
    }

    cap.token_id = stream->next_token_id++;
    cap.is_consumed = false;
    cap.channel_count = stream->channel_capacity;
    cap.spike_count = spike_count;
    cap.spikes = stream->dma_buffer;

    return cap;
}

bool nyx_neural_read_spike(const NyxNeuralCap* cap, uint32_t index, NyxNeuralSpike* out_spike) {
    /* Linear Safety Check: Cannot read from a consumed or uninitialized capability */
    if (!cap || cap->is_consumed || !cap->spikes) return false;
    if (index >= cap->spike_count) return false;

    if (out_spike) {
        *out_spike = cap->spikes[index];
    }
    return true;
}

bool nyx_neural_conclude(NyxNeuralCap* cap) {
    /* Linear Safety Check: Double-consumption violation prevention */
    if (!cap || cap->is_consumed) return false;

    /* Mark as consumed and invalidate pointer */
    cap->is_consumed = true;
    cap->spikes = NULL;
    return true;
}
