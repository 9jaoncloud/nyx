/*
 * sophia_timesfm_agent.h — Sophia Pre-Cognitive Time-Series Forecasting Copilot
 *
 * Integrates Google TimesFM-3 Foundation Model into Sophia's AI Engine:
 *  1. Zero-Shot Pre-Cognitive Market & Telemetry Forecasting
 *  2. Natural Language Time-Series Horizon & Quantile Planning
 *  3. Dynamic Anomaly & Spike Risk Probability Scoring
 *  4. Automated Vega-Lite / Chart.js Multi-Quantile Visualizer Generation
 */

#ifndef SOPHIA_TIMESFM_AGENT_H
#define SOPHIA_TIMESFM_AGENT_H

#include "rt_timesfm_engine.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TIMESFM_DOMAIN_FINANCE_FX = 1,
    TIMESFM_DOMAIN_SERVER_TELEMETRY = 2,
    TIMESFM_DOMAIN_ENERGY_GRID = 3,
    TIMESFM_DOMAIN_WEATHER_CLIMATE = 4,
    TIMESFM_DOMAIN_ECOMMERCE_DEMAND = 5
} TimesFMDomainType;

typedef struct {
    char user_query[256];
    TimesFMDomainType domain;
    uint32_t forecast_horizon_steps;
    double current_spot_value;
    double anomaly_threshold_percent;
    bool request_quantiles;
} SophiaTimesFMIntent;

typedef struct {
    TimesFMForecastResult forecast;
    double anomaly_spike_probability;
    double expected_drift_percentage;
    char chartjs_dataset_json[1536];
    char reasoning_summary[512];
} SophiaTimesFMReport;

/* ── Sophia TimesFM APIs ─────────────────────────────────────────────────── */
SophiaTimesFMIntent sophia_parse_timesfm_intent(const char* prompt);
SophiaTimesFMReport sophia_execute_timesfm_forecast(const SophiaTimesFMIntent* intent, const double* history, size_t history_len);

#ifdef __cplusplus
}
#endif

#endif /* SOPHIA_TIMESFM_AGENT_H */
