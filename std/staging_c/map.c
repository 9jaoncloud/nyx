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
    NyxVec entries;
} Map;

NyxMap new();
void Map_insert(Map*, void*, void*);
NyxOption Map_get(Map*, void*);
NyxOption Map_nyx_remove(Map*, void*);
int Map_contains_key(Map*, void*);
NyxVec Map_keys(Map*);
NyxVec Map_values(Map*);
void Map_clear(Map*);
int64_t Map_size(Map*);
int Map_is_empty(Map*);
int64_t Map_len(Map*);
// module map
NyxMap new() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (Map){.entries = rt_vec_new(sizeof(int64_t))};
}

void Map_insert(Map* self, void* key, void* value) {
rt_vec_push(&(*self).entries, ({ NyxTuple2 _tmp_0 = (int64_t)(({ NyxTuple2 _tuple_1 = {.f0 = (void*)(key), .f1 = (void*)(value) }; _tuple_1; })); &_tmp_0; }));
}

NyxOption Map_get(Map* self, void* key) {
for (int64_t _idx_k_v = 0; _idx_k_v < (*self).entries.len; _idx_k_v++) {
    NyxTuple2 _tup_k_v = *(NyxTuple2*)rt_vec_get((*self).entries, _idx_k_v);
    int64_t k = (int64_t)(_tup_k_v.f0);
    int64_t v = (int64_t)(_tup_k_v.f1);
({ if ((k == key)) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)(v); _tmp; });
}
});
}
;
return rt_option_none();
}

NyxOption Map_remove(Map* self, void* key) {
for (int64_t i = 0LL; i < (*self).entries.len; i++) {
NyxTuple2 _tmp_k_v = rt_string_from((char*)({ NyxOption _u = rt_vec_get_at((*self).entries, (size_t)(i)); _u.data.ptr_val; }));
int64_t k = (int64_t)_tmp_k_v.f0; // tuple destructuring
int64_t v = (int64_t)_tmp_k_v.f1; // tuple destructuring
({ if ((k == key)) {
int64_t removed = rt_string_from((char*)({ NyxOption _u = rt_vec_get_at((*self).entries, (size_t)(i)); _u.data.ptr_val; }));
rt_vec_remove(&((*self).entries), i);
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)(removed.f1); _tmp; });
}
});
int64_t i = (i + 1LL);
}
;
return rt_option_none();
}

int Map_contains_key(Map* self, void* key) {
for (int64_t _idx_k__v = 0; _idx_k__v < (*self).entries.len; _idx_k__v++) {
    NyxTuple2 _tup_k__v = *(NyxTuple2*)rt_vec_get((*self).entries, _idx_k__v);
    int64_t k = (int64_t)(_tup_k__v.f0);
    int64_t _v = (int64_t)(_tup_k__v.f1);
({ if ((k == key)) {
return 1;
}
});
}
;
return 0;
}

NyxVec Map_keys(Map* self) {
NyxVec result = rt_vec_new(sizeof(int64_t));
for (int64_t _idx_k__v = 0; _idx_k__v < (*self).entries.len; _idx_k__v++) {
    NyxTuple2 _tup_k__v = *(NyxTuple2*)rt_vec_get((*self).entries, _idx_k__v);
    int64_t k = (int64_t)(_tup_k__v.f0);
    int64_t _v = (int64_t)(_tup_k__v.f1);
rt_vec_push(&result, ({ int64_t _tmp_5 = (int64_t)(k); &_tmp_5; }));
}
;
return result;
}

NyxVec Map_values(Map* self) {
NyxVec result = rt_vec_new(sizeof(int64_t));
for (int64_t _idx__k_v = 0; _idx__k_v < (*self).entries.len; _idx__k_v++) {
    NyxTuple2 _tup__k_v = *(NyxTuple2*)rt_vec_get((*self).entries, _idx__k_v);
    int64_t _k = (int64_t)(_tup__k_v.f0);
    int64_t v = (int64_t)(_tup__k_v.f1);
rt_vec_push(&result, ({ int64_t _tmp_6 = (int64_t)(v); &_tmp_6; }));
}
;
return result;
}

void Map_clear(Map* self) {
(self->entries = rt_vec_new(sizeof(int64_t)));
}

int64_t Map_size(Map* self) {
return (*self).entries.len;
}

int Map_is_empty(Map* self) {
return ((*self).entries.len == 0LL);
}

int64_t Map_len(Map* self) {
return (*self).entries.len;
}

