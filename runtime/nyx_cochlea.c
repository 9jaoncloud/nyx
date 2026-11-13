/**
 * Nyx Native Runtime - Spiking Silicon Cochlea & Gammatone Bio-Acoustic Filterbank
 */

#include "../include/nyx_cochlea.h"
#include <string.h>

void nyx_cochlea_init(NyxSpikingCochlea* cochlea, float sample_rate_hz) {
    if (!cochlea) return;
    memset(cochlea, 0, sizeof(NyxSpikingCochlea));
    cochlea->sample_rate_hz = (sample_rate_hz <= 0.0f) ? 16000.0f : sample_rate_hz;

    // Logarithmically spaced ERB (Equivalent Rectangular Bandwidth) cochlea channels (100 Hz to 8000 Hz)
    float min_freq = 100.0f;
    float max_freq = 8000.0f;
    float factor = powf(max_freq / min_freq, 1.0f / (float)(NYX_COCHLEA_CHANNELS - 1));

    for (int i = 0; i < NYX_COCHLEA_CHANNELS; i++) {
        cochlea->channels[i].center_freq_hz = min_freq * powf(factor, (float)i);
        cochlea->channels[i].spike_threshold = 0.08f;
    }
}

uint32_t nyx_cochlea_process_pcm(NyxSpikingCochlea* cochlea, const float* samples, uint32_t sample_count) {
    if (!cochlea || !samples || sample_count == 0) return 0;
    uint32_t spikes_generated = 0;

    for (uint32_t s = 0; s < sample_count; s++) {
        float x = samples[s];

        for (int c = 0; c < NYX_COCHLEA_CHANNELS; c++) {
            NyxCochleaChannel* ch = &cochlea->channels[c];
            float omega = 2.0f * 3.14159f * ch->center_freq_hz / cochlea->sample_rate_hz;
            float cos_w = cosf(omega);
            float sin_w = sinf(omega);

            // 4th order Gammatone resonator integration
            ch->state1 = ch->state1 * 0.95f + x * sin_w;
            ch->state2 = ch->state2 * 0.95f + ch->state1 * cos_w;
            ch->state3 = ch->state3 * 0.95f + ch->state2;
            ch->state4 = ch->state4 * 0.95f + ch->state3;

            // Half-wave rectification and spike trigger (hair cell depolarisation)
            float envelope = fabsf(ch->state4);
            if (envelope >= ch->spike_threshold) {
                ch->spike_count++;
                spikes_generated++;
                cochlea->total_acoustic_spikes++;
            }
        }
    }

    return spikes_generated;
}
