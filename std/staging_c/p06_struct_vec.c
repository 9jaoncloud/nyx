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
    rt_string_t name;
} User;

User User_new(int64_t, rt_string_t);
rt_string_t User_label(User*);
void nyx_main();

#line 1 "p06_struct_vec.nyx"
User User_new(int64_t id, rt_string_t name) {
return (User){.id = id, .name = name};
}

rt_string_t User_label(User* self) {
return rt_string_concat(rt_string_concat(rt_string_concat((*self).name, rt_string_from(" (#")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)(*self).id); _buf.ref_count = 1; _buf; })), rt_string_from(")"));
}

void nyx_main() {
Arena* __arena = arena_create();
User u = User_new(1LL, rt_string_from("Ada"));
printf("%s\n", User_label(&(u)).data);
NyxVec v = rt_vec_new(sizeof(int64_t));
rt_vec_push(&v, ({ int64_t _tmp_0 = (int64_t)(1LL); &_tmp_0; }));
rt_vec_push(&v, ({ int64_t _tmp_1 = (int64_t)(2LL); &_tmp_1; }));
printf("vec len: %lld\n", (long long)(v.len));
NyxVec items = ({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve2_0 = 10LL; rt_vec_push(&_v, &_ve2_0); int64_t _ve2_1 = 20LL; rt_vec_push(&_v, &_ve2_1); int64_t _ve2_2 = 30LL; rt_vec_push(&_v, &_ve2_2); _v; });
for (int64_t _item = 0; _item < items.len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(items, _item);
printf("item %p\n", item);
}
;
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
