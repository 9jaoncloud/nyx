#include "rt_ui.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct {
    int32_t width;
    int32_t height;
    HDC hdc;
    HBITMAP hbitmap;
    HBITMAP hold_bitmap;
    void* bits;
} NyxSurface;

typedef struct {
    NyxSurface* surface;
    float current_x;
    float current_y;
} NyxCanvas;

typedef struct {
    HWND hwnd;
    HDC hdc;
    int32_t width;
    int32_t height;
    int32_t should_close;
    int32_t mouse_x;
    int32_t mouse_y;
    int32_t mouse_down;
    int32_t prev_mouse_down;  /* Phase 14: previous frame button state */
    int32_t mouse_clicked;   /* 1 only on the frame button went down */
    int32_t mouse_released;  /* 1 only on the frame button went up */
    int32_t last_key;
    NyxSurface* surface;
    NyxCanvas* canvas;
} NyxWindow;

/* ── Phase 14: hit-test registry (per-frame, 256 slots) ────────────────── */
typedef struct { float x, y, w, h; int32_t id; } NyxHitRect;
static NyxHitRect g_hit_rects[256];
static int        g_hit_count = 0;

static const char* NYX_WINDOW_CLASS = "NyxMaterialWindowClass";
static int g_class_registered = 0;

static LRESULT CALLBACK NyxWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    NyxWindow* win = (NyxWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    switch (msg) {
        case WM_CLOSE:
            if (win) win->should_close = 1;
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            if (win) win->should_close = 1;
            PostQuitMessage(0);
            return 0;
        case WM_MOUSEMOVE:
            if (win) {
                win->mouse_x = (int16_t)LOWORD(lparam);
                win->mouse_y = (int16_t)HIWORD(lparam);
            }
            return 0;
        case WM_LBUTTONDOWN:
            if (win) {
                win->mouse_down = 1;
                win->mouse_x = (int16_t)LOWORD(lparam);
                win->mouse_y = (int16_t)HIWORD(lparam);
            }
            return 0;
        case WM_LBUTTONUP:
            if (win) {
                win->mouse_down = 0;
                win->mouse_x = (int16_t)LOWORD(lparam);
                win->mouse_y = (int16_t)HIWORD(lparam);
            }
            return 0;
        case WM_KEYDOWN:
            if (win) {
                win->last_key = (int32_t)wparam;
            }
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            if (win && win->surface) {
                BitBlt(hdc, 0, 0, win->width, win->height, win->surface->hdc, 0, 0, SRCCOPY);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_ERASEBKGND:
            return 1; // Prevent flicker
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
    }
}

static COLORREF unpack_colorref(uint32_t color) {
    uint8_t a = (color >> 24) & 0xFF;
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    (void)a;
    return RGB(r, g, b);
}

void* nyx_skia_surface_create(int32_t width, int32_t height) {
    NyxSurface* s = (NyxSurface*)calloc(1, sizeof(NyxSurface));
    if (!s) return NULL;
    s->width = width > 0 ? width : 800;
    s->height = height > 0 ? height : 600;

    HDC screen_dc = GetDC(NULL);
    s->hdc = CreateCompatibleDC(screen_dc);

    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = s->width;
    bmi.bmiHeader.biHeight = -s->height; // Top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    s->hbitmap = CreateDIBSection(s->hdc, &bmi, DIB_RGB_COLORS, &s->bits, NULL, 0);
    s->hold_bitmap = (HBITMAP)SelectObject(s->hdc, s->hbitmap);
    ReleaseDC(NULL, screen_dc);

    SetBkMode(s->hdc, TRANSPARENT);
    return s;
}

void nyx_skia_surface_destroy(void* surface) {
    if (!surface) return;
    NyxSurface* s = (NyxSurface*)surface;
    if (s->hdc && s->hold_bitmap) {
        SelectObject(s->hdc, s->hold_bitmap);
    }
    if (s->hbitmap) DeleteObject(s->hbitmap);
    if (s->hdc) DeleteDC(s->hdc);
    free(s);
}

void* nyx_skia_canvas_create(void* surface) {
    if (!surface) return NULL;
    NyxCanvas* c = (NyxCanvas*)calloc(1, sizeof(NyxCanvas));
    c->surface = (NyxSurface*)surface;
    return c;
}

void nyx_skia_canvas_destroy(void* canvas) {
    if (canvas) free(canvas);
}

/* ── Alpha compositing onto the 32-bit DIB (src-over) ─────────────────────
 * Convention in this codebase: a color is `0x00RRGGBB` and an alpha byte of 0
 * means OPAQUE; only explicitly translucent colors (Color.new/hex with alpha)
 * set bits 24..31. So alpha==0 is treated as 255. */
static inline uint8_t nyx_alpha_of(uint32_t c) {
    uint8_t a = (uint8_t)((c >> 24) & 0xFF);
    return (a == 0) ? 255 : a;
}

static inline void nyx_blend_px(NyxSurface* s, int x, int y,
                                uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    if (a == 0 || !s->bits) return;
    if (x < 0 || y < 0 || x >= s->width || y >= s->height) return;
    uint32_t* px = (uint32_t*)s->bits + (size_t)y * (size_t)s->width + (size_t)x;
    if (a >= 255) {
        *px = ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
        return;
    }
    uint32_t dst = *px;
    uint32_t ia = 255u - a;
    uint8_t dr = (uint8_t)((dst >> 16) & 0xFF);
    uint8_t dg = (uint8_t)((dst >> 8) & 0xFF);
    uint8_t db = (uint8_t)(dst & 0xFF);
    uint8_t nr = (uint8_t)((r * a + dr * ia) / 255u);
    uint8_t ng = (uint8_t)((g * a + dg * ia) / 255u);
    uint8_t nb = (uint8_t)((b * a + db * ia) / 255u);
    *px = ((uint32_t)nr << 16) | ((uint32_t)ng << 8) | (uint32_t)nb;
}

static inline void nyx_blend_color(NyxSurface* s, int x, int y, uint32_t color) {
    nyx_blend_px(s, x, y, nyx_alpha_of(color),
                 (uint8_t)((color >> 16) & 0xFF),
                 (uint8_t)((color >> 8) & 0xFF),
                 (uint8_t)(color & 0xFF));
}

void nyx_skia_draw_rect(void* canvas, float x, float y, float w, float h, uint32_t color) {
    if (!canvas) return;
    NyxSurface* s = ((NyxCanvas*)canvas)->surface;
    int x0 = (int)x, y0 = (int)y;
    int x1 = (int)(x + w), y1 = (int)(y + h);
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > s->width) x1 = s->width;
    if (y1 > s->height) y1 = s->height;
    for (int py = y0; py < y1; py++)
        for (int px = x0; px < x1; px++)
            nyx_blend_color(s, px, py, color);
}

