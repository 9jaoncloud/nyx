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
    int64_t width;
    int64_t height;
    NyxVec depth_buffer_meters;
} SpatialDepthMap;

typedef struct {
    double near_clip_m;
    double far_clip_m;
    double occlusion_softness;
} PassthroughOcclusionFilter;

void evaluate_passthrough_occlusion(SpatialDepthMap*, NyxVec*, NyxVec*);
// module visionpro_bridge
// import prelude::*
void evaluate_passthrough_occlusion(SpatialDepthMap* env_depth, NyxVec* virtual_geometry_depth, NyxVec* out_alpha_mask) {
Arena* __arena = arena_create();
int64_t pixel_count = (*env_depth).depth_buffer_meters.len;
for (int64_t i = 0LL; i < pixel_count; i++) {
({ if (((i >= virtual_geometry_depth->len) || (i >= out_alpha_mask->len))) {
break;
}
});
int64_t real_z = (*(int64_t*)rt_vec_get((*env_depth).depth_buffer_meters, i));
double virt_z = (*(double*)rt_vec_get(*virtual_geometry_depth, i));
({ if ((virt_z > real_z)) {
(out_alpha_mask[i] = (int64_t)(0LL));
} else {
(out_alpha_mask[i] = (int64_t)(255LL));
}
});
}
;
arena_destroy(__arena);
}

