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

#define TAG_Choice_Some 0
#define TAG_Choice_None 1
typedef struct {
    int tag;
    union {
        struct { int64_t f0; } Some;
    } data;
} Choice;

int64_t unwrap_or(Choice, int64_t);
int64_t nyx_main();
int64_t unwrap_or(Choice o, int64_t fallback) {
return ({
int64_t __match_result_0;
Choice __match_val_0 = o;
if (__match_val_0.tag == TAG_Choice_Some) {
    int64_t v = __match_val_0.data.Some.f0;
__match_result_0 = v;
}
 else if (__match_val_0.tag == TAG_Choice_None) {
__match_result_0 = fallback;
}
__match_result_0;
});
}

int64_t nyx_main() {
NyxOption a = (Choice){ .tag = TAG_Choice_Some, .data.Some = {.f0 = 42LL} };
Choice b = (Choice){ .tag = TAG_Choice_None };
printf("%s%lld%s%lld\n", rt_string_from("a: ").data, (long long)(unwrap_or(a, 0LL)), rt_string_from(" b: ").data, (long long)(unwrap_or(b, 0LL)));
return 0LL;
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
