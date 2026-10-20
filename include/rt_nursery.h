#ifndef RT_NURSERY_H
#define RT_NURSERY_H

#include <stdint.h>
#include <stdbool.h>
#include "rt_async.h"
#include "rt_arena.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. Structured Async Nursery ── */
typedef struct NyxNurseryTask {
    NyxFuture* future;
    void (*task_fn)(void*);
    void* arg;
    bool completed;
    bool failed;
    struct NyxNurseryTask* next;
} NyxNurseryTask;

typedef struct NyxNursery {
    uint64_t id;
    Arena* bound_region;       /* Parent lexical region bound to this nursery */
    NyxNurseryTask* task_head;
    int active_tasks;
    bool cancelled;
    uint64_t start_timestamp_ns;
} NyxNursery;

/* Nursery Lifecycle */
NyxNursery* nyx_nursery_open(Arena* region);
void nyx_nursery_spawn(NyxNursery* nursery, void (*task_fn)(void*), void* arg);
bool nyx_nursery_await_all(NyxNursery* nursery);
void nyx_nursery_cancel(NyxNursery* nursery);
void nyx_nursery_close(NyxNursery* nursery);

/* ── 2. Isolate Regions: Zero-Copy Lock-Free Region Transfer ── */
typedef struct NyxIsolateRegion {
    uint64_t region_id;
    void* base_ptr;
    size_t allocated_bytes;
    size_t capacity;
    uint32_t owning_thread_id;
    bool is_transferred;
} NyxIsolateRegion;

typedef struct NyxIsolateChannel {
    NyxIsolateRegion* volatile buffer;
    void* mutex;
    void* cond_var;
    bool closed;
} NyxIsolateChannel;

NyxIsolateRegion* nyx_isolate_region_new(size_t capacity);
void* nyx_isolate_region_alloc(NyxIsolateRegion* region, size_t size);
NyxIsolateChannel* nyx_isolate_channel_new(void);
bool nyx_isolate_channel_send(NyxIsolateChannel* ch, NyxIsolateRegion* region);
NyxIsolateRegion* nyx_isolate_channel_recv(NyxIsolateChannel* ch);
void nyx_isolate_region_free(NyxIsolateRegion* region);
void nyx_isolate_channel_free(NyxIsolateChannel* ch);

/* ── 3. Deterministic Profiler Tracing ── */
void nyx_dap_trace_nursery_event(uint64_t nursery_id, const char* event_name, uint64_t duration_ns);
void nyx_dap_trace_region_transfer(uint64_t region_id, uint32_t from_thread, uint32_t to_thread, size_t bytes);

#ifdef __cplusplus
}
#endif

#endif /* RT_NURSERY_H */
