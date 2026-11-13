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

rt_string_t new();
rt_string_t from(rt_string_t);
int64_t len(rt_string_t);
int is_empty(rt_string_t);
int contains(rt_string_t, rt_string_t);
int starts_with(rt_string_t, rt_string_t);
int ends_with(rt_string_t, rt_string_t);
NyxOption find(rt_string_t, rt_string_t);
NyxOption rfind(rt_string_t, rt_string_t);
int64_t count(rt_string_t, rt_string_t);
rt_string_t repeat(rt_string_t, int64_t);
rt_string_t reverse(rt_string_t);
NyxVec lines(rt_string_t);
NyxVec words(rt_string_t);
rt_string_t capitalize(rt_string_t);
rt_string_t swapcase(rt_string_t);
rt_string_t pad_left(rt_string_t, int64_t, char);
rt_string_t pad_right(rt_string_t, int64_t, char);
int is_alpha(rt_string_t);
int is_digit(rt_string_t);
int is_alnum(rt_string_t);
int is_lower(rt_string_t);
int is_upper(rt_string_t);
int is_whitespace(rt_string_t);
rt_string_t from_char(char);
rt_string_t from_chars(NyxVec);
rt_string_t new();
rt_string_t from(rt_string_t);
int64_t len(rt_string_t);
int is_empty(rt_string_t);
int contains(rt_string_t, rt_string_t);
int starts_with(rt_string_t, rt_string_t);
int ends_with(rt_string_t, rt_string_t);
NyxVec split(rt_string_t, rt_string_t);
rt_string_t replace(rt_string_t, rt_string_t, rt_string_t);
rt_string_t join(NyxVec, rt_string_t);
rt_string_t trim(rt_string_t);
rt_string_t to_lower(rt_string_t);
rt_string_t to_upper(rt_string_t);
NyxOption parse_int(rt_string_t);
NyxOption parse_float(rt_string_t);
rt_string_t to_string_int(int64_t);
rt_string_t to_string_float(double);
char char_at(rt_string_t, int64_t);
rt_string_t slice(rt_string_t, int64_t, int64_t);
int64_t find_index(rt_string_t, rt_string_t);
int is_space(char);
rt_string_t format(rt_string_t, NyxVec);
// module string
extern size_t strlen(const char*);
extern int strcmp(const char*, const char*);
extern char* strstr(const char*, const char*);
extern char* strcpy(char*, const char*);
extern char* strcat(char*, const char*);
extern long strtol(const char*, char**, int);
extern double strtod(const char*, char**);
extern void* memcpy(void*, const void*, size_t);
extern void* malloc(size_t);
extern void free(void*);
extern int toupper(int);
extern int tolower(int);
extern int isdigit(int);
extern int isalpha(int);
extern int isalnum(int);
extern int isspace(int);
rt_string_t new() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_from("");
}

rt_string_t from(rt_string_t s) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return s;
}

int64_t len(rt_string_t s) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
(int64_t)(strlen(((const uint8_t*)(s.data))));
});
}

int is_empty(rt_string_t s) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (s.length == 0LL);
}

int contains(rt_string_t s, rt_string_t substr) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
(strstr(((const uint8_t*)(s.data)), ((const uint8_t*)(substr.data))) != 0LL);
});
}

int starts_with(rt_string_t s, rt_string_t prefix) {
Arena* __arena = arena_create();
({ if ((prefix.length > s.length)) {
return 0;
}
});
arena_destroy(__arena);
return contains(rt_string_slice(s, 0LL, prefix.length), prefix);
}

int ends_with(rt_string_t s, rt_string_t suffix) {
Arena* __arena = arena_create();
int64_t sl = s.length;
int64_t suffix_len = suffix.length;
({ if ((suffix_len > sl)) {
return 0;
}
});
arena_destroy(__arena);
return contains(rt_string_slice(s, (sl - suffix_len), sl), suffix);
}

NyxOption find(rt_string_t s, rt_string_t substr) {
Arena* __arena = arena_create();
int64_t idx = find_index(s, substr);
arena_destroy(__arena);
if ((idx == (-1LL))) {
return rt_option_none();
} else {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)(idx); _tmp; });
}
}

