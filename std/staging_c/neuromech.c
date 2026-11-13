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
    double coxa_angle;
    double femur_angle;
    double tibia_angle;
    int is_grounded;
    double phase;
} HexapodLeg;

typedef struct {
    double pos_x;
    double pos_y;
    double heading;
    double linear_velocity;
    double angular_velocity;
    double cpg_frequency;
} HexapodBot;

HexapodBot HexapodBot_new(double, double);
void HexapodBot_drive(HexapodBot*, double, double);
void HexapodBot_step(HexapodBot*, double);
double HexapodBot_get_pos_x(HexapodBot*);
double HexapodBot_get_pos_y(HexapodBot*);
double HexapodBot_get_heading(HexapodBot*);
// module neuromech
// import prelude::*
HexapodBot HexapodBot_new(double start_x, double start_y) {
return (HexapodBot){.pos_x = start_x, .pos_y = start_y, .heading = 0, .linear_velocity = 0, .angular_velocity = 0, .cpg_frequency = 2};
}

void HexapodBot_drive(HexapodBot* self, double throttle, double steer) {
(self->linear_velocity = (throttle * 45));
(self->angular_velocity = (steer * 2.2));
}

void HexapodBot_step(HexapodBot* self, double dt_sec) {
(self->heading = ((*self).heading + ((*self).angular_velocity * dt_sec)));
(self->pos_x = ((*self).pos_x + ((cos(math, (*self).heading) * (*self).linear_velocity) * dt_sec)));
(self->pos_y = ((*self).pos_y + ((sin(math, (*self).heading) * (*self).linear_velocity) * dt_sec)));
}

double HexapodBot_get_pos_x(HexapodBot* self) {
return (*self).pos_x;
}

double HexapodBot_get_pos_y(HexapodBot* self) {
return (*self).pos_y;
}

double HexapodBot_get_heading(HexapodBot* self) {
return (*self).heading;
}

