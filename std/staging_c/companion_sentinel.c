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
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
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

#define TAG_AlignmentMode_LoyalGuardian 0
#define TAG_AlignmentMode_ProactiveWatchdog 1
#define TAG_AlignmentMode_EmpatheticCompanion 2
typedef struct {
    int tag;
    union {
    } data;
} AlignmentMode;

typedef struct {
    int64_t memory_id;
    int64_t timestamp_sec;
    double importance_weight;
    rt_string_t context_tag;
    rt_string_t memory_text;
} EpisodicMemory;

typedef struct {
    rt_string_t risk_type;
    rt_string_t downside_summary;
    double severity_score;
    int user_acknowledged;
} RiskAdvisory;

typedef struct {
    void* raw_ptr;
    AlignmentMode mode;
} CompanionSentinel;

CompanionSentinel CompanionSentinel_new(AlignmentMode);
int CompanionSentinel_remember(CompanionSentinel*, rt_string_t, rt_string_t, double);
rt_string_t CompanionSentinel_advise_and_execute(CompanionSentinel*, rt_string_t, int);
NyxOption CompanionSentinel_proactive_poll(CompanionSentinel*);
CompanionSentinel CompanionSentinel_new(AlignmentMode mode) {
return ({
int64_t ptr = rt_companion_engine_create(0LL);
(CompanionSentinel){.raw_ptr = ptr, .mode = mode};
});
}

int CompanionSentinel_remember(CompanionSentinel* self, rt_string_t tag, rt_string_t text, double importance) {
return ({
rt_companion_store_memory((*self).raw_ptr, ((const uint8_t*)(tag.data)), ((const uint8_t*)(text.data)), importance);
});
}

rt_string_t CompanionSentinel_advise_and_execute(CompanionSentinel* self, rt_string_t directive, int confirmed) {
uint8_t report_buf[1024LL] = { 0LL };
return ({
int64_t ok = rt_companion_execute_loyal_action((*self).raw_ptr, ((const uint8_t*)(directive.data)), confirmed, uint8_t_as_mut_ptr(report_buf), report_buf->len);
String_to_string(String_from_utf8_lossy((&(report_buf))));
});
}

NyxOption CompanionSentinel_proactive_poll(CompanionSentinel* self) {
uint8_t msg_buf[512LL] = { 0LL };
return ({
({ NyxOption __if_res_1; if (rt_companion_check_proactive_initiative((*self).raw_ptr, uint8_t_as_mut_ptr(msg_buf), msg_buf->len)) {
__if_res_1 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = String_to_string(String_from_utf8_lossy((&(msg_buf)))); &_stmp_; }); _tmp; });
} else {
__if_res_1 = rt_option_none();
}
__if_res_1; });
});
}

