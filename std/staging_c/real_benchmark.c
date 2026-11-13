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
    double x;
    double y;
    double z;
} Point3D;

double Point3D_distance_squared(Point3D*);
Point3D Point3D_scale(Point3D*, double);
double fast_sqrt(double);
double fast_sin(double);
int64_t nyx_main();

#line 1 "real_benchmark.nyx"
// extern nyx_clock_us
double Point3D_distance_squared(Point3D* self) {
return ((((*self).x * (*self).x) + ((*self).y * (*self).y)) + ((*self).z * (*self).z));
}

Point3D Point3D_scale(Point3D* self, double factor) {
return (Point3D){.x = ((*self).x * factor), .y = ((*self).y * factor), .z = ((*self).z * factor)};
}

double fast_sqrt(double n) {
Arena* __arena = arena_create();
({ if ((n <= 0)) {
return 0;
}
});
double val = n;
int64_t i = 0LL;
while ((i < 12LL)) {
val = (0.5 * (val + (n / val)));
i = (i + 1LL);
}
;
arena_destroy(__arena);
return val;
}

double fast_sin(double x) {
Arena* __arena = arena_create();
double x_rad = x;
while ((x_rad > 3.141592653589793)) {
x_rad = (x_rad - 6.283185307179586);
}
;
while ((x_rad < (-3.141592653589793))) {
x_rad = (x_rad + 6.283185307179586);
}
;
double x2 = (x_rad * x_rad);
double x3 = (x_rad * x2);
double x5 = (x3 * x2);
double x7 = (x5 * x2);
arena_destroy(__arena);
return (((x_rad - (x3 / 6)) + (x5 / 120)) - (x7 / 5040));
}

int64_t nyx_main() {
Arena* __arena = arena_create();
printf("%s\n", rt_string_from("================================================================================").data);
printf("%s\n", rt_string_from("      NYX PURE-NATIVE HARDWARE EMPIRICAL BENCHMARK (REAL EXECUTION)             ").data);
printf("%s\n", rt_string_from("================================================================================").data);
printf("%s\n", rt_string_from("Executing real compiled Nyx machine code on physical CPU hardware...").data);
printf("%s\n", rt_string_from("").data);
int64_t n_vec = 5000000LL;
printf("%s\n", rt_string_from("1. Running 3D Vector Math & Struct Transform (N = 5,000,000)...").data);
int64_t t0 = nyx_clock_us();
double acc_dist = 0;
int64_t i = 0LL;
while ((i < n_vec)) {
Point3D p = (Point3D){.x = ((double)(i) * 0.001), .y = ((double)(i) * 0.002), .z = ((double)(i) * 0.003)};
Point3D p_scaled = Point3D_scale(&(p), 1.05);
acc_dist = (acc_dist + Point3D_distance_squared(&(p_scaled)));
i = (i + 1LL);
}
;
int64_t t1 = nyx_clock_us();
int64_t elapsed_us1 = (t1 - t0);
double elapsed_ms1 = ((double)(elapsed_us1) / 1000);
double ops_sec1 = ({ double __if_res_2; if ((elapsed_us1 > 0LL)) {
__if_res_2 = (((double)(n_vec) / (double)(elapsed_us1)) * 1000000);
} else {
__if_res_2 = 0;
}
__if_res_2; });
printf("   -> Elapsed: %f\n", elapsed_ms1);
printf("   -> Real Throughput (Ops/sec): %f\n", ops_sec1);
printf("   -> Checksum: %f\n", acc_dist);
printf("%s\n", rt_string_from("").data);
int64_t n_math = 2000000LL;
printf("%s\n", rt_string_from("2. Running Transcendental Calculus (Sqrt + Sin) (N = 2,000,000)...").data);
int64_t t2 = nyx_clock_us();
double acc_trig = 0;
int64_t j = 0LL;
while ((j < n_math)) {
double input = ((double)(j) * 0.01);
double root = fast_sqrt((input + 1));
double s = fast_sin(root);
acc_trig = (acc_trig + s);
j = (j + 1LL);
}
;
int64_t t3 = nyx_clock_us();
int64_t elapsed_us2 = (t3 - t2);
double elapsed_ms2 = ((double)(elapsed_us2) / 1000);
double ops_sec2 = ({ double __if_res_3; if ((elapsed_us2 > 0LL)) {
__if_res_3 = (((double)(n_math) / (double)(elapsed_us2)) * 1000000);
} else {
__if_res_3 = 0;
}
__if_res_3; });
printf("   -> Elapsed: %f\n", elapsed_ms2);
printf("   -> Real Throughput (Ops/sec): %f\n", ops_sec2);
printf("   -> Checksum: %f\n", acc_trig);
printf("%s\n", rt_string_from("").data);
int64_t n_state = 10000000LL;
printf("%s\n", rt_string_from("3. Running State Transition & Integer Pipeline (N = 10,000,000)...").data);
int64_t t4 = nyx_clock_us();
int64_t state = 123456789LL;
int64_t k = 0LL;
while ((k < n_state)) {
state = (((state * 1103515245LL) + 12345LL) % 2147483648LL);
k = (k + 1LL);
}
;
int64_t t5 = nyx_clock_us();
int64_t elapsed_us3 = (t5 - t4);
double elapsed_ms3 = ((double)(elapsed_us3) / 1000);
double ops_sec3 = ({ double __if_res_4; if ((elapsed_us3 > 0LL)) {
__if_res_4 = (((double)(n_state) / (double)(elapsed_us3)) * 1000000);
} else {
__if_res_4 = 0;
}
__if_res_4; });
printf("   -> Elapsed: %f\n", elapsed_ms3);
printf("   -> Real Throughput (Ops/sec): %f\n", ops_sec3);
printf("   -> Checksum: %lld\n", (long long)(state));
printf("%s\n", rt_string_from("").data);
printf("%s\n", rt_string_from("================================================================================").data);
printf("%s\n", rt_string_from("✅ REAL EMPIRICAL MEASUREMENT COMPLETE: 100% NATIVE NYX COMPILED MACHINE CODE").data);
printf("%s\n", rt_string_from("================================================================================").data);
arena_destroy(__arena);
return 0LL;
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
