#include "runtime/rt_db.h"
#include <stdio.h>
#include <stdint.h>
#include "runtime/rt_string.h"
#include "runtime/rt_vec.h"
#include <stdlib.h>
#include <math.h>
#include "runtime/rt_map.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_option.h"
#include <string.h>
#include "runtime/rt_json.h"
#include "runtime/rt_async.h"
#include "runtime/rt_http.h"
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
} Center;

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
int64_t Clock_now_ms();
AnimationController AnimationController_new(int64_t);
double AnimationController_progress(AnimationController*);
double AnimationController_value(AnimationController*);
void AnimationController_restart(AnimationController*);
void AnimationController_reverse(AnimationController*);
Padding Padding_all(double);
Padding Padding_symmetric(double, double);
Center Center_new(double, double);
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
void* nyx_skia_surface_create(int64_t p0, int64_t p1); // extern
void nyx_skia_surface_destroy(void* p0); // extern
void* nyx_skia_canvas_create(void* p0); // extern
void nyx_skia_canvas_destroy(void* p0); // extern
void nyx_skia_draw_rect(void* p0, double p1, double p2, double p3, double p4, int64_t p5); // extern
void nyx_skia_draw_rounded_rect(void* p0, double p1, double p2, double p3, double p4, double p5, int64_t p6); // extern
void nyx_skia_draw_circle(void* p0, double p1, double p2, double p3, int64_t p4); // extern
void nyx_skia_draw_gradient_rect(void* p0, double p1, double p2, double p3, double p4, int64_t p5, int64_t p6); // extern
void nyx_skia_draw_shadow(void* p0, double p1, double p2, double p3, double p4, double p5, double p6, int64_t p7); // extern
void nyx_skia_draw_glow(void* p0, double p1, double p2, double p3, double p4, double p5, double p6, int64_t p7); // extern
void nyx_skia_draw_ripple(void* p0, double p1, double p2, double p3, double p4, int64_t p5); // extern
void nyx_skia_draw_text(void* p0, char* p1, double p2, double p3, double p4, int64_t p5); // extern
void nyx_skia_draw_line(void* p0, double p1, double p2, double p3, double p4, double p5, int64_t p6); // extern
void nyx_skia_fill(void* p0, int64_t p1); // extern
int64_t nyx_time_millis(); // extern
void nyx_skia_save(void* p0); // extern
void nyx_skia_restore(void* p0); // extern
void nyx_skia_translate(void* p0, double p1, double p2); // extern
void nyx_skia_rotate(void* p0, double p1); // extern
void nyx_skia_scale(void* p0, double p1, double p2); // extern
void* nyx_window_create(int64_t p0, int64_t p1, char* p2); // extern
void nyx_window_destroy(void* p0); // extern
int64_t nyx_window_should_close(void* p0); // extern
void nyx_window_poll_events(void* p0); // extern
void* nyx_window_canvas(void* p0); // extern
void nyx_window_swap_buffers(void* p0); // extern
int64_t nyx_window_width(void* p0); // extern
int64_t nyx_window_height(void* p0); // extern
void nyx_window_set_title(void* p0, char* p1); // extern
int64_t nyx_window_mouse_x(void* p0); // extern
int64_t nyx_window_mouse_y(void* p0); // extern
int64_t nyx_window_mouse_down(void* p0); // extern
int64_t nyx_window_last_key(void* p0); // extern
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
return ((((px >= rx) && (px <= (rx + rw))) && (py >= ry)) && (py <= (ry + rh)));
}

MaterialRenderer MaterialRenderer_new(MaterialTheme theme) {
return (MaterialRenderer){.theme = theme, .shapes = M3Shape_default(), .typography = M3Typography_default()};
}

void MaterialRenderer_draw_text_widget(MaterialRenderer* self, Canvas* canvas, TextWidget* t, double x, double y) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*t).text; rt_string_retain(&_c); _c; }), x, y, (*t).font_size, (&((*t).color)));
}

