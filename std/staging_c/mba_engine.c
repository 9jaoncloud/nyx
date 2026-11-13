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
    int64_t seed;
    int64_t complexity_depth;
} MbaEngine;

MbaEngine MbaEngine_new(int64_t, int64_t);
int64_t MbaEngine_add_linear(MbaEngine*, int64_t, int64_t);
int64_t MbaEngine_add_higher_order(MbaEngine*, int64_t, int64_t);
int64_t MbaEngine_sub_linear(MbaEngine*, int64_t, int64_t);
int64_t MbaEngine_xor_via_mba(MbaEngine*, int64_t, int64_t);
int64_t MbaEngine_disguise_constant_u32(MbaEngine*, int64_t, int64_t);
int64_t MbaEngine_disguise_constant_u64(MbaEngine*, int64_t, int64_t);
int64_t MbaEngine_evaluate_affine_layer(MbaEngine*, int64_t, int64_t);
// import std::crypto
// import std::math
MbaEngine MbaEngine_new(int64_t seed, int64_t depth) {
return (MbaEngine){.seed = seed, .complexity_depth = depth};
}

int64_t MbaEngine_add_linear(MbaEngine* self, int64_t x, int64_t y) {
int64_t xor_part = (x ^ y);
int64_t and_part = ((x & y) << 1LL);
return (xor_part + and_part);
}

int64_t MbaEngine_add_higher_order(MbaEngine* self, int64_t x, int64_t y) {
int64_t or_part = (x | y);
int64_t and_part = (x & y);
return (or_part + and_part);
}

int64_t MbaEngine_sub_linear(MbaEngine* self, int64_t x, int64_t y) {
int64_t not_y = (!y);
int64_t not_x = (!x);
int64_t p1 = ((x ^ not_y) + 1LL);
int64_t p2 = ((not_x & y) << 1LL);
return (p1 - p2);
}

int64_t MbaEngine_xor_via_mba(MbaEngine* self, int64_t x, int64_t y) {
int64_t or_term = (x | y);
int64_t and_term = (x & y);
return (or_term - and_term);
}

int64_t MbaEngine_disguise_constant_u32(MbaEngine* self, int64_t constant_val, int64_t runtime_entropy) {
int64_t a = (2654435769LL | 1LL);
int64_t a_inv = 3840719641LL;
int64_t b = 2246822507LL;
int64_t affine_transformed = ((a * runtime_entropy) + b);
int64_t recovered_entropy = ((affine_transformed - b) * a_inv);
int64_t zero_diff = (runtime_entropy - recovered_entropy);
return (constant_val + zero_diff);
}

int64_t MbaEngine_disguise_constant_u64(MbaEngine* self, int64_t constant_val, int64_t state_var) {
int64_t masked = (constant_val ^ state_var);
int64_t unmasked = ((masked | state_var) - (masked & state_var));
return ((unmasked ^ state_var) ^ (masked & (!state_var)));
}

int64_t MbaEngine_evaluate_affine_layer(MbaEngine* self, int64_t input, int64_t salt) {
int64_t t1 = ((input ^ salt) * 5871781006564002453LL);
int64_t t2 = ((t1 ^ (t1 >> 32LL)) * 3568130375630372031LL);
return (t2 ^ (t2 >> 16LL));
}

