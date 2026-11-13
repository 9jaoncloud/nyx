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

int64_t select_upstream_round_robin(int64_t, int64_t);
int64_t select_least_conn(int64_t, int64_t);
int waf_inspect_request(rt_string_t, rt_string_t, rt_string_t);
int64_t select_upstream_round_robin(int64_t upstreams_count, int64_t current_index) {
return ((current_index + 1LL) % upstreams_count);
}

int64_t select_least_conn(int64_t upstreams_count, int64_t active_conns_lowest_index) {
return active_conns_lowest_index;
}

int waf_inspect_request(rt_string_t path, rt_string_t headers, rt_string_t body) {
({ if ((path.length > 2048LL)) {
return 0;
}
});
return 1;
}

