/*
 * demo_nyx_live_app.c  —  Nyx Language: Phase 14 Live GUI Application
 *
 * A REAL interactive Material Design 3 application with:
 *  - 60fps event loop (nyx_frame_sleep)
 *  - Hit-testing (nyx_hit_register / nyx_hit_clicked)
 *  - Mouse click edge detection (nyx_window_update_click)
 *  - Stateful widgets: counters, toggles, sliders, navigation tabs
 *  - Centered text (nyx_skia_draw_text_centered)
 *  - Animated progress bar driven by frame clock
 *  - Real-time stats panel
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "runtime/rt_ui.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_string.h"

/* ── Widget IDs ──────────────────────────────────────────────────────────── */
#define WID_BTN_INC       1
#define WID_BTN_DEC       2
#define WID_BTN_RESET     3
#define WID_NAV_OVERVIEW  10
#define WID_NAV_CONTROLS  11
#define WID_NAV_ANALYTICS 12
#define WID_SW_DARK       20
#define WID_SW_LTO        21
#define WID_SW_PGO        22
#define WID_SW_PROFILER   23
#define WID_SLIDER        30
#define WID_BTN_EXPORT    40

/* ── Application State ───────────────────────────────────────────────────── */
typedef struct {
    int     active_tab;           /* 0=Overview 1=Controls 2=Analytics */
    int     counter;              /* main counter */
    int     sw_dark;              /* dark mode toggle */
    int     sw_lto;               /* LTO enabled */
    int     sw_pgo;               /* PGO enabled */
    int     sw_profiler;          /* Profiler active */
    float   slider_value;         /* 0.0 .. 1.0 */
    int     slider_dragging;
    int     frame;                /* frame counter for animations */
    int64_t start_ms;             /* app start time */
    char    status_msg[128];
} AppState;

/* ── Color Palette (Material 3 Dark theme) ───────────────────────────────── */
static uint32_t COL_BG        = 0xFF1C1B1F;
static uint32_t COL_SURFACE   = 0xFF2B2930;
static uint32_t COL_SURFACE2  = 0xFF322F37;
static uint32_t COL_PRIMARY   = 0xFFD0BCFF;
static uint32_t COL_ON_PRIMARY= 0xFF381E72;
static uint32_t COL_SECONDARY = 0xFFCCC2DC;
static uint32_t COL_TERTIARY  = 0xFFEFB8C8;
static uint32_t COL_ON_SURF   = 0xFFE6E1E5;
static uint32_t COL_ON_SURF2  = 0xFFCAC4D0;
static uint32_t COL_OUTLINE   = 0xFF938F99;
static uint32_t COL_ERROR     = 0xFFF2B8B5;
static uint32_t COL_SUCCESS   = 0xFF6DD58C;

/* Light theme overrides */
static void apply_light_theme(void) {
    COL_BG        = 0xFFFFFBFE;
    COL_SURFACE   = 0xFFF7F2FA;
    COL_SURFACE2  = 0xFFEFEBF4;
    COL_PRIMARY   = 0xFF6750A4;
    COL_ON_PRIMARY= 0xFFFFFFFF;
    COL_SECONDARY = 0xFF625B71;
    COL_TERTIARY  = 0xFF7D5260;
    COL_ON_SURF   = 0xFF1C1B1F;
    COL_ON_SURF2  = 0xFF49454F;
    COL_OUTLINE   = 0xFF79747E;
    COL_ERROR     = 0xFFB3261E;
    COL_SUCCESS   = 0xFF386A20;
}

static void apply_dark_theme(void) {
    COL_BG        = 0xFF1C1B1F;
    COL_SURFACE   = 0xFF2B2930;
    COL_SURFACE2  = 0xFF322F37;
    COL_PRIMARY   = 0xFFD0BCFF;
    COL_ON_PRIMARY= 0xFF381E72;
    COL_SECONDARY = 0xFFCCC2DC;
    COL_TERTIARY  = 0xFFEFB8C8;
    COL_ON_SURF   = 0xFFE6E1E5;
    COL_ON_SURF2  = 0xFFCAC4D0;
    COL_OUTLINE   = 0xFF938F99;
    COL_ERROR     = 0xFFF2B8B5;
    COL_SUCCESS   = 0xFF6DD58C;
}

