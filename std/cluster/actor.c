#include <stdio.h>
#include "runtime/rt_io.h"
#include "runtime/rt_option.h"
#include "runtime/rt_async.h"
#include "runtime/rt_arena.h"
#include <string.h>
#include "runtime/rt_string.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_json.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_db.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "runtime/rt_map.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_http.h"
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
    rt_string_t actor_id;
    rt_string_t node_id;
    int64_t mailbox_capacity;
    int is_active;
} ClusterActorRef;

ClusterActorRef ClusterActorRef_new(rt_string_t, rt_string_t);
int ClusterActorRef_submit_remote_write(ClusterActorRef*, int64_t);
// module std.cluster.actor
ClusterActorRef ClusterActorRef_new(rt_string_t id, rt_string_t node) {
return (ClusterActorRef){.actor_id = id, .node_id = node, .mailbox_capacity = 65536LL, .is_active = 1};
}

int ClusterActorRef_submit_remote_write(ClusterActorRef* self, int64_t payload_size) {
if ((payload_size > 0LL)) {
return 1;
} else {
return 0;
}
}

