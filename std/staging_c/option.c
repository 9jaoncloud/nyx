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

int Option_is_some(Option*);
int Option_is_none(Option*);
void* Option_unwrap(Option*);
void* Option_unwrap_or(Option*, void*);
// module option
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
return ({
int __match_result_1;
void* __match_val_1 = self;
if ((*__match_val_1).tag == OPTION_NONE) {
__match_result_1 = 1;
}
 else {
__match_result_1 = 0;
}
__match_result_1;
});
}

void* Option_unwrap(Option* self) {
return ({
void* __match_result_2;
NyxOption __match_val_2 = self;
if (__match_val_2.tag == OPTION_SOME) {
    void* v = __match_val_2.data.ptr_val;
__match_result_2 = v;
}
 else if (__match_val_2.tag == OPTION_NONE) {
__match_result_2 = panic(rt_string_from("unwrap called on None"));
}
__match_result_2;
});
}

void* Option_unwrap_or(Option* self, void* default) {
return ({
void* __match_result_3;
NyxOption __match_val_3 = self;
if (__match_val_3.tag == OPTION_SOME) {
    void* v = __match_val_3.data.ptr_val;
__match_result_3 = v;
}
 else if (__match_val_3.tag == OPTION_NONE) {
__match_result_3 = default;
}
__match_result_3;
});
}

