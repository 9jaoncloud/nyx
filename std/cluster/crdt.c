#include <stdint.h>
#include "runtime/rt_http.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_async.h"
#include "runtime/rt_string.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_option.h"
#include "runtime/rt_db.h"
#include "runtime/rt_io.h"
#include "runtime/rt_arena.h"
#include <stdio.h>
#include <string.h>
#include "runtime/rt_vec.h"
#include <math.h>
#include "runtime/rt_json.h"
#include <stdlib.h>
#include "runtime/rt_map.h"
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

typedef struct {
    int64_t vector_clock;
    int64_t node_count;
    int is_converged;
} EpistemicStateCrdt;

EpistemicStateCrdt EpistemicStateCrdt_new();
int64_t EpistemicStateCrdt_merge_state(EpistemicStateCrdt*, int64_t);
// module std.cluster.crdt
EpistemicStateCrdt EpistemicStateCrdt_new() {
return (EpistemicStateCrdt){.vector_clock = 1LL, .node_count = 3LL, .is_converged = 1};
}

int64_t EpistemicStateCrdt_merge_state(EpistemicStateCrdt* self, int64_t remote_clock) {
if ((remote_clock > (*self).vector_clock)) {
return remote_clock;
} else {
return (*self).vector_clock;
}
}

