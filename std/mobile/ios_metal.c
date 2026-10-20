#include "runtime/rt_db.h"
#include <stdio.h>
#include <stdlib.h>
#include "runtime/rt_map.h"
#include <stdint.h>
#include "runtime/rt_vec.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_io.h"
#include "runtime/rt_async.h"
#include <math.h>
#include "runtime/rt_option.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_string.h"
#include <string.h>
#include "runtime/rt_json.h"
#include "runtime/rt_http.h"
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
    int64_t surface_width;
    int64_t surface_height;
    int64_t sample_count;
    int is_direct_scanout;
} MetalSwapchain;

MetalSwapchain MetalSwapchain_new(int64_t, int64_t);
int MetalSwapchain_present_frame_zero_copy(MetalSwapchain*);
// module std.mobile.ios_metal
MetalSwapchain MetalSwapchain_new(int64_t w, int64_t h) {
return (MetalSwapchain){.surface_width = w, .surface_height = h, .sample_count = 4LL, .is_direct_scanout = 1};
}

int MetalSwapchain_present_frame_zero_copy(MetalSwapchain* self) {
if (((*self).is_direct_scanout == 1)) {
return 1;
} else {
return 0;
}
}

