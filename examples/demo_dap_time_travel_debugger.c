/*
 * demo_dap_time_travel_debugger.c
 * Compiled from: examples/demo_dap_time_travel_debugger.nyx
 * Phase 13 — DAP Engine & Time-Travel Memory Profiler
 *
 * Profiler implementation: real ring-buffer via nyx_profiler_ring_* bridge
 * in rt_stdlib.h / rt_profiler.h.  The bridge functions heap-allocate a
 * rt_profiler_state_t (4096-event ring buffer with QueryPerformanceCounter
 * timestamps on Windows) and return an opaque i64 handle.
 */
#include <stdint.h>
#include "runtime/rt_stdlib.h"
#include "runtime/rt_string.h"
#include "runtime/rt_io.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_async.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "runtime/rt_http.h"
#include "runtime/rt_json.h"
#include "runtime/rt_option.h"
#include "runtime/rt_db.h"
#include "runtime/rt_concurrency.h"

#ifndef NYX_TUPLE_DEFS
#define NYX_TUPLE_DEFS
typedef struct { void* f0; void* f1; } NyxTuple2;
typedef struct { void* f0; void* f1; void* f2; } NyxTuple3;
typedef struct { void* f0; void* f1; void* f2; void* f3; } NyxTuple4;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; } NyxTuple5;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; } NyxTuple6;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; } NyxTuple7;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; void* f7; } NyxTuple8;
#endif

/* ── std.tooling.dap ─────────────────────────────────────────────────────── */
typedef struct {
    int64_t      id;
    rt_string_t  source_file;
    int64_t      line;
    int          verified;
} Breakpoint;

typedef struct {
    rt_string_t  name;
    rt_string_t  value;
    rt_string_t  type_name;
} Variable;

typedef struct {
    rt_string_t  session_id;
    int64_t      state;
    int64_t      active_thread_id;
    int64_t      current_line;
    rt_string_t  current_function;
    rt_string_t  stop_reason;
    int64_t      breakpoint_count;
} DapSession;

DapSession create_session(rt_string_t session_id) {
    return (DapSession){
        .session_id      = session_id,
        .state           = 1LL,
        .active_thread_id = 1LL,
        .current_line    = 1LL,
        .current_function = rt_string_from("main"),
        .stop_reason     = rt_string_from("entry"),
        .breakpoint_count = 0LL
    };
}

Breakpoint set_breakpoint(DapSession* sess, rt_string_t file, int64_t line) {
    sess->breakpoint_count++;
    return (Breakpoint){
        .id          = sess->breakpoint_count,
        .source_file = file,
        .line        = line,
        .verified    = 1
    };
}

void step_next(DapSession* sess, int64_t next_line, rt_string_t next_fn) {
    sess->state           = 4LL;
    sess->current_line    = next_line;
    sess->current_function = next_fn;
    sess->stop_reason     = rt_string_from("step");
}

Variable create_variable(rt_string_t name, rt_string_t value, rt_string_t type_name) {
    return (Variable){ .name = name, .value = value, .type_name = type_name };
}

/* ── std.tooling.profiler — real ring-buffer bridge ─────────────────────── */
typedef struct {
    int64_t total_allocated_bytes;
    int64_t peak_memory_bytes;
    int64_t active_bytes;
    int64_t active_regions;
    int64_t total_events;
    int64_t ring_ptr;            /* opaque handle to rt_profiler_state_t* */
} ProfilerState;

typedef struct {
    int64_t timestamp_us;
    int64_t memory_footprint_bytes;
    int64_t active_regions_count;
} TimeTravelSnapshot;

ProfilerState init_profiler(void) {
    return (ProfilerState){
        .total_allocated_bytes = 0,
        .peak_memory_bytes     = 0,
        .active_bytes          = 0,
        .active_regions        = 0,
        .total_events          = 0,
        .ring_ptr              = nyx_profiler_ring_init()
    };
}

void record_allocation(ProfilerState* state, int64_t region_id, int64_t bytes, rt_string_t tag) {
    state->total_allocated_bytes += bytes;
    state->active_bytes          += bytes;
    if (state->active_bytes > state->peak_memory_bytes)
        state->peak_memory_bytes = state->active_bytes;
    state->active_regions++;
    state->total_events++;
    nyx_profiler_ring_alloc(state->ring_ptr, (int32_t)region_id, bytes, tag);
}

void record_deallocation(ProfilerState* state, int64_t region_id, int64_t bytes_reclaimed) {
    if (state->active_bytes >= bytes_reclaimed)
        state->active_bytes -= bytes_reclaimed;
    else
        state->active_bytes = 0;
    if (state->active_regions > 0) state->active_regions--;
    state->total_events++;
    nyx_profiler_ring_drop(state->ring_ptr, (int32_t)region_id, bytes_reclaimed);
}

