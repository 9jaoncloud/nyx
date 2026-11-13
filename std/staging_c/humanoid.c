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
    int64_t joint_id;
    double target_angle_rad;
    double current_angle_rad;
    double torque_nm;
    double velocity_rad_s;
} JointState;

typedef struct {
    int64_t muscle_id;
    double tension_newtons;
    double max_tension;
    double activation_level;
} NeuromuscularActuator;

typedef struct {
    NyxVec joints;
    NyxVec actuators;
    double center_of_mass_x;
    double center_of_mass_y;
    double center_of_mass_z;
    double zero_moment_point_x;
    double zero_moment_point_y;
} BipedalHumanoidKinematics;

BipedalHumanoidKinematics BipedalHumanoidKinematics_init_32dof();
void BipedalHumanoidKinematics_update_1khz_tick(BipedalHumanoidKinematics*, double);
// module std.robotics.humanoid
// import std::math
// import std::io
BipedalHumanoidKinematics BipedalHumanoidKinematics_init_32dof() {
NyxVec joints_vec = rt_vec_new(sizeof(int64_t));
NyxVec act_vec = rt_vec_new(sizeof(int64_t));
for (int64_t i = 0LL; i < 32LL; i++) {
rt_vec_push(&joints_vec, ({ JointState _tmp_0 = (int64_t)((JointState){.joint_id = (int64_t)(i), .target_angle_rad = 0, .current_angle_rad = 0, .torque_nm = 0, .velocity_rad_s = 0}); &_tmp_0; }));
rt_vec_push(&act_vec, ({ NeuromuscularActuator _tmp_1 = (int64_t)((NeuromuscularActuator){.muscle_id = (int64_t)(i), .tension_newtons = 50, .max_tension = 450, .activation_level = 0.15}); &_tmp_1; }));
}
;
return (BipedalHumanoidKinematics){.joints = joints_vec, .actuators = act_vec, .center_of_mass_x = 0, .center_of_mass_y = 0, .center_of_mass_z = 0.85, .zero_moment_point_x = 0, .zero_moment_point_y = 0};
}

void BipedalHumanoidKinematics_update_1khz_tick(BipedalHumanoidKinematics* self, double dt_seconds) {
(self->zero_moment_point_x = ((*self).center_of_mass_x - (((*self).center_of_mass_z / 9.81) * 0.02)));
(self->zero_moment_point_y = ((*self).center_of_mass_y - (((*self).center_of_mass_z / 9.81) * 0.01)));
for (int64_t i = 0LL; i < (*self).joints.len; i++) {
double error = (((*(int64_t*)rt_vec_get((*self).joints, i))).target_angle_rad - ((*(int64_t*)rt_vec_get((*self).joints, i))).current_angle_rad);
double kp = 180;
double kd = 12.5;
double computed_torque = ((kp * error) - (kd * ((*(int64_t*)rt_vec_get((*self).joints, i))).velocity_rad_s));
((*(int64_t*)rt_vec_get((*self).joints, i)).torque_nm = computed_torque);
((*(int64_t*)rt_vec_get((*self).joints, i)).current_angle_rad = (((*(int64_t*)rt_vec_get((*self).joints, i))).current_angle_rad + (((*(int64_t*)rt_vec_get((*self).joints, i))).velocity_rad_s * dt_seconds)));
((*(int64_t*)rt_vec_get((*self).actuators, i)).activation_level = min(1, max(0, (abs(computed_torque) / 200))));
}
;
}

