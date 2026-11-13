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

int64_t nyx_main();
int64_t nyx_main() {
int64_t i = 0LL;
int64_t acc = 0LL;
while ((i < 10LL)) {
acc = (acc + i);
i = (i + 1LL);
}
;
printf("%s%lld\n", rt_string_from("While acc: ").data, (long long)(acc));
int64_t x = 42LL;
({
int64_t __match_result_0;
int64_t __match_val_0 = x;
if (__match_val_0 == 0LL) {
printf("%s\n", rt_string_from("zero").data);
}
 else if (__match_val_0 == 42LL) {
printf("%s\n", rt_string_from("the answer").data);
}
 else {
printf("%s\n", rt_string_from("other").data);
}
__match_result_0;
});
return 0LL;
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
