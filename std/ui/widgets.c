#include "runtime/rt_json.h"
#include <stdint.h>
#include "runtime/rt_db.h"
#include "runtime/rt_http.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_io.h"
#include "runtime/rt_map.h"
#include "runtime/rt_option.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_async.h"
#include "runtime/rt_vec.h"
#include <math.h>
#include "runtime/rt_concurrency.h"
#include "runtime/rt_string.h"
#include <stdio.h>
#include <stdlib.h>
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
    rt_string_t tag;
    double width;
    double height;
    double padding;
    int64_t elevation;
    rt_string_t content;
} UiElement;

typedef struct {
    double value;
    double min_val;
    double max_val;
    double step;
} SliderState;

typedef struct {
    double start_x;
    double start_y;
    double current_x;
    double current_y;
    int is_active;
} SwipeTracker;

UiElement UiElement_new_card(rt_string_t, double, int64_t);
UiElement UiElement_new_text(rt_string_t);
UiElement UiElement_new_container(rt_string_t, double);
SliderState SliderState_new(double, double, double);
double SliderState_update_val(SliderState*, double);
// module std.ui.widgets
UiElement UiElement_new_card(rt_string_t title, double w, int64_t elev) {
return (UiElement){.tag = rt_string_from("Card"), .width = w, .height = 120, .padding = 16, .elevation = elev, .content = title};
}

UiElement UiElement_new_text(rt_string_t text_content) {
return (UiElement){.tag = rt_string_from("Text"), .width = 0, .height = 24, .padding = 4, .elevation = 0LL, .content = text_content};
}

UiElement UiElement_new_container(rt_string_t dir, double pad) {
return (UiElement){.tag = rt_string_from("Container"), .width = 1, .height = 1, .padding = pad, .elevation = 0LL, .content = dir};
}

SliderState SliderState_new(double init_val, double min_v, double max_v) {
return (SliderState){.value = init_val, .min_val = min_v, .max_val = max_v, .step = 0.01};
}

double SliderState_update_val(SliderState* self, double new_val) {
if ((new_val < (*self).min_val)) {
return (*self).min_val;
} else if ((new_val > (*self).max_val)) {
return (*self).max_val;
} else {
return new_val;
}
}