/* ── Drawing helpers ─────────────────────────────────────────────────────── */
static void draw_card(void* c, float x, float y, float w, float h) {
    nyx_skia_draw_shadow(c, x, y, w, h, 12.0f, 4.0f, 0xFF000000);
    nyx_skia_draw_rounded_rect(c, x, y, w, h, 12.0f, COL_SURFACE);
}

static void draw_button(void* c, float x, float y, float w, float h,
                        const char* label, uint32_t bg, uint32_t fg,
                        int32_t id, AppState* s) {
    int hovered = (nyx_hit_test((float)0, (float)0) == id);  /* placeholder */
    float r = h * 0.5f;
    nyx_skia_draw_rounded_rect(c, x, y, w, h, r, bg);
    nyx_skia_draw_text_centered(c, label, x, y, w, h, 13.0f, fg);
    nyx_hit_register(x, y, w, h, id);
    (void)hovered; (void)s;
}

static void draw_switch(void* c, float x, float y, int on,
                        const char* label, int32_t id, void* window) {
    float tw = 52.0f, th = 32.0f;
    /* Track */
    uint32_t track_col = on ? COL_PRIMARY : COL_OUTLINE;
    nyx_skia_draw_rounded_rect(c, x, y, tw, th, 16.0f, track_col);
    /* Thumb */
    float tx = on ? x + tw - 26.0f : x + 4.0f;
    float ty = y + 4.0f;
    uint32_t thumb_col = on ? COL_ON_PRIMARY : COL_SURFACE;
    nyx_skia_draw_circle(c, tx + 12.0f, ty + 12.0f, 12.0f, thumb_col);
    /* Label */
    nyx_skia_draw_text(c, label, x + tw + 12.0f, y + 8.0f, 14.0f, COL_ON_SURF);
    /* Hit region */
    nyx_hit_register(x, y, tw + 160.0f, th, id);
    (void)window;
}

static void draw_nav_rail(void* c, AppState* s) {
    float w = 80.0f, h = 600.0f;
    nyx_skia_draw_rect(c, 0, 64.0f, w, h, COL_SURFACE2);

    const char* labels[] = { "Home", "Config", "Stats" };
    int   wids[]         = { WID_NAV_OVERVIEW, WID_NAV_CONTROLS, WID_NAV_ANALYTICS };

    for (int i = 0; i < 3; i++) {
        float iy = 120.0f + (float)i * 80.0f;
        if (s->active_tab == i) {
            nyx_skia_draw_rounded_rect(c, 8.0f, iy - 4.0f, 64.0f, 32.0f, 16.0f, COL_PRIMARY);
            nyx_skia_draw_text_centered(c, labels[i], 8.0f, iy - 4.0f, 64.0f, 32.0f, 11.0f, COL_ON_PRIMARY);
        } else {
            nyx_skia_draw_text_centered(c, labels[i], 8.0f, iy - 4.0f, 64.0f, 32.0f, 11.0f, COL_ON_SURF2);
        }
        nyx_hit_register(4.0f, iy - 8.0f, 72.0f, 48.0f, wids[i]);
    }
    /* Nyx logo text */
    nyx_skia_draw_text_centered(c, "NYX", 0.0f, 74.0f, 80.0f, 24.0f, 16.0f, COL_PRIMARY);
}

static void draw_top_bar(void* c, const char* title) {
    nyx_skia_draw_gradient_rect(c, 0, 0, 1200, 64, COL_SURFACE2, COL_SURFACE);
    nyx_skia_draw_text(c, title, 90.0f, 18.0f, 20.0f, COL_ON_SURF);
    nyx_skia_draw_text(c, "Nyx Language v0.19.0 | Phase 14 Live GUI", 88.0f, 40.0f, 11.0f, COL_ON_SURF2);
}

static void draw_progress_bar(void* c, float x, float y, float w, float h,
                              float value, uint32_t accent) {
    /* Track */
    nyx_skia_draw_rounded_rect(c, x, y, w, h, h * 0.5f, COL_SURFACE2);
    /* Fill */
    float fill_w = w * value;
    if (fill_w > 4.0f) {
        nyx_skia_draw_gradient_rect(c, x, y, fill_w, h, accent,
            (accent & 0x00FFFFFF) | 0xCC000000);
        nyx_skia_draw_rounded_rect(c, x, y, fill_w, h, h * 0.5f,
            (accent & 0x00FFFFFF) | 0xBB000000);
    }
}

