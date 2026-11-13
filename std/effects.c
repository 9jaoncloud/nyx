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

#define TAG_Option_Some 0
#define TAG_Option_None 1
typedef struct {
    int tag;
    union {
        struct { void* f0; } Some;
    } data;
} Option;

#define TAG_Result_Ok 0
#define TAG_Result_Err 1
typedef struct {
    int tag;
    union {
        struct { void* f0; } Ok;
        struct { void* f0; } Err;
    } data;
} Result;

typedef struct {
    int64_t resource;
    int is_active;
} ScopeGuard;

typedef struct {
    NyxVec data;
    int64_t rows;
    int64_t cols;
    int64_t stride_row;
} TensorView2D;

typedef struct {
    NyxOption val;
} Affine;

int Option_is_some(Option*);
int Option_is_none(Option*);
void* Option_unwrap(Option*);
void* Option_unwrap_or(Option*, void*);
NyxOption Option_map(Option*, void* (*)(void*));
NyxOption Option_and_then(Option*, NyxOption (*)(void*));
NyxOption Option_filter(Option*, int (*)(void*));
int Result_is_ok(Result*);
int Result_is_err(Result*);
void* Result_unwrap(Result*);
void* Result_unwrap_or(Result*, void*);
NyxResult Result_map(Result*, void* (*)(void*));
NyxResult Result_map_err(Result*, void* (*)(void*));
NyxResult Result_and_then(Result*, NyxResult (*)(void*));
void* Result_expect(Result*, rt_string_t);
void print(rt_string_t);
void println(rt_string_t);
void eprint(rt_string_t);
void eprintln(rt_string_t);
rt_string_t to_string(void*);
NyxResult to_int(rt_string_t);
NyxResult to_float(rt_string_t);
NyxOption Vec_get(Vec*, int64_t);
NyxOption Vec_first(Vec*);
NyxOption Vec_last(Vec*);
int Vec_contains(Vec*, void*);
NyxOption Vec_index_of(Vec*, void*);
int Vec_is_empty(Vec*);
NyxVec Vec_repeat(Vec*, int64_t);
NyxVec Vec_reverse(Vec*);
int String_is_empty(String*);
int String_starts_with(String*, rt_string_t);
int String_ends_with(String*, rt_string_t);
int String_contains(String*, rt_string_t);
rt_string_t String_trim(String*);
rt_string_t String_to_upper(String*);
rt_string_t String_to_lower(String*);
rt_string_t String_replace(String*, rt_string_t, rt_string_t);
NyxVec String_split(String*, rt_string_t);
rt_string_t String_join(String*, NyxVec);
rt_string_t String_substr(String*, int64_t, int64_t);
NyxVec String_chars(String*);
int64_t Default_nyx_default();
double Default_nyx_default();
int Default_nyx_default();
rt_string_t Default_nyx_default();
int64_t nyx_min(int64_t, int64_t);
int64_t nyx_max(int64_t, int64_t);
int64_t clamp(int64_t, int64_t, int64_t);
ScopeGuard ScopeGuard_new(void*);
void ScopeGuard_dismiss(ScopeGuard*);
TensorView2D TensorView2D_new(NyxVec, int64_t, int64_t);
void* TensorView2D_get(TensorView2D*, int64_t, int64_t);
Affine Affine_new(void*);
NyxOption Affine_consume(Affine*);
int64_t abs_val(int64_t);
int64_t pow(int64_t, int64_t);
// module prelude
int Option_is_some(Option* self) {
return ({
int __match_result_0;
NyxOption* __match_val_0 = self;
if ((*__match_val_0).tag == OPTION_SOME) {
__match_result_0 = 1;
}
 else if ((*__match_val_0).tag == OPTION_NONE) {
__match_result_0 = 0;
}
__match_result_0;
});
}

int Option_is_none(Option* self) {
return (!({ NyxOption _o = self; _o.tag == OPTION_SOME; }));
}

void* Option_unwrap(Option* self) {
return ({
void* __match_result_1;
NyxOption* __match_val_1 = self;
if ((*__match_val_1).tag == OPTION_SOME) {
    void* v = (*__match_val_1).data.ptr_val;
__match_result_1 = v;
}
 else if ((*__match_val_1).tag == OPTION_NONE) {
__match_result_1 = panic(rt_string_from("unwrap called on None"));
}
__match_result_1;
});
}

