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
    rt_string_t memory_id;
    rt_string_t agent_id;
    Point location;
    rt_string_t category;
    rt_string_t content;
    int64_t timestamp;
} SpatialMemoryEntity;

typedef struct {
    rt_string_t store_name;
    NyxVec memories;
} SpatialAgentMemoryStore;

SpatialMemoryEntity SpatialMemoryEntity_new(rt_string_t, rt_string_t, double, double, rt_string_t, rt_string_t, int64_t);
SpatialAgentMemoryStore SpatialAgentMemoryStore_new(rt_string_t);
void SpatialAgentMemoryStore_insert_memory(SpatialAgentMemoryStore*, SpatialMemoryEntity);
NyxVec SpatialAgentMemoryStore_recall_nearby_memories(SpatialAgentMemoryStore*, void*, double);
NyxVec SpatialAgentMemoryStore_recall_by_agent_and_category(SpatialAgentMemoryStore*, rt_string_t, rt_string_t);
// module memory
// import std::io
// import std::vec
// import std::map
// import std::gis::geo
SpatialMemoryEntity SpatialMemoryEntity_new(rt_string_t id, rt_string_t agent_id, double lat, double lon, rt_string_t category, rt_string_t text, int64_t time_ms) {
return (SpatialMemoryEntity){.memory_id = id, .agent_id = agent_id, .location = Point_new(lat, lon), .category = category, .content = text, .timestamp = time_ms};
}

SpatialAgentMemoryStore SpatialAgentMemoryStore_new(rt_string_t name) {
return (SpatialAgentMemoryStore){.store_name = name, .memories = rt_vec_new(sizeof(int64_t))};
}

void SpatialAgentMemoryStore_insert_memory(SpatialAgentMemoryStore* self, SpatialMemoryEntity mem) {
rt_vec_push(&(*self).memories, ({ SpatialMemoryEntity _tmp_0 = (int64_t)(mem); &_tmp_0; }));
}

NyxVec SpatialAgentMemoryStore_recall_nearby_memories(SpatialAgentMemoryStore* self, void* agent_pos, double radius_km) {
NyxVec results = rt_vec_new(sizeof(SpatialMemoryEntity));
for (int64_t i = 0LL; i < (*self).memories.len; i++) {
int64_t m = rt_vec_get_at((*self).memories, (size_t)(i));
int64_t dist = distance_to_km(agent_pos, (&(m.location)));
({ if ((dist <= radius_km)) {
rt_vec_push(&results, ({ int64_t _tmp_2 = (int64_t)(m); &_tmp_2; }));
}
});
}
;
return results;
}

NyxVec SpatialAgentMemoryStore_recall_by_agent_and_category(SpatialAgentMemoryStore* self, rt_string_t agent_id, rt_string_t category) {
NyxVec results = rt_vec_new(sizeof(SpatialMemoryEntity));
for (int64_t i = 0LL; i < (*self).memories.len; i++) {
int64_t m = rt_vec_get_at((*self).memories, (size_t)(i));
({ if ((rt_string_eq(m.agent_id, agent_id) && rt_string_eq(m.category, category))) {
rt_vec_push(&results, ({ int64_t _tmp_4 = (int64_t)(m); &_tmp_4; }));
}
});
}
;
return results;
}

