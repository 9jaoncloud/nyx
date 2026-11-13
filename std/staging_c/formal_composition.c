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

int prove_layer4_goal_safety(double, double);
double prove_layer6_kinematic_bounds(double, double, double);
double prove_wbc_qp_torque_bounds(double, double);
int prove_coulomb_friction_cone(double, double, double);
int verify_end_to_end_composition_pipeline(double, double, double, double, double);
// module formal_composition
// import prelude::*
// import formal::*
int prove_layer4_goal_safety(double goal_risk, double goal_priority) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (goal_risk < 0.25);
}

double prove_layer6_kinematic_bounds(double q_cmd, double q_min, double q_max) {
Arena* __arena = arena_create();
double clamped = q_cmd;
({ if ((clamped < q_min)) {
clamped = q_min;
} else if ((clamped > q_max)) {
clamped = q_max;
}
});
arena_destroy(__arena);
return clamped;
}

double prove_wbc_qp_torque_bounds(double raw_torque, double tau_limit) {
Arena* __arena = arena_create();
double safe_torque = raw_torque;
({ if ((safe_torque > tau_limit)) {
safe_torque = tau_limit;
} else if ((safe_torque < (-tau_limit))) {
safe_torque = (-tau_limit);
}
});
arena_destroy(__arena);
return safe_torque;
}

int prove_coulomb_friction_cone(double f_normal, double f_tangential, double mu) {
Arena* __arena = arena_create();
double max_tangential = (mu * f_normal);
double abs_tangential = ({ double __if_res_3; if ((f_tangential < 0)) {
__if_res_3 = (-f_tangential);
} else {
__if_res_3 = f_tangential;
}
__if_res_3; });
arena_destroy(__arena);
return (abs_tangential <= max_tangential);
}

int verify_end_to_end_composition_pipeline(double goal_risk, double raw_q_cmd, double raw_torque, double f_normal, double f_tangential) {
Arena* __arena = arena_create();
int l4_safe = prove_layer4_goal_safety(goal_risk, 0.9);
({ if ((!l4_safe)) {
return 0;
}
});
double safe_q = prove_layer6_kinematic_bounds(raw_q_cmd, (-3.14159), 3.14159);
double safe_tau = prove_wbc_qp_torque_bounds(raw_torque, 180);
int contact_safe = prove_coulomb_friction_cone(f_normal, f_tangential, 0.6);
arena_destroy(__arena);
return (((safe_tau >= (-180)) && (safe_tau <= 180)) && contact_safe);
}

