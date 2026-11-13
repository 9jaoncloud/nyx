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

#define TAG_VoiceState_Idle 0
#define TAG_VoiceState_Listening 1
#define TAG_VoiceState_Thinking 2
#define TAG_VoiceState_Speaking 3
#define TAG_VoiceState_Interrupted 4
typedef struct {
    int tag;
    union {
    } data;
} VoiceState;

#define TAG_VoiceLanguage_English 0
#define TAG_VoiceLanguage_NigerianPidgin 1
#define TAG_VoiceLanguage_Hausa 2
#define TAG_VoiceLanguage_Yoruba 3
#define TAG_VoiceLanguage_Igbo 4
typedef struct {
    int tag;
    union {
    } data;
} VoiceLanguage;

typedef struct {
    rt_string_t name;
    VoiceLanguage language;
    VoiceState state;
    int barge_in_enabled;
} VoiceAgent;

VoiceAgent VoiceAgent_create(rt_string_t, VoiceLanguage);
int VoiceAgent_process_audio_frame(VoiceAgent*, NyxVec);
int VoiceAgent_speak(VoiceAgent*, rt_string_t);
// module voice_agent
VoiceAgent VoiceAgent_create(rt_string_t name, VoiceLanguage lang) {
return (VoiceAgent){.name = name, .language = lang, .state = (VoiceState){ .tag = TAG_VoiceState_Idle }, .barge_in_enabled = 1};
}

int VoiceAgent_process_audio_frame(VoiceAgent* self, NyxVec pcm_data) {
return 1;
}

int VoiceAgent_speak(VoiceAgent* self, rt_string_t text) {
return 1;
}

