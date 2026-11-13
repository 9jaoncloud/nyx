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

#define TAG_WebRTCState_Disconnected 0
#define TAG_WebRTCState_Connecting 1
#define TAG_WebRTCState_Connected 2
#define TAG_WebRTCState_Failed 3
#define TAG_WebRTCState_Closed 4
typedef struct {
    int tag;
    union {
    } data;
} WebRTCState;

typedef struct {
    rt_string_t id;
    int is_video;
    int64_t sample_rate;
} MediaTrack;

typedef struct {
    rt_string_t url;
    rt_string_t room;
    rt_string_t identity;
    WebRTCState state;
} LiveKitRoom;

LiveKitRoom LiveKitRoom_connect(rt_string_t, rt_string_t, rt_string_t, rt_string_t);
int LiveKitRoom_publish_audio(LiveKitRoom*, rt_string_t);
int LiveKitRoom_publish_camera(LiveKitRoom*, rt_string_t, int64_t, int64_t);
// module webrtc
LiveKitRoom LiveKitRoom_connect(rt_string_t url, rt_string_t room, rt_string_t identity, rt_string_t token) {
return (LiveKitRoom){.url = url, .room = room, .identity = identity, .state = (WebRTCState){ .tag = TAG_WebRTCState_Connected }};
}

int LiveKitRoom_publish_audio(LiveKitRoom* self, rt_string_t track_id) {
return 1;
}

int LiveKitRoom_publish_camera(LiveKitRoom* self, rt_string_t track_id, int64_t width, int64_t height) {
return 1;
}

