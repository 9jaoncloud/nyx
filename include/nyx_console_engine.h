/*
 * nyx_console_engine.h — Nyx Console-Grade Game & Graphics Subsystem
 *
 * Native Console-Grade Hardware Architecture:
 *  1. Lockless PM4-Style GPU Command Packet Ring Buffers
 *  2. Unified Memory Architecture (UMA) Host-Visible VRAM Region Allocator
 *  3. Stateless Vulkan 1.3 Dynamic Rendering & Push Descriptors
 *  4. Asynchronous Compute & Graphics Synchronization via Timeline Semaphores
 *  5. Low-Latency Sub-5ms Circular Audio DMA Ring
 *
 * Zero-GC, sub-millisecond execution, compliant with modern Vulkan 1.3 pipelines.
 */

#ifndef NYX_CONSOLE_ENGINE_H
#define NYX_CONSOLE_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. PM4-Style GPU Command Packet Ring Buffer ─────────────────────────── */

typedef enum {
    NYX_CMD_NOP                 = 0x00,
    NYX_CMD_SET_PIPELINE        = 0x01,
    NYX_CMD_SET_VIEWPORT_SCISSOR= 0x02,
    NYX_CMD_PUSH_CONSTANTS      = 0x03,
    NYX_CMD_BIND_VERTEX_BUFFER  = 0x04,
    NYX_CMD_BIND_INDEX_BUFFER   = 0x05,
    NYX_CMD_DRAW_INDEXED        = 0x06,
    NYX_CMD_DISPATCH_COMPUTE    = 0x07,
    NYX_CMD_TIMELINE_SIGNAL     = 0x08,
    NYX_CMD_TIMELINE_WAIT       = 0x09,
    NYX_CMD_BARRIER_SYNC        = 0x0A
} NyxCmdType;

/* Packet Header Format (32-bit): [Type: 8-bit | ID: 8-bit | Word Count: 16-bit] */
typedef union {
    struct {
        uint8_t  type;
        uint8_t  sub_id;
        uint16_t word_count;
    } s;
    uint32_t raw;
} NyxCmdHeader;

#define NYX_CMD_RING_CAPACITY_WORDS 65536 // 256 KB contiguous ring buffer

typedef struct {
    uint32_t ring[NYX_CMD_RING_CAPACITY_WORDS];
    volatile uint32_t head; // Producer cursor (Gameplay thread)
    volatile uint32_t tail; // Consumer cursor (Render/GPU worker)
    uint64_t total_packets_processed;
} NyxCommandRingBuffer;

NyxCommandRingBuffer* nyx_cmd_ring_create(void);
void nyx_cmd_ring_destroy(NyxCommandRingBuffer* ring);
bool nyx_cmd_ring_push(NyxCommandRingBuffer* ring, NyxCmdType type, uint8_t sub_id, const uint32_t* payload, uint16_t word_count);
bool nyx_cmd_ring_pop(NyxCommandRingBuffer* ring, NyxCmdHeader* out_hdr, uint32_t* out_payload, uint16_t max_words);
void nyx_cmd_ring_flush_to_gpu(NyxCommandRingBuffer* ring, void* vk_command_buffer);

/* ── 2. Unified Memory Architecture (UMA) Region Allocator ───────────────── */

typedef struct {
    uint8_t* base_ptr;              // Host-mapped direct VRAM pointer (ReBAR)
    uint64_t gpu_device_address;     // 64-bit device address for buffer references
    size_t   total_bytes;
    volatile size_t allocated_bytes;
    bool     is_device_local;
} NyxUmaAllocator;

NyxUmaAllocator* nyx_uma_allocator_create(size_t capacity_bytes, bool host_visible_device_local);
void nyx_uma_allocator_destroy(NyxUmaAllocator* alloc);
void* nyx_uma_allocate(NyxUmaAllocator* alloc, size_t size_bytes, size_t alignment, uint64_t* out_gpu_addr);
void nyx_uma_reset(NyxUmaAllocator* alloc);

/* ── 3. Vulkan 1.3 Stateless Dynamic Rendering Context ───────────────────── */

typedef struct {
    float x, y, width, height;
    float min_depth, max_depth;
} NyxViewport;

typedef struct {
    int32_t x, y;
    uint32_t width, height;
} NyxRect2D;

typedef struct {
    void*        color_image_view;
    void*        depth_image_view;
    NyxViewport  viewport;
    NyxRect2D    render_area;
    float        clear_color[4];
    float        clear_depth;
    bool         is_rendering;
} NyxDynamicRenderPass;

NyxDynamicRenderPass nyx_dynamic_render_create(uint32_t width, uint32_t height);
void nyx_dynamic_render_begin(NyxDynamicRenderPass* pass, void* vk_cmd_buf);
void nyx_dynamic_render_end(NyxDynamicRenderPass* pass, void* vk_cmd_buf);

/* ── 4. Asynchronous Compute & Timeline Semaphore Synchronization ────────── */

typedef struct {
    uint64_t current_timeline_value;
    void*    vk_timeline_semaphore_handle;
} NyxTimelineSync;

NyxTimelineSync nyx_timeline_create(uint64_t initial_val);
void nyx_timeline_destroy(NyxTimelineSync* sync);
void nyx_timeline_signal_cpu(NyxTimelineSync* sync, uint64_t new_val);
bool nyx_timeline_wait_cpu(NyxTimelineSync* sync, uint64_t wait_val, uint64_t timeout_ns);
uint64_t nyx_timeline_get_value(NyxTimelineSync* sync);

/* ── 5. Circular Audio DMA Ring (Sub-5ms Low Latency) ────────────────────── */

#define NYX_AUDIO_DMA_SAMPLES 4096 // 4096 stereo samples (~85ms buffer at 48kHz, updated in 2.5ms frames)

typedef struct {
    float ring_l[NYX_AUDIO_DMA_SAMPLES];
    float ring_r[NYX_AUDIO_DMA_SAMPLES];
    volatile uint32_t write_cursor;
    volatile uint32_t read_cursor;
    uint32_t sample_rate; // 48000 Hz standard
} NyxAudioDmaRing;

NyxAudioDmaRing* nyx_audio_dma_create(uint32_t sample_rate);
void nyx_audio_dma_destroy(NyxAudioDmaRing* audio);
size_t nyx_audio_dma_write_stereo(NyxAudioDmaRing* audio, const float* l, const float* r, size_t count);
size_t nyx_audio_dma_read_stereo(NyxAudioDmaRing* audio, float* out_l, float* out_r, size_t count);

#ifdef __cplusplus
}
#endif

#endif /* NYX_CONSOLE_ENGINE_H */
