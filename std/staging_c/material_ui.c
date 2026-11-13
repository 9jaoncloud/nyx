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

#define TAG_ComponentType_ButtonFilled 0
#define TAG_ComponentType_ButtonTonal 1
#define TAG_ComponentType_ButtonOutlined 2
#define TAG_ComponentType_ButtonText 3
#define TAG_ComponentType_CardElevated 4
#define TAG_ComponentType_CardOutlined 5
#define TAG_ComponentType_TextField 6
#define TAG_ComponentType_Switch 7
#define TAG_ComponentType_Slider 8
#define TAG_ComponentType_FloatingActionButton 9
#define TAG_ComponentType_NavigationRail 10
#define TAG_ComponentType_AppBar 11
typedef struct {
    int tag;
    union {
    } data;
} ComponentType;

#define TAG_MaterialElevation_Level0 0
#define TAG_MaterialElevation_Level1 1
#define TAG_MaterialElevation_Level2 2
#define TAG_MaterialElevation_Level3 3
#define TAG_MaterialElevation_Level4 4
#define TAG_MaterialElevation_Level5 5
typedef struct {
    int tag;
    union {
    } data;
} MaterialElevation;

#define TAG_MaterialCornerRadius_None 0
#define TAG_MaterialCornerRadius_ExtraSmall 1
#define TAG_MaterialCornerRadius_Small 2
#define TAG_MaterialCornerRadius_Medium 3
#define TAG_MaterialCornerRadius_Large 4
#define TAG_MaterialCornerRadius_ExtraLarge 5
#define TAG_MaterialCornerRadius_Full 6
typedef struct {
    int tag;
    union {
    } data;
} MaterialCornerRadius;

typedef struct {
    double r;
    double g;
    double b;
    double a;
} ColorRGBA;

typedef struct {
    ColorRGBA primary;
    ColorRGBA on_primary;
    ColorRGBA primary_container;
    ColorRGBA on_primary_container;
    ColorRGBA secondary;
    ColorRGBA on_secondary;
    ColorRGBA secondary_container;
    ColorRGBA on_secondary_container;
    ColorRGBA surface;
    ColorRGBA on_surface;
    ColorRGBA surface_variant;
    ColorRGBA on_surface_variant;
    ColorRGBA background;
    ColorRGBA on_background;
    ColorRGBA outline;
    ColorRGBA outline_variant;
    ColorRGBA error;
    ColorRGBA on_error;
    int is_dark_mode;
} MaterialTheme;

typedef struct {
    ComponentType comp_type;
    double x;
    double y;
    double width;
    double height;
    MaterialElevation elevation;
    MaterialCornerRadius corner_radius;
    int is_hovered;
    int is_pressed;
    int is_focused;
    int is_disabled;
    double ripple_x;
    double ripple_y;
    double ripple_progress;
    rt_string_t label;
    double state_value;
} MaterialComponent;

MaterialTheme material_create_dark_theme();
void render_material_card(MaterialComponent*, MaterialTheme*, NyxVec*, int64_t, int64_t);
// module material_ui
// import prelude::*
MaterialTheme material_create_dark_theme() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (MaterialTheme){.is_dark_mode = 1, .primary = (ColorRGBA){.r = 0.82, .g = 0.77, .b = 1, .a = 1}, .on_primary = (ColorRGBA){.r = 0.22, .g = 0.15, .b = 0.42, .a = 1}, .primary_container = (ColorRGBA){.r = 0.31, .g = 0.24, .b = 0.53, .a = 1}, .on_primary_container = (ColorRGBA){.r = 0.91, .g = 0.87, .b = 1, .a = 1}, .secondary = (ColorRGBA){.r = 0.8, .g = 0.78, .b = 0.88, .a = 1}, .on_secondary = (ColorRGBA){.r = 0.2, .g = 0.18, .b = 0.27, .a = 1}, .secondary_container = (ColorRGBA){.r = 0.29, .g = 0.27, .b = 0.37, .a = 1}, .on_secondary_container = (ColorRGBA){.r = 0.9, .g = 0.88, .b = 0.97, .a = 1}, .surface = (ColorRGBA){.r = 0.07, .g = 0.09, .b = 0.15, .a = 1}, .on_surface = (ColorRGBA){.r = 0.91, .g = 0.9, .b = 0.94, .a = 1}, .surface_variant = (ColorRGBA){.r = 0.11, .g = 0.15, .b = 0.24, .a = 1}, .on_surface_variant = (ColorRGBA){.r = 0.79, .g = 0.77, .b = 0.84, .a = 1}, .background = (ColorRGBA){.r = 0.04, .g = 0.06, .b = 0.1, .a = 1}, .on_background = (ColorRGBA){.r = 0.91, .g = 0.9, .b = 0.94, .a = 1}, .outline = (ColorRGBA){.r = 0.58, .g = 0.56, .b = 0.63, .a = 1}, .outline_variant = (ColorRGBA){.r = 0.27, .g = 0.26, .b = 0.31, .a = 1}, .error = (ColorRGBA){.r = 0.95, .g = 0.55, .b = 0.53, .a = 1}, .on_error = (ColorRGBA){.r = 0.38, .g = 0, .b = 0.02, .a = 1}};
}

void render_material_card(MaterialComponent* card, MaterialTheme* theme, NyxVec* framebuffer, int64_t fb_width, int64_t fb_height) {
Arena* __arena = arena_create();
int64_t x0 = (int64_t)((*card).x);
int64_t y0 = (int64_t)((*card).y);
int64_t x1 = (int64_t)(((*card).x + (*card).width));
int64_t y1 = (int64_t)(((*card).y + (*card).height));
ColorRGBA bg = ({ ColorRGBA __if_res_1; if ((*card).is_hovered) {
__if_res_1 = (&((*theme).primary_container));
} else {
__if_res_1 = (&((*theme).surface_variant));
}
__if_res_1; });
int64_t pixel_color = (((((int64_t)((bg.a * 255)) << 24LL) | ((int64_t)((bg.r * 255)) << 16LL)) | ((int64_t)((bg.g * 255)) << 8LL)) | (int64_t)((bg.b * 255)));
for (int64_t y = y0; y < y1; y++) {
({ if ((y >= fb_height)) {
break;
}
});
for (int64_t x = x0; x < x1; x++) {
({ if ((x >= fb_width)) {
break;
}
});
int64_t idx = ((y * fb_width) + x);
({ if ((idx < framebuffer->len)) {
(framebuffer[idx] = pixel_color);
}
});
}
;
}
;
arena_destroy(__arena);
}

