#include <math.h>
#include "runtime/rt_vec.h"
#include "runtime/rt_db.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_io.h"
#include "runtime/rt_map.h"
#include "runtime/rt_json.h"
#include "runtime/rt_option.h"
#include "runtime/rt_http.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "runtime/rt_string.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_async.h"
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
    int64_t region_id;
    int64_t allocated_bytes;
    int64_t capacity_bytes;
    double gc_pause_micros;
    int is_overlay_active;
} MemoryRegionProfile;

MemoryRegionProfile MemoryRegionProfile_new(int64_t, int64_t);
int64_t MemoryRegionProfile_record_allocation(MemoryRegionProfile*, int64_t);
double MemoryRegionProfile_get_utilization_ratio(MemoryRegionProfile*);
// module std.ui.profiler
MemoryRegionProfile MemoryRegionProfile_new(int64_t id, int64_t capacity) {
return (MemoryRegionProfile){.region_id = id, .allocated_bytes = 0LL, .capacity_bytes = capacity, .gc_pause_micros = 0, .is_overlay_active = 1};
}

int64_t MemoryRegionProfile_record_allocation(MemoryRegionProfile* self, int64_t bytes) {
int64_t new_total = ((*self).allocated_bytes + bytes);
return new_total;
}

double MemoryRegionProfile_get_utilization_ratio(MemoryRegionProfile* self) {
if (((*self).capacity_bytes > 0LL)) {
double used = (double)((*self).allocated_bytes);
double total = (double)((*self).capacity_bytes);
return (used / total);
} else {
return 0;
}
}