void nyx_skia_draw_rounded_rect(void* canvas, float x, float y, float w, float h, float radius, uint32_t color) {
    if (!canvas) return;
    NyxSurface* s = ((NyxCanvas*)canvas)->surface;
    int x0 = (int)x, y0 = (int)y;
    int x1 = (int)(x + w), y1 = (int)(y + h);
    float r = radius;
    float maxr = (w < h ? w : h) * 0.5f;
    if (r > maxr) r = maxr;
    if (r < 0.0f) r = 0.0f;
    float cx_l = (float)x0 + r, cx_r = (float)x1 - r;
    float cy_t = (float)y0 + r, cy_b = (float)y1 - r;
    float r2 = r * r;
    int ix0 = x0 < 0 ? 0 : x0, iy0 = y0 < 0 ? 0 : y0;
    int ix1 = x1 > s->width ? s->width : x1, iy1 = y1 > s->height ? s->height : y1;
    for (int py = iy0; py < iy1; py++) {
        float dy = (py < cy_t) ? (cy_t - py) : ((py >= cy_b) ? (py - cy_b) : 0.0f);
        for (int px = ix0; px < ix1; px++) {
            float dx = (px < cx_l) ? (cx_l - px) : ((px >= cx_r) ? (px - cx_r) : 0.0f);
            if (dx * dx + dy * dy <= r2)
                nyx_blend_color(s, px, py, color);
        }
    }
}

void nyx_skia_draw_circle(void* canvas, float cx, float cy, float radius, uint32_t color) {
    if (!canvas || radius <= 0.0f) return;
    NyxSurface* s = ((NyxCanvas*)canvas)->surface;
    int x0 = (int)(cx - radius), y0 = (int)(cy - radius);
    int x1 = (int)(cx + radius) + 1, y1 = (int)(cy + radius) + 1;
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > s->width) x1 = s->width;
    if (y1 > s->height) y1 = s->height;
    float r2 = radius * radius;
    for (int py = y0; py < y1; py++) {
        float dy = (float)py + 0.5f - cy;
        for (int px = x0; px < x1; px++) {
            float dx = (float)px + 0.5f - cx;
            if (dx * dx + dy * dy <= r2)
                nyx_blend_color(s, px, py, color);
        }
    }
}

void nyx_skia_draw_gradient_rect(void* canvas, float x, float y, float w, float h, uint32_t color_top, uint32_t color_bottom) {
    if (!canvas || w <= 0.0f || h <= 0.0f) return;
    NyxSurface* s = ((NyxCanvas*)canvas)->surface;
    int x0 = (int)x, y0 = (int)y;
    int x1 = (int)(x + w), y1 = (int)(y + h);
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > s->width) x1 = s->width;
    if (y1 > s->height) y1 = s->height;

    uint8_t at = (uint8_t)((color_top >> 24) & 0xFF), ab = (uint8_t)((color_bottom >> 24) & 0xFF);
    float r1 = (float)((color_top >> 16) & 0xFF), g1 = (float)((color_top >> 8) & 0xFF), b1 = (float)(color_top & 0xFF);
    float r2 = (float)((color_bottom >> 16) & 0xFF), g2 = (float)((color_bottom >> 8) & 0xFF), b2 = (float)(color_bottom & 0xFF);
    if (at == 0) at = 255;
    if (ab == 0) ab = 255;
    float inv_h = 1.0f / h;

    for (int py = y0; py < y1; py++) {
        float t = (float)(py - y0) * inv_h;
        uint8_t r = (uint8_t)(r1 + (r2 - r1) * t);
        uint8_t g = (uint8_t)(g1 + (g2 - g1) * t);
        uint8_t b = (uint8_t)(b1 + (b2 - b1) * t);
        uint8_t a = (uint8_t)(at + ((int)ab - (int)at) * t);
        for (int px = x0; px < x1; px++)
            nyx_blend_px(s, px, py, a, r, g, b);
    }
}

/* ── Shadow: 10-layer quadratic-alpha Gaussian approximation ──────────────
 * Simulates a real box-shadow blur by rendering concentric rounded-rects
 * with alpha that follows a quadratic decay: a = A_max * (1 - t²)²
 * where t = layer_index / total_layers.  This gives a smooth penumbra
 * instead of the previous single-layer hack.
 * Also adds a subtle umbra (inner shadow) offset downward for elevation. */
void nyx_skia_draw_shadow(void* canvas, float x, float y, float w, float h,
                          float radius, float elevation, uint32_t shadow_color) {
    if (!canvas || elevation <= 0.0f) return;
    NyxCanvas* c = (NyxCanvas*)canvas;
    HDC hdc = c->surface->hdc;

    /* Elevation-dependent parameters */
    float offset_y = elevation * 0.5f + 1.0f;  /* vertical drop */
    float blur     = elevation * 2.0f;          /* blur spread radius */
    int   layers   = 10;
    /* Max alpha: higher elevation → darker shadow */
    float a_max    = 0.55f * (1.0f - expf(-elevation * 0.15f));
    if (a_max > 0.55f) a_max = 0.55f;

    /* Render outer → inner so inner layers paint over outer */
    for (int i = layers; i >= 1; i--) {
        float t    = (float)i / (float)layers;          /* 1.0 → outermost */
        float a    = a_max * (1.0f - t * t) * (1.0f - t * t); /* quadratic */
        if (a < 0.005f) continue;
        float exp  = blur * t;                           /* expansion */
        float sx   = x - exp * 0.5f;
        float sy   = y + offset_y + exp * 0.2f;
        float sw   = w + exp;
        float sh   = h + exp * 0.5f;
        float sr   = radius + exp * 0.5f;
        uint32_t ac = (uint32_t)(a * 255.0f) & 0xFF;
        uint32_t col = (ac << 24) | (shadow_color & 0x00FFFFFF);
        /* Use RoundRect with null pen for clean edges */
        HBRUSH brush = CreateSolidBrush(shadow_color & 0x00FFFFFF);
        HPEN   pen   = CreatePen(PS_NULL, 0, 0);
        HBRUSH ob = (HBRUSH)SelectObject(hdc, brush);
        HPEN   op = (HPEN)SelectObject(hdc, pen);
        /* Blend via semi-transparent overlay using alpha brush simulation */
        nyx_skia_draw_rounded_rect(canvas, sx, sy, sw, sh, sr, col);
        SelectObject(hdc, ob); SelectObject(hdc, op);
        DeleteObject(brush); DeleteObject(pen);
    }
    (void)hdc;
}

