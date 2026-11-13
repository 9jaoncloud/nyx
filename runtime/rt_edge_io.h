/*
 * rt_edge_io.h — Deterministic Non-Blocking Region I/O Substrate for Edge & IoT
 *
 * Implements:
 *  1. Zero-Allocation Ring Buffer streaming directly into Nyx Region Arenas
 *  2. Embedded peripheral abstractions (UART, SPI, I2C, GPIO)
 *  3. Non-blocking asynchronous event readiness polling
 */

#ifndef RT_EDGE_IO_H
#define RT_EDGE_IO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "rt_arena.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NYX_EDGE_RING_CAPACITY 4096

typedef enum {
    NYX_PERIPH_UART = 1,
    NYX_PERIPH_SPI = 2,
    NYX_PERIPH_I2C = 3,
    NYX_PERIPH_GPIO = 4
} NyxPeriphType;

typedef struct {
    uint8_t buffer[NYX_EDGE_RING_CAPACITY];
    volatile size_t head;
    volatile size_t tail;
    volatile size_t count;
    bool overflow;
} NyxEdgeRing;

typedef struct {
    NyxPeriphType type;
    uint32_t port_id;
    uint32_t baud_or_freq;
    NyxEdgeRing rx_ring;
    NyxEdgeRing tx_ring;
    bool is_initialized;
} NyxEdgeDevice;

/* Ring Buffer Core */
void nyx_edge_ring_init(NyxEdgeRing* ring);
size_t nyx_edge_ring_write(NyxEdgeRing* ring, const uint8_t* data, size_t len);
size_t nyx_edge_ring_read(NyxEdgeRing* ring, uint8_t* out_data, size_t max_len);
size_t nyx_edge_ring_available(const NyxEdgeRing* ring);

/* Edge Peripheral Device Lifecycle */
bool nyx_edge_device_init(NyxEdgeDevice* dev, NyxPeriphType type, uint32_t port_id, uint32_t baud_or_freq);
size_t nyx_edge_device_transmit(NyxEdgeDevice* dev, const void* src, size_t len);
size_t nyx_edge_device_receive_into_arena(NyxEdgeDevice* dev, Arena* arena, size_t max_bytes, uint8_t** out_slice);

#ifdef __cplusplus
}
#endif

#endif /* RT_EDGE_IO_H */
