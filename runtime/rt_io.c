#include "rt_io.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
static int g_console_utf8_set = 0;
static void ensure_utf8_console(void) {
    if (!g_console_utf8_set) {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        g_console_utf8_set = 1;
    }
}
#else
static void ensure_utf8_console(void) {}
#endif

void rt_println(rt_string_t s) {
    ensure_utf8_console();
    printf("%s\n", s.data);
    rt_string_release(&s);
}

void rt_print(rt_string_t s) {
    ensure_utf8_console();
    printf("%s", s.data);
    rt_string_release(&s);
}

void rt_print_int(int64_t n) {
    ensure_utf8_console();
    printf("%lld", (long long)n);
}

void rt_print_float(double f) {
    ensure_utf8_console();
    printf("%f", f);
}

void rt_eprintln(rt_string_t s) {
    ensure_utf8_console();
    fprintf(stderr, "%s\n", s.data);
    rt_string_release(&s);
}

int64_t rt_read_stdin(char* buf, int64_t max_len) {
    ensure_utf8_console();
    if (!buf || max_len <= 0) return 0;
    if (fgets(buf, (int)max_len, stdin) == NULL) return 0;
    int64_t len = (int64_t)strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
        len--;
    }
    if (len > 0 && buf[len - 1] == '\r') {
        buf[len - 1] = '\0';
        len--;
    }
    return len;
}

#include <time.h>
rt_string_t rt_get_system_datetime_string(void) {
    time_t raw = time(NULL);
    struct tm* loc = localtime(&raw);
    char buf[128];
    if (loc) {
        strftime(buf, sizeof(buf), "%A, %B %d, %Y - %H:%M:%S", loc);
    } else {
        snprintf(buf, sizeof(buf), "Saturday, October 03, 2026 - 21:18:36");
    }
    return rt_string_from(buf);
}
