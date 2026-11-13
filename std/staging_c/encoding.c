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

rt_string_t base64_encode(NyxVec);
NyxVec base64_decode(rt_string_t);
rt_string_t hex_encode(NyxVec);
NyxVec hex_decode(rt_string_t);
int utf8_validate(rt_string_t);
char char_at_base64(int64_t);
int64_t base64_val(char);
int64_t hex_val(char);
// module encoding
extern void* malloc(size_t);
extern void free(void*);
extern size_t strlen(const char*);
extern void* memcpy(void*, const void*, size_t);
rt_string_t base64_encode(NyxVec data) {
Arena* __arena = arena_create();
int64_t len = data.len;
({ if ((len == 0LL)) {
return rt_string_from("");
}
});
rt_string_t result = rt_string_from("");
int64_t i = 0LL;
while ((i < len)) {
int64_t b0 = (int64_t)(rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(data, (size_t)(i)); _u.data.ptr_val; })));
int64_t b1 = ({ int64_t __if_res_2; if (((i + 1LL) < len)) {
__if_res_2 = (int64_t)(rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(data, (size_t)((i + 1LL))); _u.data.ptr_val; })));
} else {
__if_res_2 = 0LL;
}
__if_res_2; });
int64_t b2 = ({ int64_t __if_res_3; if (((i + 2LL) < len)) {
__if_res_3 = (int64_t)(rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(data, (size_t)((i + 2LL))); _u.data.ptr_val; })));
} else {
__if_res_3 = 0LL;
}
__if_res_3; });
int64_t triple = (((b0 << 16LL) | (b1 << 8LL)) | b2);
result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)char_at_base64(((triple >> 18LL) & 63LL))); _s.ref_count = 1; _s; }));
result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)char_at_base64(((triple >> 12LL) & 63LL))); _s.ref_count = 1; _s; }));
({ rt_string_t __if_res_4; if (((i + 1LL) < len)) {
__if_res_4 = (result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)char_at_base64(((triple >> 6LL) & 63LL))); _s.ref_count = 1; _s; })));
} else {
__if_res_4 = (result = rt_string_concat(result, rt_string_from("=")));
}
__if_res_4; });
({ rt_string_t __if_res_5; if (((i + 2LL) < len)) {
__if_res_5 = (result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)char_at_base64((triple & 63LL))); _s.ref_count = 1; _s; })));
} else {
__if_res_5 = (result = rt_string_concat(result, rt_string_from("=")));
}
__if_res_5; });
i = (i + 3LL);
}
;
arena_destroy(__arena);
return result;
}

NyxVec base64_decode(rt_string_t s) {
Arena* __arena = arena_create();
NyxVec result = rt_vec_new(sizeof(int64_t));
int64_t slen = rt_string_len(s);
int64_t i = 0LL;
while ((i < slen)) {
int64_t c0 = base64_val(char_at(s, i));
int64_t c1 = ({ int64_t __if_res_6; if (((i + 1LL) < slen)) {
__if_res_6 = base64_val(char_at(s, (i + 1LL)));
} else {
__if_res_6 = 0LL;
}
__if_res_6; });
int64_t c2 = ({ int64_t __if_res_7; if ((((i + 2LL) < slen) && (char_at(s, (i + 2LL)) != '='))) {
__if_res_7 = base64_val(char_at(s, (i + 2LL)));
} else {
__if_res_7 = 0LL;
}
__if_res_7; });
int64_t c3 = ({ int64_t __if_res_8; if ((((i + 3LL) < slen) && (char_at(s, (i + 3LL)) != '='))) {
__if_res_8 = base64_val(char_at(s, (i + 3LL)));
} else {
__if_res_8 = 0LL;
}
__if_res_8; });
int64_t triple = ((((c0 << 18LL) | (c1 << 12LL)) | (c2 << 6LL)) | c3);
rt_vec_push(&result, ({ int64_t _tmp_8 = (int64_t)((int64_t)(((triple >> 16LL) & 255LL))); &_tmp_8; }));
({ if ((((i + 2LL) < slen) && (char_at(s, (i + 2LL)) != '='))) {
rt_vec_push(&result, ({ int64_t _tmp_10 = (int64_t)((int64_t)(((triple >> 8LL) & 255LL))); &_tmp_10; }));
}
});
({ if ((((i + 3LL) < slen) && (char_at(s, (i + 3LL)) != '='))) {
rt_vec_push(&result, ({ int64_t _tmp_12 = (int64_t)((int64_t)((triple & 255LL))); &_tmp_12; }));
}
});
i = (i + 4LL);
}
;
arena_destroy(__arena);
return result;
}

rt_string_t hex_encode(NyxVec data) {
Arena* __arena = arena_create();
rt_string_t result = rt_string_from("");
rt_string_t hex_chars = rt_string_from("0123456789abcdef");
for (int64_t i = 0LL; i < data.len; i++) {
int64_t byte = (int64_t)(rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(data, (size_t)(i)); _u.data.ptr_val; })));
int64_t hi = ((byte >> 4LL) & 15LL);
int64_t lo = (byte & 15LL);
result = rt_string_concat(rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)char_at(hex_chars, hi)); _s.ref_count = 1; _s; })), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)char_at(hex_chars, lo)); _s.ref_count = 1; _s; }));
}
;
arena_destroy(__arena);
return result;
}

NyxVec hex_decode(rt_string_t s) {
Arena* __arena = arena_create();
NyxVec result = rt_vec_new(sizeof(int64_t));
int64_t slen = rt_string_len(s);
int64_t i = 0LL;
while (((i + 1LL) < slen)) {
int64_t hi = hex_val(char_at(s, i));
int64_t lo = hex_val(char_at(s, (i + 1LL)));
rt_vec_push(&result, ({ int64_t _tmp_13 = (int64_t)((int64_t)(((hi << 4LL) | lo))); &_tmp_13; }));
i = (i + 2LL);
}
;
arena_destroy(__arena);
return result;
}

int utf8_validate(rt_string_t s) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return 1;
}

char char_at_base64(int64_t index) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if (((index >= 0LL) && (index < 64LL))) {
return char_at(BASE64_CHARS, index);
} else {
return '=';
}
}

int64_t base64_val(char c) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if (((c >= 'A') && (c <= 'Z'))) {
return ((int64_t)(c) - (int64_t)('A'));
} else if (((c >= 'a') && (c <= 'z'))) {
return (((int64_t)(c) - (int64_t)('a')) + 26LL);
} else if (((c >= '0') && (c <= '9'))) {
return (((int64_t)(c) - (int64_t)('0')) + 52LL);
} else if ((c == '+')) {
return 62LL;
} else if ((c == '/')) {
return 63LL;
} else {
return 0LL;
}
}

int64_t hex_val(char c) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if (((c >= '0') && (c <= '9'))) {
return ((int64_t)(c) - (int64_t)('0'));
} else if (((c >= 'a') && (c <= 'f'))) {
return (((int64_t)(c) - (int64_t)('a')) + 10LL);
} else if (((c >= 'A') && (c <= 'F'))) {
return (((int64_t)(c) - (int64_t)('A')) + 10LL);
} else {
return 0LL;
}
}

