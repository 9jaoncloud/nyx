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
#include "runtime/rt_concurrency.h"
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
void nyx_sha256(int64_t* p0, int64_t p1, int64_t* p2); // extern
void nyx_sha512(int64_t* p0, int64_t p1, int64_t* p2); // extern
void nyx_md5(int64_t* p0, int64_t p1, int64_t* p2); // extern
void nyx_hmac_sha256(int64_t* p0, int64_t p1, int64_t* p2, int64_t p3, int64_t* p4); // extern
void nyx_random_bytes(int64_t* p0, int64_t p1); // extern
int64_t nyx_aes_encrypt(int64_t* p0, int64_t p1, int64_t* p2, int64_t p3, int64_t* p4); // extern
int64_t nyx_aes_decrypt(int64_t* p0, int64_t p1, int64_t* p2, int64_t p3, int64_t* p4); // extern
NyxVec sha256(rt_string_t data) {
int64_t out[32LL] = { 0LL };
nyx_sha256(((char*)(data.data)), (int64_t)(data.length), (&((*(int64_t*)rt_vec_get(out, 0LL)))));
return Vec_from_slice(out);
}

NyxVec sha256_bytes(NyxVec data) {
int64_t out[32LL] = { 0LL };
nyx_sha256(data, (int64_t)(data.len), (&((*(int64_t*)rt_vec_get(out, 0LL)))));
return Vec_from_slice(out);
}

NyxVec sha512(rt_string_t data) {
int64_t out[64LL] = { 0LL };
nyx_sha512(((char*)(data.data)), (int64_t)(data.length), (&((*(int64_t*)rt_vec_get(out, 0LL)))));
return Vec_from_slice(out);
}

NyxVec md5(rt_string_t data) {
int64_t out[16LL] = { 0LL };
nyx_md5(((char*)(data.data)), (int64_t)(data.length), (&((*(int64_t*)rt_vec_get(out, 0LL)))));
return Vec_from_slice(out);
}

NyxVec hmac_sha256(rt_string_t key, rt_string_t data) {
int64_t out[32LL] = { 0LL };
nyx_hmac_sha256(((char*)(key.data)), (int64_t)(key.length), ((char*)(data.data)), (int64_t)(data.length), (&((*(int64_t*)rt_vec_get(out, 0LL)))));
return Vec_from_slice(out);
}

NyxVec random_bytes(int64_t count) {
int64_t buf[256LL] = { 0LL };
int64_t n = ({ int64_t __if_res_1; if ((count > 256LL)) {
__if_res_1 = 256LL;
} else {
__if_res_1 = count;
}
__if_res_1; });
nyx_random_bytes((&((*(int64_t*)rt_vec_get(buf, 0LL)))), n);
return Vec_from_slice(/* slice */ 0);
}

int64_t random_int(int64_t min, int64_t max) {
NyxVec bytes = random_bytes(8LL);
int64_t val = (((((((((int64_t)((*(int64_t*)rt_vec_get(bytes, 0LL))) << 56LL) | ((int64_t)((*(int64_t*)rt_vec_get(bytes, 1LL))) << 48LL)) | ((int64_t)((*(int64_t*)rt_vec_get(bytes, 2LL))) << 40LL)) | ((int64_t)((*(int64_t*)rt_vec_get(bytes, 3LL))) << 32LL)) | ((int64_t)((*(int64_t*)rt_vec_get(bytes, 4LL))) << 24LL)) | ((int64_t)((*(int64_t*)rt_vec_get(bytes, 5LL))) << 16LL)) | ((int64_t)((*(int64_t*)rt_vec_get(bytes, 6LL))) << 8LL)) | (int64_t)((*(int64_t*)rt_vec_get(bytes, 7LL))));
return (min + (abs(val) % ((max - min) + 1LL)));
}

rt_string_t sha256_hex(rt_string_t data) {
NyxVec hash = sha256(data);
return bytes_to_hex(hash);
}

rt_string_t md5_hex(rt_string_t data) {
NyxVec hash = md5(data);
return bytes_to_hex(hash);
}

rt_string_t bytes_to_hex(NyxVec bytes) {
rt_string_t hex_chars = rt_string_from("0123456789abcdef");
rt_string_t result = rt_string_from("");
for (int64_t _b = 0; _b < bytes.len; _b++) {
    int64_t b = *(int64_t*)rt_vec_get(bytes, _b);
char hi = hex_chars.data[(int64_t)((b >> 4LL))];
char lo = hex_chars.data[(int64_t)((b & 15LL))];
result = rt_string_concat(rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)int64_t_from_char(String, hi)); _s.ref_count = 1; _s; })), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)int64_t_from_char(String, lo)); _s.ref_count = 1; _s; }));
}
;
return result;
}

NyxResult hex_to_bytes(rt_string_t hex) {
({ if (((hex.length % 2LL) != 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Hex string must have even length"); _stmp_.data; }); _tmp; });
}
});
NyxVec result = rt_vec_new(sizeof(int64_t));
int64_t i = 0LL;
while ((i < hex.length)) {
int64_t hi = ({ NyxResult _qres = hex_char_to_nibble(rt_string_char_at(hex, i)); if (_qres.tag == RESULT_ERR) { return _qres; } (int64_t)_qres.data.int_val; });
int64_t lo = ({ NyxResult _qres = hex_char_to_nibble(rt_string_char_at(hex, (i + 1LL))); if (_qres.tag == RESULT_ERR) { return _qres; } (int64_t)_qres.data.int_val; });
rt_vec_push(&result, ({ int64_t _tmp_2 = ((hi << 4LL) | lo); &_tmp_2; }));
i = (i + 2LL);
}
;
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = result; &_stmp_; }); _tmp; });
}

NyxResult hex_char_to_nibble(char ch) {
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
return sha256_hex(rt_string_concat(rt_string_concat(salt, password), salt));
}

int verify_password(rt_string_t password, rt_string_t salt, rt_string_t expected_hash) {
return rt_string_eq(hash_password(password, salt), expected_hash);
}

