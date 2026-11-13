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

#define TAG_ButtonVariant_Filled 0
#define TAG_ButtonVariant_Elevated 1
#define TAG_ButtonVariant_Tonal 2
#define TAG_ButtonVariant_Outlined 3
#define TAG_ButtonVariant_Text 4
typedef struct {
    int tag;
    union {
    } data;
} ButtonVariant;

#define TAG_Alignment_TopLeft 0
#define TAG_Alignment_TopCenter 1
#define TAG_Alignment_TopRight 2
#define TAG_Alignment_CenterLeft 3
#define TAG_Alignment_Center 4
#define TAG_Alignment_CenterRight 5
#define TAG_Alignment_BottomLeft 6
#define TAG_Alignment_BottomCenter 7
#define TAG_Alignment_BottomRight 8
typedef struct {
    int tag;
    union {
    } data;
} Alignment;

#define TAG_CardVariant_Elevated 0
#define TAG_CardVariant_Filled 1
#define TAG_CardVariant_Outlined 2
typedef struct {
    int tag;
    union {
    } data;
} CardVariant;

typedef struct {
    double r;
    double g;
    double b;
    double a;
} Color;

typedef struct {
    Color primary;
    Color on_primary;
    Color primary_container;
    Color on_primary_container;
    Color secondary;
    Color on_secondary;
    Color secondary_container;
    Color on_secondary_container;
    Color tertiary;
    Color on_tertiary;
    Color tertiary_container;
    Color on_tertiary_container;
    Color background;
    Color on_background;
    Color surface;
    Color on_surface;
    Color surface_variant;
    Color on_surface_variant;
    Color surface_container;
    Color surface_container_high;
    Color outline;
    Color outline_variant;
    Color error;
    Color on_error;
    Color error_container;
    int is_dark;
} MaterialTheme;

typedef struct {
    double none;
    double extra_small;
    double small;
    double medium;
    double large;
    double extra_large;
    double full;
} M3Shape;

typedef struct {
    double display_large;
    double headline_large;
    double headline_medium;
    double title_large;
    double title_medium;
    double body_large;
    double body_medium;
    double label_large;
    double label_medium;
} M3Typography;

typedef struct {
    void* native;
} Canvas;

typedef struct {
    void* native;
} Surface;

typedef struct {
    void* native;
} Window;

typedef struct {
    rt_string_t text;
    double font_size;
    Color color;
} TextWidget;

typedef struct {
    rt_string_t label;
    ButtonVariant variant;
    double width;
    double height;
} ButtonWidget;

typedef struct {
    rt_string_t title;
    rt_string_t subtitle;
    CardVariant variant;
    double width;
    double height;
    double elevation;
} CardWidget;

typedef struct {
    rt_string_t text;
    Color bg_color;
    Color text_color;
} BadgeWidget;

typedef struct {
    rt_string_t label;
    rt_string_t icon;
    int is_active;
} NavItem;

typedef struct {
    NyxVec items;
    double width;
} NavigationRailWidget;

typedef struct {
    rt_string_t title;
    rt_string_t subtitle;
    double width;
} TopAppBarWidget;

typedef struct {
    int is_on;
    double width;
    double height;
} SwitchWidget;

typedef struct {
    double value;
    double width;
    double height;
} SliderWidget;

typedef struct {
    int is_checked;
    rt_string_t label;
    double size;
} CheckboxWidget;

typedef struct {
    rt_string_t text;
    rt_string_t label;
    int is_focused;
    double width;
    double height;
} TextFieldWidget;

typedef struct {
    double progress;
    double width;
    double height;
} ProgressBarWidget;

typedef struct {
    MaterialTheme theme;
    M3Shape shapes;
    M3Typography typography;
} MaterialRenderer;

typedef struct {
    Color surface_bg;
    Color border_stroke;
    Color specular_sheen;
    Color accent_glow;
    Color text_primary;
    Color text_secondary;
    Color shadow_ambient;
    double blur_radius;
    double corner_radius;
} GlassTokens;

typedef struct {
    double width;
    double height;
    double corner_radius;
    int has_specular_sheen;
} GlassSurfaceWidget;

typedef struct {
    rt_string_t title;
    rt_string_t subtitle;
    rt_string_t badge_text;
    double width;
    double height;
    double corner_radius;
    int is_hovered;
    double elevation;
} GlassCardWidget;

typedef struct {
    rt_string_t title;
    rt_string_t subtitle;
    double width;
    double height;
    double tilt_x;
    double tilt_y;
    double depth_offset;
} Layered3DCardWidget;

typedef struct {
    rt_string_t label;
    double width;
    double height;
    int is_hovered;
    int is_pressed;
    double glow_spread;
} GlassButtonWidget;

typedef struct {
    double progress;
    double width;
    double height;
    rt_string_t hover_time_text;
    int is_hovered;
} GlassScrubberWidget;

typedef struct {
    NyxVec bar_heights;
    double bar_width;
    double bar_spacing;
    double max_height;
    int show_peak_markers;
} SpectrumVisualizerWidget;

typedef struct {
    rt_string_t prompt_title;
    NyxVec lines;
    double width;
    double height;
} AeroTerminalWidget;

typedef struct {
    rt_string_t text;
    int is_pulsing;
    Color custom_accent;
} GlassBadgeWidget;

typedef struct {
    int is_on;
    double width;
    double height;
} GlassSwitchWidget;

typedef struct {
    GlassTokens tokens;
} GlassRenderer;

typedef struct {
} Clock;

typedef struct {
    int64_t start_time;
    int64_t duration_ms;
    int is_running;
    int is_reverse;
} AnimationController;

typedef struct {
    double left;
    double top;
    double right;
    double bottom;
} Padding;

typedef struct {
    double width;
    double height;
} CenterBox;

typedef struct {
    double width;
    double height;
} SizedBox;

typedef struct {
    int64_t flex;
} Expanded;

typedef struct {
    Alignment alignment;
} Align;

typedef struct {
    double stiffness;
    double damping;
    double position;
    double velocity;
    double target;
} SpringSimulation;

typedef struct {
    int is_dark;
    Color primary_color;
    Color background_color;
    Color surface_color;
    Color text_color;
} ThemeProvider;

