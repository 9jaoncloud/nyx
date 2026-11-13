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

typedef struct {
    rt_string_t node_id;
    int64_t counter;
} ClockEntry;

typedef struct {
    NyxVec entries;
} VectorClock;

typedef struct {
    rt_string_t value;
    int64_t timestamp_epoch_ms;
    rt_string_t node_id;
} LwwRegister;

typedef struct {
    rt_string_t item;
    rt_string_t tag_uuid;
    int is_deleted;
} OrSetElement;

typedef struct {
    NyxVec elements;
    rt_string_t node_id;
} ObservedRemoveSet;

typedef struct {
    rt_string_t mutation_id;
    rt_string_t entity_table;
    rt_string_t payload_crdt_json;
    int64_t timestamp_ms;
    int64_t retry_attempts;
} OfflineMutation;

typedef struct {
    rt_string_t database_name;
    int is_hardware_encrypted;
    NyxVec mutation_queue;
    int is_online;
} OfflineFirstStore;

VectorClock VectorClock_new(rt_string_t);
void VectorClock_increment(VectorClock*, rt_string_t);
LwwRegister LwwRegister_new(rt_string_t, rt_string_t);
void LwwRegister_set(LwwRegister*, rt_string_t, int64_t);
int LwwRegister_merge(LwwRegister*, LwwRegister);
ObservedRemoveSet ObservedRemoveSet_new(rt_string_t);
void ObservedRemoveSet_add(ObservedRemoveSet*, rt_string_t, rt_string_t);
void ObservedRemoveSet_nyx_remove(ObservedRemoveSet*, rt_string_t);
int ObservedRemoveSet_contains(ObservedRemoveSet*, rt_string_t);
void ObservedRemoveSet_merge(ObservedRemoveSet*, ObservedRemoveSet);
OfflineFirstStore OfflineFirstStore_open(rt_string_t);
rt_string_t OfflineFirstStore_enqueue_offline_write(OfflineFirstStore*, rt_string_t, rt_string_t);
int64_t OfflineFirstStore_flush_sync_queue(OfflineFirstStore*);
// module sync
// import std::string
// import std::vec
// import std::option
// import std::result
VectorClock VectorClock_new(rt_string_t local_node_id) {
NyxVec entries = rt_vec_new(sizeof(int64_t));
rt_vec_push(&entries, ({ ClockEntry _tmp_0 = (int64_t)((ClockEntry){.node_id = local_node_id, .counter = 1LL}); &_tmp_0; }));
return (VectorClock){.entries = entries};
}

void VectorClock_increment(VectorClock* self, rt_string_t local_node_id) {
for (int64_t i = 0LL; i < (*self).entries.len; i++) {
({ if (rt_string_eq(((*(int64_t*)rt_vec_get((*self).entries, i))).node_id, local_node_id)) {
((*(int64_t*)rt_vec_get((*self).entries, i)).counter = (((*(int64_t*)rt_vec_get((*self).entries, i))).counter + 1LL));
return;
}
});
}
;
rt_vec_push(&(*self).entries, ({ ClockEntry _tmp_2 = (int64_t)((ClockEntry){.node_id = local_node_id, .counter = 1LL}); &_tmp_2; }));
}

LwwRegister LwwRegister_new(rt_string_t initial_val, rt_string_t node_id) {
return (LwwRegister){.value = initial_val, .timestamp_epoch_ms = 1741180000000LL, .node_id = node_id};
}

void LwwRegister_set(LwwRegister* self, rt_string_t new_val, int64_t epoch_ms) {
(self->value = new_val);
(self->timestamp_epoch_ms = epoch_ms);
}