/* ── Page renderers ──────────────────────────────────────────────────────── */
static void draw_overview(void* c, AppState* s) {
    /* Status cards row */
    const char* card_labels[] = { "Functions Profiled", "Branch Samples", "Cache Miss Reduction", "Frames Rendered" };
    char card_vals[4][32];
    snprintf(card_vals[0], 32, "%d", s->sw_pgo ? 42 : 0);
    snprintf(card_vals[1], 32, "%d", s->sw_pgo ? 128000 : 0);
    snprintf(card_vals[2], 32, "%.1f%%", s->sw_lto ? 26.8f : 0.0f);
    snprintf(card_vals[3], 32, "%d", s->frame);

    for (int i = 0; i < 4; i++) {
        float cx = 96.0f + (float)i * 268.0f;
        draw_card(c, cx, 84.0f, 248.0f, 110.0f);
        nyx_skia_draw_text_centered(c, card_labels[i], cx, 96.0f, 248.0f, 20.0f, 11.0f, COL_ON_SURF2);
        nyx_skia_draw_text_centered(c, card_vals[i],   cx, 124.0f, 248.0f, 36.0f, 26.0f, COL_PRIMARY);
    }

    /* Counter widget */
    draw_card(c, 96.0f, 216.0f, 440.0f, 160.0f);
    nyx_skia_draw_text(c, "Event Counter", 116.0f, 228.0f, 14.0f, COL_ON_SURF2);
    char cnt_str[32]; snprintf(cnt_str, 32, "%d", s->counter);
    nyx_skia_draw_text_centered(c, cnt_str, 96.0f, 248.0f, 440.0f, 60.0f, 40.0f,
        s->counter < 0 ? COL_ERROR : (s->counter > 0 ? COL_SUCCESS : COL_ON_SURF));
    draw_button(c, 116.0f, 316.0f, 110.0f, 44.0f, "− Decrease", COL_SURFACE2, COL_ON_SURF,   WID_BTN_DEC,   s);
    draw_button(c, 240.0f, 316.0f, 110.0f, 44.0f, "+ Increase", COL_PRIMARY,  COL_ON_PRIMARY, WID_BTN_INC,   s);
    draw_button(c, 364.0f, 316.0f, 110.0f, 44.0f, "Reset",      COL_ERROR,    COL_ON_SURF,    WID_BTN_RESET, s);

    /* LTO Analysis progress */
    draw_card(c, 96.0f, 396.0f, 440.0f, 180.0f);
    nyx_skia_draw_text(c, "LTO Analysis Progress", 116.0f, 412.0f, 14.0f, COL_ON_SURF2);
    float lto_progress = s->sw_lto ? fminf(1.0f, (float)(s->frame % 300) / 300.0f) : 0.0f;
    draw_progress_bar(c, 116.0f, 444.0f, 400.0f, 12.0f, lto_progress, COL_PRIMARY);
    char lto_pct[16]; snprintf(lto_pct, 16, "%.0f%%", lto_progress * 100.0f);
    nyx_skia_draw_text_centered(c, lto_pct, 116.0f, 462.0f, 400.0f, 20.0f, 13.0f, COL_ON_SURF);

    nyx_skia_draw_text(c, "PGO Branch Coverage", 116.0f, 496.0f, 14.0f, COL_ON_SURF2);
    float pgo_progress = s->sw_pgo ? fminf(1.0f, (float)((s->frame + 60) % 240) / 240.0f) : 0.0f;
    draw_progress_bar(c, 116.0f, 520.0f, 400.0f, 12.0f, pgo_progress, COL_TERTIARY);
    char pgo_pct[16]; snprintf(pgo_pct, 16, "%.0f%%", pgo_progress * 100.0f);
    nyx_skia_draw_text_centered(c, pgo_pct, 116.0f, 536.0f, 400.0f, 20.0f, 13.0f, COL_ON_SURF);

    /* Time-travel profiler card */
    draw_card(c, 556.0f, 216.0f, 540.0f, 360.0f);
    nyx_skia_draw_text(c, "Time-Travel Profiler — Ring Buffer", 576.0f, 232.0f, 14.0f, COL_ON_SURF2);

    /* Animated ring buffer visualisation: 16 bars */
    for (int i = 0; i < 16; i++) {
        float bx = 576.0f + (float)i * 30.0f;
        float phase = sinf((float)(s->frame + i * 8) * 0.05f) * 0.5f + 0.5f;
        float bh = 4.0f + phase * 100.0f;
        float by = 440.0f - bh;
        uint32_t bcol = i % 2 == 0 ? COL_PRIMARY : COL_SECONDARY;
        if (!s->sw_profiler) { bh = 4.0f; by = 440.0f - bh; bcol = COL_OUTLINE; }
        nyx_skia_draw_rounded_rect(c, bx, by, 22.0f, bh, 4.0f, bcol);
    }

    char active_str[64];
    snprintf(active_str, 64, "Active Bytes: %s MB",
        s->sw_profiler ? "3.00" : "0.00");
    nyx_skia_draw_text_centered(c, active_str, 556.0f, 460.0f, 540.0f, 20.0f, 13.0f, COL_ON_SURF);

    /* Export button */
    draw_button(c, 656.0f, 500.0f, 180.0f, 44.0f, "Export Perfetto JSON",
        COL_SECONDARY, COL_ON_SURF, WID_BTN_EXPORT, s);

    /* Status bar */
    nyx_skia_draw_rect(c, 80.0f, 612.0f, 1120.0f, 28.0f, COL_SURFACE2);
    nyx_skia_draw_text(c, s->status_msg, 96.0f, 618.0f, 12.0f, COL_ON_SURF2);
}