void MaterialRenderer_draw_text(MaterialRenderer* self, Canvas* canvas, TextWidget* t, double x, double y) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*t).text; rt_string_retain(&_c); _c; }), x, y, (*t).font_size, (&((*t).color)));
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
Canvas_draw_shadow(canvas, x, y, (*b).width, (*b).height, radius, elev, ({ Color _utmp_ = Color_hex(1073741824LL); &_utmp_; }));
}
});
({ if ((bg.a > 0)) {
Canvas_draw_rounded_rect(canvas, x, y, (*b).width, (*b).height, radius, (&(bg)));
}
});
({ if (((*b).variant.tag == (ButtonVariant){ .tag = TAG_ButtonVariant_Outlined }.tag)) {
Canvas_draw_rounded_rect(canvas, x, y, (*b).width, (*b).height, radius, (&(((*self).theme).outline)));
}
});
int64_t text_y = (y + (((*b).height - ((*self).typography).label_large) / 2));
int64_t text_x = (x + 16);
Canvas_draw_text(canvas, ({ rt_string_t _c = (*b).label; rt_string_retain(&_c); _c; }), text_x, text_y, ((*self).typography).label_large, (&(fg)));
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
Canvas_draw_shadow(canvas, x, y, (*c).width, (*c).height, ((*self).shapes).medium, elev, ({ Color _utmp_ = Color_hex(805306368LL); &_utmp_; }));
}
});
Canvas_draw_rounded_rect(canvas, x, y, (*c).width, (*c).height, ((*self).shapes).medium, (&(bg)));
({ if ((border_color.a > 0)) {
Canvas_draw_rounded_rect(canvas, x, y, (*c).width, (*c).height, ((*self).shapes).medium, (&(border_color)));
}
});
({ if (((*c).title.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*c).title; rt_string_retain(&_c); _c; }), (x + 16), (y + 16), ((*self).typography).title_medium, (&(((*self).theme).on_surface)));
}
});
({ if (((*c).subtitle.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*c).subtitle; rt_string_retain(&_c); _c; }), (x + 16), (y + 42), ((*self).typography).body_medium, (&(((*self).theme).on_surface_variant)));
}
});
}

void MaterialRenderer_draw_badge(MaterialRenderer* self, Canvas* canvas, BadgeWidget* b, double x, double y) {
int64_t bw = (((double)((*b).text.length) * 8) + 16);
double bh = 22;
Canvas_draw_rounded_rect(canvas, x, y, bw, bh, ((*self).shapes).small, (&((*b).bg_color)));
Canvas_draw_text(canvas, ({ rt_string_t _c = (*b).text; rt_string_retain(&_c); _c; }), (x + 8), (y + 4), ((*self).typography).label_medium, (&((*b).text_color)));
}

void MaterialRenderer_draw_navigation_rail(MaterialRenderer* self, Canvas* canvas, NavigationRailWidget* rail, double x, double y, double h) {
Canvas_draw_rect(canvas, x, y, (*rail).width, h, (&(((*self).theme).surface_container)));
}