NyxOption rfind(rt_string_t s, rt_string_t substr) {
Arena* __arena = arena_create();
int64_t sl = s.length;
int64_t sub_len = substr.length;
({ if ((sub_len > sl)) {
return rt_option_none();
}
});
int64_t i = (sl - sub_len);
while ((i >= 0LL)) {
({ if (rt_string_eq(slice(s, i, (i + sub_len)), substr)) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)(i); _tmp; });
}
});
i = (i - 1LL);
}
;
arena_destroy(__arena);
return rt_option_none();
}

int64_t count(rt_string_t s, rt_string_t substr) {
Arena* __arena = arena_create();
int64_t result = 0LL;
int64_t pos = 0LL;
int64_t sl = s.length;
int64_t sub_len = substr.length;
while (((pos + sub_len) <= sl)) {
({ int64_t __if_res_5; if (rt_string_eq(slice(s, pos, (pos + sub_len)), substr)) {
result = (result + 1LL);
__if_res_5 = (pos = (pos + sub_len));
} else {
__if_res_5 = (pos = (pos + 1LL));
}
__if_res_5; });
}
;
arena_destroy(__arena);
return result;
}

rt_string_t repeat(rt_string_t s, int64_t n) {
Arena* __arena = arena_create();
rt_string_t result = rt_string_from("");
int64_t i = 0LL;
while ((i < n)) {
result = rt_string_concat(result, ({ rt_string_t _c = s; rt_string_retain(&_c); _c; }));
i = (i + 1LL);
}
;
arena_destroy(__arena);
return result;
}

rt_string_t reverse(rt_string_t s) {
Arena* __arena = arena_create();
int64_t sl = s.length;
rt_string_t result = rt_string_from("");
int64_t i = sl;
while ((i > 0LL)) {
i = (i - 1LL);
result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)char_at(s, i)); _s.ref_count = 1; _s; }));
}
;
arena_destroy(__arena);
return result;
}

NyxVec lines(rt_string_t s) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return split(s, rt_string_from("\n"));
}

NyxVec words(rt_string_t s) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return split(trim(s), rt_string_from(" "));
}

rt_string_t capitalize(rt_string_t s) {
Arena* __arena = arena_create();
({ if ((s.length == 0LL)) {
return s;
}
});
char first = char_at(s, 0LL);
char upper_first = ({ char __if_res_7; if (((first >= 'a') && (first <= 'z'))) {
__if_res_7 = (char)(((int64_t)(first) - 32LL));
} else {
__if_res_7 = first;
}
__if_res_7; });
arena_destroy(__arena);
return rt_string_concat(rt_string_from_char((char)(upper_first)), slice(s, 1LL, s.length));
}

rt_string_t swapcase(rt_string_t s) {
Arena* __arena = arena_create();
int64_t sl = s.length;
rt_string_t result = rt_string_from("");
for (int64_t i = 0LL; i < sl; i++) {
char c = char_at(s, i);
({ rt_string_t __if_res_8; if (((c >= 'a') && (c <= 'z'))) {
__if_res_8 = (result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)(char)(((int64_t)(c) - 32LL))); _s.ref_count = 1; _s; })));
} else if (((c >= 'A') && (c <= 'Z'))) {
__if_res_8 = (result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)(char)(((int64_t)(c) + 32LL))); _s.ref_count = 1; _s; })));
} else {
__if_res_8 = (result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)c); _s.ref_count = 1; _s; })));
}
__if_res_8; });
}
;
arena_destroy(__arena);
return result;
}

rt_string_t pad_left(rt_string_t s, int64_t width, char pad) {
Arena* __arena = arena_create();
int64_t sl = s.length;
({ if ((sl >= width)) {
return s;
}
});
arena_destroy(__arena);
return rt_string_concat(repeat(rt_string_from_char((char)(pad)), (width - sl)), s);
}

rt_string_t pad_right(rt_string_t s, int64_t width, char pad) {
Arena* __arena = arena_create();
int64_t sl = s.length;
({ if ((sl >= width)) {
return s;
}
});
arena_destroy(__arena);
return rt_string_concat(s, repeat(rt_string_from_char((char)(pad)), (width - sl)));
}