Color Color_new(double, double, double, double);
Color Color_rgb(double, double, double);
Color Color_hex(int64_t);
int64_t Color_to_u32(Color*);
Color Color_with_alpha(Color*, double);
Color Color_lerp(Color*, Color*, double);
Color Color_white();
Color Color_black();
Color Color_red();
Color Color_green();
Color Color_blue();
Color Color_transparent();
MaterialTheme MaterialTheme_dark();
MaterialTheme MaterialTheme_light();
M3Shape M3Shape_nyx_default();
M3Typography M3Typography_nyx_default();
Canvas Canvas_new(Surface*);
void Canvas_draw_rect(Canvas*, double, double, double, double, Color*);
void Canvas_draw_rounded_rect(Canvas*, double, double, double, double, double, Color*);
void Canvas_draw_circle(Canvas*, double, double, double, Color*);
void Canvas_draw_gradient_rect(Canvas*, double, double, double, double, Color*, Color*);
void Canvas_draw_shadow(Canvas*, double, double, double, double, double, double, Color*);
void Canvas_draw_glow(Canvas*, double, double, double, double, double, double, Color*);
void Canvas_draw_ripple(Canvas*, double, double, double, double, Color*);
void Canvas_draw_text(Canvas*, rt_string_t, double, double, double, Color*);
void Canvas_draw_line(Canvas*, double, double, double, double, double, Color*);
void Canvas_fill(Canvas*, Color*);
void Canvas_save(Canvas*);
void Canvas_restore(Canvas*);
void Canvas_translate(Canvas*, double, double);
void Canvas_rotate(Canvas*, double);
void Canvas_scale(Canvas*, double, double);
Surface Surface_new(int64_t, int64_t);
Window Window_new(int64_t, int64_t, rt_string_t);
int Window_should_close(Window*);
void Window_poll_events(Window*);
Canvas Window_canvas(Window*);
void Window_swap_buffers(Window*);
int64_t Window_get_width(Window*);
int64_t Window_get_height(Window*);
void Window_set_title(Window*, rt_string_t);
int64_t Window_mouse_x(Window*);
int64_t Window_mouse_y(Window*);
int Window_is_mouse_down(Window*);
int64_t Window_last_key(Window*);
int point_in_rect(double, double, double, double, double, double);
MaterialRenderer MaterialRenderer_new(MaterialTheme);
void MaterialRenderer_draw_text_widget(MaterialRenderer*, Canvas*, TextWidget*, double, double);
void MaterialRenderer_draw_text(MaterialRenderer*, Canvas*, TextWidget*, double, double);
void MaterialRenderer_draw_button(MaterialRenderer*, Canvas*, ButtonWidget*, double, double);
void MaterialRenderer_draw_card(MaterialRenderer*, Canvas*, CardWidget*, double, double);
void MaterialRenderer_draw_badge(MaterialRenderer*, Canvas*, BadgeWidget*, double, double);
void MaterialRenderer_draw_navigation_rail(MaterialRenderer*, Canvas*, NavigationRailWidget*, double, double, double);
void MaterialRenderer_draw_top_app_bar(MaterialRenderer*, Canvas*, TopAppBarWidget*, double, double);
void MaterialRenderer_draw_switch(MaterialRenderer*, Canvas*, SwitchWidget*, double, double);
void MaterialRenderer_draw_slider(MaterialRenderer*, Canvas*, SliderWidget*, double, double);
void MaterialRenderer_draw_checkbox(MaterialRenderer*, Canvas*, CheckboxWidget*, double, double);
void MaterialRenderer_draw_text_field(MaterialRenderer*, Canvas*, TextFieldWidget*, double, double);
void MaterialRenderer_draw_progress_bar(MaterialRenderer*, Canvas*, ProgressBarWidget*, double, double);
void MaterialRenderer_draw_animated_card(MaterialRenderer*, Canvas*, CardWidget*, double, double, double, int);
void MaterialRenderer_draw_glow_button(MaterialRenderer*, Canvas*, rt_string_t, double, double, double, double, double);
GlassTokens GlassTokens_cyber_void();
GlassTokens GlassTokens_apple_acrylic();
GlassTokens GlassTokens_neon_pulse();
GlassRenderer GlassRenderer_new(GlassTokens);
void GlassRenderer_draw_glass_surface(GlassRenderer*, Canvas*, GlassSurfaceWidget*, double, double);
void GlassRenderer_draw_glass_card(GlassRenderer*, Canvas*, GlassCardWidget*, double, double);
void GlassRenderer_draw_layered_3d_card(GlassRenderer*, Canvas*, Layered3DCardWidget*, double, double);
void GlassRenderer_draw_glass_button(GlassRenderer*, Canvas*, GlassButtonWidget*, double, double);
void GlassRenderer_draw_glass_scrubber(GlassRenderer*, Canvas*, GlassScrubberWidget*, double, double);
void GlassRenderer_draw_spectrum_visualizer(GlassRenderer*, Canvas*, SpectrumVisualizerWidget*, double, double);
void GlassRenderer_draw_aero_terminal(GlassRenderer*, Canvas*, AeroTerminalWidget*, double, double);
void GlassRenderer_draw_glass_badge(GlassRenderer*, Canvas*, GlassBadgeWidget*, double, double);
void GlassRenderer_draw_glass_switch(GlassRenderer*, Canvas*, GlassSwitchWidget*, double, double);
int64_t Clock_now_ms();
AnimationController AnimationController_new(int64_t);
double AnimationController_progress(AnimationController*);
double AnimationController_value(AnimationController*);
void AnimationController_restart(AnimationController*);
void AnimationController_reverse(AnimationController*);
Padding Padding_all(double);
Padding Padding_symmetric(double, double);
CenterBox CenterBox_new(double, double);
SizedBox SizedBox_new(double, double);
SizedBox SizedBox_square(double);
Expanded Expanded_new(int64_t);
Align Align_new(Alignment);
SpringSimulation SpringSimulation_new(double, double, double);
double SpringSimulation_update(SpringSimulation*, double);
ThemeProvider ThemeProvider_new(int);
void ThemeProvider_toggle_theme(ThemeProvider*);
void debugDumpApp();
// module ui
// extern nyx_skia_surface_create
// extern nyx_skia_surface_destroy
// extern nyx_skia_canvas_create
// extern nyx_skia_canvas_destroy
// extern nyx_skia_draw_rect
// extern nyx_skia_draw_rounded_rect
// extern nyx_skia_draw_circle
// extern nyx_skia_draw_gradient_rect
// extern nyx_skia_draw_shadow
// extern nyx_skia_draw_glow
// extern nyx_skia_draw_ripple
// extern nyx_skia_draw_text
// extern nyx_skia_draw_text_ex
// extern nyx_skia_draw_icon
// extern nyx_skia_draw_ripple_anim
// extern nyx_clip_push_rect
// extern nyx_clip_pop
// extern nyx_measure_text_width
// extern nyx_measure_text_height
// extern nyx_skia_draw_text_centered
// extern nyx_frame_sleep
// extern nyx_hit_begin_frame
// extern nyx_hit_register
// extern nyx_hit_test
// extern nyx_hit_clicked
// extern nyx_window_update_click
// extern nyx_window_mouse_clicked
// extern nyx_window_mouse_released
// extern nyx_skia_draw_line
// extern nyx_skia_fill
// extern nyx_time_millis
// extern nyx_skia_save
// extern nyx_skia_restore
// extern nyx_skia_translate
// extern nyx_skia_rotate
// extern nyx_skia_scale
// extern nyx_window_create
// extern nyx_window_destroy
// extern nyx_window_should_close
// extern nyx_window_poll_events
// extern nyx_window_canvas
// extern nyx_window_swap_buffers
// extern nyx_window_width
// extern nyx_window_height
// extern nyx_window_set_title
// extern nyx_window_mouse_x
// extern nyx_window_mouse_y
// extern nyx_window_mouse_down
// extern nyx_window_last_key
Color Color_new(double r, double g, double b, double a) {
return (Color){.r = r, .g = g, .b = b, .a = a};
}

Color Color_rgb(double r, double g, double b) {
return (Color){.r = r, .g = g, .b = b, .a = 1};
}

Color Color_hex(int64_t value) {
return (Color){.r = ((double)(((value >> 16LL) & 255LL)) / 255), .g = ((double)(((value >> 8LL) & 255LL)) / 255), .b = ((double)((value & 255LL)) / 255), .a = ((double)(((value >> 24LL) & 255LL)) / 255)};
}

int64_t Color_to_u32(Color* self) {
int64_t r = (int64_t)(((*self).r * 255));
int64_t g = (int64_t)(((*self).g * 255));
int64_t b = (int64_t)(((*self).b * 255));
int64_t a = (int64_t)(((*self).a * 255));
return ((((a << 24LL) | (r << 16LL)) | (g << 8LL)) | b);
}

Color Color_with_alpha(Color* self, double alpha) {
return (Color){.r = (*self).r, .g = (*self).g, .b = (*self).b, .a = alpha};
}

Color Color_lerp(Color* self, Color* other, double t) {
return (Color){.r = ((*self).r + (((*other).r - (*self).r) * t)), .g = ((*self).g + (((*other).g - (*self).g) * t)), .b = ((*self).b + (((*other).b - (*self).b) * t)), .a = ((*self).a + (((*other).a - (*self).a) * t))};
}

Color Color_white() {
return (Color){.r = 1, .g = 1, .b = 1, .a = 1};
}

Color Color_black() {
return (Color){.r = 0, .g = 0, .b = 0, .a = 1};
}

Color Color_red() {
return (Color){.r = 1, .g = 0, .b = 0, .a = 1};
}

Color Color_green() {
return (Color){.r = 0, .g = 1, .b = 0, .a = 1};
}

Color Color_blue() {
return (Color){.r = 0, .g = 0, .b = 1, .a = 1};
}

Color Color_transparent() {
return (Color){.r = 0, .g = 0, .b = 0, .a = 0};
}

MaterialTheme MaterialTheme_dark() {
return (MaterialTheme){.primary = Color_hex(4291869951LL), .on_primary = Color_hex(4281867890LL), .primary_container = Color_hex(4283381643LL), .on_primary_container = Color_hex(4293582335LL), .secondary = Color_hex(4291609308LL), .on_secondary = Color_hex(4281544001LL), .secondary_container = Color_hex(4283057240LL), .on_secondary_container = Color_hex(4293451512LL), .tertiary = Color_hex(4293900488LL), .on_tertiary = Color_hex(4282983730LL), .tertiary_container = Color_hex(4284693320LL), .on_tertiary_container = Color_hex(4294957284LL), .background = Color_hex(4279505432LL), .on_background = Color_hex(4293320937LL), .surface = Color_hex(4279505432LL), .on_surface = Color_hex(4293320937LL), .surface_variant = Color_hex(4282991951LL), .on_surface_variant = Color_hex(4291478736LL), .surface_container = Color_hex(4280360742LL), .surface_container_high = Color_hex(4281018672LL), .outline = Color_hex(4287860633LL), .outline_variant = Color_hex(4282991951LL), .error = Color_hex(4294097077LL), .on_error = Color_hex(4284486672LL), .error_container = Color_hex(4287372568LL), .is_dark = 1};
}