void MaterialRenderer_draw_top_app_bar(MaterialRenderer* self, Canvas* canvas, TopAppBarWidget* bar, double x, double y) {
Canvas_draw_rect(canvas, x, y, (*bar).width, 64, (&(((*self).theme).surface)));
Canvas_draw_text(canvas, ({ rt_string_t _c = (*bar).title; rt_string_retain(&_c); _c; }), (x + 24), (y + 18), ((*self).typography).title_large, (&(((*self).theme).on_surface)));
({ if (((*bar).subtitle.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*bar).subtitle; rt_string_retain(&_c); _c; }), (x + 24), (y + 42), ((*self).typography).label_medium, (&(((*self).theme).on_surface_variant)));
}
});
Canvas_draw_line(canvas, x, (y + 64), (x + (*bar).width), (y + 64), 1, (&(((*self).theme).outline_variant)));
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
int64_t radius = (h * 0.5);
({ if ((*sw).is_on) {
Canvas_draw_rounded_rect(canvas, x, y, w, h, radius, (&(((*self).theme).primary)));
int64_t thumb_x = (((x + w) - radius) - 8);
int64_t thumb_y = (y + 4);
int64_t thumb_r = (radius - 4);
Canvas_draw_rounded_rect(canvas, thumb_x, thumb_y, (thumb_r * 2), (thumb_r * 2), thumb_r, (&(((*self).theme).on_primary)));
} else {
Canvas_draw_rounded_rect(canvas, x, y, w, h, radius, (&(((*self).theme).surface_container_high)));
Canvas_draw_rounded_rect(canvas, x, y, w, h, radius, (&(((*self).theme).outline)));
int64_t thumb_x = (x + 6);
int64_t thumb_y = (y + 6);
int64_t thumb_r = (radius - 6);
Canvas_draw_rounded_rect(canvas, thumb_x, thumb_y, (thumb_r * 2), (thumb_r * 2), thumb_r, (&(((*self).theme).outline)));
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
int64_t track_y = (y + ((h - track_h) * 0.5));
double clamp_val = ({ double __if_res_16; if (((*sl).value < 0)) {
__if_res_16 = 0;
} else if (((*sl).value > 1)) {
__if_res_16 = 1;
} else {
__if_res_16 = (*sl).value;
}
__if_res_16; });
Canvas_draw_rounded_rect(canvas, x, track_y, w, track_h, 3, (&(((*self).theme).surface_container_high)));
int64_t active_w = (w * clamp_val);
({ if ((active_w > 0)) {
Canvas_draw_rounded_rect(canvas, x, track_y, active_w, track_h, 3, (&(((*self).theme).primary)));
}
});
int64_t thumb_x = ((x + active_w) - 6);
int64_t thumb_y = (y + ((h - 18) * 0.5));
Canvas_draw_shadow(canvas, thumb_x, thumb_y, 12, 18, 4, 2, ({ Color _utmp_ = Color_hex(805306368LL); &_utmp_; }));
Canvas_draw_rounded_rect(canvas, thumb_x, thumb_y, 12, 18, 4, (&(((*self).theme).primary)));
}

void MaterialRenderer_draw_checkbox(MaterialRenderer* self, Canvas* canvas, CheckboxWidget* cb, double x, double y) {
double sz = ({ double __if_res_18; if (((*cb).size > 0)) {
__if_res_18 = (*cb).size;
} else {
__if_res_18 = 20;
}
__if_res_18; });
({ if ((*cb).is_checked) {
Canvas_draw_rounded_rect(canvas, x, y, sz, sz, ((*self).shapes).extra_small, (&(((*self).theme).primary)));
Canvas_draw_text(canvas, rt_string_from("v"), (x + 4), (y + 2), ((*self).typography).label_medium, (&(((*self).theme).on_primary)));
} else {
Canvas_draw_rounded_rect(canvas, x, y, sz, sz, ((*self).shapes).extra_small, (&(((*self).theme).surface)));
Canvas_draw_rounded_rect(canvas, x, y, sz, sz, ((*self).shapes).extra_small, (&(((*self).theme).outline)));
}
});
({ if (((*cb).label.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*cb).label; rt_string_retain(&_c); _c; }), ((x + sz) + 12), (y + 2), ((*self).typography).body_medium, (&(((*self).theme).on_surface)));
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
Canvas_draw_rounded_rect(canvas, x, y, w, h, ((*self).shapes).extra_small, (&(((*self).theme).surface_container)));
({ if (((*tf).label.length > 0LL)) {
Color* label_color = ({ Color* __if_res_24; if ((*tf).is_focused) {
__if_res_24 = (&(((*self).theme).primary));
} else {
__if_res_24 = (&(((*self).theme).on_surface_variant));
}
__if_res_24; });
Canvas_draw_text(canvas, ({ rt_string_t _c = (*tf).label; rt_string_retain(&_c); _c; }), (x + 16), (y + 8), ((*self).typography).label_medium, label_color);
}
});
rt_string_t display_text = ({ rt_string_t __if_res_25; if (((*tf).text.length > 0LL)) {
__if_res_25 = ({ rt_string_t _c = (*tf).text; rt_string_retain(&_c); _c; });
} else {
__if_res_25 = rt_string_from("Type here...");
}
__if_res_25; });
Color* text_color = ({ Color* __if_res_26; if (((*tf).text.length > 0LL)) {
__if_res_26 = (&(((*self).theme).on_surface));
} else {
__if_res_26 = (&(((*self).theme).outline));
}
__if_res_26; });
Canvas_draw_text(canvas, display_text, (x + 16), (y + 28), ((*self).typography).body_large, text_color);
Color* line_color = ({ Color* __if_res_27; if ((*tf).is_focused) {
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
Canvas_draw_rect(canvas, x, ((y + h) - line_h), w, line_h, line_color);
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
Canvas_draw_rounded_rect(canvas, x, y, w, h, 4, (&(((*self).theme).surface_container_high)));
int64_t fill_w = (w * clamp_val);
({ if ((fill_w > 0)) {
Canvas_draw_rounded_rect(canvas, x, y, fill_w, h, 4, (&(((*self).theme).primary)));
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
int64_t current_elevation = ({ int64_t __if_res_35; if (is_hovered) {
__if_res_35 = (3 + (anim_progress * 2));
} else {
__if_res_35 = 1;
}
__if_res_35; });
({ if (is_hovered) {
Canvas_draw_glow(canvas, x, y, w, h, radius, (8 * anim_progress), (&(((*self).theme).primary)));
}
});
Canvas_draw_shadow(canvas, x, y, w, h, radius, current_elevation, ({ Color _utmp_ = Color_hex(1073741824LL); &_utmp_; }));
Canvas_draw_rounded_rect(canvas, x, y, w, h, radius, (&(((*self).theme).surface_container)));
({ if (((*card).title.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*card).title; rt_string_retain(&_c); _c; }), (x + 16), (y + 20), ((*self).typography).title_medium, (&(((*self).theme).on_surface)));
}
});
({ if (((*card).subtitle.length > 0LL)) {
Canvas_draw_text(canvas, ({ rt_string_t _c = (*card).subtitle; rt_string_retain(&_c); _c; }), (x + 16), (y + 46), ((*self).typography).body_medium, (&(((*self).theme).on_surface_variant)));
}
});
}

void MaterialRenderer_draw_glow_button(MaterialRenderer* self, Canvas* canvas, rt_string_t label, double x, double y, double w, double h, double glow_alpha) {
double radius = ((*self).shapes).full;
({ if ((glow_alpha > 0)) {
Canvas_draw_glow(canvas, x, y, w, h, radius, (12 * glow_alpha), (&(((*self).theme).primary)));
}
});
Canvas_draw_rounded_rect(canvas, x, y, w, h, radius, (&(((*self).theme).primary)));
Canvas_draw_text(canvas, label, (x + (w * 0.2)), (y + (h * 0.3)), ((*self).typography).label_large, (&(((*self).theme).on_primary)));
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
int64_t raw = ((double)(elapsed) / (double)((*self).duration_ms));
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

Center Center_new(double width, double height) {
return (std.ui.Alignment){ .tag = TAG_std.ui.Alignment_Center, .data.Center = { .f0 = width, .f1 = height } };
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
int64_t force = (-((*self).stiffness * ((*self).position - (*self).target)));
int64_t damping_force = (-((*self).damping * (*self).velocity));
int64_t accel = (force + damping_force);
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
({
nyx_skia_draw_text(0LL, ((char*)(rt_string_from("Nyx UI Tree Dump: [Window -> Scaffold -> Column -> (Padding, SizedBox, Expanded)]").data)), 10, 30, 12, 2173499647LL);
});
}

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

void nyx_main();
// import std::ui
void nyx_main() {
printf("%s\n", rt_string_from("=== Nyx Google Material Design 3 Desktop Dashboard ===").data);
printf("%s\n", rt_string_from("Initializing Material Design 3 UI Engine & Native Window...").data);
MaterialTheme theme = MaterialTheme_dark();
MaterialRenderer renderer = MaterialRenderer_new(theme);
Window window = Window_new(960LL, 640LL, rt_string_from("Nyx Material 3 Studio Dashboard"));
Canvas canvas = Window_canvas(&(window));
printf("%s\n", rt_string_from("Rendering Material 3 components...").data);
Canvas_fill(&(canvas), (&(theme.background)));
NyxVec nav_items = rt_vec_new(sizeof(int64_t));
rt_vec_push(&nav_items, ({ NavItem _tmp_0 = (NavItem){.label = rt_string_from("Overview"), .icon = rt_string_from("dashboard"), .is_active = 1}; &_tmp_0; }));
rt_vec_push(&nav_items, ({ NavItem _tmp_1 = (NavItem){.label = rt_string_from("Analytics"), .icon = rt_string_from("analytics"), .is_active = 0}; &_tmp_1; }));
rt_vec_push(&nav_items, ({ NavItem _tmp_2 = (NavItem){.label = rt_string_from("Nodes"), .icon = rt_string_from("dns"), .is_active = 0}; &_tmp_2; }));
rt_vec_push(&nav_items, ({ NavItem _tmp_3 = (NavItem){.label = rt_string_from("Settings"), .icon = rt_string_from("settings"), .is_active = 0}; &_tmp_3; }));
NavigationRailWidget rail = (NavigationRailWidget){.items = nav_items, .width = 180};
MaterialRenderer_draw_navigation_rail(&(renderer), (&(canvas)), (&(rail)), 0, 0, 640);
TopAppBarWidget app_bar = (TopAppBarWidget){.title = rt_string_from("Nyx Cloud Enterprise Monitor"), .subtitle = rt_string_from("Material 3 Autonomous Cluster / v0.13.0"), .width = 780};
MaterialRenderer_draw_top_app_bar(&(renderer), (&(canvas)), (&(app_bar)), 180, 0);
CardWidget card1 = (CardWidget){.title = rt_string_from("Cluster Throughput"), .subtitle = rt_string_from("1.48M ops/sec (+14.2%)"), .variant = (CardVariant){ .tag = TAG_CardVariant_Elevated }, .width = 230, .height = 120, .elevation = 3};
MaterialRenderer_draw_card(&(renderer), (&(canvas)), (&(card1)), 204, 88);
BadgeWidget badge1 = (BadgeWidget){.text = rt_string_from("OPTIMAL"), .bg_color = Color_hex(4279983648LL), .text_color = Color_hex(4291356361LL)};
MaterialRenderer_draw_badge(&(renderer), (&(canvas)), (&(badge1)), 350, 100);
CardWidget card2 = (CardWidget){.title = rt_string_from("P99 Latency"), .subtitle = rt_string_from("180 microseconds"), .variant = (CardVariant){ .tag = TAG_CardVariant_Elevated }, .width = 230, .height = 120, .elevation = 3};
MaterialRenderer_draw_card(&(renderer), (&(canvas)), (&(card2)), 458, 88);
BadgeWidget badge2 = (BadgeWidget){.text = rt_string_from("FAST"), .bg_color = Color_hex(4279060385LL), .text_color = Color_hex(4290502395LL)};
MaterialRenderer_draw_badge(&(renderer), (&(canvas)), (&(badge2)), 620, 100);
CardWidget card3 = (CardWidget){.title = rt_string_from("Memory Pressure"), .subtitle = rt_string_from("12% Bump / 0% GC Pause"), .variant = (CardVariant){ .tag = TAG_CardVariant_Elevated }, .width = 230, .height = 120, .elevation = 3};
MaterialRenderer_draw_card(&(renderer), (&(canvas)), (&(card3)), 712, 88);
BadgeWidget badge3 = (BadgeWidget){.text = rt_string_from("REGION HEAP"), .bg_color = Color_hex(4283045004LL), .text_color = Color_hex(4292984551LL)};
MaterialRenderer_draw_badge(&(renderer), (&(canvas)), (&(badge3)), 830, 100);
CardWidget main_panel = (CardWidget){.title = rt_string_from("Real-time Node Distribution & Concurrency Stream"), .subtitle = rt_string_from("Structured nursery tasks running asynchronously across all cores"), .variant = (CardVariant){ .tag = TAG_CardVariant_Outlined }, .width = 738, .height = 300, .elevation = 0};
MaterialRenderer_draw_card(&(renderer), (&(canvas)), (&(main_panel)), 204, 230);
ButtonWidget btn_filled = (ButtonWidget){.label = rt_string_from("Deploy Pipeline"), .variant = (ButtonVariant){ .tag = TAG_ButtonVariant_Filled }, .width = 160, .height = 40};
MaterialRenderer_draw_button(&(renderer), (&(canvas)), (&(btn_filled)), 224, 470);
ButtonWidget btn_tonal = (ButtonWidget){.label = rt_string_from("Trigger Scale-Out"), .variant = (ButtonVariant){ .tag = TAG_ButtonVariant_Tonal }, .width = 170, .height = 40};
MaterialRenderer_draw_button(&(renderer), (&(canvas)), (&(btn_tonal)), 400, 470);
ButtonWidget btn_outlined = (ButtonWidget){.label = rt_string_from("View Telemetry"), .variant = (ButtonVariant){ .tag = TAG_ButtonVariant_Outlined }, .width = 160, .height = 40};
MaterialRenderer_draw_button(&(renderer), (&(canvas)), (&(btn_outlined)), 586, 470);
Window_swap_buffers(&(window));
printf("%s\n", rt_string_from("Material 3 UI rendered successfully to screen.").data);
Window_poll_events(&(window));
printf("%s\n", rt_string_from("Dashboard initialization complete.").data);
}


int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
