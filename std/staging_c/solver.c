#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "runtime/rt_arena.h"
#include "runtime/rt_string.h"
#include "runtime/rt_io.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_option.h"
#include "runtime/rt_map.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_async.h"
#include "runtime/rt_json.h"
#include "runtime/rt_db.h"
#include "runtime/rt_http.h"
#include "runtime/rt_net.h"
#include "runtime/rt_crypto.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_time.h"
#include "runtime/rt_nursery.h"
#include "runtime/rt_collections.h"
#include "runtime/rt_math.h"
#include "runtime/rt_format.h"
#include "runtime/rt_thread.h"
#include "runtime/rt_atomic.h"
#include "runtime/nyx_generational.h"
#include "include/nyx_multidialect.h"
/* Odin-inspired Native Execution Context */
typedef struct NyxContext {
    void* nursery;
    void* temp_nursery;
    uint64_t thread_id;
    void (*logger)(const char* msg);
    void* user_data;
} NyxContext;
#if defined(_MSC_VER)
__declspec(thread) NyxContext nyx_current_context = {0};
#else
__thread NyxContext nyx_current_context = {0};
#endif
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
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

typedef struct {
    int64_t complaint_id;
    int64_t latency_sla_us;
    int64_t keyword_count;
    int has_zero_gc;
} GrievanceTelemetry;

int64_t classify_complaint_severity(int64_t, int64_t);
int verify_telemetry(GrievanceTelemetry);
int64_t nyx_main();

#line 1 "solver.nyx"
// module apps.seo_complaint_solver.solver
int64_t classify_complaint_severity(int64_t length, int64_t keywords) {
Arena* __arena = arena_create();
int64_t score = 1LL;
({ if ((length > 50LL)) {
score = (score + 2LL);
}
});
({ if ((keywords > 10LL)) {
score = (score + 3LL);
}
});
arena_destroy(__arena);
return score;
}

int verify_telemetry(GrievanceTelemetry telemetry) {
Arena* __arena = arena_create();
({ if ((telemetry.has_zero_gc == 1)) {
return 1;
}
});
arena_destroy(__arena);
return 0;
}

int64_t nyx_main() {
Arena* __arena = arena_create();
GrievanceTelemetry telemetry = (GrievanceTelemetry){.complaint_id = 101LL, .latency_sla_us = 450LL, .keyword_count = 24LL, .has_zero_gc = 1};
int is_verified = verify_telemetry(telemetry);
int64_t severity = classify_complaint_severity(68LL, telemetry.keyword_count);
({ if ((is_verified == 1)) {
return 0LL;
}
});
arena_destroy(__arena);
return 1LL;
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