MaterialTheme MaterialTheme_light() {
return (MaterialTheme){.primary = Color_hex(4284960932LL), .on_primary = Color_white(), .primary_container = Color_hex(4293582335LL), .on_primary_container = Color_hex(4280352861LL), .secondary = Color_hex(4284636017LL), .on_secondary = Color_white(), .secondary_container = Color_hex(4293451512LL), .on_secondary_container = Color_hex(4280097067LL), .tertiary = Color_hex(4286403168LL), .on_tertiary = Color_white(), .tertiary_container = Color_hex(4294957284LL), .on_tertiary_container = Color_hex(4281405725LL), .background = Color_hex(4294899711LL), .on_background = Color_hex(4280097568LL), .surface = Color_hex(4294899711LL), .on_surface = Color_hex(4280097568LL), .surface_variant = Color_hex(4293386476LL), .on_surface_variant = Color_hex(4282991951LL), .surface_container = Color_hex(4294176247LL), .surface_container_high = Color_hex(4293715696LL), .outline = Color_hex(4286149758LL), .outline_variant = Color_hex(4291478736LL), .error = Color_hex(4289930782LL), .on_error = Color_white(), .error_container = Color_hex(4294565596LL), .is_dark = 0};
}

M3Shape M3Shape_default() {
return (M3Shape){.none = 0, .extra_small = 4, .small = 8, .medium = 12, .large = 16, .extra_large = 28, .full = 9999};
}

M3Typography M3Typography_default() {
return (M3Typography){.display_large = 48, .headline_large = 32, .headline_medium = 24, .title_large = 20, .title_medium = 16, .body_large = 16, .body_medium = 14, .label_large = 14, .label_medium = 12};
}

Canvas Canvas_new(Surface* surface) {
return (Canvas){.native = ({
nyx_skia_canvas_create((*surface).native);
})};
}

void Canvas_draw_rect(Canvas* self, double x, double y, double w, double h, Color* color) {
({
nyx_skia_draw_rect((*self).native, x, y, w, h, Color_to_u32(color));
});
}

void Canvas_draw_rounded_rect(Canvas* self, double x, double y, double w, double h, double radius, Color* color) {
({
nyx_skia_draw_rounded_rect((*self).native, x, y, w, h, radius, Color_to_u32(color));
});
}

void Canvas_draw_circle(Canvas* self, double cx, double cy, double radius, Color* color) {
({
nyx_skia_draw_circle((*self).native, cx, cy, radius, Color_to_u32(color));
});
}

void Canvas_draw_gradient_rect(Canvas* self, double x, double y, double w, double h, Color* color_top, Color* color_bottom) {
({
nyx_skia_draw_gradient_rect((*self).native, x, y, w, h, Color_to_u32(color_top), Color_to_u32(color_bottom));
});
}

void Canvas_draw_shadow(Canvas* self, double x, double y, double w, double h, double radius, double elevation, Color* color) {
({
nyx_skia_draw_shadow((*self).native, x, y, w, h, radius, elevation, Color_to_u32(color));
});
}

void Canvas_draw_glow(Canvas* self, double x, double y, double w, double h, double radius, double spread, Color* color) {
({
nyx_skia_draw_glow((*self).native, x, y, w, h, radius, spread, Color_to_u32(color));
});
}

void Canvas_draw_ripple(Canvas* self, double cx, double cy, double radius, double alpha, Color* color) {
({
nyx_skia_draw_ripple((*self).native, cx, cy, radius, alpha, Color_to_u32(color));
});
}

void Canvas_draw_text(Canvas* self, rt_string_t text, double x, double y, double size, Color* color) {
({
nyx_skia_draw_text((*self).native, text.data, x, y, size, Color_to_u32(color));
});
}

void Canvas_draw_line(Canvas* self, double x1, double y1, double x2, double y2, double width, Color* color) {
({
nyx_skia_draw_line((*self).native, x1, y1, x2, y2, width, Color_to_u32(color));
});
}

void Canvas_fill(Canvas* self, Color* color) {
({
nyx_skia_fill((*self).native, Color_to_u32(color));
});
}

void Canvas_save(Canvas* self) {
({
nyx_skia_save((*self).native);
});
}

void Canvas_restore(Canvas* self) {
({
nyx_skia_restore((*self).native);
});
}

void Canvas_translate(Canvas* self, double dx, double dy) {
({
nyx_skia_translate((*self).native, dx, dy);
});
}

void Canvas_rotate(Canvas* self, double degrees) {
({
nyx_skia_rotate((*self).native, degrees);
});
}

void Canvas_scale(Canvas* self, double sx, double sy) {
({
nyx_skia_scale((*self).native, sx, sy);
});
}

Surface Surface_new(int64_t width, int64_t height) {
return (Surface){.native = ({
nyx_skia_surface_create(width, height);
})};
}

Window Window_new(int64_t width, int64_t height, rt_string_t title) {
return (Window){.native = ({
nyx_window_create(width, height, title.data);
})};
}

int Window_should_close(Window* self) {
return ({
(nyx_window_should_close((*self).native) != 0LL);
});
}

void Window_poll_events(Window* self) {
({
nyx_window_poll_events((*self).native);
});
}

Canvas Window_canvas(Window* self) {
return (Canvas){.native = ({
nyx_window_canvas((*self).native);
})};
}

void Window_swap_buffers(Window* self) {
({
nyx_window_swap_buffers((*self).native);
});
}

int64_t Window_get_width(Window* self) {
return ({
nyx_window_width((*self).native);
});
}

int64_t Window_get_height(Window* self) {
return ({
nyx_window_height((*self).native);
});
}

void Window_set_title(Window* self, rt_string_t title) {
({
nyx_window_set_title((*self).native, title.data);
});
}

int64_t Window_mouse_x(Window* self) {
return ({
nyx_window_mouse_x((*self).native);
});
}

int64_t Window_mouse_y(Window* self) {
return ({
nyx_window_mouse_y((*self).native);
});
}

int Window_is_mouse_down(Window* self) {
return ({
(nyx_window_mouse_down((*self).native) != 0LL);
});
}

int64_t Window_last_key(Window* self) {
return ({
nyx_window_last_key((*self).native);
});
}

int point_in_rect(double px, double py, double rx, double ry, double rw, double rh) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ((((px >= rx) && (px <= (rx + rw))) && (py >= ry)) && (py <= (ry + rh)));
}

MaterialRenderer MaterialRenderer_new(MaterialTheme theme) {
return (MaterialRenderer){.theme = theme, .shapes = M3Shape_default(), .typography = M3Typography_default()};
}

void MaterialRenderer_draw_text_widget(MaterialRenderer* self, Canvas* canvas, TextWidget* t, double x, double y) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*t).text; rt_string_retain(&_c); _c; }), x, y, (*t).font_size, ({ Color _matmp_4 = (&((*t).color)); &_matmp_4; }));
}

void MaterialRenderer_draw_text(MaterialRenderer* self, Canvas* canvas, TextWidget* t, double x, double y) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*t).text; rt_string_retain(&_c); _c; }), x, y, (*t).font_size, ({ Color _matmp_4 = (&((*t).color)); &_matmp_4; }));
}

