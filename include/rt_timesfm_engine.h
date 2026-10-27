/*
 * rt_timesfm_engine.h — Google TimesFM (Time-Series Foundation Model) in Nyx Runtime
 * 
 * Implements Google Research TimesFM-3 Patched-Decoder Transformer Architecture:
 *  1. Patch Tokenization (Groups contiguous time windows into vector embeddings)
 *  2. Causal Multi-Head Self-Attention with Residual Feed-Forward Blocks
 *  3. Zero-Shot Autoregressive Future Horizon Forecasting (Univariate & Multivariate)
 *  4. Probabilistic Quantile Uncertainty Estimation (10th, 50th, 90th percentiles)
 *  5. Dynamic Exogenous Covariate Integration (Holidays, Weather, Market Flags)
 */

#ifndef RT_TIMESFM_ENGINE_H
#define RT_TIMESFM_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TIMESFM_DEFAULT_PATCH_LEN 32
#define TIMESFM_EMBED_DIM 128
#define TIMESFM_NUM_HEADS 4
#define TIMESFM_HEAD_DIM (TIMESFM_EMBED_DIM / TIMESFM_NUM_HEADS)
#define TIMESFM_MAX_HORIZON 512

/* ── 1. Configuration & Input Data Structures ────────────────────────────── */
typedef struct {
    uint32_t patch_length;       // e.g. 32 timesteps per patch
    uint32_t context_length;     // e.g. 512 input history timesteps
    uint32_t horizon_length;     // e.g. 96 forecast timesteps
    uint32_t embedding_dim;      // e.g. 128 transformer channels
    uint32_t num_layers;         // e.g. 4 transformer blocks
    bool compute_quantiles;      // 10th, 50th (median), 90th percentiles
    bool multivariate_mode;      // Joint cross-series attention
} TimesFMConfig;

typedef struct {
    const double* timestamps_unix;
    const double* history_values;
    size_t history_count;
    const double* exogenous_covariates; // optional external features
    size_t covariate_dim;
} TimesFMTimeSeriesInput;

typedef struct {
    double point_forecast[TIMESFM_MAX_HORIZON]; // Median / Mean trajectory
    double quantile_10[TIMESFM_MAX_HORIZON];    // 10th percentile (Lower bound)
    double quantile_50[TIMESFM_MAX_HORIZON];    // 50th percentile (Median)
    double quantile_90[TIMESFM_MAX_HORIZON];    // 90th percentile (Upper bound)
    size_t forecast_count;
    double forecast_confidence;
    double execution_time_ms;
} TimesFMForecastResult;

/* ── 2. Core TimesFM Patched-Decoder API ──────────────────────────────────── */
TimesFMConfig timesfm_create_default_config(uint32_t history_len, uint32_t horizon_len);

// Tokenize continuous time-series history into patched vector embeddings
size_t timesfm_patch_tokenize(const double* values, size_t count, uint32_t patch_len, float* out_patches, size_t max_patches);

// Zero-shot autoregressive patched transformer inference
TimesFMForecastResult timesfm_forecast_zero_shot(const TimesFMConfig* config, const TimesFMTimeSeriesInput* input);

// Evaluate Mean Absolute Scaled Error (MASE) & Continuous Ranked Probability Score (CRPS)
double timesfm_calculate_mase(const double* actuals, const double* forecast, const double* history, size_t n, size_t m, int seasonality);

#ifdef __cplusplus
}
#endif

#endif /* RT_TIMESFM_ENGINE_H */
