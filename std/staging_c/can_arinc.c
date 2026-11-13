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

typedef struct {
    int64_t raw_word;
    int64_t label_octal;
    int64_t sdi;
    int64_t ssm;
    int parity_valid;
} Arinc429Word;

typedef struct {
    int64_t message_id;
    int64_t node_id;
    int64_t data_type;
    int64_t service_code;
    int64_t message_code;
    int64_t* payload_bytes;
} CanAerospaceMessage;

Arinc429Word decode_arinc429(int64_t);
int64_t encode_arinc429_bnr(int64_t, int64_t, int64_t, int64_t);
CanAerospaceMessage build_can_aerospace_frame(int64_t, int64_t, int64_t, int64_t, NyxVec);
// import std::core::arena
// import std::core::strict
Arinc429Word decode_arinc429(int64_t raw) {
Arena* __arena = arena_create();
int64_t label = (int64_t)((raw & 255LL));
int64_t sdi = (int64_t)(((raw >> 8LL) & 3LL));
int64_t ssm = (int64_t)(((raw >> 29LL) & 3LL));
int64_t bits_set = 0LL;
int64_t temp = raw;
while ((temp > 0LL)) {
bits_set = (bits_set + (temp & 1LL));
temp = (temp >> 1LL);
}
;
int parity_ok = ((bits_set % 2LL) != 0LL);
arena_destroy(__arena);
return (Arinc429Word){.raw_word = raw, .label_octal = label, .sdi = sdi, .ssm = ssm, .parity_valid = parity_ok};
}

int64_t encode_arinc429_bnr(int64_t label_octal, int64_t sdi, int64_t value_int19, int64_t ssm) {
Arena* __arena = arena_create();
int64_t word = ((int64_t)(label_octal) & 255LL);
word = (word | (((int64_t)(sdi) & 3LL) << 8LL));
word = (word | (((int64_t)(value_int19) & 524287LL) << 10LL));
word = (word | (((int64_t)(ssm) & 3LL) << 29LL));
int64_t count = 0LL;
int64_t temp = word;
while ((temp > 0LL)) {
count = (count + (temp & 1LL));
temp = (temp >> 1LL);
}
;
({ if (((count % 2LL) == 0LL)) {
word = (word | 2147483648LL);
}
});
arena_destroy(__arena);
return word;
}

CanAerospaceMessage build_can_aerospace_frame(int64_t node_id, int64_t data_type, int64_t service_code, int64_t message_code, NyxVec data) {
Arena* __arena = arena_create();
int64_t can_id = (768LL + (int64_t)(node_id));
NyxVec payload = ({ NyxVec _v = rt_vec_new(sizeof(int64_t)); int64_t _ve1_0 = node_id; rt_vec_push(&_v, &_ve1_0); int64_t _ve1_1 = data_type; rt_vec_push(&_v, &_ve1_1); int64_t _ve1_2 = service_code; rt_vec_push(&_v, &_ve1_2); int64_t _ve1_3 = message_code; rt_vec_push(&_v, &_ve1_3); int64_t _ve1_4 = (*(int64_t*)rt_vec_get(data, 0LL)); rt_vec_push(&_v, &_ve1_4); int64_t _ve1_5 = (*(int64_t*)rt_vec_get(data, 1LL)); rt_vec_push(&_v, &_ve1_5); int64_t _ve1_6 = (*(int64_t*)rt_vec_get(data, 2LL)); rt_vec_push(&_v, &_ve1_6); int64_t _ve1_7 = (*(int64_t*)rt_vec_get(data, 3LL)); rt_vec_push(&_v, &_ve1_7); _v; });
arena_destroy(__arena);
return (CanAerospaceMessage){.message_id = can_id, .node_id = node_id, .data_type = data_type, .service_code = service_code, .message_code = message_code, .payload_bytes = payload};
}