void MaterialRenderer_draw_button(MaterialRenderer* self, Canvas* canvas, ButtonWidget* b, double x, double y) {
Color bg = ((*self).theme).primary;
Color fg = ((*self).theme).on_primary;
double elev = 0;
double radius = ((*self).shapes).full;
({ double __if_res_1; if (((*b).variant.tag == (ButtonVariant){ .tag = TAG_ButtonVariant_Filled }.tag)) {
bg = ((*self).theme).primary;
fg = ((*self).theme).on_primary;
__if_res_1 = (elev = 0);
} else if (((*b).variant.tag == (ButtonVariant){ .tag = TAG_ButtonVariant_Elevated }.tag)) {
bg = ((*self).theme).surface_container_high;
fg = ((*self).theme).primary;
__if_res_1 = (elev = 2);
} else if (((*b).variant.tag == (ButtonVariant){ .tag = TAG_ButtonVariant_Tonal }.tag)) {
bg = ((*self).theme).secondary_container;
fg = ((*self).theme).on_secondary_container;
__if_res_1 = (elev = 0);
} else if (((*b).variant.tag == (ButtonVariant){ .tag = TAG_ButtonVariant_Outlined }.tag)) {
bg = Color_transparent();
fg = ((*self).theme).primary;
__if_res_1 = (elev = 0);
} else {
bg = Color_transparent();
fg = ((*self).theme).primary;
__if_res_1 = (elev = 0);
}
__if_res_1; });
({ if ((elev > 0)) {
Canvas_draw_shadow(canvas, x, y, (*b).width, (*b).height, radius, elev, ({ Color _matmp_6 = ({ Color _utmp_ = Color_hex(1073741824LL); &_utmp_; }); &_matmp_6; }));
}
});
({ if ((bg.a > 0)) {
Canvas_draw_rounded_rect(canvas, x, y, (*b).width, (*b).height, radius, ({ Color _matmp_5 = (&(bg)); &_matmp_5; }));
}
});
({ if (((*b).variant.tag == (ButtonVariant){ .tag = TAG_ButtonVariant_Outlined }.tag)) {
Canvas_draw_rounded_rect(canvas, x, y, (*b).width, (*b).height, radius, ({ Color _matmp_5 = (&(((*self).theme).outline)); &_matmp_5; }));
}
});
double text_y = (y + (((*b).height - ((*self).typography).label_large) / 2));
double text_x = (x + 16);
Canvas_draw_text(canvas, ({ rt_string_t _c = (*b).label; rt_string_retain(&_c); _c; }), text_x, text_y, ((*self).typography).label_large, ({ Color _matmp_4 = (&(fg)); &_matmp_4; }));
}

void MaterialRenderer_draw_card(MaterialRenderer* self, Canvas* canvas, CardWidget* c, double x, double y) {
Color bg = ((*self).theme).surface_container_high;
Color border_color = Color_transparent();
double elev = (*c).elevation;
({ if (((*c).variant.tag == (CardVariant){ .tag = TAG_CardVariant_Elevated }.tag)) {
bg = ((*self).theme).surface_container_high;
border_color = Color_transparent();
elev = (*c).elevation;
} else if (((*c).variant.tag == (CardVariant){ .tag = TAG_CardVariant_Filled }.tag)) {
bg = ((*self).theme).surface_container;
border_color = Color_transparent();
elev = 0;
} else if (((*c).variant.tag == (CardVariant){ .tag = TAG_CardVariant_Outlined }.tag)) {
bg = ((*self).theme).surface;
border_color = ((*self).theme).outline_variant;
elev = 0;
}
});
({ if ((elev > 0)) {
Canvas_draw_shadow(canvas, x, y, (*c).width, (*c).height, ((*self).shapes).medium, elev, ({ Color _matmp_6 = ({ Color _utmp_ = Color_hex(805306368LL); &_utmp_; }); &_matmp_6; }));
}
});
Canvas_draw_rounded_rect(canvas, x, y, (*c).width, (*c).height, ((*self).shapes).medium, ({ Color _matmp_5 = (&(bg)); &_matmp_5; }));
({ if ((border_color.a > 0)) {
Canvas_draw_rounded_rect(canvas, x, y, (*c).width, (*c).height, ((*self).shapes).medium, ({ Color _matmp_5 = (&(border_color)); &_matmp_5; }));
}
});
({ if (((*c).title.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*c).title; rt_string_retain(&_c); _c; }), (x + 16), (y + 16), ((*self).typography).title_medium, ({ Color _matmp_4 = (&(((*self).theme).on_surface)); &_matmp_4; }));
}
});
({ if (((*c).subtitle.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*c).subtitle; rt_string_retain(&_c); _c; }), (x + 16), (y + 42), ((*self).typography).body_medium, ({ Color _matmp_4 = (&(((*self).theme).on_surface_variant)); &_matmp_4; }));
}
});
}

void MaterialRenderer_draw_badge(MaterialRenderer* self, Canvas* canvas, BadgeWidget* b, double x, double y) {
double bw = (((double)((*b).text.length) * 8) + 16);
double bh = 22;
Canvas_draw_rounded_rect(canvas, x, y, bw, bh, ((*self).shapes).small, ({ Color _matmp_5 = (&((*b).bg_color)); &_matmp_5; }));
Canvas_draw_text(canvas, ({ rt_string_t _c = (*b).text; rt_string_retain(&_c); _c; }), (x + 8), (y + 4), ((*self).typography).label_medium, ({ Color _matmp_4 = (&((*b).text_color)); &_matmp_4; }));
}

void MaterialRenderer_draw_navigation_rail(MaterialRenderer* self, Canvas* canvas, NavigationRailWidget* rail, double x, double y, double h) {
Canvas_draw_rect(canvas, x, y, (*rail).width, h, ({ Color _matmp_4 = (&(((*self).theme).surface_container)); &_matmp_4; }));
}

void MaterialRenderer_draw_top_app_bar(MaterialRenderer* self, Canvas* canvas, TopAppBarWidget* bar, double x, double y) {
Canvas_draw_rect(canvas, x, y, (*bar).width, 64, ({ Color _matmp_4 = (&(((*self).theme).surface)); &_matmp_4; }));
Canvas_draw_text(canvas, ({ rt_string_t _c = (*bar).title; rt_string_retain(&_c); _c; }), (x + 24), (y + 18), ((*self).typography).title_large, ({ Color _matmp_4 = (&(((*self).theme).on_surface)); &_matmp_4; }));
({ if (((*bar).subtitle.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*bar).subtitle; rt_string_retain(&_c); _c; }), (x + 24), (y + 42), ((*self).typography).label_medium, ({ Color _matmp_4 = (&(((*self).theme).on_surface_variant)); &_matmp_4; }));
}
});
Canvas_draw_line(canvas, x, (y + 64), (x + (*bar).width), (y + 64), 1, ({ Color _matmp_5 = (&(((*self).theme).outline_variant)); &_matmp_5; }));
}

void MaterialRenderer_draw_switch(MaterialRenderer* self, Canvas* canvas, SwitchWidget* sw, double x, double y) {
double w = ({ double __if_res_11; if (((*sw).width > 0)) {
__if_res_11 = (*sw).width;
} else {
__if_res_11 = 52;
}
__if_res_11; });
double h = ({ double __if_res_12; if (((*sw).height > 0)) {
__if_res_12 = (*sw).height;
} else {
__if_res_12 = 32;
}
__if_res_12; });
double radius = (h * 0.5);
({ if ((*sw).is_on) {
Canvas_draw_rounded_rect(canvas, x, y, w, h, radius, ({ Color _matmp_5 = (&(((*self).theme).primary)); &_matmp_5; }));
double thumb_x = (((x + w) - radius) - 8);
double thumb_y = (y + 4);
double thumb_r = (radius - 4);
Canvas_draw_rounded_rect(canvas, thumb_x, thumb_y, (thumb_r * 2), (thumb_r * 2), thumb_r, ({ Color _matmp_5 = (&(((*self).theme).on_primary)); &_matmp_5; }));
} else {
Canvas_draw_rounded_rect(canvas, x, y, w, h, radius, ({ Color _matmp_5 = (&(((*self).theme).surface_container_high)); &_matmp_5; }));
Canvas_draw_rounded_rect(canvas, x, y, w, h, radius, ({ Color _matmp_5 = (&(((*self).theme).outline)); &_matmp_5; }));
double thumb_x = (x + 6);
double thumb_y = (y + 6);
double thumb_r = (radius - 6);
Canvas_draw_rounded_rect(canvas, thumb_x, thumb_y, (thumb_r * 2), (thumb_r * 2), thumb_r, ({ Color _matmp_5 = (&(((*self).theme).outline)); &_matmp_5; }));
}
});
}