void* Option_unwrap_or(Option* self, void* default) {
return ({
void* __match_result_2;
NyxOption* __match_val_2 = self;
if ((*__match_val_2).tag == OPTION_SOME) {
    void* v = (*__match_val_2).data.ptr_val;
__match_result_2 = v;
}
 else if ((*__match_val_2).tag == OPTION_NONE) {
__match_result_2 = default;
}
__match_result_2;
});
}

NyxOption Option_map(Option* self, void* f) {
return ({
NyxOption __match_result_3;
NyxOption* __match_val_3 = self;
if ((*__match_val_3).tag == OPTION_SOME) {
    void* v = (*__match_val_3).data.ptr_val;
__match_result_3 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)(f(v)); _tmp; });
}
 else if ((*__match_val_3).tag == OPTION_NONE) {
__match_result_3 = rt_option_none();
}
__match_result_3;
});
}

NyxOption Option_and_then(Option* self, void* f) {
return ({
NyxOption __match_result_4;
NyxOption* __match_val_4 = self;
if ((*__match_val_4).tag == OPTION_SOME) {
    void* v = (*__match_val_4).data.ptr_val;
__match_result_4 = f(v);
}
 else if ((*__match_val_4).tag == OPTION_NONE) {
__match_result_4 = rt_option_none();
}
__match_result_4;
});
}

NyxOption Option_filter(Option* self, void* pred) {
return ({
NyxOption __match_result_5;
NyxOption* __match_val_5 = self;
if ((*__match_val_5).tag == OPTION_SOME) {
    void* v = (*__match_val_5).data.ptr_val;
__match_result_5 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)(v); _tmp; });
}
 else {
__match_result_5 = rt_option_none();
}
__match_result_5;
});
}

int Result_is_ok(Result* self) {
return ({
int __match_result_6;
NyxResult* __match_val_6 = self;
if ((*__match_val_6).tag == RESULT_OK) {
__match_result_6 = 1;
}
 else if ((*__match_val_6).tag == RESULT_ERR) {
__match_result_6 = 0;
}
__match_result_6;
});
}

int Result_is_err(Result* self) {
return (!({ NyxResult _r = self; _r.tag == RESULT_OK; }));
}

void* Result_unwrap(Result* self) {
return ({
void* __match_result_7;
NyxResult* __match_val_7 = self;
if ((*__match_val_7).tag == RESULT_OK) {
    void* v = (*__match_val_7).data.ptr_val;
__match_result_7 = v;
}
 else if ((*__match_val_7).tag == RESULT_ERR) {
__match_result_7 = panic(rt_string_from("unwrap called on Err"));
}
__match_result_7;
});
}

void* Result_unwrap_or(Result* self, void* default) {
return ({
void* __match_result_8;
NyxResult* __match_val_8 = self;
if ((*__match_val_8).tag == RESULT_OK) {
    void* v = (*__match_val_8).data.ptr_val;
__match_result_8 = v;
}
 else if ((*__match_val_8).tag == RESULT_ERR) {
__match_result_8 = default;
}
__match_result_8;
});
}

NyxResult Result_map(Result* self, void* f) {
return ({
NyxResult __match_result_9;
NyxResult* __match_val_9 = self;
if ((*__match_val_9).tag == RESULT_OK) {
    void* v = (*__match_val_9).data.ptr_val;
__match_result_9 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(f(v)); _tmp; });
}
 else if ((*__match_val_9).tag == RESULT_ERR) {
    rt_string_t e = rt_string_from((char*)(*__match_val_9).data.ptr_val);
__match_result_9 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = e; _stmp_.data; }); _tmp; });
}
__match_result_9;
});
}

NyxResult Result_map_err(Result* self, void* f) {
return ({
NyxResult __match_result_10;
NyxResult* __match_val_10 = self;
if ((*__match_val_10).tag == RESULT_OK) {
    void* v = (*__match_val_10).data.ptr_val;
__match_result_10 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)(v); _tmp; });
}
 else if ((*__match_val_10).tag == RESULT_ERR) {
    rt_string_t e = rt_string_from((char*)(*__match_val_10).data.ptr_val);
__match_result_10 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.int_val = (int64_t)(f(e)); _tmp; });
}
__match_result_10;
});
}

