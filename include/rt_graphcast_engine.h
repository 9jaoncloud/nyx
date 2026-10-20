/*
 * rt_graphcast_engine.h — Google DeepMind GraphCast Spherical Mesh Weather & Climate Engine
 *
 * Implements:
 *  1. Spherical Multi-Mesh Graph Neural Network (0.25° Global Resolution)
 *  2. Autoregressive 10-Day Medium-Range Atmospheric State Prediction
 *  3. Temperature, Geopotential Height (Z500), Surface Wind & Precipitation
 *  4. Extreme Weather & Cyclone Eye Trajectory Tracking
 */

#ifndef RT_GRAPHCAST_ENGINE_H
#define RT_GRAPHCAST_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    double lat;
    double lon;
    float surface_temp_k;
    float wind_u_ms;
    float wind_v_ms;
    float geopotential_z500;
    float precip_6hr_mm;
} AtmosphericStateNode;

typedef struct {
    char target_region[64];
    AtmosphericStateNode trajectory_6hr[40]; // 10 days = 40 steps of 6 hours
    size_t step_count;
    float peak_surface_wind_ms;
    float cumulative_rainfall_mm;
    bool extreme_storm_warning;
    double inference_seconds;
} GraphCastForecastReport;

GraphCastForecastReport graphcast_forecast_10day(double lat, double lon, const char* region_name);

#ifdef __cplusplus
}
#endif

#endif /* RT_GRAPHCAST_ENGINE_H */
