#include "runtime/rt_vec.h"
#include <string.h>
#include "runtime/rt_io.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_http.h"
#include "runtime/rt_json.h"
#include "runtime/rt_option.h"
#include "runtime/rt_db.h"
#include "runtime/rt_map.h"
#include <stdlib.h>
#include "runtime/rt_arena.h"
#include "runtime/rt_async.h"
#include "runtime/rt_string.h"
#include <stdio.h>
#include "runtime/rt_stdlib.h"
#include <math.h>
#include <stdint.h>
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
printf("%s\n", rt_string_from("=== Nyx Google Material Design 3 Interactive Studio ===").data);
printf("%s\n", rt_string_from("Initializing Material 3 Engine and UI components...").data);
int64_t theme = dark();
MaterialRenderer renderer = new(theme);
Window window = new(1024LL, 720LL, rt_string_from("Nyx Material 3 Interactive Studio"));
int64_t canvas = Window_canvas(&(window));
int64_t_fill(canvas, (&(theme.background)));
TopAppBarWidget app_bar = (TopAppBarWidget){.title = rt_string_from("Material 3 Interactive Component Studio"), .subtitle = rt_string_from("Google MD3 Standard Design Tokens & Controls"), .width = 1024};
MaterialRenderer_draw_top_app_bar(&(renderer), (&(canvas)), (&(app_bar)), 0, 0);
CardWidget left_card = (CardWidget){.title = rt_string_from("System Configuration Controls"), .subtitle = rt_string_from("Interactive switches, sliders, and form components"), .variant = (ui.CardVariant).Elevated, .width = 480, .height = 600, .elevation = 2};
MaterialRenderer_draw_card(&(renderer), (&(canvas)), (&(left_card)), 24, 88);
TextFieldWidget tf = (TextFieldWidget){.text = rt_string_from("node-us-east-cluster-04"), .label = rt_string_from("Target Cluster Host"), .is_focused = 1, .width = 432, .height = 56};
MaterialRenderer_draw_text_field(&(renderer), (&(canvas)), (&(tf)), 48, 160);
SwitchWidget sw1 = (SwitchWidget){.is_on = 1, .width = 52, .height = 32};
MaterialRenderer_draw_switch(&(renderer), (&(canvas)), (&(sw1)), 48, 240);
TextWidget sw1_txt = (TextWidget){.text = rt_string_from("Autonomous Dynamic Load Balancing"), .font_size = 14, .color = theme.on_surface};
MaterialRenderer_draw_text(&(renderer), (&(canvas)), (&(sw1_txt)), 112, 246);
SwitchWidget sw2 = (SwitchWidget){.is_on = 0, .width = 52, .height = 32};
MaterialRenderer_draw_switch(&(renderer), (&(canvas)), (&(sw2)), 48, 290);
TextWidget sw2_txt = (TextWidget){.text = rt_string_from("Low-latency Fast-Path Profiler"), .font_size = 14, .color = theme.on_surface};
MaterialRenderer_draw_text(&(renderer), (&(canvas)), (&(sw2_txt)), 112, 296);
CheckboxWidget cb1 = (CheckboxWidget){.is_checked = 1, .label = rt_string_from("Enable SIMD Vectorized Parsing"), .size = 20};
MaterialRenderer_draw_checkbox(&(renderer), (&(canvas)), (&(cb1)), 48, 345);
CheckboxWidget cb2 = (CheckboxWidget){.is_checked = 1, .label = rt_string_from("Zero-copy Region Memory Reclamation"), .size = 20};
MaterialRenderer_draw_checkbox(&(renderer), (&(canvas)), (&(cb2)), 48, 385);
CheckboxWidget cb3 = (CheckboxWidget){.is_checked = 0, .label = rt_string_from("Verbose Hardware Interrupt Logging"), .size = 20};
MaterialRenderer_draw_checkbox(&(renderer), (&(canvas)), (&(cb3)), 48, 425);
TextWidget sl1_label = (TextWidget){.text = rt_string_from("Memory Quota Pressure: 82%"), .font_size = 14, .color = theme.on_surface};
MaterialRenderer_draw_text(&(renderer), (&(canvas)), (&(sl1_label)), 48, 470);
SliderWidget sl1 = (SliderWidget){.value = 0.82, .width = 432, .height = 16};
MaterialRenderer_draw_slider(&(renderer), (&(canvas)), (&(sl1)), 48, 495);
TextWidget sl2_label = (TextWidget){.text = rt_string_from("Worker Thread Affinity Rate: 45%"), .font_size = 14, .color = theme.on_surface};
MaterialRenderer_draw_text(&(renderer), (&(canvas)), (&(sl2_label)), 48, 530);
SliderWidget sl2 = (SliderWidget){.value = 0.45, .width = 432, .height = 16};
MaterialRenderer_draw_slider(&(renderer), (&(canvas)), (&(sl2)), 48, 555);
ButtonWidget btn_save = (ButtonWidget){.label = rt_string_from("Save Settings"), .variant = (ui.ButtonVariant).Filled, .width = 160, .height = 40};
MaterialRenderer_draw_button(&(renderer), (&(canvas)), (&(btn_save)), 48, 615);
ButtonWidget btn_discard = (ButtonWidget){.label = rt_string_from("Discard Changes"), .variant = (ui.ButtonVariant).Outlined, .width = 160, .height = 40};
MaterialRenderer_draw_button(&(renderer), (&(canvas)), (&(btn_discard)), 220, 615);
CardWidget right_card = (CardWidget){.title = rt_string_from("Live Runtime Telemetry"), .subtitle = rt_string_from("Real-time thread & buffer metrics"), .variant = (ui.CardVariant).Outlined, .width = 480, .height = 600, .elevation = 0};
MaterialRenderer_draw_card(&(renderer), (&(canvas)), (&(right_card)), 520, 88);
BadgeWidget badge_opt = (BadgeWidget){.text = rt_string_from("ACTIVE 240Hz"), .bg_color = hex(4279983648LL), .text_color = hex(4291356361LL)};
MaterialRenderer_draw_badge(&(renderer), (&(canvas)), (&(badge_opt)), 840, 100);
TextWidget pb1_label = (TextWidget){.text = rt_string_from("Heap Buffer Utilization (68%)"), .font_size = 14, .color = theme.on_surface};
MaterialRenderer_draw_text(&(renderer), (&(canvas)), (&(pb1_label)), 544, 160);
ProgressBarWidget pb1 = (ProgressBarWidget){.progress = 0.68, .width = 432, .height = 8};
MaterialRenderer_draw_progress_bar(&(renderer), (&(canvas)), (&(pb1)), 544, 185);
TextWidget pb2_label = (TextWidget){.text = rt_string_from("Async Nursery Task Saturation (91%)"), .font_size = 14, .color = theme.on_surface};
MaterialRenderer_draw_text(&(renderer), (&(canvas)), (&(pb2_label)), 544, 220);
ProgressBarWidget pb2 = (ProgressBarWidget){.progress = 0.91, .width = 432, .height = 8};
MaterialRenderer_draw_progress_bar(&(renderer), (&(canvas)), (&(pb2)), 544, 245);
TextWidget pb3_label = (TextWidget){.text = rt_string_from("SIMD Pipeline Vector Bandwidth (34%)"), .font_size = 14, .color = theme.on_surface};
MaterialRenderer_draw_text(&(renderer), (&(canvas)), (&(pb3_label)), 544, 280);
ProgressBarWidget pb3 = (ProgressBarWidget){.progress = 0.34, .width = 432, .height = 8};
MaterialRenderer_draw_progress_bar(&(renderer), (&(canvas)), (&(pb3)), 544, 305);
CardWidget sub_card = (CardWidget){.title = rt_string_from("Autonomous Nursery Worker"), .subtitle = rt_string_from("32 nursery threads responding to lock-free channels"), .variant = (ui.CardVariant).Filled, .width = 432, .height = 140, .elevation = 0};
MaterialRenderer_draw_card(&(renderer), (&(canvas)), (&(sub_card)), 544, 360);
ButtonWidget btn_action = (ButtonWidget){.label = rt_string_from("Inspect Thread Dumps"), .variant = (ui.ButtonVariant).Tonal, .width = 200, .height = 38};
MaterialRenderer_draw_button(&(renderer), (&(canvas)), (&(btn_action)), 560, 440);
int64_t mx = Window_mouse_x(&(window));
int64_t my = Window_mouse_y(&(window));
int64_t mdown = Window_is_mouse_down(&(window));
printf("%s\n", rt_string_from("Window input state initialized. Mouse: (x, y), is_down").data);
Window_swap_buffers(&(window));
Window_poll_events(&(window));
printf("%s\n", rt_string_from("Material 3 Interactive Studio rendered successfully with all form widgets.").data);
}


int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