int is_alpha(rt_string_t s) {
Arena* __arena = arena_create();
int64_t sl = s.length;
({ if ((sl == 0LL)) {
return 0;
}
});
for (int64_t i = 0LL; i < sl; i++) {
char c = char_at(s, i);
({ if ((!(((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))))) {
return 0;
}
});
}
;
arena_destroy(__arena);
return 1;
}

int is_digit(rt_string_t s) {
Arena* __arena = arena_create();
int64_t sl = s.length;
({ if ((sl == 0LL)) {
return 0;
}
});
for (int64_t i = 0LL; i < sl; i++) {
char c = char_at(s, i);
({ if (((c < '0') || (c > '9'))) {
return 0;
}
});
}
;
arena_destroy(__arena);
return 1;
}

int is_alnum(rt_string_t s) {
Arena* __arena = arena_create();
int64_t sl = s.length;
({ if ((sl == 0LL)) {
return 0;
}
});
for (int64_t i = 0LL; i < sl; i++) {
char c = char_at(s, i);
({ if ((!((((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) || ((c >= '0') && (c <= '9'))))) {
return 0;
}
});
}
;
arena_destroy(__arena);
return 1;
}

int is_lower(rt_string_t s) {
Arena* __arena = arena_create();
int64_t sl = s.length;
({ if ((sl == 0LL)) {
return 0;
}
});
for (int64_t i = 0LL; i < sl; i++) {
char c = char_at(s, i);
({ if ((!((c >= 'a') && (c <= 'z')))) {
return 0;
}
});
}
;
arena_destroy(__arena);
return 1;
}

int is_upper(rt_string_t s) {
Arena* __arena = arena_create();
int64_t sl = s.length;
({ if ((sl == 0LL)) {
return 0;
}
});
for (int64_t i = 0LL; i < sl; i++) {
char c = char_at(s, i);
({ if ((!((c >= 'A') && (c <= 'Z')))) {
return 0;
}
});
}
;
arena_destroy(__arena);
return 1;
}

int is_whitespace(rt_string_t s) {
Arena* __arena = arena_create();
int64_t sl = s.length;
({ if ((sl == 0LL)) {
return 0;
}
});
for (int64_t i = 0LL; i < sl; i++) {
char c = char_at(s, i);
({ if ((!is_space(c))) {
return 0;
}
});
}
;
arena_destroy(__arena);
return 1;
}

rt_string_t from_char(char c) {
Arena* __arena = arena_create();
rt_string_t result = rt_string_from("");
result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)c); _s.ref_count = 1; _s; }));
arena_destroy(__arena);
return result;
}

rt_string_t from_chars(NyxVec chars) {
Arena* __arena = arena_create();
rt_string_t result = rt_string_from("");
for (int64_t _c = 0; _c < chars.len; _c++) {
    char c = *(char*)rt_vec_get(chars, _c);
result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)c); _s.ref_count = 1; _s; }));
}
;
arena_destroy(__arena);
return result;
}

rt_string_t new() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_from("");
}

rt_string_t from(rt_string_t s) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return s;
}

int64_t len(rt_string_t s) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
(int64_t)(strlen(((const uint8_t*)(s.data))));
});
}

int is_empty(rt_string_t s) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (s.length == 0LL);
}

int contains(rt_string_t s, rt_string_t substr) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
(strstr(((const uint8_t*)(s.data)), ((const uint8_t*)(substr.data))) != 0LL);
});
}

int starts_with(rt_string_t s, rt_string_t prefix) {
Arena* __arena = arena_create();
({ if ((prefix.length > s.length)) {
return 0;
}
});
arena_destroy(__arena);
return contains(rt_string_slice(s, 0LL, prefix.length), prefix);
}

int ends_with(rt_string_t s, rt_string_t suffix) {
Arena* __arena = arena_create();
int64_t sl = s.length;
int64_t suffix_len = suffix.length;
({ if ((suffix_len > sl)) {
return 0;
}
});
arena_destroy(__arena);
return contains(rt_string_slice(s, (sl - suffix_len), sl), suffix);
}

