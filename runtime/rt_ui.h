#ifndef RT_UI_H
#define RT_UI_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ── Surface and Canvas ──────────────────────────────────────────────────────
void* nyx_skia_surface_create(int32_t width, int32_t height);
void  nyx_skia_surface_destroy(void* surface);
void* nyx_skia_canvas_create(void* surface);
void  nyx_skia_canvas_destroy(void* canvas);

// ── Drawing primitives ──────────────────────────────────────────────────────
void nyx_skia_draw_rect(void* canvas, float x, float y, float w, float h, uint32_t color);
void nyx_skia_draw_rounded_rect(void* canvas, float x, float y, float w, float h, float radius, uint32_t color);
void nyx_skia_draw_circle(void* canvas, float cx, float cy, float radius, uint32_t color);
void nyx_skia_draw_gradient_rect(void* canvas, float x, float y, float w, float h, uint32_t color_top, uint32_t color_bottom);
// Shadow: 10-layer quadratic-alpha blur approximation (elevation 1-16 dp)
void nyx_skia_draw_shadow(void* canvas, float x, float y, float w, float h, float radius, float elevation, uint32_t shadow_color);
// Glow: 8-layer 1/d^2 additive halo
void nyx_skia_draw_glow(void* canvas, float x, float y, float w, float h, float radius, float spread, uint32_t glow_color);
// Ripple: animated ring — elapsed_norm 0.0→1.0 controls expansion (0=start,1=end)
void nyx_skia_draw_ripple(void* canvas, float cx, float cy, float max_radius, float elapsed_norm, uint32_t ripple_color);
// Ripple animated shorthand: pass elapsed_ms since click (350ms = full)
void nyx_skia_draw_ripple_anim(void* canvas, float cx, float cy, float max_radius, int64_t elapsed_ms, uint32_t ripple_color);
void nyx_skia_draw_text(void* canvas, const char* text, float x, float y, float size, uint32_t color);
// Extended text: weight 100-900 (300=Light,400=Normal,500=Medium,700=Bold), italic=0/1
void nyx_skia_draw_text_ex(void* canvas, const char* text, float x, float y, float size, int weight, int italic, uint32_t color);
void nyx_skia_draw_line(void* canvas, float x1, float y1, float x2, float y2, float stroke_width, uint32_t color);
void nyx_skia_fill(void* canvas, uint32_t color);
// Icon rendering via Segoe MDL2 Assets (Windows 10+): pass Unicode codepoint
// e.g. 0xE748=Home 0xE700=Settings 0xE9D9=Analytics 0xE71E=Search 0xE74D=Save
void nyx_skia_draw_icon(void* canvas, uint32_t codepoint, float x, float y, float size, uint32_t color);
// Clip region: push/pop a scissor rectangle (uses IntersectClipRect + SaveDC)
void nyx_clip_push_rect(void* canvas, float x, float y, float w, float h);
void nyx_clip_pop(void* canvas);

// ── Composite Material 3 Widgets (Phase 15) ─────────────────────────────────
void nyx_ui_draw_modal(void* canvas, const char* title, const char* message, bool is_open, int32_t ok_id, int32_t cancel_id);
void nyx_ui_draw_dropdown(void* canvas, float x, float y, float w, float h, const char* label, const char** options, int option_count, int selected_idx, bool is_expanded, int32_t id_base);
void nyx_ui_draw_data_table(void* canvas, float x, float y, float w, float h, const char** headers, int header_count, const char*** rows, int row_count, const float* col_widths);
void nyx_ui_draw_toast(void* canvas, const char* message, int toast_type, float alpha);

// ── Text measurement (Phase 14) ─────────────────────────────────────────────
// Returns the pixel width of `text` rendered at `size` pt with Segoe UI.
// Requires a live HDC — call only from within a frame (after window created).
float nyx_measure_text_width(void* canvas, const char* text, float size);
// Returns the pixel height of a line at `size` pt.
float nyx_measure_text_height(float size);
// Draw text centered in rect [x, y, w, h]
void nyx_skia_draw_text_centered(void* canvas, const char* text, float x, float y, float w, float h, float size, uint32_t color);

// ── Timing and frame clock ──────────────────────────────────────────────────
int64_t nyx_time_millis(void);
// Sleep until at least `target_ms` milliseconds have elapsed since `frame_start_ms`.
// Use to cap the frame rate at 60 fps (target_ms = 16).
void    nyx_frame_sleep(int64_t frame_start_ms, int64_t target_ms);

// ── Canvas transform state ──────────────────────────────────────────────────
void nyx_skia_save(void* canvas);
void nyx_skia_restore(void* canvas);
void nyx_skia_translate(void* canvas, float dx, float dy);
void nyx_skia_rotate(void* canvas, float degrees);
void nyx_skia_scale(void* canvas, float sx, float sy);

// ── Windowing ───────────────────────────────────────────────────────────────
void*   nyx_window_create(int32_t width, int32_t height, const char* title);
void    nyx_window_destroy(void* window);
int32_t nyx_window_should_close(void* window);
void    nyx_window_poll_events(void* window);
void*   nyx_window_canvas(void* window);
void    nyx_window_swap_buffers(void* window);
int32_t nyx_window_width(void* window);
int32_t nyx_window_height(void* window);
void    nyx_window_set_title(void* window, const char* title);
int32_t nyx_window_mouse_x(void* window);
int32_t nyx_window_mouse_y(void* window);
int32_t nyx_window_mouse_down(void* window);
int32_t nyx_window_last_key(void* window);

// ── Mouse click edge detection (Phase 14) ──────────────────────────────────
// Call nyx_window_update_click(window) once per frame AFTER poll_events.
// nyx_window_mouse_clicked() returns 1 only on the frame the button was pressed.
// nyx_window_mouse_released() returns 1 only on the frame the button was released.
void    nyx_window_update_click(void* window);
int32_t nyx_window_mouse_clicked(void* window);
int32_t nyx_window_mouse_released(void* window);

// ── Hit testing (Phase 14) ──────────────────────────────────────────────────
// Clear the hit-rect table at the start of each frame.
void nyx_hit_begin_frame(void);
// Register a named hit region. Call after drawing each interactive widget.
// `id` is a user-assigned integer (0..255). Returns id for chaining.
int32_t nyx_hit_register(float x, float y, float w, float h, int32_t id);
// Test mouse position against all registered rects. Returns id or -1.
int32_t nyx_hit_test(float mx, float my);
// Convenience: returns 1 if widget `id` was clicked this frame.
int32_t nyx_hit_clicked(void* window, int32_t id);

// ── Lifecycle ────────────────────────────────────────────────────────────────
// Called once at the start of main() (already called by bootstrap-generated main).
void rt_ui_init(void);

#ifdef __cplusplus
}
#endif

#endif // RT_UI_H

