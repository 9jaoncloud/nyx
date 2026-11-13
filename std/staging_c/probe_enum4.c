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

#define TAG_MaybeInt_Just 0
#define TAG_MaybeInt_Nothing 1
typedef struct {
    int tag;
    union {
        struct { int64_t f0; } Just;
    } data;
} MaybeInt;

int64_t unwrap_or(MaybeInt, int64_t);
int64_t nyx_main();
int64_t unwrap_or(MaybeInt o, int64_t fallback) {
return ({
int64_t __match_result_0;
MaybeInt __match_val_0 = o;
if (__match_val_0.tag == TAG_MaybeInt_Just) {
    int64_t v = __match_val_0.data.Just.f0;
__match_result_0 = v;
}
 else if (__match_val_0.tag == TAG_MaybeInt_Nothing) {
__match_result_0 = fallback;
}
__match_result_0;
});
}

int64_t nyx_main() {
MaybeInt a = (MaybeInt){ .tag = TAG_MaybeInt_Just, .data.Just = {.f0 = 42LL} };
MaybeInt b = (MaybeInt){ .tag = TAG_MaybeInt_Nothing };
printf("%s%lld%s%lld\n", rt_string_from("a: ").data, (long long)(unwrap_or(a, 0LL)), rt_string_from(" b: ").data, (long long)(unwrap_or(b, 0LL)));
return 0LL;
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