void MaterialRenderer_draw_slider(MaterialRenderer* self, Canvas* canvas, SliderWidget* sl, double x, double y) {
double w = ({ double __if_res_14; if (((*sl).width > 0)) {
__if_res_14 = (*sl).width;
} else {
__if_res_14 = 200;
}
__if_res_14; });
double h = ({ double __if_res_15; if (((*sl).height > 0)) {
__if_res_15 = (*sl).height;
} else {
__if_res_15 = 16;
}
__if_res_15; });
double track_h = 6;
double track_y = (y + ((h - track_h) * 0.5));
double clamp_val = ({ double __if_res_16; if (((*sl).value < 0)) {
__if_res_16 = 0;
} else if (((*sl).value > 1)) {
__if_res_16 = 1;
} else {
__if_res_16 = (*sl).value;
}
__if_res_16; });
Canvas_draw_rounded_rect(canvas, x, track_y, w, track_h, 3, ({ Color _matmp_5 = (&(((*self).theme).surface_container_high)); &_matmp_5; }));
double active_w = (w * clamp_val);
({ if ((active_w > 0)) {
Canvas_draw_rounded_rect(canvas, x, track_y, active_w, track_h, 3, ({ Color _matmp_5 = (&(((*self).theme).primary)); &_matmp_5; }));
}
});
double thumb_x = ((x + active_w) - 6);
double thumb_y = (y + ((h - 18) * 0.5));
Canvas_draw_shadow(canvas, thumb_x, thumb_y, 12, 18, 4, 2, ({ Color _matmp_6 = ({ Color _utmp_ = Color_hex(805306368LL); &_utmp_; }); &_matmp_6; }));
Canvas_draw_rounded_rect(canvas, thumb_x, thumb_y, 12, 18, 4, ({ Color _matmp_5 = (&(((*self).theme).primary)); &_matmp_5; }));
}

void MaterialRenderer_draw_checkbox(MaterialRenderer* self, Canvas* canvas, CheckboxWidget* cb, double x, double y) {
double sz = ({ double __if_res_18; if (((*cb).size > 0)) {
__if_res_18 = (*cb).size;
} else {
__if_res_18 = 20;
}
__if_res_18; });
({ if ((*cb).is_checked) {
Canvas_draw_rounded_rect(canvas, x, y, sz, sz, ((*self).shapes).extra_small, ({ Color _matmp_5 = (&(((*self).theme).primary)); &_matmp_5; }));
Canvas_draw_text(canvas, rt_string_from("v"), (x + 4), (y + 2), ((*self).typography).label_medium, ({ Color _matmp_4 = (&(((*self).theme).on_primary)); &_matmp_4; }));
} else {
Canvas_draw_rounded_rect(canvas, x, y, sz, sz, ((*self).shapes).extra_small, ({ Color _matmp_5 = (&(((*self).theme).surface)); &_matmp_5; }));
Canvas_draw_rounded_rect(canvas, x, y, sz, sz, ((*self).shapes).extra_small, ({ Color _matmp_5 = (&(((*self).theme).outline)); &_matmp_5; }));
}
});
({ if (((*cb).label.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*cb).label; rt_string_retain(&_c); _c; }), ((x + sz) + 12), (y + 2), ((*self).typography).body_medium, ({ Color _matmp_4 = (&(((*self).theme).on_surface)); &_matmp_4; }));
}
});
}

void MaterialRenderer_draw_text_field(MaterialRenderer* self, Canvas* canvas, TextFieldWidget* tf, double x, double y) {
double w = ({ double __if_res_21; if (((*tf).width > 0)) {
__if_res_21 = (*tf).width;
} else {
__if_res_21 = 240;
}
__if_res_21; });
double h = ({ double __if_res_22; if (((*tf).height > 0)) {
__if_res_22 = (*tf).height;
} else {
__if_res_22 = 56;
}
__if_res_22; });
Canvas_draw_rounded_rect(canvas, x, y, w, h, ((*self).shapes).extra_small, ({ Color _matmp_5 = (&(((*self).theme).surface_container)); &_matmp_5; }));
({ if (((*tf).label.length > 0LL)) {
Color label_color = ({ Color __if_res_24; if ((*tf).is_focused) {
__if_res_24 = (&(((*self).theme).primary));
} else {
__if_res_24 = (&(((*self).theme).on_surface_variant));
}
__if_res_24; });
Canvas_draw_text(canvas, ({ rt_string_t _c = (*tf).label; rt_string_retain(&_c); _c; }), (x + 16), (y + 8), ((*self).typography).label_medium, &(label_color));
}
});
rt_string_t display_text = ({ rt_string_t __if_res_25; if (((*tf).text.length > 0LL)) {
__if_res_25 = ({ rt_string_t _c = (*tf).text; rt_string_retain(&_c); _c; });
} else {
__if_res_25 = rt_string_from("Type here...");
}
__if_res_25; });
Color text_color = ({ Color __if_res_26; if (((*tf).text.length > 0LL)) {
__if_res_26 = (&(((*self).theme).on_surface));
} else {
__if_res_26 = (&(((*self).theme).outline));
}
__if_res_26; });
Canvas_draw_text(canvas, display_text, (x + 16), (y + 28), ((*self).typography).body_large, &(text_color));
Color line_color = ({ Color __if_res_27; if ((*tf).is_focused) {
__if_res_27 = (&(((*self).theme).primary));
} else {
__if_res_27 = (&(((*self).theme).outline_variant));
}
__if_res_27; });
double line_h = ({ double __if_res_28; if ((*tf).is_focused) {
__if_res_28 = 2;
} else {
__if_res_28 = 1;
}
__if_res_28; });
Canvas_draw_rect(canvas, x, ((y + h) - line_h), w, line_h, &(line_color));
}

void MaterialRenderer_draw_progress_bar(MaterialRenderer* self, Canvas* canvas, ProgressBarWidget* pb, double x, double y) {
double w = ({ double __if_res_29; if (((*pb).width > 0)) {
__if_res_29 = (*pb).width;
} else {
__if_res_29 = 240;
}
__if_res_29; });
double h = ({ double __if_res_30; if (((*pb).height > 0)) {
__if_res_30 = (*pb).height;
} else {
__if_res_30 = 8;
}
__if_res_30; });
double clamp_val = ({ double __if_res_31; if (((*pb).progress < 0)) {
__if_res_31 = 0;
} else if (((*pb).progress > 1)) {
__if_res_31 = 1;
} else {
__if_res_31 = (*pb).progress;
}
__if_res_31; });
Canvas_draw_rounded_rect(canvas, x, y, w, h, 4, ({ Color _matmp_5 = (&(((*self).theme).surface_container_high)); &_matmp_5; }));
double fill_w = (w * clamp_val);
({ if ((fill_w > 0)) {
Canvas_draw_rounded_rect(canvas, x, y, fill_w, h, 4, ({ Color _matmp_5 = (&(((*self).theme).primary)); &_matmp_5; }));
}
});
}

void MaterialRenderer_draw_animated_card(MaterialRenderer* self, Canvas* canvas, CardWidget* card, double x, double y, double anim_progress, int is_hovered) {
double w = ({ double __if_res_33; if (((*card).width > 0)) {
__if_res_33 = (*card).width;
} else {
__if_res_33 = 260;
}
__if_res_33; });
double h = ({ double __if_res_34; if (((*card).height > 0)) {
__if_res_34 = (*card).height;
} else {
__if_res_34 = 160;
}
__if_res_34; });
double radius = ((*self).shapes).medium;
double current_elevation = ({ double __if_res_35; if (is_hovered) {
__if_res_35 = (3 + (anim_progress * 2));
} else {
__if_res_35 = 1;
}
__if_res_35; });
({ if (is_hovered) {
Canvas_draw_glow(canvas, x, y, w, h, radius, (8 * anim_progress), ({ Color _matmp_6 = (&(((*self).theme).primary)); &_matmp_6; }));
}
});
Canvas_draw_shadow(canvas, x, y, w, h, radius, current_elevation, ({ Color _matmp_6 = ({ Color _utmp_ = Color_hex(1073741824LL); &_utmp_; }); &_matmp_6; }));
Canvas_draw_rounded_rect(canvas, x, y, w, h, radius, ({ Color _matmp_5 = (&(((*self).theme).surface_container)); &_matmp_5; }));
({ if (((*card).title.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*card).title; rt_string_retain(&_c); _c; }), (x + 16), (y + 20), ((*self).typography).title_medium, ({ Color _matmp_4 = (&(((*self).theme).on_surface)); &_matmp_4; }));
}
});
({ if (((*card).subtitle.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*card).subtitle; rt_string_retain(&_c); _c; }), (x + 16), (y + 46), ((*self).typography).body_medium, ({ Color _matmp_4 = (&(((*self).theme).on_surface_variant)); &_matmp_4; }));
}
});
}

