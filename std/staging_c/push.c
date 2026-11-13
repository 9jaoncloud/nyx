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

#define TAG_PushPlatform_AppleApns 0
#define TAG_PushPlatform_GoogleFcm 1
#define TAG_PushPlatform_NyxMeshDirect 2
typedef struct {
    int tag;
    union {
    } data;
} PushPlatform;

#define TAG_PushPriority_HighImmediate 0
#define TAG_PushPriority_NormalEnergySaving 1
#define TAG_PushPriority_SilentBackground 2
typedef struct {
    int tag;
    union {
    } data;
} PushPriority;

typedef struct {
    rt_string_t action_id;
    rt_string_t label;
    int is_destructive;
    int requires_authentication;
} NotificationAction;

typedef struct {
    rt_string_t message_id;
    rt_string_t title;
    rt_string_t body;
    NyxOption deep_link_uri;
    PushPriority priority;
    NyxOption badge_count;
    NyxOption sound_name;
    rt_string_t custom_data_json;
    NyxVec actions;
} UnifiedPushPayload;

typedef struct {
    rt_string_t scheme;
    rt_string_t host;
    rt_string_t path;
    NyxVec query_params;
} DeepLinkRoute;

typedef struct {
    rt_string_t device_token;
    PushPlatform active_platform;
    int is_registered;
    int64_t unread_badge;
} PushManager;

NyxOption DeepLinkRoute_parse(rt_string_t);
PushManager PushManager_new(PushPlatform);
int PushManager_handle_incoming_notification(PushManager*, UnifiedPushPayload);
void PushManager_register_action_category(PushManager*, rt_string_t, NyxVec);
// module push
// import std::string
// import std::vec
// import std::option
// import std::result
NyxOption DeepLinkRoute_parse(rt_string_t raw_uri) {
return (std.option.Option){ .tag = TAG_std.option.Option_Some, .data.Some = {.f0 = (DeepLinkRoute){.scheme = rt_string_from("nyx"), .host = rt_string_from("wallet"), .path = rt_string_from("/confirm_payment"), .query_params = rt_vec_new(sizeof(int64_t))}} };
}

PushManager PushManager_new(PushPlatform platform) {
PushManager token = ({
rt_string_t __match_result_0;
PushPlatform __match_val_0 = platform;
if (__match_val_0.tag == TAG_PushPlatform_AppleApns) {
__match_result_0 = rt_string_from("apns_dev_tok_84f901...apple");
}
 else if (__match_val_0.tag == TAG_PushPlatform_GoogleFcm) {
__match_result_0 = rt_string_from("fcm_v1_d892a_cba...google");
}
 else if (__match_val_0.tag == TAG_PushPlatform_NyxMeshDirect) {
__match_result_0 = rt_string_from("nyx_p2p_node_mesh_771");
}
__match_result_0;
});
return (PushManager){.device_token = PushManager_to_string(&token), .active_platform = platform, .is_registered = 1, .unread_badge = 0LL};
}

int PushManager_handle_incoming_notification(PushManager* self, UnifiedPushPayload payload) {
/* iflet */ 0;
({ if ((payload.priority.tag == (PushPriority){ .tag = TAG_PushPriority_SilentBackground }.tag)) {
return 1;
}
});
return 1;
}

void PushManager_register_action_category(PushManager* self, rt_string_t category_id, NyxVec actions) {
}

