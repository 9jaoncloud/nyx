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

NyxVec sha256(rt_string_t);
NyxVec sha256_bytes(NyxVec);
NyxVec sha512(rt_string_t);
NyxVec md5(rt_string_t);
NyxVec hmac_sha256(rt_string_t, rt_string_t);
NyxVec random_bytes(int64_t);
int64_t random_int(int64_t, int64_t);
rt_string_t sha256_hex(rt_string_t);
rt_string_t md5_hex(rt_string_t);
rt_string_t bytes_to_hex(NyxVec);
NyxResult hex_to_bytes(rt_string_t);
NyxResult hex_char_to_nibble(char);
rt_string_t hash_password(rt_string_t, rt_string_t);
int verify_password(rt_string_t, rt_string_t, rt_string_t);
// module crypto
// extern nyx_sha256
// extern nyx_sha512
// extern nyx_md5
// extern nyx_hmac_sha256
// extern nyx_random_bytes
// extern nyx_aes_encrypt
// extern nyx_aes_decrypt
NyxVec sha256(rt_string_t data) {
Arena* __arena = arena_create();
uint8_t out[32LL] = { 0LL };
nyx_sha256(((const uint8_t*)(data.data)), (int64_t)(data.length), (&(out[0LL])));
NyxVec result = rt_vec_new(sizeof(int64_t));
for (int64_t i = 0LL; i < 32LL; i++) {
rt_vec_push(&result, ({ int64_t _tmp_0 = (int64_t)(out[i]); &_tmp_0; }));
}
;
arena_destroy(__arena);
return result;
}

NyxVec sha256_bytes(NyxVec data) {
Arena* __arena = arena_create();
uint8_t out[32LL] = { 0LL };
nyx_sha256(((const uint8_t*)(data.data)), (int64_t)(data.len), (&(out[0LL])));
NyxVec result = rt_vec_new(sizeof(int64_t));
for (int64_t i = 0LL; i < 32LL; i++) {
rt_vec_push(&result, ({ int64_t _tmp_1 = (int64_t)(out[i]); &_tmp_1; }));
}
;
arena_destroy(__arena);
return result;
}

NyxVec sha512(rt_string_t data) {
Arena* __arena = arena_create();
uint8_t out[64LL] = { 0LL };
nyx_sha512(((const uint8_t*)(data.data)), (int64_t)(data.length), (&(out[0LL])));
NyxVec result = rt_vec_new(sizeof(int64_t));
for (int64_t i = 0LL; i < 64LL; i++) {
rt_vec_push(&result, ({ int64_t _tmp_2 = (int64_t)(out[i]); &_tmp_2; }));
}
;
arena_destroy(__arena);
return result;
}

NyxVec md5(rt_string_t data) {
Arena* __arena = arena_create();
uint8_t out[16LL] = { 0LL };
nyx_md5(((const uint8_t*)(data.data)), (int64_t)(data.length), (&(out[0LL])));
NyxVec result = rt_vec_new(sizeof(int64_t));
for (int64_t i = 0LL; i < 16LL; i++) {
rt_vec_push(&result, ({ int64_t _tmp_3 = (int64_t)(out[i]); &_tmp_3; }));
}
;
arena_destroy(__arena);
return result;
}

NyxVec hmac_sha256(rt_string_t key, rt_string_t data) {
Arena* __arena = arena_create();
uint8_t out[32LL] = { 0LL };
nyx_hmac_sha256(((const uint8_t*)(key.data)), (int64_t)(key.length), ((const uint8_t*)(data.data)), (int64_t)(data.length), (&(out[0LL])));
NyxVec result = rt_vec_new(sizeof(int64_t));
for (int64_t i = 0LL; i < 32LL; i++) {
rt_vec_push(&result, ({ int64_t _tmp_4 = (int64_t)(out[i]); &_tmp_4; }));
}
;
arena_destroy(__arena);
return result;
}

NyxVec random_bytes(int64_t count) {
Arena* __arena = arena_create();
uint8_t buf[256LL] = { 0LL };
int64_t n = ({ int64_t __if_res_6; if ((count > 256LL)) {
__if_res_6 = 256LL;
} else {
__if_res_6 = count;
}
__if_res_6; });
nyx_random_bytes((&(buf[0LL])), n);
NyxVec result = rt_vec_new(sizeof(int64_t));
for (int64_t i = 0LL; i < n; i++) {
rt_vec_push(&result, ({ int64_t _tmp_6 = (int64_t)(buf[i]); &_tmp_6; }));
}
;
arena_destroy(__arena);
return result;
}