void MaterialRenderer_draw_glow_button(MaterialRenderer* self, Canvas* canvas, rt_string_t label, double x, double y, double w, double h, double glow_alpha) {
double radius = ((*self).shapes).full;
({ if ((glow_alpha > 0)) {
Canvas_draw_glow(canvas, x, y, w, h, radius, (12 * glow_alpha), ({ Color _matmp_6 = (&(((*self).theme).primary)); &_matmp_6; }));
}
});
Canvas_draw_rounded_rect(canvas, x, y, w, h, radius, ({ Color _matmp_5 = (&(((*self).theme).primary)); &_matmp_5; }));
Canvas_draw_text(canvas, label, (x + (w * 0.2)), (y + (h * 0.3)), ((*self).typography).label_large, ({ Color _matmp_4 = (&(((*self).theme).on_primary)); &_matmp_4; }));
}

GlassTokens GlassTokens_cyber_void() {
return (GlassTokens){.surface_bg = Color_new(0.06, 0.09, 0.16, 0.68), .border_stroke = Color_new(1, 1, 1, 0.14), .specular_sheen = Color_new(1, 1, 1, 0.28), .accent_glow = Color_hex(9133302LL), .text_primary = Color_hex(16317180LL), .text_secondary = Color_hex(9741240LL), .shadow_ambient = Color_hex(1610612736LL), .blur_radius = 16, .corner_radius = 16};
}

GlassTokens GlassTokens_apple_acrylic() {
return (GlassTokens){.surface_bg = Color_new(0.95, 0.95, 0.98, 0.55), .border_stroke = Color_new(1, 1, 1, 0.45), .specular_sheen = Color_new(1, 1, 1, 0.65), .accent_glow = Color_hex(440020LL), .text_primary = Color_hex(988970LL), .text_secondary = Color_hex(4674921LL), .shadow_ambient = Color_hex(620756992LL), .blur_radius = 24, .corner_radius = 20};
}

GlassTokens GlassTokens_neon_pulse() {
return (GlassTokens){.surface_bg = Color_new(0.04, 0.05, 0.1, 0.82), .border_stroke = Color_new(0.02, 0.71, 0.83, 0.4), .specular_sheen = Color_new(0.93, 0.93, 0.1, 0.5), .accent_glow = Color_hex(15485081LL), .text_primary = Color_hex(16777215LL), .text_secondary = Color_hex(13358561LL), .shadow_ambient = Color_hex(1879048192LL), .blur_radius = 18, .corner_radius = 14};
}

GlassRenderer GlassRenderer_new(GlassTokens tokens) {
return (GlassRenderer){.tokens = tokens};
}

void GlassRenderer_draw_glass_surface(GlassRenderer* self, Canvas* canvas, GlassSurfaceWidget* surface, double x, double y) {
double r = ({ double __if_res_40; if (((*surface).corner_radius > 0)) {
__if_res_40 = (*surface).corner_radius;
} else {
__if_res_40 = ((*self).tokens).corner_radius;
}
__if_res_40; });
Canvas_draw_shadow(canvas, x, y, (*surface).width, (*surface).height, r, 6, ({ Color _matmp_6 = (&(((*self).tokens).shadow_ambient)); &_matmp_6; }));
Canvas_draw_rounded_rect(canvas, x, y, (*surface).width, (*surface).height, r, ({ Color _matmp_5 = (&(((*self).tokens).surface_bg)); &_matmp_5; }));
Canvas_draw_rounded_rect(canvas, x, y, (*surface).width, (*surface).height, r, ({ Color _matmp_5 = (&(((*self).tokens).border_stroke)); &_matmp_5; }));
({ if (((*surface).has_specular_sheen && ((*surface).width > 20))) {
Canvas_draw_line(canvas, (x + r), (y + 1), ((x + (*surface).width) - r), (y + 1), 1.2, ({ Color _matmp_5 = (&(((*self).tokens).specular_sheen)); &_matmp_5; }));
}
});
}

void GlassRenderer_draw_glass_card(GlassRenderer* self, Canvas* canvas, GlassCardWidget* card, double x, double y) {
double r = ({ double __if_res_42; if (((*card).corner_radius > 0)) {
__if_res_42 = (*card).corner_radius;
} else {
__if_res_42 = ((*self).tokens).corner_radius;
}
__if_res_42; });
double elev = ({ double __if_res_43; if ((*card).is_hovered) {
__if_res_43 = ((*card).elevation + 4);
} else {
__if_res_43 = (*card).elevation;
}
__if_res_43; });
({ if ((*card).is_hovered) {
Canvas_draw_glow(canvas, x, y, (*card).width, (*card).height, r, 14, ({ Color _matmp_6 = (&(((*self).tokens).accent_glow)); &_matmp_6; }));
}
});
Canvas_draw_shadow(canvas, x, y, (*card).width, (*card).height, r, elev, ({ Color _matmp_6 = (&(((*self).tokens).shadow_ambient)); &_matmp_6; }));
Canvas_draw_rounded_rect(canvas, x, y, (*card).width, (*card).height, r, ({ Color _matmp_5 = (&(((*self).tokens).surface_bg)); &_matmp_5; }));
Canvas_draw_rounded_rect(canvas, x, y, (*card).width, (*card).height, r, ({ Color _matmp_5 = (&(((*self).tokens).border_stroke)); &_matmp_5; }));
Canvas_draw_line(canvas, (x + r), (y + 1), ((x + (*card).width) - r), (y + 1), 1.5, ({ Color _matmp_5 = (&(((*self).tokens).specular_sheen)); &_matmp_5; }));
({ if (((*card).title.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*card).title; rt_string_retain(&_c); _c; }), (x + 18), (y + 24), 16, ({ Color _matmp_4 = (&(((*self).tokens).text_primary)); &_matmp_4; }));
}
});
({ if (((*card).subtitle.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*card).subtitle; rt_string_retain(&_c); _c; }), (x + 18), (y + 48), 13, ({ Color _matmp_4 = (&(((*self).tokens).text_secondary)); &_matmp_4; }));
}
});
({ if (((*card).badge_text.length > 0LL)) {
double bw = (((double)((*card).badge_text.length) * 7.5) + 18);
double bx = (((x + (*card).width) - bw) - 16);
double by = (y + 16);
Canvas_draw_rounded_rect(canvas, bx, by, bw, 22, 11, ({ Color _matmp_5 = (&(((*self).tokens).border_stroke)); &_matmp_5; }));
Canvas_draw_text(canvas, ({ rt_string_t _c = (*card).badge_text; rt_string_retain(&_c); _c; }), (bx + 9), (by + 4), 11, ({ Color _matmp_4 = (&(((*self).tokens).accent_glow)); &_matmp_4; }));
}
});
}

void GlassRenderer_draw_layered_3d_card(GlassRenderer* self, Canvas* canvas, Layered3DCardWidget* card, double x, double y) {
double r = ((*self).tokens).corner_radius;
Canvas_draw_shadow(canvas, x, y, (*card).width, (*card).height, r, 8, ({ Color _matmp_6 = (&(((*self).tokens).shadow_ambient)); &_matmp_6; }));
Canvas_draw_rounded_rect(canvas, x, y, (*card).width, (*card).height, r, ({ Color _matmp_5 = (&(((*self).tokens).surface_bg)); &_matmp_5; }));
Canvas_draw_rounded_rect(canvas, x, y, (*card).width, (*card).height, r, ({ Color _matmp_5 = (&(((*self).tokens).border_stroke)); &_matmp_5; }));
double fx = ((x + 12) + ((*card).tilt_x * 0.4));
double fy = ((y + 12) + ((*card).tilt_y * 0.4));
double fw = ((*card).width - 24);
double fh = ((*card).height - 24);
double fr = (r - 4);
Canvas_draw_shadow(canvas, fx, fy, fw, fh, fr, (*card).depth_offset, ({ Color _matmp_6 = (&(((*self).tokens).shadow_ambient)); &_matmp_6; }));
Canvas_draw_glow(canvas, fx, fy, fw, fh, fr, 8, ({ Color _matmp_6 = (&(((*self).tokens).accent_glow)); &_matmp_6; }));
Canvas_draw_rounded_rect(canvas, fx, fy, fw, fh, fr, ({ Color _matmp_5 = (&(((*self).tokens).surface_bg)); &_matmp_5; }));
Canvas_draw_rounded_rect(canvas, fx, fy, fw, fh, fr, ({ Color _matmp_5 = (&(((*self).tokens).specular_sheen)); &_matmp_5; }));
({ if (((*card).title.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*card).title; rt_string_retain(&_c); _c; }), (fx + 16), (fy + 24), 17, ({ Color _matmp_4 = (&(((*self).tokens).text_primary)); &_matmp_4; }));
}
});
({ if (((*card).subtitle.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*card).subtitle; rt_string_retain(&_c); _c; }), (fx + 16), (fy + 50), 13, ({ Color _matmp_4 = (&(((*self).tokens).text_secondary)); &_matmp_4; }));
}
});
}

