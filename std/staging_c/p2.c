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
    rt_string_t* labels;
    int64_t tag;
} CBox;

typedef struct {
    CBox box_ref;
    double score;
} CUser;

rt_string_t describe(CBox*);
void nyx_main();
// module probe.p2
rt_string_t describe(CBox* b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_concat(rt_string_concat(({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)(*(int64_t*)rt_vec_get((*b).labels, 0LL))); _s.ref_count = 1; _s; }), rt_string_from("/")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)(&((*(int64_t*)rt_vec_get((*b).labels, 1LL))))); _s.ref_count = 1; _s; }));
}

void nyx_main() {
Arena* __arena = arena_create();
CBox cb = (CBox){.labels = ({ NyxVec _v = rt_vec_new(sizeof(rt_string_t)); rt_string_t _ve0_0 = rt_string_from("one"); rt_vec_push(&_v, &_ve0_0); rt_string_t _ve0_1 = rt_string_from("two"); rt_vec_push(&_v, &_ve0_1); rt_string_t _ve0_2 = rt_string_from("three"); rt_vec_push(&_v, &_ve0_2); rt_string_t _ve0_3 = rt_string_from("four"); rt_vec_push(&_v, &_ve0_3); _v; }), .tag = 7LL};
CUser u = (CUser){.box_ref = cb, .score = 1.5};
rt_string_t d = describe(({ CBox _atmp_0 = (&(u.box_ref)); &_atmp_0; }));
printf("d=%s tag=%lld score=%f\n", d.data, (long long)((u.box_ref).tag), u.score);
assert_true(((u.box_ref).tag == 7LL));
printf("%s\n", rt_string_from("P2 OK").data);
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
