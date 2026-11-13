/*
 * rt_time.h — Nyx Runtime Chrono, Timezones & ISO-8601 Engine
 */

#ifndef RT_TIME_H
#define RT_TIME_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int32_t year;
    int32_t month;  /* 1..12 */
    int32_t day;    /* 1..31 */
    int32_t hour;   /* 0..23 */
    int32_t minute; /* 0..59 */
    int32_t second; /* 0..59 */
    int32_t millisecond; /* 0..999 */
    int32_t tz_offset_minutes; /* e.g. -300 for EST (-5h) */
    char tz_name[8]; /* "UTC", "EST", "PST", "GMT", "CET", "JST" */
} NyxDateTime;

typedef struct {
    int64_t seconds;
    int32_t nanoseconds;
} NyxDuration;

/* Constructors & Clocks */
NyxDateTime nyx_datetime_now_utc(void);
NyxDateTime nyx_datetime_now_local(void);
NyxDateTime nyx_datetime_create(int32_t y, int32_t m, int32_t d, int32_t h, int32_t min, int32_t s, const char* tz);

/* ISO-8601 Formatting & Parsing */
void nyx_datetime_to_iso8601(const NyxDateTime* dt, char* out_buf, size_t buf_len);
bool nyx_datetime_parse_iso8601(const char* str, NyxDateTime* out_dt);

/* Timezone Conversion */
NyxDateTime nyx_datetime_to_timezone(const NyxDateTime* dt, const char* target_tz);

/* Duration Arithmetic */
NyxDuration nyx_duration_from_days(int64_t days);
NyxDuration nyx_duration_from_hours(int64_t hours);
NyxDuration nyx_duration_from_minutes(int64_t minutes);
NyxDuration nyx_duration_from_seconds(int64_t seconds);
NyxDateTime nyx_datetime_add(const NyxDateTime* dt, NyxDuration dur);
NyxDateTime nyx_datetime_sub(const NyxDateTime* dt, NyxDuration dur);
NyxDuration nyx_datetime_diff(const NyxDateTime* a, const NyxDateTime* b);

#ifdef __cplusplus
}
#endif

#endif /* RT_TIME_H */