void GlassRenderer_draw_glass_button(GlassRenderer* self, Canvas* canvas, GlassButtonWidget* btn, double x, double y) {
double r = ((*btn).height * 0.5);
({ if (((*btn).is_hovered || ((*btn).glow_spread > 0))) {
double spread = ({ double __if_res_51; if (((*btn).glow_spread > 0)) {
__if_res_51 = (*btn).glow_spread;
} else {
__if_res_51 = 12;
}
__if_res_51; });
Canvas_draw_glow(canvas, x, y, (*btn).width, (*btn).height, r, spread, ({ Color _matmp_6 = (&(((*self).tokens).accent_glow)); &_matmp_6; }));
}
});
Color bg = ((*self).tokens).surface_bg;
({ if ((*btn).is_pressed) {
bg = ((*self).tokens).accent_glow;
}
});
Canvas_draw_shadow(canvas, x, y, (*btn).width, (*btn).height, r, 2, ({ Color _matmp_6 = (&(((*self).tokens).shadow_ambient)); &_matmp_6; }));
Canvas_draw_rounded_rect(canvas, x, y, (*btn).width, (*btn).height, r, ({ Color _matmp_5 = (&(bg)); &_matmp_5; }));
Canvas_draw_rounded_rect(canvas, x, y, (*btn).width, (*btn).height, r, ({ Color _matmp_5 = (&(((*self).tokens).border_stroke)); &_matmp_5; }));
Canvas_draw_line(canvas, (x + r), (y + 1), ((x + (*btn).width) - r), (y + 1), 1.2, ({ Color _matmp_5 = (&(((*self).tokens).specular_sheen)); &_matmp_5; }));
double tx = (x + 16);
double ty = (y + (((*btn).height - 14) * 0.5));
Canvas_draw_text(canvas, ({ rt_string_t _c = (*btn).label; rt_string_retain(&_c); _c; }), tx, ty, 14, ({ Color _matmp_4 = (&(((*self).tokens).text_primary)); &_matmp_4; }));
}

void GlassRenderer_draw_glass_scrubber(GlassRenderer* self, Canvas* canvas, GlassScrubberWidget* sc, double x, double y) {
double track_h = 6;
double track_y = (y + (((*sc).height - track_h) * 0.5));
double clamp_p = ({ double __if_res_53; if (((*sc).progress < 0)) {
__if_res_53 = 0;
} else if (((*sc).progress > 1)) {
__if_res_53 = 1;
} else {
__if_res_53 = (*sc).progress;
}
__if_res_53; });
Canvas_draw_rounded_rect(canvas, x, track_y, (*sc).width, track_h, 3, ({ Color _matmp_5 = (&(((*self).tokens).surface_bg)); &_matmp_5; }));
Canvas_draw_rounded_rect(canvas, x, track_y, (*sc).width, track_h, 3, ({ Color _matmp_5 = (&(((*self).tokens).border_stroke)); &_matmp_5; }));
double active_w = ((*sc).width * clamp_p);
({ if ((active_w > 0)) {
Canvas_draw_glow(canvas, x, track_y, active_w, track_h, 3, 4, ({ Color _matmp_6 = (&(((*self).tokens).accent_glow)); &_matmp_6; }));
Canvas_draw_rounded_rect(canvas, x, track_y, active_w, track_h, 3, ({ Color _matmp_5 = (&(((*self).tokens).accent_glow)); &_matmp_5; }));
}
});
double thumb_x = ((x + active_w) - 6);
double thumb_y = (y + (((*sc).height - 16) * 0.5));
Canvas_draw_glow(canvas, thumb_x, thumb_y, 12, 16, 6, 6, ({ Color _matmp_6 = (&(((*self).tokens).accent_glow)); &_matmp_6; }));
Canvas_draw_rounded_rect(canvas, thumb_x, thumb_y, 12, 16, 6, ({ Color _matmp_5 = (&(((*self).tokens).text_primary)); &_matmp_5; }));
({ if (((*sc).is_hovered && ((*sc).hover_time_text.length > 0LL))) {
double tip_w = 48;
double tip_h = 22;
double tip_x = ((thumb_x - (tip_w * 0.5)) + 6);
double tip_y = ((y - tip_h) - 6);
Canvas_draw_shadow(canvas, tip_x, tip_y, tip_w, tip_h, 6, 2, ({ Color _matmp_6 = (&(((*self).tokens).shadow_ambient)); &_matmp_6; }));
Canvas_draw_rounded_rect(canvas, tip_x, tip_y, tip_w, tip_h, 6, ({ Color _matmp_5 = (&(((*self).tokens).surface_bg)); &_matmp_5; }));
Canvas_draw_rounded_rect(canvas, tip_x, tip_y, tip_w, tip_h, 6, ({ Color _matmp_5 = (&(((*self).tokens).border_stroke)); &_matmp_5; }));
Canvas_draw_text(canvas, ({ rt_string_t _c = (*sc).hover_time_text; rt_string_retain(&_c); _c; }), (tip_x + 8), (tip_y + 4), 11, ({ Color _matmp_4 = (&(((*self).tokens).text_primary)); &_matmp_4; }));
}
});
}

void GlassRenderer_draw_spectrum_visualizer(GlassRenderer* self, Canvas* canvas, SpectrumVisualizerWidget* spec, double x, double y) {
double cur_x = x;
int64_t count = (*spec).bar_heights.len;
int64_t i = 0LL;
while ((i < count)) {
int64_t val = rt_vec_get_at((*spec).bar_heights, (size_t)(i));
double clamp_h = ({ double __if_res_56; if ((val > (*spec).max_height)) {
__if_res_56 = (*spec).max_height;
} else if ((val < 2)) {
__if_res_56 = 2;
} else {
__if_res_56 = val;
}
__if_res_56; });
double bar_y = ((y + (*spec).max_height) - clamp_h);
Canvas_draw_rounded_rect(canvas, cur_x, bar_y, (*spec).bar_width, clamp_h, ((*spec).bar_width * 0.5), ({ Color _matmp_5 = (&(((*self).tokens).accent_glow)); &_matmp_5; }));
({ if ((*spec).show_peak_markers) {
double peak_y = (bar_y - 3);
Canvas_draw_rect(canvas, cur_x, peak_y, (*spec).bar_width, 1.5, ({ Color _matmp_4 = (&(((*self).tokens).text_primary)); &_matmp_4; }));
}
});
cur_x = ((cur_x + (*spec).bar_width) + (*spec).bar_spacing);
i = (i + 1LL);
}
;
}

void GlassRenderer_draw_aero_terminal(GlassRenderer* self, Canvas* canvas, AeroTerminalWidget* term, double x, double y) {
double r = 12;
Canvas_draw_shadow(canvas, x, y, (*term).width, (*term).height, r, 6, ({ Color _matmp_6 = (&(((*self).tokens).shadow_ambient)); &_matmp_6; }));
Canvas_draw_rounded_rect(canvas, x, y, (*term).width, (*term).height, r, ({ Color _matmp_5 = (&(((*self).tokens).surface_bg)); &_matmp_5; }));
Canvas_draw_rounded_rect(canvas, x, y, (*term).width, (*term).height, r, ({ Color _matmp_5 = (&(((*self).tokens).border_stroke)); &_matmp_5; }));
Canvas_draw_line(canvas, x, (y + 32), (x + (*term).width), (y + 32), 1, ({ Color _matmp_5 = (&(((*self).tokens).border_stroke)); &_matmp_5; }));
Canvas_draw_circle(canvas, (x + 16), (y + 16), 4.5, ({ Color _matmp_3 = ({ Color _utmp_ = Color_hex(15680580LL); &_utmp_; }); &_matmp_3; }));
Canvas_draw_circle(canvas, (x + 28), (y + 16), 4.5, ({ Color _matmp_3 = ({ Color _utmp_ = Color_hex(16096779LL); &_utmp_; }); &_matmp_3; }));
Canvas_draw_circle(canvas, (x + 40), (y + 16), 4.5, ({ Color _matmp_3 = ({ Color _utmp_ = Color_hex(1096065LL); &_utmp_; }); &_matmp_3; }));
({ if (((*term).prompt_title.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*term).prompt_title; rt_string_retain(&_c); _c; }), (x + 56), (y + 10), 12, ({ Color _matmp_4 = (&(((*self).tokens).text_secondary)); &_matmp_4; }));
}
});
int64_t line_count = (*term).lines.len;
double line_y = (y + 46);
int64_t j = 0LL;
while ((j < line_count)) {
int64_t l = rt_vec_get_at((*term).lines, (size_t)(j));
Canvas_draw_text(canvas, l, (x + 16), line_y, 12, ({ Color _matmp_4 = (&(((*self).tokens).text_primary)); &_matmp_4; }));
line_y = (line_y + 18);
j = (j + 1LL);
}
;
}