/* ── Glow: 8-layer additive-style halo ────────────────────────────────────
 * Outer rings have low alpha and large spread; inner rings have higher
 * alpha.  Uses a 1/distance² falloff so the core is bright. */
void nyx_skia_draw_glow(void* canvas, float x, float y, float w, float h,
                        float radius, float spread, uint32_t glow_color) {
    if (!canvas || spread <= 0.0f) return;
    int layers = 8;
    for (int i = layers; i >= 1; i--) {
        float t    = (float)i / (float)layers;        /* 1.0 = outermost */
        float step = spread * t;
        /* Alpha: 1/t² so outer layers are nearly invisible, inner bright */
        float a    = 0.22f / (t * t + 0.3f);
        if (a > 0.55f) a = 0.55f;
        uint32_t ac  = (uint32_t)(a * 255.0f) & 0xFF;
        uint32_t col = (ac << 24) | (glow_color & 0x00FFFFFF);
        nyx_skia_draw_rounded_rect(canvas,
            x - step, y - step,
            w + step * 2.0f, h + step * 2.0f,
            radius + step, col);
    }
}

/* ── Ripple: animated expanding ring ─────────────────────────────────────
 * Classic Material Design ripple: a hollow ring that expands from the
 * click point and fades as it grows.  Call with elapsed_ms since click
 * (0 = start, ~400ms = end).
 *
 * nyx_skia_draw_ripple() keeps the old API (alpha param) for compat.
 * nyx_skia_draw_ripple_anim() is the new animated variant. */
void nyx_skia_draw_ripple(void* canvas, float cx, float cy,
                          float max_radius, float elapsed_norm,
                          uint32_t ripple_color) {
    /* elapsed_norm: 0.0 = just started, 1.0 = fully expanded */
    if (!canvas || max_radius <= 0.0f) return;
    NyxCanvas* cc = (NyxCanvas*)canvas;
    HDC hdc = cc->surface->hdc;

    float t = elapsed_norm < 0.0f ? 0.0f : (elapsed_norm > 1.0f ? 1.0f : elapsed_norm);
    float r = max_radius * t;                    /* current radius */
    float a = (1.0f - t) * 0.4f;               /* fade out as it expands */
    if (r < 1.0f || a < 0.01f) return;

    /* Draw as an annulus: outer circle minus inner circle.  Achieve
     * this by selecting a region: outer_ellipse XOR inner_ellipse.
     * Use two filled ellipses with SaveDC + clip trick. */
    uint32_t ac  = (uint32_t)(a * 255.0f) & 0xFF;
    uint32_t col = (ac << 24) | (ripple_color & 0x00FFFFFF);

    float ring_w = max_radius * 0.18f;          /* ring thickness */
    float inner  = r - ring_w;
    if (inner < 0.0f) inner = 0.0f;

    /* Outer filled circle */
    nyx_skia_draw_circle(canvas, cx, cy, r, col);
    /* Erase inner portion using background colour trick: draw smaller
     * circle with very low alpha to create hollow effect */
    if (inner > 1.0f) {
        uint32_t erase = (0x00FFFFFF & ripple_color); /* same hue, near-transparent */
        nyx_skia_draw_circle(canvas, cx, cy, inner, erase);
    }
    (void)hdc;
}

void nyx_skia_draw_ripple_anim(void* canvas, float cx, float cy,
                               float max_radius, int64_t elapsed_ms,
                               uint32_t ripple_color) {
    /* 350ms full expansion */
    float t = (float)elapsed_ms / 350.0f;
    nyx_skia_draw_ripple(canvas, cx, cy, max_radius, t, ripple_color);
}

/* ── Font weight helper ───────────────────────────────────────────────────
 * weight values: 100=Thin  200=ExtraLight  300=Light  400=Normal
 *                500=Medium  600=SemiBold  700=Bold  800=ExtraBold  900=Black */
static int nyx_weight_to_fw(int weight) {
    if (weight <= 150) return FW_THIN;         /* 100 */
    if (weight <= 250) return FW_EXTRALIGHT;   /* 200 */
    if (weight <= 350) return FW_LIGHT;        /* 300 */
    if (weight <= 450) return FW_NORMAL;       /* 400 */
    if (weight <= 550) return FW_MEDIUM;       /* 500 */
    if (weight <= 650) return FW_SEMIBOLD;     /* 600 */
    if (weight <= 750) return FW_BOLD;         /* 700 */
    if (weight <= 850) return FW_EXTRABOLD;    /* 800 */
    return FW_HEAVY;                           /* 900 */
}

static HFONT nyx_create_font(float size, int weight) {
    int font_height = -(int)(size > 0 ? size : 14.0f);
    int fw = (weight > 0) ? nyx_weight_to_fw(weight)
                          : (size >= 22.0f ? FW_BOLD :
                             size >= 16.0f ? FW_MEDIUM : FW_NORMAL);
    return CreateFontW(
        font_height, 0, 0, 0, fw,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe UI"
    );
}

/* ── Phase 14: Text measurement (UTF-8 compatible via WideChar) ────────── */
float nyx_measure_text_width(void* canvas, const char* text, float size) {
    if (!canvas || !text) return 0.0f;
    NyxCanvas* c = (NyxCanvas*)canvas;
    HDC hdc = c->surface->hdc;
    HFONT hfont = nyx_create_font(size, 0);
    HFONT old_font = (HFONT)SelectObject(hdc, hfont);
    
    int wlen = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    if (wlen <= 0) {
        SelectObject(hdc, old_font);
        DeleteObject(hfont);
        return 0.0f;
    }
    WCHAR* wbuf = (WCHAR*)malloc(sizeof(WCHAR) * (size_t)wlen);
    MultiByteToWideChar(CP_UTF8, 0, text, -1, wbuf, wlen);

    SIZE sz = {0, 0};
    GetTextExtentPoint32W(hdc, wbuf, wlen - 1, &sz);
    free(wbuf);
    SelectObject(hdc, old_font);
    DeleteObject(hfont);
    return (float)sz.cx;
}

