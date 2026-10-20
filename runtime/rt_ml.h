#ifndef RT_ML_H
#define RT_ML_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ── 1. High-Performance SIMD/BLAS Matrix Multiplication (GEMM) ── */

// Cache-blocked Matrix Multiplication: C = alpha * (A @ B) + beta * C
// A: M x K, B: K x N, C: M x N
static inline void rt_ml_gemm(
    int M, int N, int K,
    float alpha,
    const float* A, int lda,
    const float* B, int ldb,
    float beta,
    float* C, int ldc
) {
    // Cache tile sizes
    const int BLOCK_M = 64;
    const int BLOCK_N = 64;
    const int BLOCK_K = 64;

    // Apply beta scale
    if (beta == 0.0f) {
        for (int i = 0; i < M; i++) {
            memset(&C[i * ldc], 0, N * sizeof(float));
        }
    } else if (beta != 1.0f) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                C[i * ldc + j] *= beta;
            }
        }
    }

    // Tiled GEMM
    for (int bm = 0; bm < M; bm += BLOCK_M) {
        int im_max = (bm + BLOCK_M < M) ? bm + BLOCK_M : M;
        for (int bn = 0; bn < N; bn += BLOCK_N) {
            int in_max = (bn + BLOCK_N < N) ? bn + BLOCK_N : N;
            for (int bk = 0; bk < K; bk += BLOCK_K) {
                int ik_max = (bk + BLOCK_K < K) ? bk + BLOCK_K : K;

                for (int i = bm; i < im_max; i++) {
                    for (int k = bk; k < ik_max; k++) {
                        float a_ik = alpha * A[i * lda + k];
                        for (int j = bn; j < in_max; j++) {
                            C[i * ldc + j] += a_ik * B[k * ldb + j];
                        }
                    }
                }
            }
        }
    }
}

/* ── 2. Numerically Stable Softmax ── */

static inline void rt_ml_softmax(const float* in, float* out, int size) {
    float max_val = in[0];
    for (int i = 1; i < size; i++) {
        if (in[i] > max_val) max_val = in[i];
    }
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        out[i] = expf(in[i] - max_val);
        sum += out[i];
    }
    float inv_sum = 1.0f / sum;
    for (int i = 0; i < size; i++) {
        out[i] *= inv_sum;
    }
}

/* ── 3. LayerNorm & RMSNorm ── */

static inline void rt_ml_rmsnorm(const float* in, const float* gamma, float* out, int size, float eps) {
    float sum_sq = 0.0f;
    for (int i = 0; i < size; i++) sum_sq += in[i] * in[i];
    float rms = 1.0f / sqrtf((sum_sq / (float)size) + eps);
    for (int i = 0; i < size; i++) {
        out[i] = in[i] * rms * (gamma ? gamma[i] : 1.0f);
    }
}

/* ── 4. SwiGLU Activation ── */

static inline void rt_ml_swiglu(const float* gate, const float* up, float* out, int size) {
    for (int i = 0; i < size; i++) {
        // silu(gate) = gate * sigmoid(gate)
        float silu_gate = gate[i] / (1.0f + expf(-gate[i]));
        out[i] = silu_gate * up[i];
    }
}

/* ── 5. Cosine Similarity for Vector Embeddings ── */

static inline float rt_ml_cosine_similarity(const float* a, const float* b, int dim) {
    float dot = 0.0f, norm_a = 0.0f, norm_b = 0.0f;
    for (int i = 0; i < dim; i++) {
        dot += a[i] * b[i];
        norm_a += a[i] * a[i];
        norm_b += b[i] * b[i];
    }
    float denom = sqrtf(norm_a) * sqrtf(norm_b);
    return denom > 1e-9f ? (dot / denom) : 0.0f;
}

/* ── 6. SIMD Numerical Sanitizer & Clamping Pass (@clamp) ── */

#if defined(__AVX2__) || (defined(_MSC_VER) && defined(__AVX2__))
#include <immintrin.h>
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

static inline void rt_ml_clamp_finite(const float* in, float* out, int size, float min_val, float max_val) {
    int i = 0;

#if defined(__AVX2__)
    __m256 v_min = _mm256_set1_ps(min_val);
    __m256 v_max = _mm256_set1_ps(max_val);
    __m256 v_zero = _mm256_setzero_ps();

    for (; i <= size - 8; i += 8) {
        __m256 v = _mm256_loadu_ps(&in[i]);
        
        // Detect NaN: compare unordered with itself
        __m256 is_nan = _mm256_cmp_ps(v, v, _CMP_UNORD_Q);
        
        // Clamp min and max
        __m256 clamped = _mm256_min_ps(_mm256_max_ps(v, v_min), v_max);
        
        // If NaN, replace with 0.0f
        __m256 res = _mm256_blendv_ps(clamped, v_zero, is_nan);
        _mm256_storeu_ps(&out[i], res);
    }
#elif (defined(__ARM_NEON) || defined(__ARM_NEON__))
    float32x4_t v_min = vdupq_n_f32(min_val);
    float32x4_t v_max = vdupq_n_f32(max_val);

    for (; i <= size - 4; i += 4) {
        float32x4_t v = vld1q_f32(&in[i]);
        uint32x4_t is_nan = vmvnq_u32(vceqq_f32(v, v));
        float32x4_t clamped = vminq_f32(vmaxq_f32(v, v_min), v_max);
        float32x4_t res = vbslq_f32(is_nan, vdupq_n_f32(0.0f), clamped);
        vst1q_f32(&out[i], res);
    }
#endif

    // Clean scalar fallback for remainder elements or non-vector targets
    for (; i < size; i++) {
        float val = in[i];
        if (isnan(val) || isinf(val)) {
            val = (val < 0.0f) ? min_val : (val > 0.0f ? max_val : 0.0f);
        } else if (val < min_val) {
            val = min_val;
        } else if (val > max_val) {
            val = max_val;
        }
        out[i] = val;
    }
}

#endif // RT_ML_H