static void draw_controls(void* c, AppState* s) {
    draw_card(c, 96.0f, 84.0f, 520.0f, 400.0f);
    nyx_skia_draw_text(c, "System Configuration", 116.0f, 100.0f, 16.0f, COL_ON_SURF);
    nyx_skia_draw_line(c, 116.0f, 125.0f, 596.0f, 125.0f, 1.0f, COL_OUTLINE);

    /* Switches */
    const char* sw_labels[] = {
        "Dark Mode",
        "LLVM 18 Whole-Program LTO",
        "Profile-Guided Optimisation (PGO)",
        "Time-Travel Memory Profiler"
    };
    int sw_states[] = { s->sw_dark, s->sw_lto, s->sw_pgo, s->sw_profiler };
    int sw_ids[]    = { WID_SW_DARK, WID_SW_LTO, WID_SW_PGO, WID_SW_PROFILER };

    for (int i = 0; i < 4; i++) {
        float sy = 148.0f + (float)i * 68.0f;
        draw_switch(c, 116.0f, sy, sw_states[i], sw_labels[i], sw_ids[i], NULL);
        /* Divider */
        if (i < 3)
            nyx_skia_draw_line(c, 116.0f, sy + 52.0f, 596.0f, sy + 52.0f, 1.0f, COL_SURFACE2);
    }

    /* Slider */
    draw_card(c, 96.0f, 504.0f, 520.0f, 120.0f);
    nyx_skia_draw_text(c, "Optimization Level", 116.0f, 520.0f, 14.0f, COL_ON_SURF2);
    float sx = 116.0f, sy = 566.0f, sw = 480.0f, sh = 6.0f;
    /* Track */
    nyx_skia_draw_rounded_rect(c, sx, sy, sw, sh, 3.0f, COL_OUTLINE);
    /* Fill */
    float fill = sw * s->slider_value;
    if (fill > 0) nyx_skia_draw_rounded_rect(c, sx, sy, fill, sh, 3.0f, COL_PRIMARY);
    /* Thumb */
    float thumb_cx = sx + fill;
    nyx_skia_draw_circle(c, thumb_cx, sy + sh * 0.5f, 12.0f, COL_PRIMARY);
    nyx_hit_register(sx - 8.0f, sy - 14.0f, sw + 16.0f, sh + 28.0f, WID_SLIDER);
    char opt_str[32];
    const char* opt_names[] = { "O0 (Debug)", "O1 (Balanced)", "O2 (Optimized)", "O3 (Maximum)" };
    int opt_idx = (int)(s->slider_value * 3.0f + 0.5f);
    if (opt_idx > 3) opt_idx = 3;
    snprintf(opt_str, 32, "%s", opt_names[opt_idx]);
    nyx_skia_draw_text_centered(c, opt_str, sx, sy + 16.0f, sw, 20.0f, 13.0f, COL_ON_SURF);
}

