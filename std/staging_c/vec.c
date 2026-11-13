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
    uint8_t* data;
    int64_t len;
    int64_t cap;
} Vec;

typedef struct {
    uint8_t* vec;
    int64_t index;
} VecIter;

NyxVec new();
NyxVec with_capacity(int64_t);
NyxVec from_slice(NyxVec*);
void Vec_push(Vec*, void*);
NyxOption Vec_pop(Vec*);
int64_t Vec_len(Vec*);
int Vec_is_empty(Vec*);
int64_t Vec_capacity(Vec*);
NyxOption Vec_get(Vec*, int64_t);
int Vec_set(Vec*, int64_t, void*);
NyxOption Vec_first(Vec*);
NyxOption Vec_last(Vec*);
int Vec_contains(Vec*, void*);
NyxOption Vec_index_of(Vec*, void*);
void Vec_clear(Vec*);
VecIter Vec_iter(Vec*);
NyxVec Vec_map(Vec*, void* (*)(void*));
NyxVec Vec_filter(Vec*, int (*)(void*));
void* Vec_fold(Vec*, void*, void* (*)(void*, void*));
void* Vec_sum(Vec*);
void Vec_sort(Vec*);
void Vec_reverse(Vec*);
NyxVec Vec_slice(Vec*, int64_t, int64_t);
NyxOption VecIter_next(VecIter*);
void clear(NyxVec*);
void insert(NyxVec*, int64_t, void*);
NyxOption nyx_remove(NyxVec*, int64_t);
void extend(NyxVec*, NyxVec);
NyxVec take(NyxVec*, int64_t);
NyxVec skip(NyxVec*, int64_t);
void fill(NyxVec*, void*);
rt_string_t join(NyxVec*, rt_string_t);
NyxVec zip(NyxVec*, NyxVec*);
// module vec
extern void* malloc(size_t);
extern void* realloc(void*, size_t);
extern void free(void*);
extern void* memcpy(void*, const void*, size_t);
extern void* memmove(void*, const void*, size_t);
NyxVec new() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
uint8_t* ptr = malloc((int64_t)((INITIAL_CAP * sizeof(void*))));
(Vec){.data = (uint8_t*)(ptr), .len = 0LL, .cap = INITIAL_CAP};
});
}

NyxVec with_capacity(int64_t cap) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
uint8_t* ptr = malloc((int64_t)((cap * sizeof(void*))));
(Vec){.data = (uint8_t*)(ptr), .len = 0LL, .cap = cap};
});
}

NyxVec from_slice(NyxVec* slice) {
Arena* __arena = arena_create();
int64_t count = (int64_t)(slice->len);
arena_destroy(__arena);
return ({
uint8_t* ptr = malloc((int64_t)((count * sizeof(void*))));
memcpy((uint8_t*)(ptr), (uint8_t*)(((const uint8_t*)(slice))), (int64_t)((count * sizeof(void*))));
(Vec){.data = (uint8_t*)(ptr), .len = count, .cap = count};
});
}

void Vec_push(Vec* self, void* item) {
({ if (((*self).len >= (*self).cap)) {
int64_t new_cap = ((*self).cap * 2LL);
({
(self->data = (uint8_t*)(realloc((uint8_t*)((*self).data), (int64_t)((new_cap * sizeof(void*))))));
});
(self->cap = new_cap);
}
});
({
*uint8_t_add((*self).data, (int64_t)((*self).len)) = item;
});
(self->len = ((*self).len + 1LL));
}

NyxOption Vec_pop(Vec* self) {
({ if (((*self).len == 0LL)) {
return rt_option_none();
}
});
(self->len = ((*self).len - 1LL));
return ({
({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)(uint8_t_add((*self).data, (int64_t)((*self).len))); _tmp; });
});
}

int64_t Vec_len(Vec* self) {
return (*self).len;
}

int Vec_is_empty(Vec* self) {
return ((*self).len == 0LL);
}

int64_t Vec_capacity(Vec* self) {
return (*self).cap;
}

NyxOption Vec_get(Vec* self, int64_t index) {
({ if (((index < 0LL) || (index >= (*self).len))) {
return rt_option_none();
}
});
return ({
({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)(uint8_t_add((*self).data, (int64_t)(index))); _tmp; });
});
}

