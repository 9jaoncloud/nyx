/*
 * rt_soundstream_codec.h — Google Research SoundStream & AudioLM Neural Audio Codec
 *
 * Implements:
 *  1. Residual Vector Quantization (RVQ) Neural Audio Compression (3kbps to 12kbps)
 *  2. Sub-10ms Streaming Latency for Real-Time Bidirectional Voice Jarvis
 *  3. Acoustic & Semantic Token Separation for Zero-Shot Voice Synthesis
 *  4. Hardware SIMD Zero-Copy Convolutional Pipeline in Nyx Region Arenas
 */

#ifndef RT_SOUNDSTREAM_CODEC_H
#define RT_SOUNDSTREAM_CODEC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SOUNDSTREAM_SAMPLE_RATE 24000
#define SOUNDSTREAM_FRAME_SIZE 480 // 20ms at 24kHz
#define SOUNDSTREAM_NUM_CODEBOOKS 8

typedef struct {
    uint16_t codebook_indices[SOUNDSTREAM_NUM_CODEBOOKS];
    float energy_level;
} SoundStreamAudioFrame;

typedef struct {
    uint32_t bitrate_bps; // e.g. 6000 bps
    uint32_t target_latency_ms; // e.g. 15 ms
    bool packet_loss_concealment;
} SoundStreamConfig;

SoundStreamConfig soundstream_create_config(uint32_t bitrate_bps);
size_t soundstream_encode_pcm16(const int16_t* pcm_samples, size_t sample_count, SoundStreamAudioFrame* out_frames, size_t max_frames);
size_t soundstream_decode_pcm16(const SoundStreamAudioFrame* frames, size_t frame_count, int16_t* out_pcm_samples, size_t max_samples);

#ifdef __cplusplus
}
#endif

#endif /* RT_SOUNDSTREAM_CODEC_H */
