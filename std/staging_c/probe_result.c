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

#define TAG_Outcome_Ok 0
#define TAG_Outcome_Err 1
typedef struct {
    int tag;
    union {
        struct { int64_t f0; } Ok;
        struct { rt_string_t f0; } Err;
    } data;
} Outcome;

Outcome divide(int64_t, int64_t);
int64_t nyx_main();
Outcome divide(int64_t a, int64_t b) {
({ if ((b == 0LL)) {
return (Outcome){ .tag = TAG_Outcome_Err, .data.Err = {.f0 = rt_string_from("division by zero")} };
}
});
return (Outcome){ .tag = TAG_Outcome_Ok, .data.Ok = {.f0 = (a / b)} };
}

int64_t nyx_main() {
Outcome r1 = divide(100LL, 5LL);
({
int64_t __match_result_1;
Outcome __match_val_1 = r1;
if (__match_val_1.tag == TAG_Outcome_Ok) {
    int64_t v = __match_val_1.data.Ok.f0;
printf("%s%lld\n", rt_string_from("100/5 = ").data, (long long)(v));
}
 else if (__match_val_1.tag == TAG_Outcome_Err) {
    rt_string_t msg = __match_val_1.data.Err.f0;
printf("%s%s\n", rt_string_from("error: ").data, msg.data);
}
__match_result_1;
});
Outcome r2 = divide(1LL, 0LL);
({
int64_t __match_result_2;
Outcome __match_val_2 = r2;
if (__match_val_2.tag == TAG_Outcome_Ok) {
    int64_t v = __match_val_2.data.Ok.f0;
printf("%s%lld\n", rt_string_from("1/0 = ").data, (long long)(v));
}
 else if (__match_val_2.tag == TAG_Outcome_Err) {
    rt_string_t msg = __match_val_2.data.Err.f0;
printf("%s%s\n", rt_string_from("error: ").data, msg.data);
}
__match_result_2;
});
return 0LL;
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