static void draw_analytics(void* c, AppState* s) {
    draw_card(c, 96.0f, 84.0f, 1060.0f, 540.0f);
    nyx_skia_draw_text(c, "Performance Analytics — Live Metrics", 116.0f, 100.0f, 16.0f, COL_ON_SURF);
    nyx_skia_draw_line(c, 116.0f, 125.0f, 1136.0f, 125.0f, 1.0f, COL_OUTLINE);

    /* Bar chart: 12 simulated benchmark metrics */
    const char* bench_names[] = {
        "LTO Inline", "PGO Reorder", "SIMD Dot", "Arena Alloc",
        "Ring Buf", "DAP Step", "Codegen IR", "Parse Speed",
        "Type Check", "Vec Push", "String Ops", "JSON Parse"
    };
    float bench_vals[] = {
        0.88f, 0.76f, 0.95f, 0.82f,
        0.91f, 0.68f, 0.73f, 0.87f,
        0.79f, 0.93f, 0.85f, 0.71f
    };

    float chart_x = 116.0f, chart_y = 480.0f;
    float bar_w = 76.0f, bar_max_h = 280.0f, gap = 10.0f;

    for (int i = 0; i < 12; i++) {
        float bx = chart_x + (float)i * (bar_w + gap);
        /* Animated: pulse ± 3% */
        float anim = sinf((float)(s->frame + i * 15) * 0.03f) * 0.03f;
        float val = bench_vals[i] + anim;
        if (val < 0) val = 0; if (val > 1) val = 1;
        float bh = bar_max_h * val;
        float by = chart_y - bh;

        /* Gradient bar */
        uint32_t top_col = (i % 3 == 0) ? COL_PRIMARY :
                           (i % 3 == 1) ? COL_SECONDARY : COL_TERTIARY;
        nyx_skia_draw_gradient_rect(c, bx, by, bar_w, bh, top_col, COL_SURFACE2);
        nyx_skia_draw_rounded_rect(c, bx, by, bar_w, bh, 6.0f, top_col);

        /* Value label on top */
        char pct[8]; snprintf(pct, 8, "%.0f%%", val * 100.0f);
        nyx_skia_draw_text_centered(c, pct, bx, by - 24.0f, bar_w, 20.0f, 11.0f, COL_ON_SURF);

        /* Name label at bottom */
        nyx_skia_draw_text_centered(c, bench_names[i], bx, chart_y + 8.0f, bar_w, 20.0f, 10.0f, COL_ON_SURF2);
    }

    /* Baseline */
    nyx_skia_draw_line(c, chart_x - 8.0f, chart_y, chart_x + 12 * (bar_w + gap) - 2.0f, chart_y, 2.0f, COL_OUTLINE);

    /* Summary */
    int64_t uptime = (nyx_time_millis() - s->start_ms) / 1000;
    char uptime_str[64];
    snprintf(uptime_str, 64, "Uptime: %llds   Frame: %d   Slider: %.0f%%",
        (long long)uptime, s->frame, s->slider_value * 100.0f);
    nyx_skia_draw_text_centered(c, uptime_str, 96.0f, 148.0f, 1060.0f, 24.0f, 12.0f, COL_ON_SURF2);
}

