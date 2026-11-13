#include <stdint.h>
#include "runtime/rt_http.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_string.h"
#include <string.h>
#include "runtime/rt_map.h"
#include <math.h>
#include "runtime/rt_vec.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_json.h"
#include "runtime/rt_option.h"
#include "runtime/rt_db.h"
#include "runtime/rt_async.h"
#include <stdlib.h>
#include <stdio.h>
#include "runtime/rt_io.h"
#include "runtime/rt_stdlib.h"
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
    double r;
    double g;
    double b;
    double a;
} Color;

typedef struct {
    Color primary;
    Color on_primary;
    Color surface;
    Color on_surface;
    Color surface_variant;
    Color outline;
    double glass_opacity;
} ThemeTokens;

ThemeTokens ThemeTokens_new_dark_md3();
// module std.ui.theme
ThemeTokens ThemeTokens_new_dark_md3() {
return (ThemeTokens){.primary = (Color){.r = 0.22, .g = 0.74, .b = 0.97, .a = 1}, .on_primary = (Color){.r = 0.01, .g = 0.02, .b = 0.05, .a = 1}, .surface = (Color){.r = 0.04, .g = 0.06, .b = 0.11, .a = 1}, .on_surface = (Color){.r = 0.97, .g = 0.98, .b = 0.99, .a = 1}, .surface_variant = (Color){.r = 0.08, .g = 0.11, .b = 0.19, .a = 0.85}, .outline = (Color){.r = 0.54, .g = 0.36, .b = 0.96, .a = 0.45}, .glass_opacity = 0.85};
}