NyxVec split(rt_string_t s, rt_string_t delim) {
Arena* __arena = arena_create();
NyxVec result = rt_vec_new(sizeof(rt_string_t));
int64_t delim_len = delim.length;
({ if ((delim_len == 0LL)) {
rt_vec_push(&result, ({ rt_string_t _tmp_25 = (int64_t)(s); &_tmp_25; }));
return result;
}
});
int64_t start = 0LL;
int64_t sl = s.length;
while ((start < sl)) {
rt_string_t rest = rt_string_slice(s, start, sl);
({ int64_t __if_res_27; if (contains(rest, delim)) {
int64_t pos = find_index(rest, delim);
rt_vec_push(&result, ({ rt_string_t _tmp_27 = (int64_t)(rt_string_slice(s, start, (start + pos))); &_tmp_27; }));
__if_res_27 = (start = ((start + pos) + delim_len));
} else {
rt_vec_push(&result, ({ rt_string_t _tmp_28 = (int64_t)(rt_string_slice(s, start, sl)); &_tmp_28; }));
break;
}
__if_res_27; });
}
;
arena_destroy(__arena);
return result;
}

rt_string_t replace(rt_string_t s, rt_string_t from, rt_string_t to) {
Arena* __arena = arena_create();
NyxVec parts = split(s, from);
arena_destroy(__arena);
return join(parts, to);
}

rt_string_t join(NyxVec parts, rt_string_t sep) {
Arena* __arena = arena_create();
rt_string_t result = rt_string_from("");
for (int64_t i = 0LL; i < parts.len; i++) {
({ if ((i > 0LL)) {
result = rt_string_concat(result, sep);
}
});
result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(parts, (size_t)(i)); _u.data.ptr_val; }))); _s.ref_count = 1; _s; }));
}
;
arena_destroy(__arena);
return result;
}

rt_string_t trim(rt_string_t s) {
Arena* __arena = arena_create();
int64_t sl = s.length;
int64_t start = 0LL;
while (((start < sl) && is_space(char_at(s, start)))) {
start = (start + 1LL);
}
;
int64_t end = sl;
while (((end > start) && is_space(char_at(s, (end - 1LL))))) {
end = (end - 1LL);
}
;
arena_destroy(__arena);
return rt_string_slice(s, start, end);
}

rt_string_t to_lower(rt_string_t s) {
Arena* __arena = arena_create();
int64_t sl = s.length;
rt_string_t result = rt_string_from("");
for (int64_t i = 0LL; i < sl; i++) {
char c = char_at(s, i);
({ rt_string_t __if_res_31; if (((c >= 'A') && (c <= 'Z'))) {
__if_res_31 = (result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)(char)(((int64_t)(c) + 32LL))); _s.ref_count = 1; _s; })));
} else {
__if_res_31 = (result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)c); _s.ref_count = 1; _s; })));
}
__if_res_31; });
}
;
arena_destroy(__arena);
return result;
}

rt_string_t to_upper(rt_string_t s) {
Arena* __arena = arena_create();
int64_t sl = s.length;
rt_string_t result = rt_string_from("");
for (int64_t i = 0LL; i < sl; i++) {
char c = char_at(s, i);
({ rt_string_t __if_res_32; if (((c >= 'a') && (c <= 'z'))) {
__if_res_32 = (result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)(char)(((int64_t)(c) - 32LL))); _s.ref_count = 1; _s; })));
} else {
__if_res_32 = (result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)c); _s.ref_count = 1; _s; })));
}
__if_res_32; });
}
;
arena_destroy(__arena);
return result;
}

NyxOption parse_int(rt_string_t s) {
Arena* __arena = arena_create();
rt_string_t trimmed = trim(s);
({ if (is_empty(trimmed)) {
return rt_option_none();
}
});
arena_destroy(__arena);
return ({
uint8_t* end = 0LL;
int64_t val = strtol(((const uint8_t*)(trimmed.data)), (&(end)), 10LL);
({ Option __if_res_34; if ((end == ((const uint8_t*)(trimmed.data)))) {
__if_res_34 = rt_option_none();
} else {
__if_res_34 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)((int64_t)(val)); _tmp; });
}
__if_res_34; });
});
}