int64_t random_int(int64_t min, int64_t max) {
Arena* __arena = arena_create();
NyxVec bytes = random_bytes(8LL);
int64_t val = (((((((((int64_t)((*(int64_t*)rt_vec_get(bytes, 0LL))) << 56LL) | ((int64_t)((*(int64_t*)rt_vec_get(bytes, 1LL))) << 48LL)) | ((int64_t)((*(int64_t*)rt_vec_get(bytes, 2LL))) << 40LL)) | ((int64_t)((*(int64_t*)rt_vec_get(bytes, 3LL))) << 32LL)) | ((int64_t)((*(int64_t*)rt_vec_get(bytes, 4LL))) << 24LL)) | ((int64_t)((*(int64_t*)rt_vec_get(bytes, 5LL))) << 16LL)) | ((int64_t)((*(int64_t*)rt_vec_get(bytes, 6LL))) << 8LL)) | (int64_t)((*(int64_t*)rt_vec_get(bytes, 7LL))));
arena_destroy(__arena);
return (min + (abs(val) % ((max - min) + 1LL)));
}

rt_string_t sha256_hex(rt_string_t data) {
Arena* __arena = arena_create();
NyxVec hash = sha256(data);
arena_destroy(__arena);
return bytes_to_hex(hash);
}

rt_string_t md5_hex(rt_string_t data) {
Arena* __arena = arena_create();
NyxVec hash = md5(data);
arena_destroy(__arena);
return bytes_to_hex(hash);
}

rt_string_t bytes_to_hex(NyxVec bytes) {
Arena* __arena = arena_create();
rt_string_t hex_chars = rt_string_from("0123456789abcdef");
rt_string_t result = rt_string_from("");
for (int64_t _b = 0; _b < bytes.len; _b++) {
    int64_t b = *(int64_t*)rt_vec_get(bytes, _b);
char hi = hex_chars.data[(int64_t)((b >> 4LL))];
char lo = hex_chars.data[(int64_t)((b & 15LL))];
result = rt_string_concat(rt_string_concat(result, rt_string_from_char((char)(hi))), rt_string_from_char((char)(lo)));
}
;
arena_destroy(__arena);
return result;
}

NyxResult hex_to_bytes(rt_string_t hex) {
Arena* __arena = arena_create();
({ if (((hex.length % 2LL) != 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Hex string must have even length"); _stmp_.data; }); _tmp; });
}
});
NyxVec result = rt_vec_new(sizeof(int64_t));
int64_t i = 0LL;
while ((i < hex.length)) {
int64_t hi = ({ NyxResult _qres = hex_char_to_nibble(rt_string_char_at(hex, i)); if (_qres.tag == RESULT_ERR) { return _qres; } (int64_t)_qres.data.int_val; });
int64_t lo = ({ NyxResult _qres = hex_char_to_nibble(rt_string_char_at(hex, (i + 1LL))); if (_qres.tag == RESULT_ERR) { return _qres; } (int64_t)_qres.data.int_val; });
rt_vec_push(&result, ({ int64_t _tmp_8 = (int64_t)(((hi << 4LL) | lo)); &_tmp_8; }));
i = (i + 2LL);
}
;
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = result; &_stmp_; }); _tmp; });
}

NyxResult hex_char_to_nibble(char ch) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if (((ch >= '0') && (ch <= '9'))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(((int64_t)(ch) - 48LL)); _tmp; });
} else if (((ch >= 'a') && (ch <= 'f'))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)((((int64_t)(ch) - 97LL) + 10LL)); _tmp; });
} else if (((ch >= 'A') && (ch <= 'F'))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)((((int64_t)(ch) - 65LL) + 10LL)); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Invalid hex character"); _stmp_.data; }); _tmp; });
}
}

rt_string_t hash_password(rt_string_t password, rt_string_t salt) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return sha256_hex(rt_string_concat(rt_string_concat(salt, password), salt));
}

int verify_password(rt_string_t password, rt_string_t salt, rt_string_t expected_hash) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_eq(hash_password(password, salt), expected_hash);
}

