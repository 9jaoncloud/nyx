#include "runtime/rt_async.h"
#include "runtime/rt_arena.h"
#include <math.h>
#include "runtime/rt_string.h"
#include "runtime/rt_stdlib.h"
#include <stdio.h>
#include <string.h>
#include "runtime/rt_db.h"
#include <stdint.h>
#include "runtime/rt_io.h"
#include <stdlib.h>
#include "runtime/rt_json.h"
#include "runtime/rt_http.h"
#include "runtime/rt_option.h"
#include "runtime/rt_map.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_vec.h"
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
    int64_t neuron_count;
    int64_t synapse_count;
    double decay_factor;
    double membrane_threshold;
    int64_t live_spikes_offset;
    int64_t target_fps;
} FlyBrainConnectome;

FlyBrainConnectome FlyBrainConnectome_new_drosophila_v1();
int64_t FlyBrainConnectome_compute_step_simd(FlyBrainConnectome*, int64_t);
// module std.simd.connectome
FlyBrainConnectome FlyBrainConnectome_new_drosophila_v1() {
return (FlyBrainConnectome){.neuron_count = 139255LL, .synapse_count = 54500000LL, .decay_factor = 0.95, .membrane_threshold = 1, .live_spikes_offset = 675584LL, .target_fps = 120LL};
}

int64_t FlyBrainConnectome_compute_step_simd(FlyBrainConnectome* self, int64_t live_spikes) {
if ((live_spikes > 0LL)) {
return live_spikes;
} else {
return 0LL;
}
}