NyxResult Result_and_then(Result* self, void* f) {
return ({
NyxResult __match_result_11;
NyxResult* __match_val_11 = self;
if ((*__match_val_11).tag == RESULT_OK) {
    void* v = (*__match_val_11).data.ptr_val;
__match_result_11 = f(v);
}
 else if ((*__match_val_11).tag == RESULT_ERR) {
    rt_string_t e = rt_string_from((char*)(*__match_val_11).data.ptr_val);
__match_result_11 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = e; _stmp_.data; }); _tmp; });
}
__match_result_11;
});
}

void* Result_expect(Result* self, rt_string_t msg) {
return ({
void* __match_result_12;
NyxResult* __match_val_12 = self;
if ((*__match_val_12).tag == RESULT_OK) {
    void* v = (*__match_val_12).data.ptr_val;
__match_result_12 = v;
}
 else if ((*__match_val_12).tag == RESULT_ERR) {
__match_result_12 = panic(msg);
}
__match_result_12;
});
}

void print(rt_string_t s) {
Arena* __arena = arena_create();
rt_print(s);
arena_destroy(__arena);
}

void println(rt_string_t s) {
Arena* __arena = arena_create();
rt_println(s);
arena_destroy(__arena);
}

void eprint(rt_string_t s) {
Arena* __arena = arena_create();
eprint(s);
arena_destroy(__arena);
}

void eprintln(rt_string_t s) {
Arena* __arena = arena_create();
rt_eprintln(s);
arena_destroy(__arena);
}

rt_string_t to_string(void* v) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return void_to_string(v);
}

NyxResult to_int(rt_string_t s) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(rt_string_parse_int(s)); _tmp; });
}

NyxResult to_float(rt_string_t s) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(rt_string_parse_float(s)); _tmp; });
}

NyxOption Vec_get(Vec* self, int64_t index) {
if ((index < self->len)) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)((*(void**)rt_vec_get(*self, index))); _tmp; });
} else {
return rt_option_none();
}
}

NyxOption Vec_first(Vec* self) {
if ((self->len > 0LL)) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)((*(void**)rt_vec_get(*self, 0LL))); _tmp; });
} else {
return rt_option_none();
}
}

NyxOption Vec_last(Vec* self) {
int64_t l = self->len;
if ((l > 0LL)) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)((*(void**)rt_vec_get(*self, (l - 1LL)))); _tmp; });
} else {
return rt_option_none();
}
}

int Vec_contains(Vec* self, void* value) {
for (int64_t _v = 0; _v < ((NyxVec*)(self))->len; _v++) {
    void* v = *(void**)rt_vec_get(*((NyxVec*)(self)), _v);
({ if ((v == value)) {
return 1;
}
});
}
;
return 0;
}

NyxOption Vec_index_of(Vec* self, void* value) {
for (int64_t i = 0LL; i < self->len; i++) {
({ if (((*(void**)rt_vec_get(*self, i)) == value)) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)(i); _tmp; });
}
});
}
;
return rt_option_none();
}

int Vec_is_empty(Vec* self) {
return (self->len == 0LL);
}

NyxVec Vec_repeat(Vec* self, int64_t n) {
NyxVec result = rt_vec_new(sizeof(int64_t));
for (int64_t _ = 0LL; _ < n; _++) {
for (int64_t _v = 0; _v < ((NyxVec*)(self))->len; _v++) {
    void* v = *(void**)rt_vec_get(*((NyxVec*)(self)), _v);
rt_vec_push(&result, (void*)(v));
}
;
}
;
return result;
}

NyxVec Vec_reverse(Vec* self) {
NyxVec result = rt_vec_new(sizeof(int64_t));
int64_t len = self->len;
int64_t i = len;
while ((i > 0LL)) {
i = (i - 1LL);
rt_vec_push(&result, (void*)((*(void**)rt_vec_get(*self, i))));
}
;
return result;
}

int String_is_empty(String* self) {
return (self->len == 0LL);
}

int String_starts_with(String* self, rt_string_t prefix) {
return ((self->len >= prefix.length) && rt_string_eq(rt_string_substr(self, 0LL, prefix.length), prefix));
}

