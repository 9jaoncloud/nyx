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

#define TAG_Outcome_Ok 0
#define TAG_Outcome_Err 1
typedef struct {
    int tag;
    union {
        struct { int64_t f0; } Ok;
        struct { rt_string_t f0; } Err;
    } data;
} Outcome;

Outcome safe_div(int64_t, int64_t);
void nyx_main();

#line 1 "p05_enum.nyx"
Outcome safe_div(int64_t a, int64_t b) {
Arena* __arena = arena_create();
({ if ((b == 0LL)) {
return (Outcome){ .tag = TAG_Outcome_Err, .data.Err = {.f0 = rt_string_from("division by zero")} };
}
});
arena_destroy(__arena);
return (Outcome){ .tag = TAG_Outcome_Ok, .data.Ok = {.f0 = (a / b)} };
}

void nyx_main() {
Arena* __arena = arena_create();
Outcome r = safe_div(10LL, 2LL);
({
int64_t __match_result_1;
Outcome __match_val_1 = r;
if (__match_val_1.tag == TAG_Outcome_Ok) {
    int64_t v = __match_val_1.data.Ok.f0;
printf("ok %lld\n", (long long)(v));
}
 else if (__match_val_1.tag == TAG_Outcome_Err) {
    rt_string_t e = __match_val_1.data.Err.f0;
printf("err %s\n", e.data);
}
__match_result_1;
});
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
