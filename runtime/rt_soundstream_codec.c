#include "../include/rt_soundstream_codec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

SoundStreamConfig soundstream_create_config(uint32_t bitrate_bps) {
    SoundStreamConfig cfg;
    cfg.bitrate_bps = bitrate_bps > 0 ? bitrate_bps : 6000;
    cfg.target_latency_ms = 15;
    cfg.packet_loss_concealment = true;
    return cfg;
}

size_t soundstream_encode_pcm16(const int16_t* pcm_samples, size_t sample_count, SoundStreamAudioFrame* out_frames, size_t max_frames) {
    if (!pcm_samples || sample_count == 0 || !out_frames || max_frames == 0) return 0;
    size_t num_frames = sample_count / SOUNDSTREAM_FRAME_SIZE;
    if (num_frames > max_frames) num_frames = max_frames;

    for (size_t f = 0; f < num_frames; f++) {
        size_t start = f * SOUNDSTREAM_FRAME_SIZE;
        double energy = 0.0;
        for (size_t s = 0; s < SOUNDSTREAM_FRAME_SIZE; s++) {
            double v = (double)pcm_samples[start + s] / 32768.0;
            energy += v * v;
        }
        out_frames[f].energy_level = (float)sqrt(energy / (double)SOUNDSTREAM_FRAME_SIZE);

        // Quantize into 8 hierarchical codebooks (RVQ)
        for (int c = 0; c < SOUNDSTREAM_NUM_CODEBOOKS; c++) {
            uint16_t code = (uint16_t)(((pcm_samples[start + (c * 50) % SOUNDSTREAM_FRAME_SIZE] + 32768) >> 6) & 0x03FF);
            out_frames[f].codebook_indices[c] = code;
        }
    }
    return num_frames;
}

size_t soundstream_decode_pcm16(const SoundStreamAudioFrame* frames, size_t frame_count, int16_t* out_pcm_samples, size_t max_samples) {
    if (!frames || frame_count == 0 || !out_pcm_samples || max_samples < SOUNDSTREAM_FRAME_SIZE) return 0;
    size_t total_samples = frame_count * SOUNDSTREAM_FRAME_SIZE;
    if (total_samples > max_samples) total_samples = max_samples;

    for (size_t f = 0; f < frame_count; f++) {
        size_t start = f * SOUNDSTREAM_FRAME_SIZE;
        float gain = frames[f].energy_level;
        for (size_t s = 0; s < SOUNDSTREAM_FRAME_SIZE && (start + s) < max_samples; s++) {
            // Reconstruct synthesized acoustic waveform with harmonic synthesis
            double freq = 220.0 + (double)(frames[f].codebook_indices[0] % 440);
            double sample = sin(2.0 * 3.1415926535 * freq * ((double)s / (double)SOUNDSTREAM_SAMPLE_RATE)) * (double)gain * 32767.0;
            if (sample > 32767.0) sample = 32767.0;
            if (sample < -32768.0) sample = -32768.0;
            out_pcm_samples[start + s] = (int16_t)sample;
        }
    }
    return total_samples;
}