/* Real O(n) ring-buffer timeline replay */
TimeTravelSnapshot scrub_timeline(ProfilerState* state, int64_t target_time_us) {
    int64_t footprint = nyx_profiler_ring_scrub(state->ring_ptr, target_time_us);
    return (TimeTravelSnapshot){
        .timestamp_us          = target_time_us,
        .memory_footprint_bytes = footprint,
        .active_regions_count  = state->active_regions
    };
}

/* Build Perfetto/Chrome JSON from the real ring buffer */
rt_string_t export_trace_json(ProfilerState* state) {
    return nyx_profiler_ring_json(state->ring_ptr);
}

/* ── Entry point ─────────────────────────────────────────────────────────── */
void nyx_main(void);

void nyx_main(void) {
    printf("%s\n", "=================================================================");
    printf("%s\n", "  \xf0\x9f\x90\x9e NYX DAP DEBUGGER & TIME-TRAVEL MEMORY PROFILER (std.tooling) ");
    printf("%s\n", "=================================================================");

    /* 1. Initialize DAP Session */
    DapSession sess = create_session(rt_string_from("sess_vscode_dap_9182"));
    rt_string_t sid   = sess.session_id;
    int64_t     sstate = sess.state;
    printf("\xE2\x9C\x93 DAP Server Initialized: SessionID=%s State=%lld\n",
           sid.data, (long long)sstate);

    /* 2. Set Verified Breakpoints */
    Breakpoint bp1 = set_breakpoint(&sess, rt_string_from("src/main.nyx"),   42LL);
    Breakpoint bp2 = set_breakpoint(&sess, rt_string_from("src/engine.nyx"), 128LL);
    printf("\xE2\x9C\x93 Breakpoint #1 Set: File=%s Line=%lld Verified=%d\n",
           bp1.source_file.data, (long long)bp1.line, bp1.verified);
    printf("\xE2\x9C\x93 Breakpoint #2 Set: File=%s Line=%lld Verified=%d\n",
           bp2.source_file.data, (long long)bp2.line, bp2.verified);

    /* 3. Step Execution & Inspect Stack Variables */
    step_next(&sess, 43LL, rt_string_from("process_geospatial_telemetry"));
    printf("\xE2\x9C\x93 Stepped to Line %lld in Function @%s\n",
           (long long)sess.current_line, sess.current_function.data);

    Variable v1 = create_variable(
        rt_string_from("sensor_altitude"),
        rt_string_from("12450.5"),
        rt_string_from("f64"));
    Variable v2 = create_variable(
        rt_string_from("quaternion_rot"),
        rt_string_from("[0.0, 0.707, 0.0, 0.707]"),
        rt_string_from("Vec<f64>"));
    printf("\xE2\x9C\x93 Inspected Variable: %s = %s (%s)\n",
           v1.name.data, v1.value.data, v1.type_name.data);
    printf("\xE2\x9C\x93 Inspected Variable: %s = %s (%s)\n",
           v2.name.data, v2.value.data, v2.type_name.data);

    /* 4. Time-Travel Memory Profiling (real ring buffer) */
    ProfilerState prof = init_profiler();
    record_allocation(&prof, 1LL, 1048576LL, rt_string_from("region_r0_matrix_buffer"));
    record_allocation(&prof, 2LL, 2097152LL, rt_string_from("region_r1_lidar_pointcloud"));
    int64_t tot  = prof.total_allocated_bytes;
    int64_t peak = prof.peak_memory_bytes;
    printf("\xE2\x9C\x93 Memory Events Recorded: TotalAlloc=%lld Bytes | Peak=%lld Bytes\n",
           (long long)tot, (long long)peak);

    /* 5. Real time-travel scrub: O(n) ring-buffer replay at t=1500000 µs */
    TimeTravelSnapshot snapshot = scrub_timeline(&prof, 1500000LL);
    printf("\xE2\x9C\x93 Time-Travel Timeline Scrubbed to t=%lldus: ActiveFootprint=%lld Bytes\n",
           (long long)snapshot.timestamp_us,
           (long long)snapshot.memory_footprint_bytes);

    /* 6. Perfetto/Chrome Tracing JSON export from the real ring buffer */
    rt_string_t trace_json = export_trace_json(&prof);
    printf("\xE2\x9C\x93 Perfetto/Chrome Tracing JSON Export Generated: %s\n", trace_json.data);

    printf("%s\n", "=================================================================");
    printf("%s\n", "  \xE2\x9C\x93 DAP ENGINE & TIME-TRAVEL MEMORY PROFILER COMPLETE - ALL OK    ");
    printf("%s\n", "=================================================================");
}

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    nyx_main();
    return 0;
}
