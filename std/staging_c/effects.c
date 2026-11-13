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

#define TAG_EffectType_Yield 0
#define TAG_EffectType_AsyncAwait 1
#define TAG_EffectType_BargeIn 2
#define TAG_EffectType_SwarmMsg 3
#define TAG_EffectType_FailRecover 4
typedef struct {
    int tag;
    union {
    } data;
} EffectType;

typedef struct {
    EffectType effect_type;
    int64_t payload;
    int is_handled;
} EffectSignal;

typedef struct {
    int64_t depth;
    int is_active;
} EffectHandler;

EffectHandler EffectHandler_create();
void EffectHandler_perform(EffectHandler*, EffectType, int64_t);
void EffectHandler_resume(EffectHandler*, int64_t);
// module effects
// import prelude::*
EffectHandler EffectHandler_create() {
return (EffectHandler){.depth = (int64_t)(0LL), .is_active = 1};
}

void EffectHandler_perform(EffectHandler* self, EffectType effect, int64_t payload) {
}

void EffectHandler_resume(EffectHandler* self, int64_t value) {
}

