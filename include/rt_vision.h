#ifndef RT_VISION_H
#define RT_VISION_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ── 1. Image Data Container ── */

typedef struct {
    int width;
    int height;
    int channels;
    uint8_t* data;
} NyxImageMat;

static inline NyxImageMat rt_vision_create_mat(int width, int height, int channels) {
    NyxImageMat m;
    m.width = width;
    m.height = height;
    m.channels = channels;
    m.data = (uint8_t*)calloc(width * height * channels, 1);
    return m;
}

static inline void rt_vision_free_mat(NyxImageMat* m) {
    if (m && m->data) {
        free(m->data);
        m->data = NULL;
    }
}

/* ── 2. Sobel Edge Gradient Magnitude ── */

static inline void rt_vision_sobel_gradient(
    const uint8_t* src_gray,
    uint8_t* dst_edges,
    int width,
    int height
) {
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            // Horizontal Sobel kernel
            int gx = -src_gray[(y - 1) * width + (x - 1)] + src_gray[(y - 1) * width + (x + 1)]
                     -2 * src_gray[y * width + (x - 1)] + 2 * src_gray[y * width + (x + 1)]
                     -src_gray[(y + 1) * width + (x - 1)] + src_gray[(y + 1) * width + (x + 1)];

            // Vertical Sobel kernel
            int gy = -src_gray[(y - 1) * width + (x - 1)] - 2 * src_gray[(y - 1) * width + x] - src_gray[(y - 1) * width + (x + 1)]
                     +src_gray[(y + 1) * width + (x - 1)] + 2 * src_gray[(y + 1) * width + x] + src_gray[(y + 1) * width + (x + 1)];

            int mag = (int)sqrt(gx * gx + gy * gy);
            if (mag > 255) mag = 255;
            dst_edges[y * width + x] = (uint8_t)mag;
        }
    }
}

/* ── 3. YOLO DNN Bounding Box Structure ── */

typedef struct {
    float x1, y1, x2, y2;
    float confidence;
    int class_id;
} NyxBoundingBox;

static inline float rt_vision_box_iou(NyxBoundingBox a, NyxBoundingBox b) {
    float ix1 = a.x1 > b.x1 ? a.x1 : b.x1;
    float iy1 = a.y1 > b.y1 ? a.y1 : b.y1;
    float ix2 = a.x2 < b.x2 ? a.x2 : b.x2;
    float iy2 = a.y2 < b.y2 ? a.y2 : b.y2;

    float iw = ix2 - ix1;
    float ih = iy2 - iy1;
    if (iw <= 0.0f || ih <= 0.0f) return 0.0f;

    float inter = iw * ih;
    float area_a = (a.x2 - a.x1) * (a.y2 - a.y1);
    float area_b = (b.x2 - b.x1) * (b.y2 - b.y1);
    return inter / (area_a + area_b - inter);
}

#endif // RT_VISION_H
