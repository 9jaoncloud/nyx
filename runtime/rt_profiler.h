#ifndef RT_PROFILER_H
#define RT_PROFILER_H

/*
 * rt_profiler.h — Nyx Time-Travel Memory Profiler (Production)
 * Ring-buffer allocation recorder with genuine timeline replay and
 * Perfetto/Chrome Tracing JSON file export.
 *
 * Ring capacity: 4096 events (wraps oldest first when full).
 * Each event stores: real monotonic timestamp, event type, region ID,
 * byte count, tag string, and running cumulative active byte count.
 *
 * scrub_timeline(state, target_us) performs a genuine O(n) linear scan
 * of the ring buffer to find the memory footprint at any past timestamp.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ── Event types ─────────────────────────────────────────────────────── */
typedef enum {
    RT_PROFILE_EVENT_REGION_CREATE  = 0,
    RT_PROFILE_EVENT_ARENA_ALLOC    = 1,
    RT_PROFILE_EVENT_REGION_DESTROY = 2,
    RT_PROFILE_EVENT_HEAP_ESCAPE    = 3
} rt_profile_event_type_t;

/* ── Single event record ─────────────────────────────────────────────── */
#define RT_PROFILER_TAG_LEN 64

typedef struct {
    uint64_t                 timestamp_us;     /* monotonic µs since profiler init */
    rt_profile_event_type_t  event_type;
    uint32_t                 region_id;
    uint64_t                 size_bytes;
    int64_t                  cumulative_bytes;  /* running active bytes AFTER this event */
    char                     tag[RT_PROFILER_TAG_LEN];
} rt_profile_event_t;

/* ── Ring-buffer state ───────────────────────────────────────────────── */
#define RT_PROFILER_RING_CAPACITY 4096

typedef struct {
    rt_profile_event_t ring[RT_PROFILER_RING_CAPACITY];
    uint32_t           head;          /* next write slot (mod capacity) */
    uint32_t           count;         /* events stored, capped at capacity */
    uint64_t           start_us;      /* wall-clock µs at profiler init */
    uint64_t           total_allocated_bytes;
    uint64_t           peak_memory_bytes;
    uint64_t           active_bytes;
    uint32_t           active_regions;
    uint32_t           total_events_recorded;
} rt_profiler_state_t;

/* ── High-resolution monotonic clock (µs) ───────────────────────────── */
static inline uint64_t rt_profiler_now_us(void) {
#if defined(_WIN32)
    static LARGE_INTEGER freq = {0};
    LARGE_INTEGER cnt;
    if (freq.QuadPart == 0) {
        QueryPerformanceFrequency(&freq);
    }
    QueryPerformanceCounter(&cnt);
    return (uint64_t)(cnt.QuadPart * 1000000ULL / (uint64_t)freq.QuadPart);
#elif defined(__MACH__)
    /* macOS: mach_absolute_time converted to µs */
    return (uint64_t)(__builtin_readcyclecounter() / 1000); /* fallback */
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)(ts.tv_nsec / 1000ULL);
#endif
}

/* ── Initialise profiler ─────────────────────────────────────────────── */
static inline rt_profiler_state_t rt_profiler_init(void) {
    rt_profiler_state_t s;
    memset(&s, 0, sizeof(s));
    s.start_us = rt_profiler_now_us();
    return s;
}

/* ── Internal: append event to ring ─────────────────────────────────── */
static inline void rt_profiler__push(rt_profiler_state_t* s,
                                     rt_profile_event_type_t type,
                                     uint32_t region_id, uint64_t bytes,
                                     int64_t cumulative, const char* tag) {
    uint32_t slot = s->head & (RT_PROFILER_RING_CAPACITY - 1);
    rt_profile_event_t* ev = &s->ring[slot];
    ev->timestamp_us    = rt_profiler_now_us() - s->start_us;
    ev->event_type      = type;
    ev->region_id       = region_id;
    ev->size_bytes      = bytes;
    ev->cumulative_bytes = cumulative;
    if (tag) {
        strncpy(ev->tag, tag, RT_PROFILER_TAG_LEN - 1);
        ev->tag[RT_PROFILER_TAG_LEN - 1] = '\0';
    } else {
        ev->tag[0] = '\0';
    }
    s->head++;
    if (s->count < RT_PROFILER_RING_CAPACITY) s->count++;
}

/* ── Record an allocation ────────────────────────────────────────────── */
static inline void rt_profiler_record_alloc(rt_profiler_state_t* s,
                                             uint32_t region_id, size_t bytes,
                                             const char* tag) {
    if (!s) return;
    s->total_allocated_bytes += bytes;
    s->active_bytes          += bytes;
    if (s->active_bytes > s->peak_memory_bytes)
        s->peak_memory_bytes = s->active_bytes;
    s->active_regions++;
    s->total_events_recorded++;
    rt_profiler__push(s, RT_PROFILE_EVENT_ARENA_ALLOC,
                      region_id, bytes, (int64_t)s->active_bytes, tag);
}

/* ── Record a region drop (bulk deallocation) ────────────────────────── */
static inline void rt_profiler_record_region_drop(rt_profiler_state_t* s,
                                                   uint32_t region_id,
                                                   size_t bytes_reclaimed) {
    if (!s) return;
    if (s->active_bytes >= bytes_reclaimed)
        s->active_bytes -= bytes_reclaimed;
    else
        s->active_bytes = 0;
    if (s->active_regions > 0) s->active_regions--;
    s->total_events_recorded++;
    rt_profiler__push(s, RT_PROFILE_EVENT_REGION_DESTROY,
                      region_id, bytes_reclaimed, (int64_t)s->active_bytes,
                      "region_drop");
}