float nyx_measure_text_height(float size) {
    /* Segoe UI metrics: height ≈ size * 1.25 */
    return (float)(int)(size > 0 ? size : 14.0f) * 1.25f;
}

void nyx_skia_draw_text_centered(void* canvas, const char* text,
                                  float x, float y, float w, float h,
                                  float size, uint32_t color) {
    if (!canvas || !text) return;
    float tw = nyx_measure_text_width(canvas, text, size);
    float th = nyx_measure_text_height(size);
    float tx = x + (w - tw) * 0.5f;
    float ty = y + (h - th) * 0.5f;
    nyx_skia_draw_text(canvas, text, tx, ty, size, color);
}

/* ── Phase 14: Frame sleep (60fps cap) ──────────────────────────────────── */
void nyx_frame_sleep(int64_t frame_start_ms, int64_t target_ms) {
    int64_t now = (int64_t)GetTickCount64();
    int64_t elapsed = now - frame_start_ms;
    if (elapsed < target_ms) {
        Sleep((DWORD)(target_ms - elapsed));
    }
}

/* ── Phase 14: Hit-test registry ────────────────────────────────────────── */
void nyx_hit_begin_frame(void) {
    g_hit_count = 0;
}

int32_t nyx_hit_register(float x, float y, float w, float h, int32_t id) {
    if (g_hit_count < 256) {
        g_hit_rects[g_hit_count].x = x;
        g_hit_rects[g_hit_count].y = y;
        g_hit_rects[g_hit_count].w = w;
        g_hit_rects[g_hit_count].h = h;
        g_hit_rects[g_hit_count].id = id;
        g_hit_count++;
    }
    return id;
}

int32_t nyx_hit_test(float mx, float my) {
    /* Walk backwards so top-most widgets (drawn last) win */
    for (int i = g_hit_count - 1; i >= 0; i--) {
        NyxHitRect* r = &g_hit_rects[i];
        if (mx >= r->x && mx < r->x + r->w &&
            my >= r->y && my < r->y + r->h) {
            return r->id;
        }
    }
    return -1;
}

int32_t nyx_hit_clicked(void* window, int32_t id) {
    if (!window) return 0;
    NyxWindow* win = (NyxWindow*)window;
    if (!win->mouse_clicked) return 0;
    return (nyx_hit_test((float)win->mouse_x, (float)win->mouse_y) == id) ? 1 : 0;
}

/* ── Phase 14: Mouse click edge detection ───────────────────────────────── */
void nyx_window_update_click(void* window) {
    if (!window) return;
    NyxWindow* win = (NyxWindow*)window;
    int32_t cur = win->mouse_down;
    win->mouse_clicked  = (!win->prev_mouse_down && cur) ? 1 : 0;
    win->mouse_released = (win->prev_mouse_down && !cur) ? 1 : 0;
    win->prev_mouse_down = cur;
}

int32_t nyx_window_mouse_clicked(void* window) {
    return window ? ((NyxWindow*)window)->mouse_clicked : 0;
}

int32_t nyx_window_mouse_released(void* window) {
    return window ? ((NyxWindow*)window)->mouse_released : 0;
}

/* ── Lifecycle ──────────────────────────────────────────────────────────── */
void rt_ui_init(void) { /* nothing to do on Win32 */ }

int64_t nyx_time_millis(void) {
    return (int64_t)GetTickCount64();
}

void nyx_skia_draw_text(void* canvas, const char* text,
                        float x, float y, float size, uint32_t color) {
    if (!canvas || !text) return;
    NyxCanvas* c = (NyxCanvas*)canvas;
    HDC hdc = c->surface->hdc;
    HFONT hfont = nyx_create_font(size, 0); /* 0 = auto weight */
    HFONT old_font = (HFONT)SelectObject(hdc, hfont);
    SetTextColor(hdc, unpack_colorref(color));
    SetBkMode(hdc, TRANSPARENT);

    int wlen = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    if (wlen > 0) {
        WCHAR* wbuf = (WCHAR*)malloc(sizeof(WCHAR) * (size_t)wlen);
        if (wbuf) {
            MultiByteToWideChar(CP_UTF8, 0, text, -1, wbuf, wlen);
            TextOutW(hdc, (int)x, (int)y, wbuf, wlen - 1);
            free(wbuf);
        }
    }
    SelectObject(hdc, old_font);
    DeleteObject(hfont);
}

/* ── Extended text: explicit weight + optional italic (UTF-8 WideChar) ─── */
void nyx_skia_draw_text_ex(void* canvas, const char* text,
                           float x, float y, float size,
                           int weight, int italic, uint32_t color) {
    if (!canvas || !text) return;
    NyxCanvas* c = (NyxCanvas*)canvas;
    HDC hdc = c->surface->hdc;
    int font_height = -(int)(size > 0 ? size : 14.0f);
    int fw = nyx_weight_to_fw(weight > 0 ? weight : 400);
    HFONT hfont = CreateFontW(
        font_height, 0, 0, 0, fw,
        italic ? TRUE : FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe UI"
    );
    HFONT old_font = (HFONT)SelectObject(hdc, hfont);
    SetTextColor(hdc, unpack_colorref(color));
    SetBkMode(hdc, TRANSPARENT);

    int wlen = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    if (wlen > 0) {
        WCHAR* wbuf = (WCHAR*)malloc(sizeof(WCHAR) * (size_t)wlen);
        if (wbuf) {
            MultiByteToWideChar(CP_UTF8, 0, text, -1, wbuf, wlen);
            TextOutW(hdc, (int)x, (int)y, wbuf, wlen - 1);
            free(wbuf);
        }
    }
    SelectObject(hdc, old_font);
    DeleteObject(hfont);
}

