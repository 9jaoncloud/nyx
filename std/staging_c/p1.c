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
    rt_string_t name;
    double val;
} PItem;

typedef struct {
    NyxVec items;
    NyxVec nums;
    int64_t count;
} PHolder;

PHolder PHolder_new();
void PHolder_add_item(PHolder*, rt_string_t, double);
void PHolder_insert(PHolder*, int64_t, double);
double PHolder_get(PHolder*, int64_t);
void nyx_main();
// module probe.p1
PHolder PHolder_new() {
return (PHolder){.items = rt_vec_new(sizeof(int64_t)), .nums = rt_vec_new(sizeof(int64_t)), .count = 0LL};
}

void PHolder_add_item(PHolder* self, rt_string_t name, double val) {
rt_vec_push(&(*self).items, ({ PItem _tmp_0 = (int64_t)((PItem){.name = name, .val = val}); &_tmp_0; }));
(self->count = ((*self).count + 1LL));
}

void PHolder_insert(PHolder* self, int64_t key, double val) {
rt_vec_push(&(*self).nums, ({ double _tmp_1 = (int64_t)(val); &_tmp_1; }));
}

double PHolder_get(PHolder* self, int64_t idx) {
return ({ NyxOption _u = rt_vec_get_at((*self).nums, (size_t)((int64_t)(idx))); (void*)_u.data.int_val; });
}

void nyx_main() {
Arena* __arena = arena_create();
PHolder h = PHolder_new();
PHolder_add_item(&(h), rt_string_from("alpha"), 0.75);
rt_map_insert(&(h), 1LL, (int64_t)(2.5));
double x = rt_map_get(&(h), 0LL);
rt_string_t nm = ({ rt_string_t _c = (({ NyxOption _u = rt_vec_get_at(h.items, (size_t)(0LL)); (void*)_u.data.int_val; })).name; rt_string_retain(&_c); _c; });
printf("x=%f nm=%s\n", x, nm.data);
NyxVec sv = rt_vec_new(sizeof(int64_t));
rt_vec_push(&sv, ({ rt_string_t _tmp_2 = (int64_t)(rt_string_from("hello")); &_tmp_2; }));
printf("s=%lld\n", (long long)(rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(sv, (size_t)(0LL)); _u.data.ptr_val; }))));
NyxVec fv = rt_vec_new(sizeof(int64_t));
rt_vec_push(&fv, ({ double _tmp_3 = (int64_t)(0.5); &_tmp_3; }));
rt_vec_push(&fv, ({ double _tmp_4 = (int64_t)(0.25); &_tmp_4; }));
double s = (({ NyxOption _u = rt_vec_get_at(fv, (size_t)(0LL)); _u.data.float_val; }) + ({ NyxOption _u = rt_vec_get_at(fv, (size_t)(1LL)); _u.data.float_val; }));
printf("sum=%f\n", s);
assert_true((s == 0.75));
assert_true((x == 2.5));
assert_true(rt_string_eq(nm, rt_string_from("alpha")));
printf("%s\n", rt_string_from("P1 OK").data);
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
