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

int is_even(int64_t);
void nyx_main();

#line 1 "extras2.nyx"
int is_even(int64_t n) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if (((n % 2LL) == 0LL)) {
return 1;
} else {
return 0;
}
}

void nyx_main() {
Arena* __arena = arena_create();
int a = 1;
int b = 0;
({ if ((a && b)) {
printf("%s\n", rt_string_from("both").data);
} else {
printf("%s\n", rt_string_from("not both").data);
}
});
({ if ((!b)) {
printf("%s\n", rt_string_from("not b is true").data);
}
});
({ if ((a || b)) {
printf("%s\n", rt_string_from("either").data);
}
});
int64_t max_retries = 3LL;
printf("max retries %lld\n", (long long)(max_retries));
({ if (is_even(4LL)) {
printf("%s\n", rt_string_from("4 is even").data);
}
});
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