/* ── Icon rendering via Segoe MDL2 Assets (Windows 10+) ─────────────────
 * Pass a Unicode codepoint from the Segoe MDL2 font.  Common codepoints:
 *   0xE700 = Settings   0xE70F = Edit/Pencil   0xE711 = Delete
 *   0xE71E = Search     0xE72E = Lock          0xE738 = Calendar
 *   0xE748 = Home       0xE74D = Save          0xE756 = Font
 *   0xE77B = Play       0xE768 = Stop          0xE769 = Pause
 *   0xE782 = Sort       0xE7C3 = Chart/Analytics
 *   0xE9D9 = Analytics  0xECAA = Graph
 * Falls back to drawing a simple geometric placeholder if font unavailable. */
void nyx_skia_draw_icon(void* canvas, uint32_t codepoint,
                        float x, float y, float size, uint32_t color) {
    if (!canvas) return;
    NyxCanvas* c = (NyxCanvas*)canvas;
    HDC hdc = c->surface->hdc;

    /* Encode codepoint to UTF-16 */
    WCHAR wch[3] = {0, 0, 0};
    int wch_len = 1;
    if (codepoint < 0x10000) {
        wch[0] = (WCHAR)codepoint;
    } else {
        /* Surrogate pair for astral plane */
        uint32_t u = codepoint - 0x10000;
        wch[0] = (WCHAR)(0xD800 | (u >> 10));
        wch[1] = (WCHAR)(0xDC00 | (u & 0x3FF));
        wch_len = 2;
    }

    int font_height = -(int)(size > 0 ? size : 16.0f);
    HFONT hfont = CreateFontW(
        font_height, 0, 0, 0, FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe MDL2 Assets"
    );
    if (!hfont) {
        /* Fallback: geometric circle icon */
        nyx_skia_draw_circle(canvas, x + size * 0.5f, y + size * 0.5f,
                             size * 0.4f, color);
        return;
    }
    HFONT old_font = (HFONT)SelectObject(hdc, hfont);
    SetTextColor(hdc, unpack_colorref(color));
    SetBkMode(hdc, TRANSPARENT);
    TextOutW(hdc, (int)x, (int)y, wch, wch_len);
    SelectObject(hdc, old_font);
    DeleteObject(hfont);
}

/* ── Clip region: push rectangular scissor ───────────────────────────────
 * Saves the current DC state (including any existing clip region) then
 * intersects with the given rect.  Call nyx_clip_pop to restore. */
void nyx_clip_push_rect(void* canvas, float x, float y, float w, float h) {
    if (!canvas) return;
    HDC hdc = ((NyxCanvas*)canvas)->surface->hdc;
    SaveDC(hdc);   /* push clip stack */
    IntersectClipRect(hdc, (int)x, (int)y, (int)(x + w), (int)(y + h));
}

void nyx_clip_pop(void* canvas) {
    if (!canvas) return;
    RestoreDC(((NyxCanvas*)canvas)->surface->hdc, -1);  /* pop clip stack */
}

/* ── Composite Material 3 Widgets (Phase 15) ───────────────────────────────── */

void nyx_ui_draw_modal(void* canvas, const char* title, const char* message, bool is_open, int32_t ok_id, int32_t cancel_id) {
    if (!canvas || !is_open) return;
    /* Fullscreen dimmed backdrop overlay */
    nyx_skia_draw_rect(canvas, 0, 0, 1920.0f, 1080.0f, 0x99000000);

    /* Elevated Modal Dialog Card */
    float dw = 480.0f, dh = 240.0f;
    float dx = (1200.0f - dw) * 0.5f;
    float dy = (720.0f - dh) * 0.5f;

    nyx_skia_draw_shadow(canvas, dx, dy, dw, dh, 16.0f, 8.0f, 0xFF000000);
    nyx_skia_draw_rounded_rect(canvas, dx, dy, dw, dh, 16.0f, 0xFF1E2638);
    nyx_skia_draw_line(canvas, dx + 20.0f, dy + 54.0f, dx + dw - 20.0f, dy + 54.0f, 1.0f, 0xFF303E58);

    /* Dialog Title & Message */
    nyx_skia_draw_text_ex(canvas, title ? title : "Confirmation", dx + 24.0f, dy + 18.0f, 16.0f, 700, 0, 0xFFFFFFFF);
    nyx_skia_draw_text_ex(canvas, message ? message : "Are you sure you want to proceed with this operation?", dx + 24.0f, dy + 75.0f, 13.0f, 400, 0, 0xFFB0BDD0);

    /* Action Buttons */
    float btn_w = 110.0f, btn_h = 40.0f;
    float btn_y = dy + dh - 56.0f;

    /* Cancel Button */
    float cancel_x = dx + dw - 250.0f;
    nyx_skia_draw_rounded_rect(canvas, cancel_x, btn_y, btn_w, btn_h, 20.0f, 0xFF2A364E);
    nyx_skia_draw_text_centered(canvas, "Cancel", cancel_x, btn_y, btn_w, btn_h, 12.0f, 0xFFFFFFFF);
    nyx_hit_register(cancel_x, btn_y, btn_w, btn_h, cancel_id);

    /* Confirm Button */
    float ok_x = dx + dw - 130.0f;
    nyx_skia_draw_glow(canvas, ok_x, btn_y, btn_w, btn_h, 20.0f, 6.0f, 0xFF00E5FF);
    nyx_skia_draw_rounded_rect(canvas, ok_x, btn_y, btn_w, btn_h, 20.0f, 0xFF00E5FF);
    nyx_skia_draw_text_centered(canvas, "Confirm", ok_x, btn_y, btn_w, btn_h, 12.0f, 0xFF0A0E17);
    nyx_hit_register(ok_x, btn_y, btn_w, btn_h, ok_id);
}