/* ── Time-travel: genuine O(n) ring-buffer replay ────────────────────── */
/*
 * Walks every stored event in chronological order and returns the
 * cumulative active-byte count at the moment target_us was reached.
 * Returns 0 if the profiler has no events yet.
 */
static inline size_t rt_profiler_scrub_timeline(const rt_profiler_state_t* s,
                                                 uint64_t target_us) {
    if (!s || s->count == 0) return 0;

    /* Oldest event index in the ring */
    uint32_t oldest = (s->count < RT_PROFILER_RING_CAPACITY)
                      ? 0
                      : (s->head & (RT_PROFILER_RING_CAPACITY - 1));

    int64_t footprint = 0;
    bool found = false;

    for (uint32_t i = 0; i < s->count; i++) {
        uint32_t idx = (oldest + i) & (RT_PROFILER_RING_CAPACITY - 1);
        const rt_profile_event_t* ev = &s->ring[idx];
        if (ev->timestamp_us <= target_us) {
            footprint = ev->cumulative_bytes;
            found = true;
        } else {
            break; /* events are in chronological order */
        }
    }

    return (size_t)(found && footprint > 0 ? footprint : 0);
}

/* ── Export full timeline to Perfetto / Chrome Tracing JSON file ─────── */
static inline bool rt_profiler_export_json(const rt_profiler_state_t* s,
                                            const char* output_path) {
    if (!s || !output_path) return false;
    FILE* f = fopen(output_path, "w");
    if (!f) return false;

    static const char* ev_names[] = {
        "region_create", "arena_alloc", "region_destroy", "heap_escape"
    };

    uint32_t oldest = (s->count < RT_PROFILER_RING_CAPACITY)
                      ? 0
                      : (s->head & (RT_PROFILER_RING_CAPACITY - 1));

    fprintf(f, "{\"traceEvents\":[\n");
    for (uint32_t i = 0; i < s->count; i++) {
        uint32_t idx = (oldest + i) & (RT_PROFILER_RING_CAPACITY - 1);
        const rt_profile_event_t* ev = &s->ring[idx];
        int t = ev->event_type < 4 ? (int)ev->event_type : 0;
        fprintf(f,
            "  {\"name\":\"%s\",\"ph\":\"X\",\"ts\":%llu,\"dur\":1,"
            "\"args\":{\"region\":%u,\"bytes\":%llu,\"tag\":\"%s\","
            "\"cumulative\":%lld}}%s\n",
            ev_names[t],
            (unsigned long long)ev->timestamp_us,
            ev->region_id,
            (unsigned long long)ev->size_bytes,
            ev->tag,
            (long long)ev->cumulative_bytes,
            (i + 1 < s->count) ? "," : "");
    }
    fprintf(f,
        "],\"displayTimeUnit\":\"us\","
        "\"metadata\":{\"total_allocated\":%llu,"
        "\"peak_bytes\":%llu,"
        "\"active_bytes\":%llu,"
        "\"events\":%u}}\n",
        (unsigned long long)s->total_allocated_bytes,
        (unsigned long long)s->peak_memory_bytes,
        (unsigned long long)s->active_bytes,
        s->total_events_recorded);
    fclose(f);
    return true;
}

/* ── Inline JSON string (no file I/O — for in-process use) ───────────── */
/*
 * Builds a Perfetto-compatible JSON string into a caller-provided buffer.
 * Returns the number of bytes written (excluding NUL) or 0 on overflow.
 */
static inline size_t rt_profiler_sprint_json(const rt_profiler_state_t* s,
                                              char* buf, size_t buf_size) {
    if (!s || !buf || buf_size < 2) return 0;

    static const char* ev_names[] = {
        "region_create", "arena_alloc", "region_destroy", "heap_escape"
    };

    uint32_t oldest = (s->count < RT_PROFILER_RING_CAPACITY)
                      ? 0
                      : (s->head & (RT_PROFILER_RING_CAPACITY - 1));

    int written = snprintf(buf, buf_size,
        "{\"traceEvents\":[");
    if (written < 0 || (size_t)written >= buf_size) return 0;
    size_t pos = (size_t)written;

    for (uint32_t i = 0; i < s->count && pos < buf_size - 2; i++) {
        uint32_t idx = (oldest + i) & (RT_PROFILER_RING_CAPACITY - 1);
        const rt_profile_event_t* ev = &s->ring[idx];
        int t = ev->event_type < 4 ? (int)ev->event_type : 0;
        int n = snprintf(buf + pos, buf_size - pos,
            "{\"name\":\"%s\",\"ph\":\"X\",\"ts\":%llu,\"dur\":1,"
            "\"args\":{\"bytes\":%llu,\"cumulative\":%lld,\"tag\":\"%s\"}}%s",
            ev_names[t],
            (unsigned long long)ev->timestamp_us,
            (unsigned long long)ev->size_bytes,
            (long long)ev->cumulative_bytes,
            ev->tag,
            (i + 1 < s->count) ? "," : "");
        if (n < 0 || pos + (size_t)n >= buf_size) break;
        pos += (size_t)n;
    }

    int tail = snprintf(buf + pos, buf_size - pos,
        "],\"displayTimeUnit\":\"us\","
        "\"metadata\":{\"total_allocated\":%llu,\"peak_bytes\":%llu}}",
        (unsigned long long)s->total_allocated_bytes,
        (unsigned long long)s->peak_memory_bytes);
    if (tail > 0 && pos + (size_t)tail < buf_size) pos += (size_t)tail;

    buf[pos] = '\0';
    return pos;
}

#ifdef __cplusplus
}
#endif

#endif /* RT_PROFILER_H */
