/**
 * Nyx Native Runtime - Neuromorphic Event Vision (DVS/DAVIS) & Microsecond Motion Flow
 * Asynchronous (x, y, timestamp_us, polarity) event-stream processing.
 */

#ifndef NYX_EVENT_VISION_H
#define NYX_EVENT_VISION_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NYX_EVENT_BUFFER_MAX 16384

typedef struct {
    uint16_t x;
    uint16_t y;
    uint32_t timestamp_us;
    int8_t polarity; // +1 (ON event / brighten), -1 (OFF event / darken)
} NyxVisionEvent;

typedef struct {
    uint16_t width;
    uint16_t height;
    float surface_timestamp[256][256]; // Time Surface (microsecond decay)
    uint32_t total_events_processed;
    float motion_vx;
    float motion_vy;
} NyxEventSensor;

// Initializes an event-based vision sensor
void nyx_event_sensor_init(NyxEventSensor* sensor, uint16_t width, uint16_t height);

// Processes a batch of asynchronous brightness delta events
void nyx_event_sensor_process(NyxEventSensor* sensor, const NyxVisionEvent* events, uint32_t count, float tau_decay_us);

#ifdef __cplusplus
}
#endif

#endif // NYX_EVENT_VISION_H
