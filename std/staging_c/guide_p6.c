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

rt_string_t classify(int64_t);
void nyx_main();
rt_string_t classify(int64_t n) {
({ if ((n > 0LL)) {
return rt_string_from("positive");
} else if ((n < 0LL)) {
return rt_string_from("negative");
}
});
return rt_string_from("zero");
}

void nyx_main() {
rt_string_t a = classify(5LL);
rt_string_t b = classify((-3LL));
rt_string_t c = classify(0LL);
printf("%s%s%s%s%s\n", a.data, rt_string_from(" ").data, b.data, rt_string_from(" ").data, c.data);
rt_string_t msg = rt_string_concat(rt_string_from("count = "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)42LL); _buf.ref_count = 1; _buf; }));
printf("%s\n", msg.data);
printf("%s%lld\n", rt_string_from("len: ").data, (long long)(msg.length));
({
int64_t __match_result_1;
int64_t __match_val_1 = 42LL;
if (__match_val_1 == 0LL) {
printf("%s\n", rt_string_from("zero").data);
}
 else if (__match_val_1 == 42LL) {
printf("%s\n", rt_string_from("the answer").data);
}
 else {
printf("%s\n", rt_string_from("other").data);
}
__match_result_1;
});
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
