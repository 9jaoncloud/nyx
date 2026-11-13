#include "runtime/rt_string.h"
#include "runtime/rt_db.h"
#include "runtime/rt_http.h"
#include "runtime/rt_json.h"
#include <math.h>
#include "runtime/rt_io.h"
#include <stdlib.h>
#include "runtime/rt_stdlib.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_map.h"
#include "runtime/rt_option.h"
#include <stdint.h>
#include "runtime/rt_vec.h"
#include <stdio.h>
#include "runtime/rt_async.h"
#include <string.h>
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
    int64_t timestamp_micros;
    int64_t channel_id;
    double amplitude_microvolts;
    int is_verified;
} ClSpikeFrame;

typedef struct {
    int64_t target_channel;
    double amplitude_mv;
    int64_t pulse_duration_micros;
} StimDesign;

typedef struct {
    int64_t start_channel;
    int64_t channel_count;
    double frequency_hz;
    int64_t burst_duration_ms;
} BurstDesign;

ClSpikeFrame ClSpikeFrame_new(int64_t, double);
int ClSpikeFrame_validate_biological_bounds(ClSpikeFrame*);
// module std.bio.cl1
ClSpikeFrame ClSpikeFrame_new(int64_t chan, double amp) {
return (ClSpikeFrame){.timestamp_micros = 0LL, .channel_id = chan, .amplitude_microvolts = amp, .is_verified = 1};
}

int ClSpikeFrame_validate_biological_bounds(ClSpikeFrame* self) {
if (((*self).amplitude_microvolts < 5000)) {
return 1;
} else {
return 0;
}
}