int String_ends_with(String* self, rt_string_t suffix) {
int64_t l = self->len;
int64_t sl = suffix.length;
return ((l >= sl) && rt_string_eq(rt_string_substr(self, (l - sl), l), suffix));
}

int String_contains(String* self, rt_string_t needle) {
({ if ((needle.length == 0LL)) {
return 1;
}
});
({ if ((needle.length > self->len)) {
return 0;
}
});
int64_t i = 0LL;
while ((i <= (self->len - needle.length))) {
int found = 1;
int64_t j = 0LL;
while ((j < needle.length)) {
({ if ((self.data[(i + j)] != needle.data[j])) {
found = 0;
break;
}
});
j = (j + 1LL);
}
;
({ if (found) {
return 1;
}
});
i = (i + 1LL);
}
;
return 0;
}

rt_string_t String_trim(String* self) {
int64_t start = 0LL;
int64_t end = self->len;
while (((end > 0LL) && (self.data[(end - 1LL)] == (int64_t)(' ')))) {
end = (end - 1LL);
}
;
return rt_string_substr(self, start, end);
}

rt_string_t String_to_upper(String* self) {
rt_string_t result = rt_string_from("");
int64_t i = 0LL;
while ((i < self->len)) {
char b = self.data[i];
({ rt_string_t __if_res_22; if (((b >= (int64_t)('a')) && (b <= (int64_t)('z')))) {
rt_vec_push(&result, ({ int64_t _tmp_22 = (int64_t)((b - 32LL)); &_tmp_22; }));
} else {
rt_vec_push(&result, ({ char _tmp_23 = (int64_t)(b); &_tmp_23; }));
}
__if_res_22; });
i = (i + 1LL);
}
;
return result;
}

rt_string_t String_to_lower(String* self) {
rt_string_t result = rt_string_from("");
int64_t i = 0LL;
while ((i < self->len)) {
char b = self.data[i];
({ rt_string_t __if_res_25; if (((b >= (int64_t)('A')) && (b <= (int64_t)('Z')))) {
rt_vec_push(&result, ({ int64_t _tmp_25 = (int64_t)((b + 32LL)); &_tmp_25; }));
} else {
rt_vec_push(&result, ({ char _tmp_26 = (int64_t)(b); &_tmp_26; }));
}
__if_res_25; });
i = (i + 1LL);
}
;
return result;
}

rt_string_t String_replace(String* self, rt_string_t from, rt_string_t to) {
rt_string_t result = rt_string_from("");
int64_t i = 0LL;
while ((i < self->len)) {
({ int64_t __if_res_28; if ((((i + from.length) <= self->len) && rt_string_eq(rt_string_substr(self, i, (i + from.length)), from))) {
result = rt_string_concat(result, ({ rt_string_t _c = to; rt_string_retain(&_c); _c; }));
__if_res_28 = (i = (i + from.length));
} else {
rt_vec_push(&result, ({ char _tmp_28 = (int64_t)(self.data[i]); &_tmp_28; }));
__if_res_28 = (i = (i + 1LL));
}
__if_res_28; });
}
;
return result;
}

NyxVec String_split(String* self, rt_string_t delimiter) {
NyxVec result = rt_vec_new(sizeof(rt_string_t));
rt_string_t current = rt_string_from("");
int64_t i = 0LL;
while ((i < self->len)) {
({ int64_t __if_res_30; if ((((delimiter.length > 0LL) && ((i + delimiter.length) <= self->len)) && rt_string_eq(rt_string_substr(self, i, (i + delimiter.length)), delimiter))) {
rt_vec_push(&result, ({ rt_string_t _tmp_30 = (int64_t)(({ rt_string_t _c = current; rt_string_retain(&_c); _c; })); &_tmp_30; }));
current = rt_string_from("");
__if_res_30 = (i = (i + delimiter.length));
} else {
rt_vec_push(&current, ({ char _tmp_31 = (int64_t)(self.data[i]); &_tmp_31; }));
__if_res_30 = (i = (i + 1LL));
}
__if_res_30; });
}
;
rt_vec_push(&result, ({ rt_string_t _tmp_32 = (int64_t)(current); &_tmp_32; }));
return result;
}