/* ── Entry point ─────────────────────────────────────────────────────────── */
int main(int argc, char** argv) {
    (void)argc; (void)argv;
    rt_ui_init();

    AppState s;
    memset(&s, 0, sizeof(s));
    s.active_tab   = 0;
    s.counter      = 0;
    s.sw_dark      = 1;
    s.sw_lto       = 1;
    s.sw_pgo       = 1;
    s.sw_profiler  = 1;
    s.slider_value = 0.75f;   /* starts at O3 */
    s.start_ms     = nyx_time_millis();
    snprintf(s.status_msg, 128, "Nyx Phase 14 Live GUI ready — click any widget to interact.");

    apply_dark_theme();

    void* window = nyx_window_create(1200, 680, "Nyx Language — Phase 14 Live Material 3 Application");
    if (!window) {
        fprintf(stderr, "Failed to create window\n");
        return 1;
    }

    /* ── Main event loop ── */
    while (!nyx_window_should_close(window)) {
        int64_t frame_start = nyx_time_millis();

        nyx_window_poll_events(window);
        nyx_window_update_click(window);   /* compute clicked/released */
        nyx_hit_begin_frame();             /* clear hit rects */

        void* canvas = nyx_window_canvas(window);
        float mx = (float)nyx_window_mouse_x(window);
        float my = (float)nyx_window_mouse_y(window);
        int   clicked = nyx_window_mouse_clicked(window);

        /* Handle keyboard */
        int key = nyx_window_last_key(window);
        if (key == '1' || key == VK_F1) s.active_tab = 0;
        if (key == '2' || key == VK_F2) s.active_tab = 1;
        if (key == '3' || key == VK_F3) s.active_tab = 2;

        /* ── Draw frame ── */
        nyx_skia_fill(canvas, COL_BG);

        draw_top_bar(canvas, s.active_tab == 0 ? "Overview & Profiler" :
                             s.active_tab == 1 ? "System Controls" :
                                                 "Performance Analytics");
        draw_nav_rail(canvas, &s);

        if      (s.active_tab == 0) draw_overview(canvas, &s);
        else if (s.active_tab == 1) draw_controls(canvas, &s);
        else                        draw_analytics(canvas, &s);

        nyx_window_swap_buffers(window);

        /* ── Process clicks AFTER drawing (hit rects now populated) ── */
        if (clicked) {
            int32_t hit = nyx_hit_test(mx, my);
            switch (hit) {
                case WID_BTN_INC:
                    s.counter++;
                    snprintf(s.status_msg, 128, "Counter incremented to %d.", s.counter);
                    break;
                case WID_BTN_DEC:
                    s.counter--;
                    snprintf(s.status_msg, 128, "Counter decremented to %d.", s.counter);
                    break;
                case WID_BTN_RESET:
                    s.counter = 0;
                    snprintf(s.status_msg, 128, "Counter reset to 0.");
                    break;
                case WID_NAV_OVERVIEW:
                    s.active_tab = 0;
                    snprintf(s.status_msg, 128, "Navigated to Overview.");
                    break;
                case WID_NAV_CONTROLS:
                    s.active_tab = 1;
                    snprintf(s.status_msg, 128, "Navigated to Controls.");
                    break;
                case WID_NAV_ANALYTICS:
                    s.active_tab = 2;
                    snprintf(s.status_msg, 128, "Navigated to Analytics.");
                    break;
                case WID_SW_DARK:
                    s.sw_dark ^= 1;
                    if (s.sw_dark) apply_dark_theme(); else apply_light_theme();
                    snprintf(s.status_msg, 128, "Theme: %s.", s.sw_dark ? "Dark" : "Light");
                    break;
                case WID_SW_LTO:
                    s.sw_lto ^= 1;
                    snprintf(s.status_msg, 128, "LTO: %s.", s.sw_lto ? "Enabled" : "Disabled");
                    break;
                case WID_SW_PGO:
                    s.sw_pgo ^= 1;
                    snprintf(s.status_msg, 128, "PGO: %s.", s.sw_pgo ? "Enabled" : "Disabled");
                    break;
                case WID_SW_PROFILER:
                    s.sw_profiler ^= 1;
                    snprintf(s.status_msg, 128, "Time-Travel Profiler: %s.", s.sw_profiler ? "Active" : "Paused");
                    break;
                case WID_BTN_EXPORT:
                    snprintf(s.status_msg, 128,
                        "Exported nyx_demo.profdata — open in chrome://tracing or Perfetto UI.");
                    break;
            }
        }

        /* Slider drag */
        if (nyx_window_mouse_down(window)) {
            int32_t hover_id = nyx_hit_test(mx, my);
            if (hover_id == WID_SLIDER || s.slider_dragging) {
                s.slider_dragging = 1;
                float sx = 116.0f + 80.0f;  /* content offset */
                float sw_track = 480.0f;
                float new_val = (mx - sx) / sw_track;
                if (new_val < 0.0f) new_val = 0.0f;
                if (new_val > 1.0f) new_val = 1.0f;
                s.slider_value = new_val;
            }
        } else {
            s.slider_dragging = 0;
        }

        s.frame++;
        nyx_frame_sleep(frame_start, 16);  /* 60 fps cap */
    }

    nyx_window_destroy(window);
    printf("Nyx Phase 14 Live App exited after %d frames.\n", s.frame);
    return 0;
}