NyxOption parse_float(rt_string_t s) {
Arena* __arena = arena_create();
rt_string_t trimmed = trim(s);
({ if (is_empty(trimmed)) {
return rt_option_none();
}
});
arena_destroy(__arena);
return ({
uint8_t* end = 0LL;
double val = strtod(((const uint8_t*)(trimmed.data)), (&(end)));
({ Option __if_res_36; if ((end == ((const uint8_t*)(trimmed.data)))) {
__if_res_36 = rt_option_none();
} else {
__if_res_36 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.float_val = (double)(val); _tmp; });
}
__if_res_36; });
});
}

rt_string_t to_string_int(int64_t n) {
Arena* __arena = arena_create();
({ if ((n == 0LL)) {
return rt_string_from("0");
}
});
rt_string_t result = rt_string_from("");
int64_t val = n;
int negative = (val < 0LL);
({ if (negative) {
val = (-val);
}
});
while ((val > 0LL)) {
char digit = (char)((val % 10LL));
result = rt_string_concat(({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)(digit + '0')); _s.ref_count = 1; _s; }), result);
val = (val / 10LL);
}
;
arena_destroy(__arena);
if (negative) {
return rt_string_concat(rt_string_from("-"), result);
} else {
return result;
}
}

rt_string_t to_string_float(double f) {
Arena* __arena = arena_create();
({ if ((f == 0)) {
return rt_string_from("0.0");
}
});
int64_t int_part = (int64_t)(f);
arena_destroy(__arena);
return rt_string_concat(rt_string_concat(to_string_int(int_part), rt_string_from(".")), to_string_int((int64_t)(((f - (double)(int_part)) * 1000))));
}

char char_at(rt_string_t s, int64_t index) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
(char)(int64_t_add(((const uint8_t*)(s.data)), (int64_t)(index)));
});
}

rt_string_t slice(rt_string_t s, int64_t start, int64_t end) {
Arena* __arena = arena_create();
int64_t len = (end - start);
({ if ((len <= 0LL)) {
return rt_string_from("");
}
});
arena_destroy(__arena);
return ({
uint8_t* buf = malloc(((int64_t)(len) + 1LL));
memcpy(buf, int64_t_add(((const uint8_t*)(s.data)), (int64_t)(start)), (int64_t)(len));
*uint8_t_add(buf, (int64_t)(len)) = 0LL;
(rt_string_t)(buf);
});
}

int64_t find_index(rt_string_t haystack, rt_string_t needle) {
Arena* __arena = arena_create();
int64_t hl = haystack.length;
int64_t nl = needle.length;
for (int64_t i = 0LL; i < ((hl - nl) + 1LL); i++) {
({ if (rt_string_eq(slice(haystack, i, (i + nl)), needle)) {
return i;
}
});
}
;
arena_destroy(__arena);
return (-1LL);
}

int is_space(char c) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ((((c == ' ') || (c == '\t')) || (c == '\n')) || (c == '\\'));
}

rt_string_t format(rt_string_t fmt, NyxVec args) {
Arena* __arena = arena_create();
rt_string_t result = rt_string_from("");
int64_t arg_idx = 0LL;
int64_t fl = fmt.length;
int64_t i = 0LL;
while ((i < fl)) {
char c = char_at(fmt, i);
({ int64_t __if_res_42; if ((((c == '{') && ((i + 1LL) < fl)) && (char_at(fmt, (i + 1LL)) == '}'))) {
({ if ((arg_idx < args.len)) {
result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(args, (size_t)(arg_idx)); _u.data.ptr_val; }))); _s.ref_count = 1; _s; }));
arg_idx = (arg_idx + 1LL);
}
});
__if_res_42 = (i = (i + 2LL));
} else {
result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)c); _s.ref_count = 1; _s; }));
__if_res_42 = (i = (i + 1LL));
}
__if_res_42; });
}
;
arena_destroy(__arena);
return result;
}

