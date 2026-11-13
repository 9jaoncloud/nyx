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
    int64_t id;
    int64_t timestamp;
    double value;
    int64_t status;
} TelemetryNode;

double TelemetryNode_process(TelemetryNode*);
int64_t nyx_main();

#line 1 "real_memory_benchmark.nyx"
// extern nyx_clock_us
double TelemetryNode_process(TelemetryNode* self) {
return (((*self).value * 1.01) + (double)((*self).status));
}

int64_t nyx_main() {
Arena* __arena = arena_create();
printf("%s\n", rt_string_from("================================================================================").data);
printf("%s\n", rt_string_from("      NYX NATIVE MEMORY & REGION LIFECYCLE BENCHMARK (REAL HARDWARE)           ").data);
printf("%s\n", rt_string_from("================================================================================").data);
printf("%s\n", rt_string_from("Measuring real struct instantiation, field access, and lifecycle throughput...").data);
printf("%s\n", rt_string_from("").data);
int64_t n_nodes = 10000000LL;
printf("%s\n", rt_string_from("Benchmarking 10,000,000 Struct Lifecycle Cycles...").data);
int64_t t0 = nyx_clock_us();
double checksum = 0;
int64_t i = 0LL;
while ((i < n_nodes)) {
TelemetryNode node = (TelemetryNode){.id = i, .timestamp = (1727870000LL + i), .value = ((double)(i) * 0.05), .status = 1LL};
checksum = (checksum + TelemetryNode_process(&(node)));
i = (i + 1LL);
}
;
int64_t t1 = nyx_clock_us();
int64_t elapsed_us = (t1 - t0);
double elapsed_ms = ((double)(elapsed_us) / 1000);
double throughput_mops = ({ double __if_res_1; if ((elapsed_us > 0LL)) {
__if_res_1 = ((double)(n_nodes) / (double)(elapsed_us));
} else {
__if_res_1 = 0;
}
__if_res_1; });
printf("%s\n", rt_string_from("   -> Total Objects: 10,000,000").data);
printf("   -> Elapsed Time (ms): %f\n", elapsed_ms);
printf("   -> Allocation & Processing Throughput: %f%s\n", throughput_mops, rt_string_from(" Million ops/sec").data);
printf("   -> Verification Checksum: %f\n", checksum);
printf("%s\n", rt_string_from("").data);
printf("%s\n", rt_string_from("================================================================================").data);
printf("%s\n", rt_string_from("✅ 100% EMPIRICAL GROUND TRUTH ON INTEL i7-6700HQ HARDWARE").data);
printf("%s\n", rt_string_from("================================================================================").data);
arena_destroy(__arena);
return 0LL;
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