int LwwRegister_merge(LwwRegister* self, LwwRegister remote) {
({ if ((remote.timestamp_epoch_ms > (*self).timestamp_epoch_ms)) {
(self->value = remote.value);
(self->timestamp_epoch_ms = remote.timestamp_epoch_ms);
(self->node_id = remote.node_id);
return 1;
} else if ((remote.timestamp_epoch_ms == (*self).timestamp_epoch_ms)) {
({ if ((remote.node_id > (*self).node_id)) {
(self->value = remote.value);
(self->node_id = remote.node_id);
return 1;
}
});
}
});
return 0;
}

ObservedRemoveSet ObservedRemoveSet_new(rt_string_t node_id) {
return (ObservedRemoveSet){.elements = rt_vec_new(sizeof(int64_t)), .node_id = node_id};
}

void ObservedRemoveSet_add(ObservedRemoveSet* self, rt_string_t item, rt_string_t tag_uuid) {
rt_vec_push(&(*self).elements, ({ OrSetElement _tmp_5 = (int64_t)((OrSetElement){.item = item, .tag_uuid = tag_uuid, .is_deleted = 0}); &_tmp_5; }));
}

void ObservedRemoveSet_remove(ObservedRemoveSet* self, rt_string_t item) {
for (int64_t i = 0LL; i < (*self).elements.len; i++) {
({ if (rt_string_eq(((*(int64_t*)rt_vec_get((*self).elements, i))).item, item)) {
((*(int64_t*)rt_vec_get((*self).elements, i)).is_deleted = 1);
}
});
}
;
}

int ObservedRemoveSet_contains(ObservedRemoveSet* self, rt_string_t item) {
for (int64_t i = 0LL; i < (*self).elements.len; i++) {
({ if ((rt_string_eq(((*(int64_t*)rt_vec_get((*self).elements, i))).item, item) && (!((*(int64_t*)rt_vec_get((*self).elements, i))).is_deleted))) {
return 1;
}
});
}
;
return 0;
}

void ObservedRemoveSet_merge(ObservedRemoveSet* self, ObservedRemoveSet remote) {
for (int64_t r = 0LL; r < remote.elements.len; r++) {
int64_t rem_elem = (&((*(int64_t*)rt_vec_get(remote.elements, r))));
int found = 0;
for (int64_t l = 0LL; l < (*self).elements.len; l++) {
({ if (rt_string_eq(((*(int64_t*)rt_vec_get((*self).elements, l))).tag_uuid, rem_elem.tag_uuid)) {
found = 1;
({ if (rem_elem.is_deleted) {
((*(int64_t*)rt_vec_get((*self).elements, l)).is_deleted = 1);
}
});
break;
}
});
}
;
({ if ((!found)) {
rt_vec_push(&(*self).elements, ({ int64_t _tmp_11 = (int64_t)(rem_elem); &_tmp_11; }));
}
});
}
;
}

OfflineFirstStore OfflineFirstStore_open(rt_string_t db_name) {
return (OfflineFirstStore){.database_name = db_name, .is_hardware_encrypted = 1, .mutation_queue = rt_vec_new(sizeof(int64_t)), .is_online = 1};
}

rt_string_t OfflineFirstStore_enqueue_offline_write(OfflineFirstStore* self, rt_string_t table, rt_string_t payload) {
rt_string_t tx_id = rt_string_concat(rt_string_from("tx_crdt_"), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)((*self).mutation_queue.len + 1LL)); _buf.ref_count = 1; _buf; }));
rt_vec_push(&(*self).mutation_queue, ({ OfflineMutation _tmp_12 = (int64_t)((OfflineMutation){.mutation_id = ({ rt_string_t _c = tx_id; rt_string_retain(&_c); _c; }), .entity_table = table, .payload_crdt_json = payload, .timestamp_ms = 1741180000000LL, .retry_attempts = 0LL}); &_tmp_12; }));
return tx_id;
}

int64_t OfflineFirstStore_flush_sync_queue(OfflineFirstStore* self) {
int64_t count = (int64_t)((*self).mutation_queue.len);
rt_vec_clear(&((*self).mutation_queue));
return count;
}