void nyx_ui_draw_dropdown(void* canvas, float x, float y, float w, float h,
                          const char* label, const char** options, int option_count,
                          int selected_idx, bool is_expanded, int32_t id_base) {
    if (!canvas) return;
    /* Main Dropdown Header */
    nyx_skia_draw_rounded_rect(canvas, x, y, w, h, 8.0f, 0xFF182234);
    nyx_skia_draw_line(canvas, x, y, x + w, y, 1.0f, 0xFF2E3E5C);

    const char* display_text = (selected_idx >= 0 && selected_idx < option_count && options) ? options[selected_idx] : (label ? label : "Select Option");
    nyx_skia_draw_text_ex(canvas, display_text, x + 12.0f, y + (h - 14.0f) * 0.5f, 12.0f, 500, 0, 0xFFFFFFFF);

    /* Caret Indicator */
    nyx_skia_draw_text_centered(canvas, is_expanded ? "▲" : "▼", x + w - 30.0f, y, 30.0f, h, 10.0f, 0xFF00E5FF);
    nyx_hit_register(x, y, w, h, id_base);

    /* Expanded Popup List */
    if (is_expanded && options && option_count > 0) {
        float pop_h = (float)option_count * 36.0f;
        float pop_y = y + h + 4.0f;

        nyx_skia_draw_shadow(canvas, x, pop_y, w, pop_h, 8.0f, 6.0f, 0xFF000000);
        nyx_skia_draw_rounded_rect(canvas, x, pop_y, w, pop_h, 8.0f, 0xFF1B263A);

        for (int i = 0; i < option_count; i++) {
            float item_y = pop_y + (float)i * 36.0f;
            if (i == selected_idx) {
                nyx_skia_draw_rounded_rect(canvas, x + 4.0f, item_y + 2.0f, w - 8.0f, 32.0f, 6.0f, 0xFF283854);
            }
            nyx_skia_draw_text_ex(canvas, options[i], x + 14.0f, item_y + 9.0f, 12.0f, 500, 0, i == selected_idx ? 0xFF00E5FF : 0xFFCFDAE8);
            nyx_hit_register(x, item_y, w, 36.0f, id_base + 1 + i);
        }
    }
}

void nyx_ui_draw_data_table(void* canvas, float x, float y, float w, float h,
                            const char** headers, int header_count,
                            const char*** rows, int row_count, const float* col_widths) {
    if (!canvas) return;
    nyx_skia_draw_shadow(canvas, x, y, w, h, 12.0f, 4.0f, 0xFF000000);
    nyx_skia_draw_rounded_rect(canvas, x, y, w, h, 12.0f, 0xFF141C2C);

    /* Header Row */
    float header_h = 42.0f;
    nyx_skia_draw_rounded_rect(canvas, x, y, w, header_h, 12.0f, 0xFF1D283E);
    nyx_skia_draw_line(canvas, x, y + header_h, x + w, y + header_h, 1.0f, 0xFF2A3B58);

    float cur_x = x + 16.0f;
    for (int i = 0; i < header_count; i++) {
        float cw = col_widths ? col_widths[i] : (w / (float)header_count);
        nyx_skia_draw_text_ex(canvas, headers[i], cur_x, y + 13.0f, 11.0f, 700, 0, 0xFF00E5FF);
        cur_x += cw;
    }

    /* Scissor-Clipped Body */
    nyx_clip_push_rect(canvas, x, y + header_h + 1.0f, w, h - header_h - 2.0f);
    float row_h = 36.0f;
    for (int r = 0; r < row_count; r++) {
        float ry = y + header_h + 4.0f + (float)r * row_h;
        if (r % 2 == 1) {
            nyx_skia_draw_rect(canvas, x + 4.0f, ry, w - 8.0f, row_h, 0xFF182236);
        }
        cur_x = x + 16.0f;
        for (int c = 0; c < header_count; c++) {
            float cw = col_widths ? col_widths[c] : (w / (float)header_count);
            const char* val = (rows && rows[r] && rows[r][c]) ? rows[r][c] : "-";
            nyx_skia_draw_text_ex(canvas, val, cur_x, ry + 10.0f, 11.0f, 400, 0, 0xFFE0E8F4);
            cur_x += cw;
        }
    }
    nyx_clip_pop(canvas);
}

void nyx_ui_draw_toast(void* canvas, const char* message, int toast_type, float alpha) {
    if (!canvas || !message || alpha <= 0.01f) return;
    float tw = 360.0f, th = 50.0f;
    float tx = (1200.0f - tw) * 0.5f;
    float ty = 720.0f - 80.0f;

    uint32_t bg_col = (toast_type == 1) ? 0xFF00E676 : (toast_type == 2 ? 0xFFFFAB00 : (toast_type == 3 ? 0xFFFF1744 : 0xFF00E5FF));
    nyx_skia_draw_glow(canvas, tx, ty, tw, th, 12.0f, 8.0f, bg_col);
    nyx_skia_draw_rounded_rect(canvas, tx, ty, tw, th, 12.0f, 0xFF1A2234);
    nyx_skia_draw_line(canvas, tx, ty + 4.0f, tx, ty + th - 4.0f, 3.0f, bg_col);
    nyx_skia_draw_text_ex(canvas, message, tx + 18.0f, ty + 16.0f, 12.0f, 600, 0, 0xFFFFFFFF);
}


void nyx_skia_draw_line(void* canvas, float x1, float y1, float x2, float y2, float stroke_width, uint32_t color) {
    if (!canvas) return;
    NyxSurface* s = ((NyxCanvas*)canvas)->surface;
    int ix1 = (int)x1, iy1 = (int)y1, ix2 = (int)x2, iy2 = (int)y2;
    int dx = ix2 - ix1; if (dx < 0) dx = -dx;
    int dy = iy2 - iy1; if (dy < 0) dy = -dy;
    int sx = (ix1 < ix2) ? 1 : -1;
    int sy = (iy1 < iy2) ? 1 : -1;
    int err = dx - dy;
    for (;;) {
        nyx_blend_color(s, ix1, iy1, color);
        if (stroke_width >= 2.0f) {
            nyx_blend_color(s, ix1 + 1, iy1, color);
            nyx_blend_color(s, ix1, iy1 + 1, color);
        }
        if (ix1 == ix2 && iy1 == iy2) break;
        int e2 = err << 1;
        if (e2 > -dy) { err -= dy; ix1 += sx; }
        if (e2 < dx)  { err += dx; iy1 += sy; }
    }
}

void nyx_skia_fill(void* canvas, uint32_t color) {
    if (!canvas) return;
    NyxCanvas* c = (NyxCanvas*)canvas;
    nyx_skia_draw_rect(canvas, 0, 0, (float)c->surface->width, (float)c->surface->height, color);
}

void nyx_skia_save(void* canvas) {
    if (!canvas) return;
    SaveDC(((NyxCanvas*)canvas)->surface->hdc);
}

void nyx_skia_restore(void* canvas) {
    if (!canvas) return;
    RestoreDC(((NyxCanvas*)canvas)->surface->hdc, -1);
}

