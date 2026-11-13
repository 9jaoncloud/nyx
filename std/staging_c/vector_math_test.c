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
} Vec3;

double dot_product(Vec3, Vec3);
Vec3 add_vec(Vec3, Vec3);
rt_string_t str(double);
void nyx_main();

#line 1 "vector_math_test.nyx"
double dot_product(Vec3 a, Vec3 b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (((a.x * b.x) + (a.y * b.y)) + (a.z * b.z));
}

Vec3 add_vec(Vec3 a, Vec3 b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (Vec3){.x = (a.x + b.x), .y = (a.y + b.y), .z = (a.z + b.z)};
}

rt_string_t str(double val) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", val); _buf.ref_count = 1; _buf; });
}

void nyx_main() {
Arena* __arena = arena_create();
Vec3 v1 = (Vec3){.x = 1, .y = 2, .z = 3};
Vec3 v2 = (Vec3){.x = 4, .y = 5, .z = 6};
double dot = dot_product(v1, v2);
Vec3 sum = add_vec(v1, v2);
printf("%s\n", rt_string_concat(rt_string_from("Vector Dot Product: "), str(dot)).data);
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("Vector Sum: ("), str(sum.x)), rt_string_from(", ")), str(sum.y)), rt_string_from(", ")), str(sum.z)), rt_string_from(")")).data);
printf("%s\n", rt_string_from("[PASS] vector_math_test completed successfully.").data);
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
