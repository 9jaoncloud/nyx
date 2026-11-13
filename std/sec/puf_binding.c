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
    int64_t* raw_entropy;
    int64_t* helper_data;
    int64_t confidence_score;
    int is_valid;
} PufFingerprint;

typedef struct {
    int64_t target_architecture;
    int64_t chip_vendor_id;
    int64_t* master_salt;
} PufBindingEngine;

// import std::crypto
// import std::math
PufBindingEngine PufBindingEngine_new(int64_t arch, int64_t vendor, NyxVec* salt) {
uint8_t s[32LL] = { 0LL };
int64_t i = 0LL;
int64_t limit = ({ int64_t __if_res_1; if ((salt->len < 32LL)) {
__if_res_1 = salt->len;
} else {
__if_res_1 = 32LL;
}
__if_res_1; });
while ((i < limit)) {
(s[i] = (*(int64_t*)rt_vec_get(*salt, i)));
i = (i + 1LL);
}
;
return (PufBindingEngine){.target_architecture = arch, .chip_vendor_id = vendor, .master_salt = s};
}

PufFingerprint PufBindingEngine_enroll_sram_puf(PufBindingEngine* self, NyxVec* sram_startup_sample) {
int64_t len = sram_startup_sample->len;
uint8_t entropy_accum[32LL] = { 0LL };
uint8_t helper[32LL] = { 0LL };
int64_t i = 0LL;
while ((i < len)) {
int64_t idx = (i % 32LL);
int64_t byte_val = (*(int64_t*)rt_vec_get(*sram_startup_sample, i));
(entropy_accum[idx] = (entropy_accum[idx] ^ int64_t_rotate_left(byte_val, (int64_t)((i % 7LL)))));
(helper[idx] = (helper[idx] ^ byte_val));
i = (i + 1LL);
}
;
crypto whitened_key = crypto_sha256((&(entropy_accum)));
return (PufFingerprint){.raw_entropy = whitened_key, .helper_data = helper, .confidence_score = 98LL, .is_valid = 1};
}

NyxVec PufBindingEngine_reconstruct_key(PufBindingEngine* self, NyxVec* runtime_sample, NyxVec* helper) {
int64_t len = runtime_sample->len;
uint8_t corrected[32LL] = { 0LL };
int64_t i = 0LL;
while ((i < len)) {
int64_t idx = (i % 32LL);
int64_t sample_byte = (*(int64_t*)rt_vec_get(*runtime_sample, i));
NyxVec helper_byte = helper[idx];
int64_t syndrome = (sample_byte ^ helper_byte);
(corrected[idx] = (corrected[idx] ^ int64_t_rotate_left(syndrome, (int64_t)((i % 7LL)))));
i = (i + 1LL);
}
;
return crypto_sha256((&(corrected)));
}

NyxVec PufBindingEngine_derive_device_bound_key(PufBindingEngine* self, NyxVec* puf_key, NyxVec* app_guid) {
uint8_t context_buffer[96LL] = { 0LL };
int64_t i = 0LL;
while ((i < 32LL)) {
(context_buffer[i] = (*(int64_t*)rt_vec_get(*puf_key, i)));
(context_buffer[(32LL + i)] = (*(int64_t*)rt_vec_get((*self).master_salt, i)));
(context_buffer[(64LL + i)] = ({ int64_t __if_res_2; if ((i < app_guid->len)) {
__if_res_2 = (*(int64_t*)rt_vec_get(*app_guid, i));
} else {
__if_res_2 = 0LL;
}
__if_res_2; }));
i = (i + 1LL);
}
;
return crypto_sha256((&(context_buffer)));
}

int PufBindingEngine_verify_silicon_identity(PufBindingEngine* self, NyxVec* candidate_key, NyxVec* golden_digest) {
crypto candidate_digest = crypto_sha256(candidate_key);
int64_t diff = 0LL;
int64_t i = 0LL;
while ((i < 32LL)) {
diff = (diff | (candidate_digest[i] ^ (*(int64_t*)rt_vec_get(*golden_digest, i))));
i = (i + 1LL);
}
;
return (diff == 0LL);
}