void GlassRenderer_draw_glass_badge(GlassRenderer* self, Canvas* canvas, GlassBadgeWidget* badge, double x, double y) {
double bw = (((double)((*badge).text.length) * 7.5) + ({ double __if_res_59; if ((*badge).is_pulsing) {
__if_res_59 = 28;
} else {
__if_res_59 = 16;
}
__if_res_59; }));
double bh = 22;
double r = (bh * 0.5);
Canvas_draw_rounded_rect(canvas, x, y, bw, bh, r, ({ Color _matmp_5 = (&(((*self).tokens).surface_bg)); &_matmp_5; }));
Canvas_draw_rounded_rect(canvas, x, y, bw, bh, r, ({ Color _matmp_5 = (&(((*self).tokens).border_stroke)); &_matmp_5; }));
double text_x = (x + 8);
({ if ((*badge).is_pulsing) {
Canvas_draw_glow(canvas, (x + 8), (y + 7), 8, 8, 4, 4, ({ Color _matmp_6 = (&((*badge).custom_accent)); &_matmp_6; }));
Canvas_draw_circle(canvas, (x + 12), (y + 11), 3.5, ({ Color _matmp_3 = (&((*badge).custom_accent)); &_matmp_3; }));
text_x = (x + 22);
}
});
Canvas_draw_text(canvas, ({ rt_string_t _c = (*badge).text; rt_string_retain(&_c); _c; }), text_x, (y + 4), 11, ({ Color _matmp_4 = (&(((*self).tokens).text_primary)); &_matmp_4; }));
}

void GlassRenderer_draw_glass_switch(GlassRenderer* self, Canvas* canvas, GlassSwitchWidget* sw, double x, double y) {
double w = ({ double __if_res_61; if (((*sw).width > 0)) {
__if_res_61 = (*sw).width;
} else {
__if_res_61 = 48;
}
__if_res_61; });
double h = ({ double __if_res_62; if (((*sw).height > 0)) {
__if_res_62 = (*sw).height;
} else {
__if_res_62 = 26;
}
__if_res_62; });
double r = (h * 0.5);
({ if ((*sw).is_on) {
Canvas_draw_glow(canvas, x, y, w, h, r, 6, ({ Color _matmp_6 = (&(((*self).tokens).accent_glow)); &_matmp_6; }));
Canvas_draw_rounded_rect(canvas, x, y, w, h, r, ({ Color _matmp_5 = (&(((*self).tokens).accent_glow)); &_matmp_5; }));
double thumb_x = (((x + w) - r) - 6);
double thumb_y = (y + 3);
double thumb_r = (r - 3);
Canvas_draw_rounded_rect(canvas, thumb_x, thumb_y, (thumb_r * 2), (thumb_r * 2), thumb_r, ({ Color _matmp_5 = (&(((*self).tokens).text_primary)); &_matmp_5; }));
} else {
Canvas_draw_rounded_rect(canvas, x, y, w, h, r, ({ Color _matmp_5 = (&(((*self).tokens).surface_bg)); &_matmp_5; }));
Canvas_draw_rounded_rect(canvas, x, y, w, h, r, ({ Color _matmp_5 = (&(((*self).tokens).border_stroke)); &_matmp_5; }));
double thumb_x = (x + 4);
double thumb_y = (y + 4);
double thumb_r = (r - 4);
Canvas_draw_rounded_rect(canvas, thumb_x, thumb_y, (thumb_r * 2), (thumb_r * 2), thumb_r, ({ Color _matmp_5 = (&(((*self).tokens).text_secondary)); &_matmp_5; }));
}
});
}

int64_t Clock_now_ms() {
return ({
nyx_time_millis();
});
}

AnimationController AnimationController_new(int64_t duration_ms) {
return (AnimationController){.start_time = ({
nyx_time_millis();
}), .duration_ms = duration_ms, .is_running = 1, .is_reverse = 0};
}

double AnimationController_progress(AnimationController* self) {
({ if ((!(*self).is_running)) {
if ((*self).is_reverse) {
return 0;
} else {
return 1;
}
}
});
int64_t now = ({
nyx_time_millis();
});
int64_t elapsed = (now - (*self).start_time);
({ if ((elapsed >= (*self).duration_ms)) {
if ((*self).is_reverse) {
return 0;
} else {
return 1;
}
}
});
double raw = ((double)(elapsed) / (double)((*self).duration_ms));
if ((*self).is_reverse) {
return (1 - raw);
} else {
return raw;
}
}

double AnimationController_value(AnimationController* self) {
double t = AnimationController_progress(self);
return (1 - (((1 - t) * (1 - t)) * (1 - t)));
}

void AnimationController_restart(AnimationController* self) {
(self->start_time = ({
nyx_time_millis();
}));
(self->is_running = 1);
}

void AnimationController_reverse(AnimationController* self) {
(self->start_time = ({
nyx_time_millis();
}));
(self->is_reverse = 1);
(self->is_running = 1);
}

Padding Padding_all(double value) {
return (Padding){.left = value, .top = value, .right = value, .bottom = value};
}

Padding Padding_symmetric(double horizontal, double vertical) {
return (Padding){.left = horizontal, .top = vertical, .right = horizontal, .bottom = vertical};
}

CenterBox CenterBox_new(double width, double height) {
return (CenterBox){.width = width, .height = height};
}

SizedBox SizedBox_new(double width, double height) {
return (SizedBox){.width = width, .height = height};
}

SizedBox SizedBox_square(double dimension) {
return (SizedBox){.width = dimension, .height = dimension};
}

Expanded Expanded_new(int64_t flex) {
return (Expanded){.flex = flex};
}

Align Align_new(Alignment alignment) {
return (Align){.alignment = alignment};
}

SpringSimulation SpringSimulation_new(double stiffness, double damping, double target) {
return (SpringSimulation){.stiffness = stiffness, .damping = damping, .position = 0, .velocity = 0, .target = target};
}

double SpringSimulation_update(SpringSimulation* self, double dt) {
double force = (-((*self).stiffness * ((*self).position - (*self).target)));
double damping_force = (-((*self).damping * (*self).velocity));
double accel = (force + damping_force);
(self->velocity = ((*self).velocity + (accel * dt)));
(self->position = ((*self).position + ((*self).velocity * dt)));
return (*self).position;
}

ThemeProvider ThemeProvider_new(int is_dark) {
if (is_dark) {
return (ThemeProvider){.is_dark = 1, .primary_color = Color_hex(11032055LL), .background_color = Color_hex(988970LL), .surface_color = Color_hex(1976635LL), .text_color = Color_hex(16317180LL)};
} else {
return (ThemeProvider){.is_dark = 0, .primary_color = Color_hex(6514417LL), .background_color = Color_hex(16317180LL), .surface_color = Color_hex(16777215LL), .text_color = Color_hex(988970LL)};
}
}

void ThemeProvider_toggle_theme(ThemeProvider* self) {
(self->is_dark = (!(*self).is_dark));
({ if ((*self).is_dark) {
(self->primary_color = Color_hex(11032055LL));
(self->background_color = Color_hex(988970LL));
(self->surface_color = Color_hex(1976635LL));
(self->text_color = Color_hex(16317180LL));
} else {
(self->primary_color = Color_hex(6514417LL));
(self->background_color = Color_hex(16317180LL));
(self->surface_color = Color_hex(16777215LL));
(self->text_color = Color_hex(988970LL));
}
});
}

void debugDumpApp() {
Arena* __arena = arena_create();
({
nyx_skia_draw_text(0LL, ((const uint8_t*)(rt_string_from("Nyx UI Tree Dump: [Window -> Scaffold -> Column -> (Padding, SizedBox, Expanded)]").data)), 10, 30, 12, 2173499647LL);
});
arena_destroy(__arena);
}