rt_string_t String_join(String* self, NyxVec items) {
rt_string_t result = rt_string_from("");
int first = 1;
for (int64_t _item = 0; _item < items.len; _item++) {
    rt_string_t item = *(rt_string_t*)rt_vec_get(items, _item);
({ if ((!first)) {
result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)self /* clone ptr */); _s.ref_count = 1; _s; }));
}
});
result = rt_string_concat(result, item);
first = 0;
}
;
return result;
}

rt_string_t String_substr(String* self, int64_t start, int64_t end) {
rt_string_t result = rt_string_from("");
int64_t i = start;
while (((i < end) && (i < self->len))) {
rt_vec_push(&result, ({ char _tmp_34 = (int64_t)(self.data[i]); &_tmp_34; }));
i = (i + 1LL);
}
;
return result;
}

NyxVec String_chars(String* self) {
NyxVec result = rt_vec_new(sizeof(int64_t));
int64_t i = 0LL;
while ((i < self->len)) {
rt_vec_push(&result, ({ char _tmp_35 = (int64_t)(self.data[i]); &_tmp_35; }));
i = (i + 1LL);
}
;
return result;
}

int64_t Default_default() {
return 0LL;
}

double Default_default() {
return 0;
}

int Default_default() {
return 0;
}

rt_string_t Default_default() {
return rt_string_from("");
}

int64_t nyx_min(int64_t a, int64_t b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((a < b)) {
return a;
} else {
return b;
}
}

int64_t nyx_max(int64_t a, int64_t b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((a > b)) {
return a;
} else {
return b;
}
}

int64_t clamp(int64_t v, int64_t lo, int64_t hi) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((v < lo)) {
return lo;
} else if ((v > hi)) {
return hi;
} else {
return v;
}
}

ScopeGuard ScopeGuard_new(void* resource) {
return (ScopeGuard){.resource = resource, .is_active = 1};
}

void ScopeGuard_dismiss(ScopeGuard* self) {
(self->is_active = 0);
}

TensorView2D TensorView2D_new(NyxVec data, int64_t rows, int64_t cols) {
return (TensorView2D){.data = data, .rows = rows, .cols = cols, .stride_row = cols};
}

void* TensorView2D_get(TensorView2D* self, int64_t r, int64_t c) {
return (&((*(int64_t*)rt_vec_get((*self).data, ((r * (*self).stride_row) + c)))));
}

Affine Affine_new(void* val) {
return (Affine){.val = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)(val); _tmp; })};
}

NyxOption Affine_consume(Affine* self) {
return ({
NyxOption __match_result_36;
NyxOption __match_val_36 = (*self).val;
if (__match_val_36.tag == OPTION_SOME) {
    void* v = __match_val_36.data.ptr_val;
(self->val = rt_option_none());
__match_result_36 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)(v); _tmp; });
}
 else if (__match_val_36.tag == OPTION_NONE) {
__match_result_36 = rt_option_none();
}
__match_result_36;
});
}

int64_t abs_val(int64_t v) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((v < 0LL)) {
return (-v);
} else {
return v;
}
}

int64_t pow(int64_t base, int64_t exp) {
Arena* __arena = arena_create();
int64_t r = 1LL;
int64_t i = 0LL;
while ((i < exp)) {
r = (r * base);
i = (i + 1LL);
}
;
arena_destroy(__arena);
return r;
}

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

#define TAG_EffectType_Yield 0
#define TAG_EffectType_AsyncAwait 1
#define TAG_EffectType_BargeIn 2
#define TAG_EffectType_SwarmMsg 3
#define TAG_EffectType_FailRecover 4
typedef struct {
    int tag;
    union {
    } data;
} EffectType;

typedef struct {
    EffectType effect_type;
    int64_t payload;
    int is_handled;
} EffectSignal;

typedef struct {
    int64_t depth;
    int is_active;
} EffectHandler;

EffectHandler EffectHandler_create();
void EffectHandler_perform(EffectHandler*, EffectType, int64_t);
void EffectHandler_resume(EffectHandler*, int64_t);
// module effects
// import prelude::*
EffectHandler EffectHandler_create() {
return (EffectHandler){.depth = (int64_t)(0LL), .is_active = 1};
}

void EffectHandler_perform(EffectHandler* self, EffectType effect, int64_t payload) {
}

void EffectHandler_resume(EffectHandler* self, int64_t value) {
}

