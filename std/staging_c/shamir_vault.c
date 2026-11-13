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
    int64_t index;
    NyxVec share_data;
} ShamirShare;

typedef struct {
    rt_string_t serial_number;
    int is_fido2_present;
    int64_t slot_id;
} HardwareSecurityToken;

typedef struct {
    int64_t threshold_k;
    int64_t total_shares_n;
} SovereignVault;

HardwareSecurityToken HardwareSecurityToken_new(rt_string_t);
NyxVec HardwareSecurityToken_challenge_response(HardwareSecurityToken*, NyxVec*);
SovereignVault SovereignVault_new(int64_t, int64_t);
NyxVec SovereignVault_split_secret(SovereignVault*, NyxVec*);
NyxResult SovereignVault_reconstruct_secret(SovereignVault*, NyxVec*);
// module std.sec.shamir_vault
// import std::crypto
// import std::io
HardwareSecurityToken HardwareSecurityToken_new(rt_string_t serial) {
return (HardwareSecurityToken){.serial_number = serial, .is_fido2_present = 1, .slot_id = 1LL};
}

NyxVec HardwareSecurityToken_challenge_response(HardwareSecurityToken* self, NyxVec* challenge) {
NyxVec resp = rt_vec_new(sizeof(int64_t));
for (int64_t _b = 0; _b < rt_vec_iter(challenge).len; _b++) {
    int64_t b = *(int64_t*)rt_vec_get(rt_vec_iter(challenge), _b);
rt_vec_push(&resp, ({ int64_t _tmp_0 = (int64_t)((b ^ 165LL)); &_tmp_0; }));
}
;
return resp;
}

SovereignVault SovereignVault_new(int64_t k, int64_t n) {
return (SovereignVault){.threshold_k = k, .total_shares_n = n};
}

NyxVec SovereignVault_split_secret(SovereignVault* self, NyxVec* secret) {
NyxVec shares = rt_vec_new(sizeof(ShamirShare));
for (int64_t i = 1LL; i <= (*self).total_shares_n; i++) {
NyxVec share_bytes = rt_vec_new(sizeof(ShamirShare));
for (int64_t _idx_idx_byte = 0; _idx_idx_byte < int64_t_enumerate(rt_vec_iter(secret)).len; _idx_idx_byte++) {
    NyxTuple2 _tup_idx_byte = *(NyxTuple2*)rt_vec_get(int64_t_enumerate(rt_vec_iter(secret)), _idx_idx_byte);
    int64_t idx = (int64_t)(_tup_idx_byte.f0);
    int64_t byte = (int64_t)(_tup_idx_byte.f1);
int64_t coef = ((((int64_t)(idx) + 7LL) * 13LL) % 251LL);
int64_t val = ((byte + (coef * i)) % 256LL);
rt_vec_push(&share_bytes, ({ int64_t _tmp_1 = (int64_t)(val); &_tmp_1; }));
}
;
rt_vec_push(&shares, ({ ShamirShare _tmp_2 = (int64_t)((ShamirShare){.index = i, .share_data = share_bytes}); &_tmp_2; }));
}
;
return shares;
}

NyxResult SovereignVault_reconstruct_secret(SovereignVault* self, NyxVec* collected_shares) {
({ if ((collected_shares->len < (int64_t)((*self).threshold_k))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Insufficient shares for threshold reconstruction"); _stmp_.data; }); _tmp; });
}
});
int64_t secret_len = ((*(int64_t*)rt_vec_get(*collected_shares, 0LL))).share_data.len;
NyxVec recovered_secret = rt_vec_new(sizeof(int64_t));
for (int64_t idx = 0LL; idx < secret_len; idx++) {
int64_t s1 = (int64_t)((*(int64_t*)rt_vec_get(((*(int64_t*)rt_vec_get(*collected_shares, 0LL))).share_data, idx)));
int64_t s2 = (int64_t)((*(int64_t*)rt_vec_get(((*(int64_t*)rt_vec_get(*collected_shares, 1LL))).share_data, idx)));
int64_t x1 = (int64_t)(((*(int64_t*)rt_vec_get(*collected_shares, 0LL))).index);
int64_t x2 = (int64_t)(((*(int64_t*)rt_vec_get(*collected_shares, 1LL))).index);
int64_t l1 = ((0LL - x2) / (x1 - x2));
int64_t l2 = ((0LL - x1) / (x2 - x1));
int64_t secret_val = (((s1 * l1) + (s2 * l2)) % 256LL);
int64_t normalized = ({ int64_t __if_res_5; if ((secret_val < 0LL)) {
__if_res_5 = (int64_t)((secret_val + 256LL));
} else {
__if_res_5 = (int64_t)(secret_val);
}
__if_res_5; });
rt_vec_push(&recovered_secret, ({ int64_t _tmp_5 = (int64_t)(normalized); &_tmp_5; }));
}
;
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = recovered_secret; &_stmp_; }); _tmp; });
}

