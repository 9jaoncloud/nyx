// examples/scientific_calculator_gui.c — Standalone Native Win32 Material Design 3 Scientific Calculator
// Powered by Nyx Runtime (libnyxrt.a / rt_ui)

#include "runtime/rt_ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
    float x, y, w, h;
    const char* label;
    uint32_t bg_color;
    uint32_t text_color;
    int id;
} GuiButton;

// State Machine
static char display_text[128] = "0";
static char history_text[128] = "";
static double previous_val = 0.0;
static char pending_op = 0;
static bool reset_on_next = false;
static double memory_val = 0.0;

static void set_display(double val) {
    if (fabs(val - (long long)val) < 1e-9) {
        snprintf(display_text, sizeof(display_text), "%lld", (long long)val);
    } else {
        snprintf(display_text, sizeof(display_text), "%.6f", val);
        // Trim trailing zeros
        char* dot = strchr(display_text, '.');
        if (dot) {
            char* end = display_text + strlen(display_text) - 1;
            while (end > dot && *end == '0') {
                *end-- = '\0';
            }
            if (end == dot) *end = '\0';
        }
    }
}

static void on_digit(const char* d) {
    if (reset_on_next || strcmp(display_text, "0") == 0) {
        snprintf(display_text, sizeof(display_text), "%s", d);
        reset_on_next = false;
    } else {
        if (strlen(display_text) < 24) {
            strncat(display_text, d, sizeof(display_text) - strlen(display_text) - 1);
        }
    }
}

static void on_decimal() {
    if (reset_on_next) {
        snprintf(display_text, sizeof(display_text), "0.");
        reset_on_next = false;
    } else if (!strchr(display_text, '.')) {
        strncat(display_text, ".", sizeof(display_text) - strlen(display_text) - 1);
    }
}

static void on_op(char op) {
    previous_val = atof(display_text);
    pending_op = op;
    snprintf(history_text, sizeof(history_text), "%s %c", display_text, op);
    reset_on_next = true;
}

static void on_equal() {
    if (pending_op == 0) return;
    double current = atof(display_text);
    double result = current;

    switch (pending_op) {
        case '+': result = previous_val + current; break;
        case '-': result = previous_val - current; break;
        case '*': result = previous_val * current; break;
        case '/':
            if (current != 0.0) result = previous_val / current;
            else {
                snprintf(display_text, sizeof(display_text), "Error: Div by 0");
                pending_op = 0;
                reset_on_next = true;
                return;
            }
            break;
        case '^': result = pow(previous_val, current); break;
    }

    snprintf(history_text, sizeof(history_text), "%s %c %s =", history_text, pending_op, display_text);
    set_display(result);
    pending_op = 0;
    reset_on_next = true;
}

static void on_func(const char* func) {
    double current = atof(display_text);
    double result = current;

    if (strcmp(func, "sin") == 0) {
        result = sin(current * (3.141592653589793 / 180.0)); // degrees
        snprintf(history_text, sizeof(history_text), "sin(%s°)", display_text);
    } else if (strcmp(func, "cos") == 0) {
        result = cos(current * (3.141592653589793 / 180.0));
        snprintf(history_text, sizeof(history_text), "cos(%s°)", display_text);
    } else if (strcmp(func, "tan") == 0) {
        result = tan(current * (3.141592653589793 / 180.0));
        snprintf(history_text, sizeof(history_text), "tan(%s°)", display_text);
    } else if (strcmp(func, "sqrt") == 0) {
        if (current >= 0.0) {
            result = sqrt(current);
            snprintf(history_text, sizeof(history_text), "√(%s)", display_text);
        } else {
            snprintf(display_text, sizeof(display_text), "Error: Domain");
            reset_on_next = true;
            return;
        }
    } else if (strcmp(func, "ln") == 0) {
        if (current > 0.0) {
            result = log(current);
            snprintf(history_text, sizeof(history_text), "ln(%s)", display_text);
        } else {
            snprintf(display_text, sizeof(display_text), "Error: Domain");
            reset_on_next = true;
            return;
        }
    } else if (strcmp(func, "fact") == 0) {
        long long n = (long long)current;
        if (n >= 0 && n <= 20) {
            double f = 1.0;
            for (long long i = 2; i <= n; i++) f *= i;
            result = f;
            snprintf(history_text, sizeof(history_text), "%lld!", n);
        } else {
            snprintf(display_text, sizeof(display_text), "Error: Overflow");
            reset_on_next = true;
            return;
        }
    }

    set_display(result);
    reset_on_next = true;
}

