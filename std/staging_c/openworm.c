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
    double voltage;
    double threshold;
    double refractory_ms;
    int spiked;
} WormNeuron;

typedef struct {
    double activation;
    double phase;
    double pos_x;
    double pos_y;
} WormMuscle;

typedef struct {
    double head_x;
    double head_y;
    double heading_angle;
    double speed;
    int64_t total_spikes;
} OpenWormOrganism;

OpenWormOrganism OpenWormOrganism_new(double, double);
void OpenWormOrganism_sense_chemical(OpenWormOrganism*, double, double, double);
int64_t OpenWormOrganism_step(OpenWormOrganism*, double);
double OpenWormOrganism_get_head_x(OpenWormOrganism*);
double OpenWormOrganism_get_head_y(OpenWormOrganism*);
int64_t OpenWormOrganism_get_total_spikes(OpenWormOrganism*);
// module openworm
// import prelude::*
OpenWormOrganism OpenWormOrganism_new(double start_x, double start_y) {
return (OpenWormOrganism){.head_x = start_x, .head_y = start_y, .heading_angle = 0, .speed = 3.5, .total_spikes = (int64_t)(0LL)};
}

void OpenWormOrganism_sense_chemical(OpenWormOrganism* self, double target_x, double target_y, double attraction_strength) {
double dx = (target_x - (*self).head_x);
double dy = (target_y - (*self).head_y);
double desired = int64_t_atan2(math, dy, dx);
(self->heading_angle = ((*self).heading_angle + ((desired - (*self).heading_angle) * 0.05)));
}

int64_t OpenWormOrganism_step(OpenWormOrganism* self, double dt_ms) {
(self->head_x = ((*self).head_x + (cos(math, (*self).heading_angle) * (*self).speed)));
(self->head_y = ((*self).head_y + (sin(math, (*self).heading_angle) * (*self).speed)));
(self->total_spikes = ((*self).total_spikes + (int64_t)(12LL)));
return (int64_t)(12LL);
}

double OpenWormOrganism_get_head_x(OpenWormOrganism* self) {
return (*self).head_x;
}

double OpenWormOrganism_get_head_y(OpenWormOrganism* self) {
return (*self).head_y;
}

int64_t OpenWormOrganism_get_total_spikes(OpenWormOrganism* self) {
return (*self).total_spikes;
}

