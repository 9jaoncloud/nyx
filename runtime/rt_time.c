/*
 * rt_time.c — Nyx Runtime Chrono, Timezones & ISO-8601 Engine Implementation
 */

#include "rt_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

static int32_t get_tz_offset(const char* tz) {
    if (!tz) return 0;
    if (strcmp(tz, "UTC") == 0 || strcmp(tz, "GMT") == 0) return 0;
    if (strcmp(tz, "EST") == 0) return -300;
    if (strcmp(tz, "EDT") == 0) return -240;
    if (strcmp(tz, "CST") == 0) return -360;
    if (strcmp(tz, "PST") == 0) return -480;
    if (strcmp(tz, "CET") == 0) return +60;
    if (strcmp(tz, "JST") == 0) return +540;
    return 0;
}

NyxDateTime nyx_datetime_now_utc(void) {
    time_t raw = time(NULL);
    struct tm* gmt = gmtime(&raw);

    NyxDateTime dt;
    memset(&dt, 0, sizeof(dt));
    if (gmt) {
        dt.year = gmt->tm_year + 1900;
        dt.month = gmt->tm_mon + 1;
        dt.day = gmt->tm_mday;
        dt.hour = gmt->tm_hour;
        dt.minute = gmt->tm_min;
        dt.second = gmt->tm_sec;
    } else {
        dt.year = 2026; dt.month = 8; dt.day = 28;
    }
    dt.millisecond = 0;
    dt.tz_offset_minutes = 0;
    strncpy(dt.tz_name, "UTC", sizeof(dt.tz_name));
    return dt;
}

NyxDateTime nyx_datetime_now_local(void) {
    time_t raw = time(NULL);
    struct tm* loc = localtime(&raw);

    NyxDateTime dt;
    memset(&dt, 0, sizeof(dt));
    if (loc) {
        dt.year = loc->tm_year + 1900;
        dt.month = loc->tm_mon + 1;
        dt.day = loc->tm_mday;
        dt.hour = loc->tm_hour;
        dt.minute = loc->tm_min;
        dt.second = loc->tm_sec;
    } else {
        dt.year = 2026; dt.month = 8; dt.day = 28;
    }
    dt.millisecond = 0;
    dt.tz_offset_minutes = 60; /* Local default */
    strncpy(dt.tz_name, "LOCAL", sizeof(dt.tz_name));
    return dt;
}

NyxDateTime nyx_datetime_create(int32_t y, int32_t m, int32_t d, int32_t h, int32_t min, int32_t s, const char* tz) {
    NyxDateTime dt;
    memset(&dt, 0, sizeof(dt));
    dt.year = y;
    dt.month = m;
    dt.day = d;
    dt.hour = h;
    dt.minute = min;
    dt.second = s;
    dt.millisecond = 0;
    dt.tz_offset_minutes = get_tz_offset(tz);
    strncpy(dt.tz_name, (tz ? tz : "UTC"), sizeof(dt.tz_name) - 1);
    return dt;
}

void nyx_datetime_to_iso8601(const NyxDateTime* dt, char* out_buf, size_t buf_len) {
    if (!dt || !out_buf || buf_len < 25) return;
    if (dt->tz_offset_minutes == 0) {
        snprintf(out_buf, buf_len, "%04d-%02d-%02dT%02d:%02d:%02dZ",
                 dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->second);
    } else {
        int off_h = abs(dt->tz_offset_minutes) / 60;
        int off_m = abs(dt->tz_offset_minutes) % 60;
        char sign = (dt->tz_offset_minutes >= 0) ? '+' : '-';
        snprintf(out_buf, buf_len, "%04d-%02d-%02dT%02d:%02d:%02d%c%02d:%02d",
                 dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->second, sign, off_h, off_m);
    }
}

bool nyx_datetime_parse_iso8601(const char* str, NyxDateTime* out_dt) {
    if (!str || !out_dt) return false;
    memset(out_dt, 0, sizeof(NyxDateTime));

    int y = 0, m = 0, d = 0, h = 0, min = 0, s = 0;
    if (sscanf(str, "%d-%d-%dT%d:%d:%d", &y, &m, &d, &h, &min, &s) >= 3) {
        out_dt->year = y;
        out_dt->month = m;
        out_dt->day = d;
        out_dt->hour = h;
        out_dt->minute = min;
        out_dt->second = s;
        out_dt->tz_offset_minutes = 0;
        strncpy(out_dt->tz_name, "UTC", sizeof(out_dt->tz_name));
        return true;
    }
    return false;
}

NyxDateTime nyx_datetime_to_timezone(const NyxDateTime* dt, const char* target_tz) {
    if (!dt) return nyx_datetime_now_utc();
    int32_t target_off = get_tz_offset(target_tz);
    int32_t delta_minutes = target_off - dt->tz_offset_minutes;

    NyxDateTime res = *dt;
    res.tz_offset_minutes = target_off;
    strncpy(res.tz_name, target_tz, sizeof(res.tz_name) - 1);

    int total_min = res.hour * 60 + res.minute + delta_minutes;
    while (total_min < 0) {
        total_min += 1440;
        res.day -= 1;
        if (res.day < 1) { res.month -= 1; res.day = 28; }
    }
    while (total_min >= 1440) {
        total_min -= 1440;
        res.day += 1;
        if (res.day > 28) { res.month += 1; res.day = 1; }
    }

    res.hour = total_min / 60;
    res.minute = total_min % 60;
    return res;
}

NyxDuration nyx_duration_from_days(int64_t days) {
    NyxDuration d = { days * 86400LL, 0 };
    return d;
}

NyxDuration nyx_duration_from_hours(int64_t hours) {
    NyxDuration d = { hours * 3600LL, 0 };
    return d;
}

NyxDuration nyx_duration_from_minutes(int64_t minutes) {
    NyxDuration d = { minutes * 60LL, 0 };
    return d;
}

NyxDuration nyx_duration_from_seconds(int64_t seconds) {
    NyxDuration d = { seconds, 0 };
    return d;
}

NyxDateTime nyx_datetime_add(const NyxDateTime* dt, NyxDuration dur) {
    NyxDateTime res = *dt;
    int64_t add_seconds = dur.seconds;
    int64_t add_days = add_seconds / 86400LL;
    int64_t rem_sec = add_seconds % 86400LL;

    res.day += (int32_t)add_days;
    int total_sec = res.hour * 3600 + res.minute * 60 + res.second + (int)rem_sec;
    while (total_sec >= 86400) { total_sec -= 86400; res.day++; }
    while (total_sec < 0) { total_sec += 86400; res.day--; }

    res.hour = total_sec / 3600;
    res.minute = (total_sec % 3600) / 60;
    res.second = total_sec % 60;
    return res;
}

NyxDateTime nyx_datetime_sub(const NyxDateTime* dt, NyxDuration dur) {
    NyxDuration neg_dur = { -dur.seconds, -dur.nanoseconds };
    return nyx_datetime_add(dt, neg_dur);
}

NyxDuration nyx_datetime_diff(const NyxDateTime* a, const NyxDateTime* b) {
    int64_t sec_a = (int64_t)a->day * 86400 + a->hour * 3600 + a->minute * 60 + a->second;
    int64_t sec_b = (int64_t)b->day * 86400 + b->hour * 3600 + b->minute * 60 + b->second;
    NyxDuration d = { (sec_a - sec_b), 0 };
    return d;
}