int main() {
    int win_w = 440;
    int win_h = 660;

    void* win = nyx_window_create(win_w, win_h, "Nyx Scientific Calculator — Material Design 3");
    if (!win) {
        fprintf(stderr, "Failed to create Win32 native window!\n");
        return 1;
    }

    void* canvas = nyx_window_canvas(win);

    // Color Palette (Material Design 3 Dark)
    uint32_t BG_SURFACE = 0xFF121212;
    uint32_t CARD_BG = 0xFF1E1E1E;
    uint32_t BTN_NUM_BG = 0xFF282828;
    uint32_t BTN_FN_BG = 0xFF352E47;
    uint32_t BTN_OP_BG = 0xFF22384A;
    uint32_t BTN_ACCENT = 0xFF38BDF8;
    uint32_t BTN_WARN = 0xFFF43F5E;
    uint32_t TEXT_WHITE = 0xFFFFFFFF;
    uint32_t TEXT_MUTED = 0xFF94A3B8;
    uint32_t TEXT_DARK = 0xFF022C22;

    // Build Keypad Grid (5 columns x 6 rows)
    GuiButton buttons[] = {
        // Row 1 (Functions)
        { 18, 140, 74, 52, "sin",  BTN_FN_BG, 0xFFC4B5FD, 101 },
        { 100, 140, 74, 52, "cos",  BTN_FN_BG, 0xFFC4B5FD, 102 },
        { 182, 140, 74, 52, "tan",  BTN_FN_BG, 0xFFC4B5FD, 103 },
        { 264, 140, 74, 52, "√",    BTN_FN_BG, 0xFFC4B5FD, 104 },
        { 346, 140, 74, 52, "C",    BTN_WARN,  TEXT_WHITE, 105 },

        // Row 2 (Advanced Math)
        { 18, 202, 74, 52, "ln",   BTN_FN_BG, 0xFFC4B5FD, 106 },
        { 100, 202, 74, 52, "xʸ",   BTN_FN_BG, 0xFFC4B5FD, 107 },
        { 182, 202, 74, 52, "n!",   BTN_FN_BG, 0xFFC4B5FD, 108 },
        { 264, 202, 74, 52, "π",    BTN_FN_BG, 0xFFC4B5FD, 109 },
        { 346, 202, 74, 52, "÷",    BTN_OP_BG, 0xFF7DD3FC, 110 },

        // Row 3 (7, 8, 9, etc)
        { 18, 264, 74, 52, "7",    BTN_NUM_BG, TEXT_WHITE, 7 },
        { 100, 264, 74, 52, "8",    BTN_NUM_BG, TEXT_WHITE, 8 },
        { 182, 264, 74, 52, "9",    BTN_NUM_BG, TEXT_WHITE, 9 },
        { 264, 264, 74, 52, "e",    BTN_FN_BG, 0xFFC4B5FD, 111 },
        { 346, 264, 74, 52, "×",    BTN_OP_BG, 0xFF7DD3FC, 112 },

        // Row 4 (4, 5, 6, -)
        { 18, 326, 74, 52, "4",    BTN_NUM_BG, TEXT_WHITE, 4 },
        { 100, 326, 74, 52, "5",    BTN_NUM_BG, TEXT_WHITE, 5 },
        { 182, 326, 74, 52, "6",    BTN_NUM_BG, TEXT_WHITE, 6 },
        { 264, 326, 74, 52, "MS",   BTN_FN_BG, 0xFF6EE7B7, 113 },
        { 346, 326, 74, 52, "−",    BTN_OP_BG, 0xFF7DD3FC, 114 },

        // Row 5 (1, 2, 3, +)
        { 18, 388, 74, 52, "1",    BTN_NUM_BG, TEXT_WHITE, 1 },
        { 100, 388, 74, 52, "2",    BTN_NUM_BG, TEXT_WHITE, 2 },
        { 182, 388, 74, 52, "3",    BTN_NUM_BG, TEXT_WHITE, 3 },
        { 264, 388, 74, 52, "MR",   BTN_FN_BG, 0xFF6EE7B7, 115 },
        { 346, 388, 74, 52, "+",    BTN_OP_BG, 0xFF7DD3FC, 116 },

        // Row 6 (0, ., +/-, MC, =)
        { 18, 450, 74, 52, "0",    BTN_NUM_BG, TEXT_WHITE, 0 },
        { 100, 450, 74, 52, ".",    BTN_NUM_BG, TEXT_WHITE, 117 },
        { 182, 450, 74, 52, "MC",   BTN_FN_BG, 0xFF6EE7B7, 118 },
        { 264, 450, 156, 52, "=",   BTN_ACCENT, TEXT_DARK, 119 },
    };
    int num_buttons = sizeof(buttons) / sizeof(buttons[0]);

    printf("==================================================\n");
    printf("🚀 Nyx Native Win32 Scientific Calculator Running!\n");
    printf("• Window Surface: 440x660 @ 120 FPS\n");
    printf("• Material Design 3 Canvas\n");
    printf("==================================================\n");

    while (!nyx_window_should_close(win)) {
        int64_t frame_start = nyx_time_millis();
        nyx_window_poll_events(win);
        nyx_window_update_click(win);

        int mouse_clicked = nyx_window_mouse_clicked(win);
        int mx = nyx_window_mouse_x(win);
        int my = nyx_window_mouse_y(win);

        // Handle Click Events
        if (mouse_clicked) {
            for (int i = 0; i < num_buttons; i++) {
                GuiButton* b = &buttons[i];
                if (mx >= b->x && mx <= (b->x + b->w) && my >= b->y && my <= (b->y + b->h)) {
                    if (b->id >= 0 && b->id <= 9) {
                        char d[4]; snprintf(d, sizeof(d), "%d", b->id);
                        on_digit(d);
                    } else if (b->id == 117) { // .
                        on_decimal();
                    } else if (b->id == 105) { // C
                        snprintf(display_text, sizeof(display_text), "0");
                        snprintf(history_text, sizeof(history_text), "");
                        pending_op = 0;
                        previous_val = 0.0;
                    } else if (b->id == 116) { // +
                        on_op('+');
                    } else if (b->id == 114) { // -
                        on_op('-');
                    } else if (b->id == 112) { // *
                        on_op('*');
                    } else if (b->id == 110) { // /
                        on_op('/');
                    } else if (b->id == 107) { // ^
                        on_op('^');
                    } else if (b->id == 119) { // =
                        on_equal();
                    } else if (b->id == 101) { // sin
                        on_func("sin");
                    } else if (b->id == 102) { // cos
                        on_func("cos");
                    } else if (b->id == 103) { // tan
                        on_func("tan");
                    } else if (b->id == 104) { // sqrt
                        on_func("sqrt");
                    } else if (b->id == 106) { // ln
                        on_func("ln");
                    } else if (b->id == 108) { // fact
                        on_func("fact");
                    } else if (b->id == 109) { // pi
                        set_display(3.141592653589793);
                        reset_on_next = true;
                    } else if (b->id == 111) { // e
                        set_display(2.718281828459045);
                        reset_on_next = true;
                    } else if (b->id == 113) { // MS
                        memory_val = atof(display_text);
                    } else if (b->id == 115) { // MR
                        set_display(memory_val);
                        reset_on_next = true;
                    } else if (b->id == 118) { // MC
                        memory_val = 0.0;
                    }
                    break;
                }
            }
        }

        // 1. Clear Surface with Background
        nyx_skia_fill(canvas, BG_SURFACE);

        // 2. Draw Top App Bar / Title
        nyx_skia_draw_text(canvas, "NYX SCIENTIFIC CALCULATOR", 20, 32, 14.0f, TEXT_MUTED);

        // 3. Draw Main Display Card (Material Card with 12px Radius)
        nyx_skia_draw_rounded_rect(canvas, 18, 44, 402, 80, 12.0f, CARD_BG);
        if (history_text[0] != '\0') {
            nyx_skia_draw_text(canvas, history_text, 34, 68, 13.0f, TEXT_MUTED);
        }
        nyx_skia_draw_text(canvas, display_text, 34, 105, 26.0f, TEXT_WHITE);

        // 4. Draw Interactive Buttons
        for (int i = 0; i < num_buttons; i++) {
            GuiButton* b = &buttons[i];
            bool is_hover = (mx >= b->x && mx <= (b->x + b->w) && my >= b->y && my <= (b->y + b->h));
            uint32_t col = b->bg_color;
            if (is_hover) {
                // Lighten on hover
                col = col + 0x00151515;
            }

            nyx_skia_draw_rounded_rect(canvas, b->x, b->y, b->w, b->h, 10.0f, col);
            nyx_skia_draw_text_centered(canvas, b->label, b->x, b->y, b->w, b->h, 16.0f, b->text_color);
        }

        // 5. Footer indicator
        nyx_skia_draw_text(canvas, "Nyx v0.28.0 · 120 FPS Zero-GC Engine", 20, 524, 11.0f, 0xFF64748B);

        // 6. Double-buffer Presentation (Flicker-Free)
        nyx_window_swap_buffers(win);

        // Cap frame rate at ~60-120 FPS
        nyx_frame_sleep(frame_start, 16);
    }

    nyx_window_destroy(win);
    return 0;
}
