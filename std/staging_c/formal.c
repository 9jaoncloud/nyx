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
    int64_t obligation_id;
    int is_precondition;
    int is_postcondition;
    int is_loop_invariant;
    int satisfied;
} FormalContract;

int check_range_contract(int64_t, int64_t, int64_t);
int check_non_zero_contract(int64_t);
int check_slice_bounds_contract(int64_t, int64_t, int64_t);
int check_u64_add_overflow(int64_t, int64_t);
FormalContract FormalContract_create_precondition(int64_t, int);
FormalContract FormalContract_create_postcondition(int64_t, int);
FormalContract FormalContract_create_loop_invariant(int64_t, int);
// module formal
// import prelude::*
int check_range_contract(int64_t val, int64_t min_val, int64_t max_val) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ((val >= min_val) && (val <= max_val));
}

int check_non_zero_contract(int64_t val) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (val != 0LL);
}

int check_slice_bounds_contract(int64_t array_len, int64_t offset, int64_t slice_len) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ((offset + slice_len) <= array_len);
}

int check_u64_add_overflow(int64_t a, int64_t b) {
Arena* __arena = arena_create();
int64_t sum = (a + b);
arena_destroy(__arena);
return (sum >= a);
}

FormalContract FormalContract_create_precondition(int64_t id, int cond) {
return (FormalContract){.obligation_id = id, .is_precondition = 1, .is_postcondition = 0, .is_loop_invariant = 0, .satisfied = cond};
}

FormalContract FormalContract_create_postcondition(int64_t id, int cond) {
return (FormalContract){.obligation_id = id, .is_precondition = 0, .is_postcondition = 1, .is_loop_invariant = 0, .satisfied = cond};
}

FormalContract FormalContract_create_loop_invariant(int64_t id, int cond) {
return (FormalContract){.obligation_id = id, .is_precondition = 0, .is_postcondition = 0, .is_loop_invariant = 1, .satisfied = cond};
}