int Vec_set(Vec* self, int64_t index, void* item) {
({ if (((index < 0LL) || (index >= (*self).len))) {
return 0;
}
});
({
*uint8_t_add((*self).data, (int64_t)(index)) = item;
});
return 1;
}

NyxOption Vec_first(Vec* self) {
return rt_vec_get_at(*((NyxVec*)(self)), (size_t)(0LL));
}

NyxOption Vec_last(Vec* self) {
if (((*self).len == 0LL)) {
return rt_option_none();
} else {
return rt_vec_get_at(*((NyxVec*)(self)), (size_t)(((*self).len - 1LL)));
}
}

int Vec_contains(Vec* self, void* item) {
for (int64_t i = 0LL; i < (*self).len; i++) {
({ if ((rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(self)), (size_t)(i)); _u.data.ptr_val; })) == (*(item)))) {
return 1;
}
});
}
;
return 0;
}

NyxOption Vec_index_of(Vec* self, void* item) {
for (int64_t i = 0LL; i < (*self).len; i++) {
({ if ((rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(self)), (size_t)(i)); _u.data.ptr_val; })) == (*(item)))) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)(i); _tmp; });
}
});
}
;
return rt_option_none();
}

void Vec_clear(Vec* self) {
(self->len = 0LL);
}

VecIter Vec_iter(Vec* self) {
return (VecIter){.vec = self, .index = 0LL};
}

NyxVec Vec_map(Vec* self, void* f) {
NyxVec result = rt_vec_new(sizeof(int64_t));
for (int64_t i = 0LL; i < (*self).len; i++) {
rt_vec_push(&result, (void*)(f(rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(self)), (size_t)(i)); _u.data.ptr_val; })))));
}
;
return result;
}

NyxVec Vec_filter(Vec* self, void* f) {
NyxVec result = rt_vec_new(sizeof(int64_t));
for (int64_t i = 0LL; i < (*self).len; i++) {
int64_t item = rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(self)), (size_t)(i)); _u.data.ptr_val; }));
({ if (f((&(item)))) {
rt_vec_push(&result, (void*)(item));
}
});
}
;
return result;
}

void* Vec_fold(Vec* self, void* initial, void* f) {
void* acc = initial;
for (int64_t i = 0LL; i < (*self).len; i++) {
acc = f(acc, rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(self)), (size_t)(i)); _u.data.ptr_val; })));
}
;
return acc;
}

void* Vec_sum(Vec* self) {
int64_t total = int64_t_default(T);
for (int64_t i = 0LL; i < (*self).len; i++) {
total = (total + rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(self)), (size_t)(i)); _u.data.ptr_val; })));
}
;
return total;
}

void Vec_sort(Vec* self) {
for (int64_t i = 1LL; i < (*self).len; i++) {
int64_t key = rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(self)), (size_t)(i)); _u.data.ptr_val; }));
int64_t j = (i - 1LL);
while (((j >= 0LL) && (rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(self)), (size_t)(j)); _u.data.ptr_val; })) > key))) {
rt_vec_set(self, (j + 1LL), rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(self)), (size_t)(j)); _u.data.ptr_val; })));
({ if ((j == 0LL)) {
break;
}
});
j = (j - 1LL);
}
;
rt_vec_set(self, (j + 1LL), key);
}
;
}

void Vec_reverse(Vec* self) {
int64_t left = 0LL;
int64_t right = ((*self).len - 1LL);
while ((left < right)) {
int64_t tmp = rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(self)), (size_t)(left)); _u.data.ptr_val; }));
rt_vec_set(self, left, rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(self)), (size_t)(right)); _u.data.ptr_val; })));
rt_vec_set(self, right, tmp);
left = (left + 1LL);
right = (right - 1LL);
}
;
}

NyxVec Vec_slice(Vec* self, int64_t start, int64_t end) {
NyxVec result = rt_vec_new(sizeof(int64_t));
for (int64_t i = start; i < end; i++) {
/* iflet */ 0;
}
;
return result;
}

NyxOption VecIter_next(VecIter* self) {
return ({
({ if (((*self).index >= (*((*((*self).vec)))).len)) {
return rt_option_none();
}
});
int64_t result = rt_map_get((NyxMap*)((*((*self).vec))), (*self).index);
(self->index = ((*self).index + 1LL));
result;
});
}

