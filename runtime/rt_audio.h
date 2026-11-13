#ifndef RT_AUDIO_H
#define RT_AUDIO_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ── 1. Cooley-Tukey Radix-2 Fast Fourier Transform (FFT) ── */

typedef struct {
    float real;
    float imag;
} NyxComplex;

static inline void rt_fft_radix2(NyxComplex* x, int N) {
    if (N <= 1) return;

    // Bit-reversal permutation
    int j = 0;
    for (int i = 0; i < N - 1; i++) {
        if (i < j) {
            NyxComplex temp = x[i];
            x[i] = x[j];
            x[j] = temp;
        }
        int k = N / 2;
        while (k <= j) {
            j -= k;
            k /= 2;
        }
        j += k;
    }

    // Cooley-Tukey computation
    for (int len = 2; len <= N; len <<= 1) {
        float angle = -2.0f * (float)M_PI / (float)len;
        NyxComplex wlen = { cosf(angle), sinf(angle) };
        for (int i = 0; i < N; i += len) {
            NyxComplex w = { 1.0f, 0.0f };
            for (int k = 0; k < len / 2; k++) {
                NyxComplex u = x[i + k];
                NyxComplex v = {
                    x[i + k + len / 2].real * w.real - x[i + k + len / 2].imag * w.imag,
                    x[i + k + len / 2].real * w.imag + x[i + k + len / 2].imag * w.real
                };
                x[i + k].real = u.real + v.real;
                x[i + k].imag = u.imag + v.imag;
                x[i + k + len / 2].real = u.real - v.real;
                x[i + k + len / 2].imag = u.imag - v.imag;

                float next_r = w.real * wlen.real - w.imag * wlen.imag;
                float next_i = w.real * wlen.imag + w.imag * wlen.real;
                w.real = next_r;
                w.imag = next_i;
            }
        }
    }
}

/* ── 2. Mel Scale & STFT Spectrogram for Whisper Speech AI ── */

static inline float rt_hz_to_mel(float hz) {
    return 2595.0f * log10f(1.0f + hz / 700.0f);
}

static inline float rt_mel_to_hz(float mel) {
    return 700.0f * (powf(10.0f, mel / 2595.0f) - 1.0f);
}

/* ── 3. Biquad IIR Audio Filter ── */

typedef struct {
    float b0, b1, b2, a1, a2;
    float x1, x2, y1, y2;
} NyxBiquadFilter;

static inline NyxBiquadFilter rt_biquad_lowpass(float sample_rate, float cutoff_hz, float q) {
    NyxBiquadFilter f;
    memset(&f, 0, sizeof(NyxBiquadFilter));
    float w0 = 2.0f * (float)M_PI * cutoff_hz / sample_rate;
    float alpha = sinf(w0) / (2.0f * q);
    float cos_w0 = cosf(w0);

    float a0 = 1.0f + alpha;
    f.b0 = ((1.0f - cos_w0) / 2.0f) / a0;
    f.b1 = (1.0f - cos_w0) / a0;
    f.b2 = ((1.0f - cos_w0) / 2.0f) / a0;
    f.a1 = (-2.0f * cos_w0) / a0;
    f.a2 = (1.0f - alpha) / a0;
    return f;
}

static inline float rt_biquad_process(NyxBiquadFilter* f, float sample) {
    float out = f->b0 * sample + f->b1 * f->x1 + f->b2 * f->x2 - f->a1 * f->y1 - f->a2 * f->y2;
    f->x2 = f->x1;
    f->x1 = sample;
    f->y2 = f->y1;
    f->y1 = out;
    return out;
}

#endif // RT_AUDIO_H
