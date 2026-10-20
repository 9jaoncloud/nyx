/*
 * rt_lyria_audio.h — Google DeepMind Lyria & MusicLM Generative Ambient Audio Engine
 *
 * Implements:
 *  1. Text-to-Audio Semantic Token Conditioning & Musical Structure Synthesis
 *  2. Ambient Cyber UI Soundscapes & Dynamic Film/Game Soundtrack Generation
 *  3. Zero-GC Direct Memory Audio Buffer Streaming (24kHz/48kHz Stereo)
 */

#ifndef RT_LYRIA_AUDIO_H
#define RT_LYRIA_AUDIO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LYRIA_GENRE_CYBER_AMBIENT = 1,
    LYRIA_GENRE_ORCHESTRAL_CINEMATIC = 2,
    LYRIA_GENRE_LOFI_STUDY_BEATS = 3,
    LYRIA_GENRE_UI_INTERACTION_CHIME = 4
} LyriaMusicGenre;

typedef struct {
    char text_prompt[256];
    LyriaMusicGenre genre;
    double duration_seconds;
    uint32_t sample_rate_hz;
    double bpm_tempo;
} LyriaGenerationSpec;

typedef struct {
    float* stereo_samples_l;
    float* stereo_samples_r;
    size_t total_samples_per_channel;
    double generation_time_ms;
    double musical_coherence_score;
} LyriaAudioBuffer;

LyriaAudioBuffer lyria_generate_ambient_audio(const LyriaGenerationSpec* spec, float* buffer_pool_l, float* buffer_pool_r, size_t pool_size);

#ifdef __cplusplus
}
#endif

#endif /* RT_LYRIA_AUDIO_H */