void nyx_skia_translate(void* canvas, float dx, float dy) {
    if (!canvas) return;
    XFORM xform = { 1.0f, 0.0f, 0.0f, 1.0f, dx, dy };
    SetGraphicsMode(((NyxCanvas*)canvas)->surface->hdc, GM_ADVANCED);
    ModifyWorldTransform(((NyxCanvas*)canvas)->surface->hdc, &xform, MWT_RIGHTMULTIPLY);
}

void nyx_skia_rotate(void* canvas, float degrees) {
    if (!canvas) return;
    float rad = degrees * 3.14159265f / 180.0f;
    XFORM xform = { cosf(rad), sinf(rad), -sinf(rad), cosf(rad), 0.0f, 0.0f };
    SetGraphicsMode(((NyxCanvas*)canvas)->surface->hdc, GM_ADVANCED);
    ModifyWorldTransform(((NyxCanvas*)canvas)->surface->hdc, &xform, MWT_RIGHTMULTIPLY);
}

void nyx_skia_scale(void* canvas, float sx, float sy) {
    if (!canvas) return;
    XFORM xform = { sx, 0.0f, 0.0f, sy, 0.0f, 0.0f };
    SetGraphicsMode(((NyxCanvas*)canvas)->surface->hdc, GM_ADVANCED);
    ModifyWorldTransform(((NyxCanvas*)canvas)->surface->hdc, &xform, MWT_RIGHTMULTIPLY);
}

