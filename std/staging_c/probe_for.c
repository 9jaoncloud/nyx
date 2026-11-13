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
int64_t total = 0LL;
for (int64_t _i = 0; _i < ({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve0_0 = 1LL; rt_vec_push(&_v, &_ve0_0); int64_t _ve0_1 = 2LL; rt_vec_push(&_v, &_ve0_1); int64_t _ve0_2 = 3LL; rt_vec_push(&_v, &_ve0_2); int64_t _ve0_3 = 4LL; rt_vec_push(&_v, &_ve0_3); int64_t _ve0_4 = 5LL; rt_vec_push(&_v, &_ve0_4); _v; }).len; _i++) {
    int64_t i = *(int64_t*)rt_vec_get(({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve0_0 = 1LL; rt_vec_push(&_v, &_ve0_0); int64_t _ve0_1 = 2LL; rt_vec_push(&_v, &_ve0_1); int64_t _ve0_2 = 3LL; rt_vec_push(&_v, &_ve0_2); int64_t _ve0_3 = 4LL; rt_vec_push(&_v, &_ve0_3); int64_t _ve0_4 = 5LL; rt_vec_push(&_v, &_ve0_4); _v; }), _i);
total = (total + i);
}
;
printf("%s%lld\n", rt_string_from("Total: ").data, (long long)(total));
return 0LL;
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
