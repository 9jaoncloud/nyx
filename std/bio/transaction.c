#include "runtime/rt_json.h"
#include "runtime/rt_io.h"
#include "runtime/rt_vec.h"
#include <stdio.h>
#include "runtime/rt_map.h"
#include "runtime/rt_stdlib.h"
#include <stdint.h>
#include "runtime/rt_option.h"
#include "runtime/rt_async.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_string.h"
#include "runtime/rt_http.h"
#include <math.h>
#include <string.h>
#include "runtime/rt_arena.h"
#include <stdlib.h>
#include "runtime/rt_db.h"
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
    int64_t target_channels;
    int64_t channels_to_interrupt;
    int is_committed;
} StimPlan;

typedef struct {
    int64_t sampling_rate_hz;
    int64_t deadline_micros;
    int64_t total_spikes_processed;
} ClosedLoopController;

StimPlan StimPlan_new();
int64_t StimPlan_sync_channels(StimPlan*, int64_t);
int StimPlan_run_atomic(StimPlan*);
ClosedLoopController ClosedLoopController_new_25khz();
int ClosedLoopController_verify_deadline(ClosedLoopController*, int64_t);
// module std.bio.transaction
StimPlan StimPlan_new() {
return (StimPlan){.target_channels = 0LL, .channels_to_interrupt = 0LL, .is_committed = 0};
}

int64_t StimPlan_sync_channels(StimPlan* self, int64_t channels) {
int64_t combined = ((*self).target_channels + channels);
return combined;
}

int StimPlan_run_atomic(StimPlan* self) {
if (((*self).target_channels >= 0LL)) {
return 1;
} else {
return 0;
}
}

ClosedLoopController ClosedLoopController_new_25khz() {
return (ClosedLoopController){.sampling_rate_hz = 25000LL, .deadline_micros = 33LL, .total_spikes_processed = 0LL};
}

int ClosedLoopController_verify_deadline(ClosedLoopController* self, int64_t elapsed_micros) {
if ((elapsed_micros <= (*self).deadline_micros)) {
return 1;
} else {
return 0;
}
}