void clear(NyxVec* v) {
Arena* __arena = arena_create();
({
(v->len = 0LL);
});
arena_destroy(__arena);
}

void insert(NyxVec* v, int64_t index, void* value) {
Arena* __arena = arena_create();
({ if (((index < 0LL) || (index > v->len))) {
return;
}
});
rt_vec_push(&v, (void*)(value));
int64_t i = (v->len - 1LL);
while ((i > index)) {
int64_t temp = rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(v)), (size_t)((i - 1LL))); _u.data.ptr_val; }));
rt_vec_set(v, i, temp);
i = (i - 1LL);
}
;
rt_vec_set(v, index, value);
arena_destroy(__arena);
}

NyxOption nyx_remove(NyxVec* v, int64_t index) {
Arena* __arena = arena_create();
({ if (((index < 0LL) || (index >= v->len))) {
return rt_option_none();
}
});
int64_t result = rt_vec_get_at(*((NyxVec*)(v)), (size_t)(index));
int64_t i = index;
while ((i < (v->len - 1LL))) {
int64_t next = rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(v)), (size_t)((i + 1LL))); _u.data.ptr_val; }));
rt_vec_set(v, i, next);
i = (i + 1LL);
}
;
(v->len = (v->len - 1LL));
arena_destroy(__arena);
return result;
}

void extend(NyxVec* v, NyxVec other) {
Arena* __arena = arena_create();
for (int64_t i = 0LL; i < other.len; i++) {
rt_vec_push(&v, (void*)(rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(other, (size_t)(i)); _u.data.ptr_val; }))));
}
;
arena_destroy(__arena);
}

NyxVec take(NyxVec* v, int64_t n) {
Arena* __arena = arena_create();
NyxVec result = rt_vec_new(sizeof(int64_t));
int64_t count = ({ int64_t __if_res_16; if ((n < v->len)) {
__if_res_16 = n;
} else {
__if_res_16 = v->len;
}
__if_res_16; });
for (int64_t i = 0LL; i < count; i++) {
rt_vec_push(&result, (void*)(rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(v)), (size_t)(i)); _u.data.ptr_val; }))));
}
;
arena_destroy(__arena);
return result;
}

NyxVec skip(NyxVec* v, int64_t n) {
Arena* __arena = arena_create();
NyxVec result = rt_vec_new(sizeof(int64_t));
int64_t i = n;
while ((i < v->len)) {
rt_vec_push(&result, (void*)(rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(v)), (size_t)(i)); _u.data.ptr_val; }))));
i = (i + 1LL);
}
;
arena_destroy(__arena);
return result;
}

void fill(NyxVec* v, void* value) {
Arena* __arena = arena_create();
for (int64_t i = 0LL; i < v->len; i++) {
rt_vec_set(v, i, value);
}
;
arena_destroy(__arena);
}

rt_string_t join(NyxVec* v, rt_string_t sep) {
Arena* __arena = arena_create();
rt_string_t result = rt_string_from("");
for (int64_t i = 0LL; i < v->len; i++) {
({ if ((i > 0LL)) {
result = rt_string_concat(result, ({ rt_string_t _c = sep; rt_string_retain(&_c); _c; }));
}
});
result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(v)), (size_t)(i)); _u.data.ptr_val; }))); _s.ref_count = 1; _s; }));
}
;
arena_destroy(__arena);
return result;
}

NyxVec zip(NyxVec* a, NyxVec* b) {
Arena* __arena = arena_create();
NyxVec result = rt_vec_new(sizeof(int64_t));
int64_t count = ({ int64_t __if_res_20; if ((a->len < b->len)) {
__if_res_20 = a->len;
} else {
__if_res_20 = b->len;
}
__if_res_20; });
for (int64_t i = 0LL; i < count; i++) {
rt_vec_push(&result, ({ NyxTuple2 _tmp_20 = (int64_t)(({ NyxTuple2 _tuple_21 = {.f0 = (void*)(rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(a)), (size_t)(i)); _u.data.ptr_val; }))), .f1 = (void*)(rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(*((NyxVec*)(b)), (size_t)(i)); _u.data.ptr_val; }))) }; _tuple_21; })); &_tmp_20; }));
}
;
arena_destroy(__arena);
return result;
}

