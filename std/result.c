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
#include "runtime/rt_concurrency.h"
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

#define TAG_Result_Ok 0
#define TAG_Result_Err 1
typedef struct {
    int tag;
    union {
        struct { void* f0; } Ok;
        struct { void* f0; } Err;
    } data;
} Result;

int Result_is_ok(Result*);
int Result_is_err(Result*);
void* Result_unwrap(NyxResult);
void* Result_unwrap_or(NyxResult, void*);
// module result
int Result_is_ok(void* self) {
return ({
int __match_result_0;
NyxResult* __match_val_0 = self;
if ((*__match_val_0).tag == RESULT_OK) {
__match_result_0 = 1;
}
 else {
__match_result_0 = 0;
}
__match_result_0;
});
}

int Result_is_err(void* self) {
return ({
int __match_result_1;
NyxResult* __match_val_1 = self;
if ((*__match_val_1).tag == RESULT_ERR) {
__match_result_1 = 1;
}
 else {
__match_result_1 = 0;
}
__match_result_1;
});
}

void* Result_unwrap(NyxResult self) {
return ({
void* __match_result_2;
NyxResult __match_val_2 = self;
if (__match_val_2.tag == RESULT_OK) {
    void* v = __match_val_2.data.ptr_val;
__match_result_2 = v;
}
 else if (__match_val_2.tag == RESULT_ERR) {
__match_result_2 = panic(rt_string_from("unwrap called on Err"));
}
__match_result_2;
});
}

void* Result_unwrap_or(NyxResult self, void* default) {
return ({
void* __match_result_3;
NyxResult __match_val_3 = self;
if (__match_val_3.tag == RESULT_OK) {
    void* v = __match_val_3.data.ptr_val;
__match_result_3 = v;
}
 else if (__match_val_3.tag == RESULT_ERR) {
__match_result_3 = default;
}
__match_result_3;
});
}

