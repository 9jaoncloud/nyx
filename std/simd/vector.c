#include "runtime/rt_async.h"
#include "runtime/rt_map.h"
#include "runtime/rt_http.h"
#include "runtime/rt_string.h"
#include "runtime/rt_io.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_vec.h"
#include <stdint.h>
#include "runtime/rt_concurrency.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "runtime/rt_json.h"
#include <stdlib.h>
#include "runtime/rt_stdlib.h"
#include "runtime/rt_db.h"
#include "runtime/rt_option.h"
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
    double v0;
    double v1;
    double v2;
    double v3;
} SimdF32x4;

SimdF32x4 SimdF32x4_splat(double);
SimdF32x4 SimdF32x4_mul(SimdF32x4*, SimdF32x4*);
SimdF32x4 SimdF32x4_add(SimdF32x4*, SimdF32x4*);
// module std.simd.vector
SimdF32x4 SimdF32x4_splat(double val) {
return (SimdF32x4){.v0 = val, .v1 = val, .v2 = val, .v3 = val};
}

SimdF32x4 SimdF32x4_mul(SimdF32x4* a, SimdF32x4* b) {
return (SimdF32x4){.v0 = ((*a).v0 * (*b).v0), .v1 = ((*a).v1 * (*b).v1), .v2 = ((*a).v2 * (*b).v2), .v3 = ((*a).v3 * (*b).v3)};
}

SimdF32x4 SimdF32x4_add(SimdF32x4* a, SimdF32x4* b) {
return (SimdF32x4){.v0 = ((*a).v0 + (*b).v0), .v1 = ((*a).v1 + (*b).v1), .v2 = ((*a).v2 + (*b).v2), .v3 = ((*a).v3 + (*b).v3)};
}

