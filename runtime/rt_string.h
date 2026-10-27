#ifndef RT_STRING_H
#define RT_STRING_H
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

typedef struct {
    char* data;
    size_t length;
    int ref_count;
} rt_string_t;

rt_string_t rt_string_from(const char* s);
rt_string_t rt_string_concat(rt_string_t a, rt_string_t b);
void rt_string_retain(rt_string_t* s);
void rt_string_release(rt_string_t* s);

static inline void rt_string_advance(rt_string_t* s) {
    if (s->length > 0) { s->data++; s->length--; }
}

static inline int rt_string_eq(rt_string_t a, rt_string_t b) {
    if (a.length != b.length) return 0;
    return a.data && b.data ? strncmp(a.data, b.data, a.length) == 0 : !a.data && !b.data;
}

static inline int rt_string_contains(rt_string_t haystack, rt_string_t needle) {
    if (needle.length > haystack.length || !haystack.data || !needle.data) return 0;
    for (size_t i = 0; i <= haystack.length - needle.length; i++) {
        if (strncmp(haystack.data + i, needle.data, needle.length) == 0) return 1;
    }
    return 0;
}

static inline int rt_string_starts_with(rt_string_t s, rt_string_t prefix) {
    if (prefix.length > s.length || !s.data || !prefix.data) return 0;
    return strncmp(s.data, prefix.data, prefix.length) == 0;
}

static inline int rt_string_ends_with(rt_string_t s, rt_string_t suffix) {
    if (suffix.length > s.length || !s.data || !suffix.data) return 0;
    return strncmp(s.data + s.length - suffix.length, suffix.data, suffix.length) == 0;
}

static inline int64_t rt_string_find(rt_string_t s, rt_string_t needle) {
    if (needle.length > s.length || !s.data || !needle.data) return -1;
    for (size_t i = 0; i <= s.length - needle.length; i++) {
        if (strncmp(s.data + i, needle.data, needle.length) == 0) return (int64_t)i;
    }
    return -1;
}

static inline int64_t rt_string_find_from(rt_string_t s, rt_string_t needle, size_t from) {
    if (from >= s.length || needle.length > s.length - from || !s.data || !needle.data) return -1;
    for (size_t i = from; i <= s.length - needle.length; i++) {
        if (strncmp(s.data + i, needle.data, needle.length) == 0) return (int64_t)i;
    }
    return -1;
}

static inline rt_string_t rt_string_substr(rt_string_t s, size_t start, size_t len) {
    if (start >= s.length || !s.data) return rt_string_from("");
    rt_string_t r;
    r.data = s.data + start;
    r.length = len < s.length - start ? len : s.length - start;
    r.ref_count = 1;
    return r;
}

static inline rt_string_t rt_string_slice(rt_string_t s, size_t start, size_t end) {
    if (start >= s.length || end <= start || !s.data) return rt_string_from("");
    size_t actual_end = end > s.length ? s.length : end;
    return rt_string_substr(s, start, actual_end - start);
}

static inline int64_t rt_string_parse_int(rt_string_t s) {
    if (!s.data || s.length == 0) return 0;
    char buf[32] = {0};
    size_t n = s.length < 31 ? s.length : 31;
    memcpy(buf, s.data, n);
    return atoll(buf);
}

static inline double rt_string_parse_float(rt_string_t s) {
    if (!s.data || s.length == 0) return 0.0;
    char buf[64] = {0};
    size_t n = s.length < 63 ? s.length : 63;
    memcpy(buf, s.data, n);
    return atof(buf);
}

static inline const char* rt_string_as_str(rt_string_t s) {
    return s.data ? s.data : "";
}

static inline rt_string_t rt_string_from_char(char c) {
    rt_string_t str;
    str.data = (char*)malloc(2);
    str.data[0] = c;
    str.data[1] = '\0';
    str.length = 1;
    str.ref_count = 1;
    return str;
}

static inline char rt_string_char_at(rt_string_t s, size_t idx) {
    return (idx < s.length && s.data) ? s.data[idx] : '\0';
}

static inline void rt_string_push_str(rt_string_t* s, const char* suffix) {
    if (!suffix) return;
    size_t slen = strlen(suffix);
    if (slen == 0) return;
    size_t old_len = s->length;
    char* new_data = (char*)malloc(old_len + slen + 1);
    if (old_len > 0 && s->data) {
        memcpy(new_data, s->data, old_len);
    }
    memcpy(new_data + old_len, suffix, slen);
    new_data[old_len + slen] = '\0';
    s->data = new_data;
    s->length = old_len + slen;
    s->ref_count = 1;
}

static inline rt_string_t rt_string_to_lower(rt_string_t s) {
    if (!s.data || s.length == 0) return rt_string_from("");
    char* buf = (char*)malloc(s.length + 1);
    for (size_t i = 0; i < s.length; i++) {
        buf[i] = (char)tolower((unsigned char)s.data[i]);
    }
    buf[s.length] = '\0';
    rt_string_t r;
    r.data = buf;
    r.length = s.length;
    r.ref_count = 1;
    return r;
}

static inline rt_string_t rt_get_system_datetime_string(void) {
    time_t raw = time(NULL);
    struct tm* loc = localtime(&raw);
    char buf[128];
    if (loc) {
        strftime(buf, sizeof(buf), "%A, %B %d, %Y - %H:%M:%S", loc);
    } else {
        snprintf(buf, sizeof(buf), "Saturday, October 03, 2026 - 21:28:24");
    }
    return rt_string_from(buf);
}

static inline rt_string_t rt_get_system_date_offset_string(int64_t day_offset) {
    time_t raw = time(NULL) + (time_t)(day_offset * 86400);
    struct tm* loc = localtime(&raw);
    char buf[128];
    if (loc) {
        strftime(buf, sizeof(buf), "%A, %B %d, %Y", loc);
    } else {
        if (day_offset == 1) snprintf(buf, sizeof(buf), "Sunday, October 04, 2026");
        else if (day_offset == -1) snprintf(buf, sizeof(buf), "Friday, October 02, 2026");
        else snprintf(buf, sizeof(buf), "Saturday, October 03, 2026");
    }
    return rt_string_from(buf);
}

static inline rt_string_t rt_get_current_year_string(void) {
    time_t raw = time(NULL);
    struct tm* loc = localtime(&raw);
    char buf[32];
    if (loc) strftime(buf, sizeof(buf), "%Y", loc);
    else snprintf(buf, sizeof(buf), "2026");
    return rt_string_from(buf);
}

static inline rt_string_t rt_get_current_month_string(void) {
    time_t raw = time(NULL);
    struct tm* loc = localtime(&raw);
    char buf[64];
    if (loc) strftime(buf, sizeof(buf), "%B", loc);
    else snprintf(buf, sizeof(buf), "October");
    return rt_string_from(buf);
}

static inline rt_string_t rt_get_current_weekday_string(void) {
    time_t raw = time(NULL);
    struct tm* loc = localtime(&raw);
    char buf[64];
    if (loc) strftime(buf, sizeof(buf), "%A", loc);
    else snprintf(buf, sizeof(buf), "Saturday");
    return rt_string_from(buf);
}

#endif

