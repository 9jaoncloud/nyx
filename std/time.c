#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "runtime/rt_arena.h"
#include "runtime/rt_string.h"
#include "runtime/rt_io.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_option.h"
#include "runtime/rt_map.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_async.h"
#include "runtime/rt_json.h"
#include "runtime/rt_db.h"
#include "runtime/rt_http.h"
#include "runtime/rt_net.h"
#include "runtime/rt_crypto.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_time.h"
#include "runtime/rt_nursery.h"
#include "runtime/rt_collections.h"
#include "runtime/rt_math.h"
#include "runtime/rt_format.h"
#include "runtime/rt_thread.h"
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

typedef NyxDuration Duration;

typedef NyxDateTime DateTime;

NyxDuration Duration_from_secs(int64_t);
NyxDuration Duration_from_millis(int64_t);
NyxDuration Duration_from_days(int64_t);
NyxDuration Duration_from_hours(int64_t);
NyxDateTime DateTime_now_utc();
NyxDateTime DateTime_new(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, rt_string_t);
NyxDateTime DateTime_to_timezone(DateTime*, rt_string_t);
rt_string_t DateTime_to_iso8601(DateTime*);
NyxDateTime DateTime_add(DateTime*, NyxDuration);
NyxDateTime DateTime_sub(DateTime*, NyxDuration);
// module time
// extern nyx_datetime_now
// extern nyx_datetime_create
// extern nyx_datetime_to_iso8601
// extern nyx_datetime_to_timezone
// extern nyx_datetime_add
// extern nyx_datetime_sub
NyxDuration Duration_from_secs(int64_t secs) {
return (Duration){.seconds = secs, .nanoseconds = 0LL};
}

NyxDuration Duration_from_millis(int64_t ms) {
return (Duration){.seconds = (ms / 1000LL), .nanoseconds = ((int64_t)((ms % 1000LL)) * 1000000LL)};
}

NyxDuration Duration_from_days(int64_t days) {
return (Duration){.seconds = (days * 86400LL), .nanoseconds = 0LL};
}

NyxDuration Duration_from_hours(int64_t hours) {
return (Duration){.seconds = (hours * 3600LL), .nanoseconds = 0LL};
}

NyxDateTime DateTime_now_utc() {
return nyx_datetime_create(2026LL, 8LL, 28LL, 6LL, 30LL, 0LL, ((const uint8_t*)(rt_string_from("UTC").data)));
}

NyxDateTime DateTime_new(int64_t year, int64_t month, int64_t day, int64_t hour, int64_t min, int64_t sec, rt_string_t tz) {
return nyx_datetime_create(year, month, day, hour, min, sec, ((const uint8_t*)(tz.data)));
}

NyxDateTime DateTime_to_timezone(DateTime* self, rt_string_t target_tz) {
return nyx_datetime_to_timezone(self, ((const uint8_t*)(target_tz.data)));
}

rt_string_t DateTime_to_iso8601(DateTime* self) {
uint8_t buf[64LL] = { 0LL };
nyx_datetime_to_iso8601(self, (&(buf[0LL])), 64LL);
return rt_string_from((const char*)(buf));
}

NyxDateTime DateTime_add(DateTime* self, NyxDuration dur) {
return nyx_datetime_add(self, dur);
}

NyxDateTime DateTime_sub(DateTime* self, NyxDuration dur) {
return nyx_datetime_sub(self, dur);
}

