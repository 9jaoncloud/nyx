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
    double bias;
    double w0;
    double w1;
} TmpLinear;

TmpLinear TmpLinear_new();
double TmpLinear_execute_step(TmpLinear*, double, double);
void nyx_main();
TmpLinear TmpLinear_new() {
return (TmpLinear){.bias = 0.25, .w0 = 1.5, .w1 = (-2)};
}

double TmpLinear_execute_step(TmpLinear* self, double f0, double f1) {
double s = (*self).bias;
s = (s + ((*self).w0 * f0));
s = (s + ((*self).w1 * f1));
return s;
}

void nyx_main() {
Arena* __arena = arena_create();
TmpLinear s0 = TmpLinear_new();
double r0 = TmpLinear_execute_step((&(s0)), 1, 2);
int64_t sc0 = (int64_t)((r0 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc0); _buf.ref_count = 1; _buf; }).data);
TmpLinear s1 = TmpLinear_new();
double r1 = TmpLinear_execute_step((&(s1)), 3, 4);
int64_t sc1 = (int64_t)((r1 * 1000000));
printf("%s\n", ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sc1); _buf.ref_count = 1; _buf; }).data);
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
