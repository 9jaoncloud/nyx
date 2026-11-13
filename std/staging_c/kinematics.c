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
    double a;
    double alpha;
    double d;
    double theta;
} DhJoint;

typedef struct {
    rt_string_t name;
    int64_t dof;
    int64_t joints_count;
    double end_effector_x;
    double end_effector_y;
    double end_effector_z;
} RobotArm;

typedef struct {
    double kp;
    double ki;
    double kd;
    double integral;
    double prev_error;
    double max_output;
} PidController;

RobotArm create_robot_arm(rt_string_t, int64_t);
void add_joint(RobotArm*, double, double, double, double);
PidController create_pid(double, double, double, double);
double update_pid(PidController*, double, double, double);
double evaluate_quintic_trajectory(double, double, double, double);
// module kinematics
// import std::io
// import std::string
// import std::vec
// import std::math
RobotArm create_robot_arm(rt_string_t name, int64_t dof) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (RobotArm){.name = name, .dof = dof, .joints_count = 0LL, .end_effector_x = 0, .end_effector_y = 0, .end_effector_z = 0};
}

void add_joint(RobotArm* arm, double a, double alpha, double d, double theta) {
Arena* __arena = arena_create();
(arm->joints_count = ((*arm).joints_count + 1LL));
double ct = cos(theta);
double st = sin(theta);
(arm->end_effector_x = ((*arm).end_effector_x + (a * ct)));
(arm->end_effector_y = ((*arm).end_effector_y + (a * st)));
(arm->end_effector_z = ((*arm).end_effector_z + d));
arena_destroy(__arena);
}

PidController create_pid(double kp, double ki, double kd, double max_output) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (PidController){.kp = kp, .ki = ki, .kd = kd, .integral = 0, .prev_error = 0, .max_output = max_output};
}

double update_pid(PidController* pid, double setpoint, double measured, double dt) {
Arena* __arena = arena_create();
double error = (setpoint - measured);
(pid->integral = ((*pid).integral + (error * dt)));
double derivative = ((error - (*pid).prev_error) / dt);
(pid->prev_error = error);
double output = ((((*pid).kp * error) + ((*pid).ki * (*pid).integral)) + ((*pid).kd * derivative));
arena_destroy(__arena);
if ((output > (*pid).max_output)) {
return (*pid).max_output;
} else if ((output < (0 - (*pid).max_output))) {
return (0 - (*pid).max_output);
} else {
return output;
}
}

double evaluate_quintic_trajectory(double t, double total_duration, double q0, double q1) {
Arena* __arena = arena_create();
double tau = (t / total_duration);
double s = ({ double __if_res_1; if ((tau > 1)) {
__if_res_1 = 1;
} else if ((tau < 0)) {
__if_res_1 = 0;
} else {
__if_res_1 = tau;
}
__if_res_1; });
double s3 = ((s * s) * s);
double s4 = (s3 * s);
double s5 = (s4 * s);
double blend = (((10 * s3) - (15 * s4)) + (6 * s5));
arena_destroy(__arena);
return (q0 + ((q1 - q0) * blend));
}

