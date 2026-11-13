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
    NyxVec encrypted_payload;
    int64_t polynomial_iv;
    int64_t tensor_dimension;
    int64_t block_checksum;
} CloakedTensorBlock;

typedef struct {
    NyxVec master_key;
    int64_t seed;
} SecretShield;

SecretShield SecretShield_new(NyxVec, int64_t);
int SecretShield_cloak_payload(SecretShield*, NyxVec*, NyxVec*, int64_t);
int SecretShield_decloak_jit(SecretShield*, NyxVec*, NyxVec*, int64_t);
int64_t SecretShield_compute_cloaked_gemm_step(SecretShield*, NyxVec*, NyxVec*, int64_t);
// module secret_shield
// import prelude::*
// import crypto::*
// import sec::*
SecretShield SecretShield_new(NyxVec key, int64_t seed) {
return (SecretShield){.master_key = key, .seed = seed};
}

int SecretShield_cloak_payload(SecretShield* self, NyxVec* plaintext, NyxVec* out_buffer, int64_t salt) {
int64_t len = plaintext->len;
int64_t i = 0LL;
while ((i < len)) {
int64_t key_byte = (*(int64_t*)rt_vec_get((*self).master_key, (i % 32LL)));
int64_t dynamic_mask = ((int64_t)((salt >> (i % 8LL))) ^ key_byte);
int64_t byte_val = (*(int64_t*)rt_vec_get(*plaintext, i));
int64_t cloaked_byte = (byte_val ^ dynamic_mask);
rt_vec_push(&out_buffer, ({ int64_t _tmp_0 = (int64_t)(cloaked_byte); &_tmp_0; }));
i = (i + 1LL);
}
;
return 1;
}

int SecretShield_decloak_jit(SecretShield* self, NyxVec* cloaked, NyxVec* plain_dest, int64_t salt) {
int64_t len = cloaked->len;
int64_t i = 0LL;
while ((i < len)) {
int64_t key_byte = (*(int64_t*)rt_vec_get((*self).master_key, (i % 32LL)));
int64_t dynamic_mask = ((int64_t)((salt >> (i % 8LL))) ^ key_byte);
int64_t cloaked_byte = (*(int64_t*)rt_vec_get(*cloaked, i));
rt_vec_push(&plain_dest, ({ int64_t _tmp_1 = (int64_t)((cloaked_byte ^ dynamic_mask)); &_tmp_1; }));
i = (i + 1LL);
}
;
return 1;
}

int64_t SecretShield_compute_cloaked_gemm_step(SecretShield* self, NyxVec* cloaked_weights, NyxVec* activations, int64_t salt) {
int64_t dot_product = 0LL;
int64_t len = ({ int64_t __if_res_3; if ((cloaked_weights->len < 64LL)) {
__if_res_3 = cloaked_weights->len;
} else {
__if_res_3 = 64LL;
}
__if_res_3; });
int64_t i = 0LL;
while ((i < len)) {
({ if ((i >= activations->len)) {
break;
}
});
int64_t key_byte = (*(int64_t*)rt_vec_get((*self).master_key, (i % 32LL)));
int64_t dynamic_mask = ((int64_t)((salt >> (i % 8LL))) ^ key_byte);
int64_t weight_val = (int64_t)(((*(int64_t*)rt_vec_get(*cloaked_weights, i)) ^ dynamic_mask));
dot_product = (dot_product + ((int64_t)(weight_val) * (int64_t)((*(int64_t*)rt_vec_get(*activations, i)))));
i = (i + 1LL);
}
;
return dot_product;
}

