/**
 * Nyx Native Runtime - Neuromorphic Event Vision (DVS/DAVIS)
 */

#include "../include/nyx_event_vision.h"
#include <string.h>

void nyx_event_sensor_init(NyxEventSensor* sensor, uint16_t width, uint16_t height) {
    if (!sensor) return;
    memset(sensor, 0, sizeof(NyxEventSensor));
    sensor->width = (width > 256) ? 256 : width;
    sensor->height = (height > 256) ? 256 : height;
}

void nyx_event_sensor_process(NyxEventSensor* sensor, const NyxVisionEvent* events, uint32_t count, float tau_decay_us) {
    if (!sensor || !events || count == 0) return;

    float sum_dx = 0.0f;
    float sum_dy = 0.0f;

    for (uint32_t i = 0; i < count; i++) {
        const NyxVisionEvent* ev = &events[i];
        if (ev->x < sensor->width && ev->y < sensor->height) {
            sensor->surface_timestamp[ev->y][ev->x] = (float)ev->timestamp_us;
            sensor->total_events_processed++;

            // Local gradient calculation across 3x3 patch
            if (ev->x > 0 && ev->x < sensor->width - 1 && ev->y > 0 && ev->y < sensor->height - 1) {
                float gx = sensor->surface_timestamp[ev->y][ev->x + 1] - sensor->surface_timestamp[ev->y][ev->x - 1];
                float gy = sensor->surface_timestamp[ev->y + 1][ev->x] - sensor->surface_timestamp[ev->y - 1][ev->x];
                sum_dx += gx * (float)ev->polarity;
                sum_dy += gy * (float)ev->polarity;
            }
        }
    }

    sensor->motion_vx = sum_dx / (float)count;
    sensor->motion_vy = sum_dy / (float)count;
}
