/**
 * Nyx Native Runtime - Spiking Silicon Cochlea & Gammatone Bio-Acoustic Filterbank
 * Ultra-low power real-time acoustic keyword and anomaly classification.
 */

#ifndef NYX_COCHLEA_H
#define NYX_COCHLEA_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NYX_COCHLEA_CHANNELS 32

typedef struct {
    float center_freq_hz;
    float state1;
    float state2;
    float state3;
    float state4;
    float spike_threshold;
    uint32_t spike_count;
} NyxCochleaChannel;

typedef struct {
    NyxCochleaChannel channels[NYX_COCHLEA_CHANNELS];
    float sample_rate_hz;
    uint32_t total_acoustic_spikes;
} NyxSpikingCochlea;

// Initializes 32-channel logarithmic biological cochlea filterbank (100 Hz - 8,000 Hz)
void nyx_cochlea_init(NyxSpikingCochlea* cochlea, float sample_rate_hz);

// Feeds raw PCM audio samples and generates output spike train
uint32_t nyx_cochlea_process_pcm(NyxSpikingCochlea* cochlea, const float* samples, uint32_t sample_count);

#ifdef __cplusplus
}
#endif

#endif // NYX_COCHLEA_H