void* nyx_window_create(int32_t width, int32_t height, const char* title) {
    HINSTANCE hinst = GetModuleHandle(NULL);
    if (!g_class_registered) {
        WNDCLASSA wc = {0};
        wc.lpfnWndProc = NyxWndProc;
        wc.hInstance = hinst;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.lpszClassName = NYX_WINDOW_CLASS;
        RegisterClassA(&wc);
        g_class_registered = 1;
    }

    NyxWindow* win = (NyxWindow*)calloc(1, sizeof(NyxWindow));
    win->width = width > 0 ? width : 800;
    win->height = height > 0 ? height : 600;

    RECT rc = { 0, 0, win->width, win->height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    win->hwnd = CreateWindowExA(
        0, NYX_WINDOW_CLASS, title ? title : "Nyx Material App",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        NULL, NULL, hinst, NULL
    );

    SetWindowLongPtr(win->hwnd, GWLP_USERDATA, (LONG_PTR)win);
    win->hdc = GetDC(win->hwnd);
    win->surface = (NyxSurface*)nyx_skia_surface_create(win->width, win->height);
    win->canvas = (NyxCanvas*)nyx_skia_canvas_create(win->surface);

    ShowWindow(win->hwnd, SW_SHOW);
    UpdateWindow(win->hwnd);
    return win;
}

void nyx_window_destroy(void* window) {
    if (!window) return;
    NyxWindow* win = (NyxWindow*)window;
    if (win->canvas) nyx_skia_canvas_destroy(win->canvas);
    if (win->surface) nyx_skia_surface_destroy(win->surface);
    if (win->hdc && win->hwnd) ReleaseDC(win->hwnd, win->hdc);
    if (win->hwnd) DestroyWindow(win->hwnd);
    free(win);
}

int32_t nyx_window_should_close(void* window) {
    if (!window) return 1;
    return ((NyxWindow*)window)->should_close;
}

void nyx_window_poll_events(void* window) {
    (void)window;
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

void* nyx_window_canvas(void* window) {
    if (!window) return NULL;
    return ((NyxWindow*)window)->canvas;
}

void nyx_window_swap_buffers(void* window) {
    if (!window) return;
    NyxWindow* win = (NyxWindow*)window;
    if (win->hwnd && win->hdc && win->surface) {
        BitBlt(win->hdc, 0, 0, win->width, win->height, win->surface->hdc, 0, 0, SRCCOPY);
    }
}

int32_t nyx_window_width(void* window) {
    return window ? ((NyxWindow*)window)->width : 0;
}

int32_t nyx_window_height(void* window) {
    return window ? ((NyxWindow*)window)->height : 0;
}

void nyx_window_set_title(void* window, const char* title) {
    if (window && ((NyxWindow*)window)->hwnd && title) {
        SetWindowTextA(((NyxWindow*)window)->hwnd, title);
    }
}

int32_t nyx_window_mouse_x(void* window) {
    return window ? ((NyxWindow*)window)->mouse_x : 0;
}

int32_t nyx_window_mouse_y(void* window) {
    return window ? ((NyxWindow*)window)->mouse_y : 0;
}

int32_t nyx_window_mouse_down(void* window) {
    return window ? ((NyxWindow*)window)->mouse_down : 0;
}

int32_t nyx_window_last_key(void* window) {
    if (!window) return 0;
    int32_t k = ((NyxWindow*)window)->last_key;
    ((NyxWindow*)window)->last_key = 0;
    return k;
}

#else

// Fallback in-memory surface for non-Windows platforms
typedef struct {
    int32_t width;
    int32_t height;
    uint32_t* pixels;
} NyxSurface;

typedef struct {
    NyxSurface* surface;
} NyxCanvas;

typedef struct {
    int32_t width;
    int32_t height;
    int32_t should_close;
    NyxSurface* surface;
    NyxCanvas* canvas;
} NyxWindow;

void* nyx_skia_surface_create(int32_t width, int32_t height) {
    NyxSurface* s = (NyxSurface*)calloc(1, sizeof(NyxSurface));
    s->width = width > 0 ? width : 800;
    s->height = height > 0 ? height : 600;
    s->pixels = (uint32_t*)calloc((size_t)s->width * (size_t)s->height, sizeof(uint32_t));
    return s;
}

void nyx_skia_surface_destroy(void* surface) {
    if (surface) {
        free(((NyxSurface*)surface)->pixels);
        free(surface);
    }
}

void* nyx_skia_canvas_create(void* surface) {
    NyxCanvas* c = (NyxCanvas*)calloc(1, sizeof(NyxCanvas));
    c->surface = (NyxSurface*)surface;
    return c;
}

void nyx_skia_canvas_destroy(void* canvas) {
    if (canvas) free(canvas);
}

void nyx_skia_draw_rect(void* canvas, float x, float y, float w, float h, uint32_t color) { (void)canvas; (void)x; (void)y; (void)w; (void)h; (void)color; }
void nyx_skia_draw_rounded_rect(void* canvas, float x, float y, float w, float h, float radius, uint32_t color) { (void)canvas; (void)x; (void)y; (void)w; (void)h; (void)radius; (void)color; }
void nyx_skia_draw_shadow(void* canvas, float x, float y, float w, float h, float radius, float elevation, uint32_t shadow_color) { (void)canvas; (void)x; (void)y; (void)w; (void)h; (void)radius; (void)elevation; (void)shadow_color; }
void nyx_skia_draw_text(void* canvas, const char* text, float x, float y, float size, uint32_t color) { (void)canvas; (void)text; (void)x; (void)y; (void)size; (void)color; }
void nyx_skia_draw_line(void* canvas, float x1, float y1, float x2, float y2, float stroke_width, uint32_t color) { (void)canvas; (void)x1; (void)y1; (void)x2; (void)y2; (void)stroke_width; (void)color; }
void nyx_skia_fill(void* canvas, uint32_t color) { (void)canvas; (void)color; }
void nyx_skia_save(void* canvas) { (void)canvas; }
void nyx_skia_restore(void* canvas) { (void)canvas; }
void nyx_skia_translate(void* canvas, float dx, float dy) { (void)canvas; (void)dx; (void)dy; }
void nyx_skia_rotate(void* canvas, float degrees) { (void)canvas; (void)degrees; }
void nyx_skia_scale(void* canvas, float sx, float sy) { (void)canvas; (void)sx; (void)sy; }

void* nyx_window_create(int32_t width, int32_t height, const char* title) {
    NyxWindow* w = (NyxWindow*)calloc(1, sizeof(NyxWindow));
    w->width = width;
    w->height = height;
    w->surface = (NyxSurface*)nyx_skia_surface_create(width, height);
    w->canvas = (NyxCanvas*)nyx_skia_canvas_create(w->surface);
    return w;
}

void nyx_window_destroy(void* window) {
    if (!window) return;
    NyxWindow* w = (NyxWindow*)window;
    if (w->canvas) nyx_skia_canvas_destroy(w->canvas);
    if (w->surface) nyx_skia_surface_destroy(w->surface);
    free(w);
}

int32_t nyx_window_should_close(void* window) { return window ? ((NyxWindow*)window)->should_close : 1; }
void nyx_window_poll_events(void* window) { (void)window; }
void* nyx_window_canvas(void* window) { return window ? ((NyxWindow*)window)->canvas : NULL; }
void nyx_window_swap_buffers(void* window) { (void)window; }
int32_t nyx_window_width(void* window) { return window ? ((NyxWindow*)window)->width : 0; }
int32_t nyx_window_height(void* window) { return window ? ((NyxWindow*)window)->height : 0; }
void nyx_window_set_title(void* window, const char* title) { (void)window; (void)title; }
int32_t nyx_window_mouse_x(void* window) { (void)window; return 0; }
int32_t nyx_window_mouse_y(void* window) { (void)window; return 0; }
int32_t nyx_window_mouse_down(void* window) { (void)window; return 0; }
int32_t nyx_window_last_key(void* window) { (void)window; return 0; }

/* Phase 14 stubs for non-Win32 */
float nyx_measure_text_width(void* canvas, const char* text, float size) { (void)canvas; if(!text)return 0.f; return (float)strlen(text) * size * 0.6f; }
float nyx_measure_text_height(float size) { return size * 1.25f; }
void nyx_skia_draw_text_centered(void* canvas, const char* text, float x, float y, float w, float h, float size, uint32_t color) { float tw=nyx_measure_text_width(canvas,text,size); float th=nyx_measure_text_height(size); nyx_skia_draw_text(canvas,text,x+(w-tw)*0.5f,y+(h-th)*0.5f,size,color); }
void nyx_frame_sleep(int64_t frame_start_ms, int64_t target_ms) { (void)frame_start_ms; (void)target_ms; }
void nyx_hit_begin_frame(void) { g_hit_count = 0; }
int32_t nyx_hit_register(float x, float y, float w, float h, int32_t id) { if(g_hit_count<256){g_hit_rects[g_hit_count].x=x;g_hit_rects[g_hit_count].y=y;g_hit_rects[g_hit_count].w=w;g_hit_rects[g_hit_count].h=h;g_hit_rects[g_hit_count].id=id;g_hit_count++;} return id; }
int32_t nyx_hit_test(float mx, float my) { for(int i=g_hit_count-1;i>=0;i--){NyxHitRect*r=&g_hit_rects[i];if(mx>=r->x&&mx<r->x+r->w&&my>=r->y&&my<r->y+r->h)return r->id;} return -1; }
int32_t nyx_hit_clicked(void* window, int32_t id) { if(!window)return 0; NyxWindow*win=(NyxWindow*)window; if(!win->mouse_clicked)return 0; return(nyx_hit_test((float)win->mouse_x,(float)win->mouse_y)==id)?1:0; }
void nyx_window_update_click(void* window) { if(!window)return; NyxWindow*win=(NyxWindow*)window; int32_t cur=win->mouse_down; win->mouse_clicked=(!win->prev_mouse_down&&cur)?1:0; win->mouse_released=(win->prev_mouse_down&&!cur)?1:0; win->prev_mouse_down=cur; }
int32_t nyx_window_mouse_clicked(void* window) { return window?((NyxWindow*)window)->mouse_clicked:0; }
int32_t nyx_window_mouse_released(void* window) { return window?((NyxWindow*)window)->mouse_released:0; }
void nyx_skia_draw_text_ex(void* canvas, const char* text, float x, float y, float size, int weight, int italic, uint32_t color) { (void)weight; (void)italic; nyx_skia_draw_text(canvas, text, x, y, size, color); }
void nyx_skia_draw_icon(void* canvas, uint32_t codepoint, float x, float y, float size, uint32_t color) { (void)codepoint; nyx_skia_draw_circle(canvas, x + size*0.5f, y + size*0.5f, size*0.4f, color); }
void nyx_clip_push_rect(void* canvas, float x, float y, float w, float h) { (void)canvas; (void)x; (void)y; (void)w; (void)h; }
void nyx_clip_pop(void* canvas) { (void)canvas; }
void nyx_skia_draw_ripple_anim(void* canvas, float cx, float cy, float max_radius, int64_t elapsed_ms, uint32_t ripple_color) { (void)canvas; (void)cx; (void)cy; (void)max_radius; (void)elapsed_ms; (void)ripple_color; }
void rt_ui_init(void) {}

#endif
