#ifndef RT_STDLIB_H
#define RT_STDLIB_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rt_string.h"
#include "rt_option.h"
#include "rt_vec.h"
#include "rt_gis.h"
#include "rt_cloud.h"
#include "rt_ml.h"
#include "rt_ai_model.h"
#include "rt_robotics.h"
#include "rt_chain.h"
#include "rt_audio.h"
#include "rt_vision.h"
#include "rt_vulkan.h"
#include "rt_llvm.h"
#include "rt_pgo.h"
#include "rt_dap.h"
#include "rt_profiler.h"

NyxResult read_to_string(rt_string_t path);
#define rt_fs_read_to_string read_to_string
NyxResult write_string(rt_string_t path, rt_string_t content);
#define rt_fs_write_string write_string
NyxResult remove_file(rt_string_t path);
#define rt_fs_remove_file remove_file

int nyx_get_argc(void);
const char* nyx_get_argv(int idx);

static inline NyxVec rt_env_args(void) {
    int count = nyx_get_argc();
    NyxVec v = rt_vec_new(sizeof(rt_string_t));
    for (int i = 0; i < count; i++) {
        rt_string_t s = rt_string_from(nyx_get_argv(i));
        rt_vec_push(&v, &s);
    }
    return v;
}

rt_string_t sha256_hex(rt_string_t data);
rt_string_t md5_hex(rt_string_t data);

static inline void assert_eq(int64_t a, int64_t b) {
    if (a != b) { printf("FAIL: assert_eq %lld != %lld\n", (long long)a, (long long)b); }
}

static inline void assert_eq_str(rt_string_t a, rt_string_t b) {
    if (strcmp(a.data, b.data) != 0) { printf("FAIL: assert_eq_str %s != %s\n", a.data, b.data); }
}

static inline void assert_true(int cond) {
    if (!cond) { printf("FAIL: assert_true\n"); }
}

static inline void assert_false(int cond) {
    if (cond) { printf("FAIL: assert_false\n"); }
}

static inline void assert_near(double a, double b, double epsilon) {
    double diff = a - b;
    if (diff < 0) diff = -diff;
    if (diff > epsilon) { printf("FAIL: assert_near %f != %f\n", a, b); }
}

static inline void assert_null(void* p) {
    if (p != NULL) { printf("FAIL: assert_null\n"); }
}

static inline void assert_not_null(void* p) {
    if (p == NULL) { printf("FAIL: assert_not_null\n"); }
}

static inline void assert_contains_str(rt_string_t haystack, rt_string_t needle) {
    if (strstr(haystack.data, needle.data) == NULL) {
        printf("FAIL: assert_contains %s not in %s\n", needle.data, haystack.data);
    }
}

static inline rt_string_t void_to_string(void* v) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%p", v);
    return rt_string_from(buf);
}

static inline rt_string_t int_to_string(int64_t v) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%lld", (long long)v);
    return rt_string_from(buf);
}

static inline rt_string_t bool_to_string(int b) {
    return rt_string_from(b ? "true" : "false");
}

static inline rt_string_t uint8_t_to_string(uint8_t v) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%u", (unsigned int)v);
    return rt_string_from(buf);
}

static inline void* push_str(void** out, rt_string_t s) {
    if (!*out) {
        *out = malloc(1);
        *((char*)*out) = 0;
    }
    size_t old_len = strlen((char*)*out);
    size_t new_len = old_len + s.length + 1;
    *out = realloc(*out, new_len);
    memcpy((char*)*out + old_len, s.data, s.length);
    *((char*)*out + new_len - 1) = 0;
    return *out;
}

static inline int64_t rt_process_run(rt_string_t cmd) {
    return (int64_t)system(cmd.data ? cmd.data : "");
}

static inline void rt_process_exit(int64_t code) {
    exit((int)code);
}

// Yield the current thread for `ms` milliseconds (used to cap render FPS).
void rt_sleep(int64_t ms);

// ─── Profiler ring-buffer C bridge ────────────────────────────────────────
// These are called by the Nyx profiler module (std.tooling.profiler) via
// its extern block.  They wrap the real rt_profiler.h ring-buffer state
// behind a heap-allocated i64 handle so Nyx can store it as a primitive field.

/* Allocate and initialise a new rt_profiler_state_t; returns opaque handle */
static inline int64_t nyx_profiler_ring_init(void) {
    rt_profiler_state_t* s = (rt_profiler_state_t*)malloc(sizeof(rt_profiler_state_t));
    if (!s) return 0;
    *s = rt_profiler_init();
    return (int64_t)(uintptr_t)s;
}

/* Record an allocation event into the ring */
static inline int64_t nyx_profiler_ring_alloc(int64_t handle, int32_t region_id,
                                               int64_t bytes, rt_string_t tag) {
    rt_profiler_state_t* s = (rt_profiler_state_t*)(uintptr_t)handle;
    if (s) rt_profiler_record_alloc(s, (uint32_t)region_id, (size_t)bytes, tag.data);
    return 0;
}

/* Record a region-drop event into the ring */
static inline int64_t nyx_profiler_ring_drop(int64_t handle, int32_t region_id,
                                              int64_t bytes) {
    rt_profiler_state_t* s = (rt_profiler_state_t*)(uintptr_t)handle;
    if (s) rt_profiler_record_region_drop(s, (uint32_t)region_id, (size_t)bytes);
    return 0;
}

/* Genuine O(n) ring-buffer replay: returns active bytes at target_us */
static inline int64_t nyx_profiler_ring_scrub(int64_t handle, int64_t target_us) {
    rt_profiler_state_t* s = (rt_profiler_state_t*)(uintptr_t)handle;
    if (!s) return 0;
    return (int64_t)rt_profiler_scrub_timeline(s, (uint64_t)target_us);
}

/* Build Perfetto/Chrome Tracing JSON from the live ring buffer */
static inline rt_string_t nyx_profiler_ring_json(int64_t handle) {
    rt_profiler_state_t* s = (rt_profiler_state_t*)(uintptr_t)handle;
    if (!s) return rt_string_from("{\"traceEvents\":[]}");
    /* Use a 64 KiB stack buffer; for large traces use heap */
    char* buf = (char*)malloc(65536);
    if (!buf) return rt_string_from("{\"traceEvents\":[]}");
    size_t len = rt_profiler_sprint_json(s, buf, 65536);
    rt_string_t result;
    result.data = buf;
    result.length = (int)len;
    result.ref_count = 1;
    return result;
}

// ─── Clock bridge ──────────────────────────────────────────────────────────
/* Returns monotonic µs since process start — callable from Nyx via extern */
static inline int64_t nyx_clock_us(void) {
    return (int64_t)rt_profiler_now_us();
}

// ─── Native OS Clipboard & System Notifications ─────────────────────────────
const char* nyx_clipboard_get_text(void);
void        nyx_clipboard_set_text(const char* text);
void        nyx_notify_send(const char* title, const char* message);

#endif
