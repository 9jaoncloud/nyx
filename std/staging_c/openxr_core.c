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

#define TAG_XREye_Left 0
#define TAG_XREye_Right 1
typedef struct {
    int tag;
    union {
    } data;
} XREye;

#define TAG_XRHand_Left 0
#define TAG_XRHand_Right 1
typedef struct {
    int tag;
    union {
    } data;
} XRHand;

typedef struct {
    Vec3 palm;
    Vec3 wrist;
    Vec3 thumb_tip;
    Vec3 index_tip;
    Vec3 middle_tip;
    Vec3 ring_tip;
    Vec3 pinky_tip;
    double pinch_strength;
    double grab_strength;
} XRHandJoints;

typedef struct {
    Vec3 position;
    void* orientation;
    Vec3 linear_velocity;
    Vec3 angular_velocity;
} XRPose;

typedef struct {
    XREye eye;
    void* view_matrix;
    void* proj_matrix;
    double fov_left_rad;
    double fov_right_rad;
    double fov_up_rad;
    double fov_down_rad;
} XREyeFrustum;

typedef struct {
    rt_string_t app_name;
    int64_t target_fps;
    int enable_passthrough;
    int enable_hand_tracking;
    int enable_foveated_rendering;
    double interpupillary_distance_mm;
} XRSessionConfig;

typedef struct {
    int64_t handle;
    XRSessionConfig config;
    int is_active;
} XRSession;

NyxResult XRSession_init(XRSessionConfig);
int XRSession_is_running(XRSession*);
NyxResult XRSession_locate_head_pose(XRSession*, int64_t);
NyxResult XRSession_locate_hand_joints(XRSession*, XRHand);
XREyeFrustum XRSession_get_eye_frustum(XRSession*, XREye, XRPose*);
void XRSession_end_session(XRSession*);
// module openxr_core
// import prelude::*
// import spatial::*
NyxResult XRSession_init(XRSessionConfig config) {
printf("%s\n", rt_string_concat(rt_string_from("🥽 [OpenXR] Initializing Native Spatial Session for: "), ({ rt_string_t _c = config.app_name; rt_string_retain(&_c); _c; })).data);
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_from("⚡ [OpenXR] Target Display Pacing: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)config.target_fps); _buf.ref_count = 1; _buf; })), rt_string_from(" FPS (Zero-GC Multiview)")).data);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ XRSession _stmp_ = (XRSession){.handle = (int64_t)(8974337LL), .config = config, .is_active = 1}; &_stmp_; }); _tmp; });
}

int XRSession_is_running(XRSession* self) {
return (*self).is_active;
}

NyxResult XRSession_locate_head_pose(XRSession* self, int64_t predicted_time_ns) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ XRPose _stmp_ = (XRPose){.position = Vec3_new(0, 1.7, 0), .orientation = int64_t_identity(Quat), .linear_velocity = Vec3_zero(), .angular_velocity = Vec3_zero()}; &_stmp_; }); _tmp; });
}

NyxResult XRSession_locate_hand_joints(XRSession* self, XRHand hand) {
double side_x = ({ double __if_res_1; if ((hand.tag == (XRHand){ .tag = TAG_XRHand_Left }.tag)) {
__if_res_1 = (-0.25);
} else {
__if_res_1 = 0.25;
}
__if_res_1; });
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ XRHandJoints _stmp_ = (XRHandJoints){.palm = Vec3_new(side_x, 1.1, (-0.4)), .wrist = Vec3_new(side_x, 1.05, (-0.45)), .thumb_tip = Vec3_new((side_x - 0.03), 1.15, (-0.38)), .index_tip = Vec3_new(side_x, 1.18, (-0.36)), .middle_tip = Vec3_new((side_x + 0.02), 1.17, (-0.36)), .ring_tip = Vec3_new((side_x + 0.04), 1.15, (-0.37)), .pinky_tip = Vec3_new((side_x + 0.06), 1.12, (-0.38)), .pinch_strength = 0, .grab_strength = 0}; &_stmp_; }); _tmp; });
}

XREyeFrustum XRSession_get_eye_frustum(XRSession* self, XREye eye, XRPose* head) {
double ipd_meters = (((*self).config).interpupillary_distance_mm / 1000);
double eye_offset = ({ double __if_res_2; if ((eye.tag == (XREye){ .tag = TAG_XREye_Left }.tag)) {
__if_res_2 = (-(ipd_meters / 2));
} else {
__if_res_2 = (ipd_meters / 2);
}
__if_res_2; });
Vec3 eye_pos = Vec3_new((((*head).position).x + eye_offset), ((*head).position).y, ((*head).position).z);
int64_t view = int64_t_look_at(Mat4, eye_pos, Vec3_new(eye_pos.x, eye_pos.y, (eye_pos.z - 1)), Vec3_new(0, 1, 0));
int64_t proj = int64_t_perspective(Mat4, 1.5708, 1, 0.05, 1000);
return (XREyeFrustum){.eye = eye, .view_matrix = view, .proj_matrix = proj, .fov_left_rad = 0.7854, .fov_right_rad = 0.7854, .fov_up_rad = 0.7854, .fov_down_rad = 0.7854};
}

void XRSession_end_session(XRSession* self) {
(self->is_active = 0);
printf("%s\n", rt_string_from("🥽 [OpenXR] Session closed gracefully.").data);
}

