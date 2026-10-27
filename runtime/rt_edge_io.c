/*
 * rt_edge_io.c — Deterministic Non-Blocking Region I/O Substrate for Edge & IoT
 */

#include "rt_edge_io.h"
#include <string.h>

void nyx_edge_ring_init(NyxEdgeRing* ring) {
    if (!ring) return;
    ring->head = 0;
    ring->tail = 0;
    ring->count = 0;
    ring->overflow = false;
    memset(ring->buffer, 0, NYX_EDGE_RING_CAPACITY);
}

size_t nyx_edge_ring_write(NyxEdgeRing* ring, const uint8_t* data, size_t len) {
    if (!ring || !data || len == 0) return 0;
    size_t written = 0;

    for (size_t i = 0; i < len; i++) {
        if (ring->count >= NYX_EDGE_RING_CAPACITY) {
            ring->overflow = true;
            break; /* Buffer full */
        }
        ring->buffer[ring->head] = data[i];
        ring->head = (ring->head + 1) % NYX_EDGE_RING_CAPACITY;
        ring->count++;
        written++;
    }

    return written;
}

size_t nyx_edge_ring_read(NyxEdgeRing* ring, uint8_t* out_data, size_t max_len) {
    if (!ring || !out_data || max_len == 0) return 0;
    size_t read_bytes = 0;

    while (read_bytes < max_len && ring->count > 0) {
        out_data[read_bytes++] = ring->buffer[ring->tail];
        ring->tail = (ring->tail + 1) % NYX_EDGE_RING_CAPACITY;
        ring->count--;
    }

    return read_bytes;
}

size_t nyx_edge_ring_available(const NyxEdgeRing* ring) {
    return ring ? ring->count : 0;
}

bool nyx_edge_device_init(NyxEdgeDevice* dev, NyxPeriphType type, uint32_t port_id, uint32_t baud_or_freq) {
    if (!dev) return false;
    dev->type = type;
    dev->port_id = port_id;
    dev->baud_or_freq = baud_or_freq;
    nyx_edge_ring_init(&dev->rx_ring);
    nyx_edge_ring_init(&dev->tx_ring);
    dev->is_initialized = true;
    return true;
}

size_t nyx_edge_device_transmit(NyxEdgeDevice* dev, const void* src, size_t len) {
    if (!dev || !dev->is_initialized || !src || len == 0) return 0;
    return nyx_edge_ring_write(&dev->tx_ring, (const uint8_t*)src, len);
}

size_t nyx_edge_device_receive_into_arena(NyxEdgeDevice* dev, Arena* arena, size_t max_bytes, uint8_t** out_slice) {
    if (!dev || !dev->is_initialized || !arena || !out_slice || max_bytes == 0) return 0;
    
    size_t avail = nyx_edge_ring_available(&dev->rx_ring);
    if (avail == 0) {
        *out_slice = NULL;
        return 0;
    }

    size_t to_read = (avail < max_bytes) ? avail : max_bytes;
    uint8_t* arena_buf = (uint8_t*)arena_alloc(arena, to_read);
    if (!arena_buf) {
        *out_slice = NULL;
        return 0;
    }

    size_t actual_read = nyx_edge_ring_read(&dev->rx_ring, arena_buf, to_read);
    *out_slice = arena_buf;
    return actual_read;
}
