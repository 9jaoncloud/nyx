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
    int64_t x;
    int64_t y;
    int64_t timestamp_us;
    int64_t polarity;
} VisionEvent;

typedef struct {
    int64_t width;
    int64_t height;
    int64_t total_events;
} EventCameraSensor;

EventCameraSensor EventCameraSensor_new(int64_t, int64_t);
void EventCameraSensor_process_batch(EventCameraSensor*, NyxVec);
int64_t EventCameraSensor_get_total_events(EventCameraSensor*);
// module event_vision
// import prelude::*
EventCameraSensor EventCameraSensor_new(int64_t width, int64_t height) {
return (EventCameraSensor){.width = width, .height = height, .total_events = (int64_t)(0LL)};
}

void EventCameraSensor_process_batch(EventCameraSensor* self, NyxVec events) {
(self->total_events = ((*self).total_events + (int64_t)(events.len)));
}

int64_t EventCameraSensor_get_total_events(EventCameraSensor* self) {
return (*self).total_events;
}

