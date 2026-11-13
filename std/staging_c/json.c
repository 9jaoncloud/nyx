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

#define TAG_Value_Null 0
#define TAG_Value_Bool 1
#define TAG_Value_Number 2
#define TAG_Value_String 3
#define TAG_Value_Array 4
#define TAG_Value_Object 5
typedef struct {
    int tag;
    union {
        struct { int f0; } Bool;
        struct { double f0; } Number;
        struct { rt_string_t f0; } String;
        struct { NyxVec f0; } Array;
        struct { NyxMap f0; } Object;
    } data;
} Value;

typedef struct {
    rt_string_t message;
    int64_t position;
    int64_t line;
    int64_t column;
} JsonError;

typedef struct {
    rt_string_t source;
    int64_t pos;
    int64_t line;
    int64_t column;
} JsonParser;

int Value_is_null(Value*);
int Value_is_bool(Value*);
int Value_is_number(Value*);
int Value_is_string(Value*);
int Value_is_array(Value*);
int Value_is_object(Value*);
NyxOption Value_as_bool(Value*);
NyxOption Value_as_number(Value*);
NyxOption Value_as_string(Value*);
NyxOption Value_as_array(Value*);
NyxOption Value_as_object(Value*);
NyxOption Value_get(Value*, rt_string_t);
NyxOption Value_get_int(Value*, rt_string_t);
NyxOption Value_get_string(Value*, rt_string_t);
NyxOption Value_index(Value*, int64_t);
int64_t Value_len(Value*);
rt_string_t JsonError_to_string(JsonError*);
JsonParser JsonParser_new(rt_string_t);
NyxResult JsonParser_parse(JsonParser*);
NyxResult JsonParser_parse_value(JsonParser*);
NyxResult JsonParser_parse_null(JsonParser*);
NyxResult JsonParser_parse_bool(JsonParser*);
NyxResult JsonParser_parse_string_value(JsonParser*);
NyxResult JsonParser_parse_raw_string(JsonParser*);
NyxResult JsonParser_parse_hex_digit(JsonParser*);
NyxResult JsonParser_parse_number(JsonParser*);
NyxResult JsonParser_parse_array(JsonParser*);
NyxResult JsonParser_parse_object(JsonParser*);
void JsonParser_skip_whitespace(JsonParser*);
char JsonParser_peek(JsonParser*);
void JsonParser_advance(JsonParser*);
int JsonParser_consume(JsonParser*, rt_string_t);
NyxResult parse(rt_string_t);
rt_string_t stringify(Value*);
rt_string_t escape_string(rt_string_t);
rt_string_t pretty(Value*);
rt_string_t pretty_indent(Value*, int64_t);
Value merge(Value*, Value*);
int deep_equal(Value*, Value*);
// module json
int Value_is_null(Value* self) {
return ({
int __match_result_0;
Value* __match_val_0 = self;
if ((*__match_val_0).tag == TAG_Value_Null) {
__match_result_0 = 1;
}
 else {
__match_result_0 = 0;
}
__match_result_0;
});
}

int Value_is_bool(Value* self) {
return ({
int __match_result_1;
Value* __match_val_1 = self;
if ((*__match_val_1).tag == TAG_Value_Bool) {
__match_result_1 = 1;
}
 else {
__match_result_1 = 0;
}
__match_result_1;
});
}

int Value_is_number(Value* self) {
return ({
int __match_result_2;
Value* __match_val_2 = self;
if ((*__match_val_2).tag == TAG_Value_Number) {
__match_result_2 = 1;
}
 else {
__match_result_2 = 0;
}
__match_result_2;
});
}

int Value_is_string(Value* self) {
return ({
int __match_result_3;
Value* __match_val_3 = self;
if ((*__match_val_3).tag == TAG_Value_String) {
__match_result_3 = 1;
}
 else {
__match_result_3 = 0;
}
__match_result_3;
});
}

int Value_is_array(Value* self) {
return ({
int __match_result_4;
Value* __match_val_4 = self;
if ((*__match_val_4).tag == TAG_Value_Array) {
__match_result_4 = 1;
}
 else {
__match_result_4 = 0;
}
__match_result_4;
});
}

int Value_is_object(Value* self) {
return ({
int __match_result_5;
Value* __match_val_5 = self;
if ((*__match_val_5).tag == TAG_Value_Object) {
__match_result_5 = 1;
}
 else {
__match_result_5 = 0;
}
__match_result_5;
});
}

NyxOption Value_as_bool(Value* self) {
return ({
NyxOption __match_result_6;
Value* __match_val_6 = self;
if ((*__match_val_6).tag == TAG_Value_Bool) {
    int b = (*__match_val_6).data.Bool.f0;
__match_result_6 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (void*)(b); _tmp; });
}
 else {
__match_result_6 = rt_option_none();
}
__match_result_6;
});
}

NyxOption Value_as_number(Value* self) {
return ({
NyxOption __match_result_7;
Value* __match_val_7 = self;
if ((*__match_val_7).tag == TAG_Value_Number) {
    double n = (*__match_val_7).data.Number.f0;
__match_result_7 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.float_val = (double)(n); _tmp; });
}
 else {
__match_result_7 = rt_option_none();
}
__match_result_7;
});
}

NyxOption Value_as_string(Value* self) {
return ({
NyxOption __match_result_8;
Value* __match_val_8 = self;
if ((*__match_val_8).tag == TAG_Value_String) {
    rt_string_t s = (*__match_val_8).data.String.f0;
__match_result_8 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = ({ rt_string_t _c = s; rt_string_retain(&_c); _c; }); _stmp_.data; }); _tmp; });
}
 else {
__match_result_8 = rt_option_none();
}
__match_result_8;
});
}

NyxOption Value_as_array(Value* self) {
return ({
NyxOption __match_result_9;
Value* __match_val_9 = self;
if ((*__match_val_9).tag == TAG_Value_Array) {
    NyxVec arr = (*__match_val_9).data.Array.f0;
__match_result_9 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = rt_vec_clone(arr); &_stmp_; }); _tmp; });
}
 else {
__match_result_9 = rt_option_none();
}
__match_result_9;
});
}

NyxOption Value_as_object(Value* self) {
return ({
NyxOption __match_result_10;
Value* __match_val_10 = self;
if ((*__match_val_10).tag == TAG_Value_Object) {
    NyxMap obj = (*__match_val_10).data.Object.f0;
__match_result_10 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ NyxMap _stmp_ = obj; &_stmp_; }); _tmp; });
}
 else {
__match_result_10 = rt_option_none();
}
__match_result_10;
});
}

NyxOption Value_get(Value* self, rt_string_t key) {
return ({
NyxOption __match_result_11;
Value* __match_val_11 = self;
if ((*__match_val_11).tag == TAG_Value_Object) {
    NyxMap obj = (*__match_val_11).data.Object.f0;
__match_result_11 = rt_map_get(&(obj), key);
}
 else {
__match_result_11 = rt_option_none();
}
__match_result_11;
});
}

NyxOption Value_get_int(Value* self, rt_string_t key) {
return ({
NyxOption __match_result_12;
NyxOption __match_val_12 = rt_map_get((NyxMap*)(self), key);
if (__match_val_12.tag == OPTION_SOME) {
    void* v = __match_val_12.data.ptr_val;
__match_result_12 = ({
NyxOption __match_result_13;
Value* __match_val_13 = v;
if ((*__match_val_13).tag == TAG_Value_Number) {
    double n = (*__match_val_13).data.Number.f0;
__match_result_13 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)((int64_t)(n)); _tmp; });
}
 else {
__match_result_13 = rt_option_none();
}
__match_result_13;
});
}
 else if (__match_val_12.tag == OPTION_NONE) {
__match_result_12 = rt_option_none();
}
__match_result_12;
});
}

NyxOption Value_get_string(Value* self, rt_string_t key) {
return ({
NyxOption __match_result_14;
NyxOption __match_val_14 = rt_map_get((NyxMap*)(self), key);
if (__match_val_14.tag == OPTION_SOME) {
    void* v = __match_val_14.data.ptr_val;
__match_result_14 = as_string(v);
}
 else if (__match_val_14.tag == OPTION_NONE) {
__match_result_14 = rt_option_none();
}
__match_result_14;
});
}

NyxOption Value_index(Value* self, int64_t i) {
return ({
NyxOption __match_result_15;
Value* __match_val_15 = self;
if ((*__match_val_15).tag == TAG_Value_Array) {
    NyxVec arr = (*__match_val_15).data.Array.f0;
__match_result_15 = rt_vec_get_at(arr, (size_t)(i));
}
 else {
__match_result_15 = rt_option_none();
}
__match_result_15;
});
}

int64_t Value_len(Value* self) {
return ({
int64_t __match_result_16;
Value* __match_val_16 = self;
if ((*__match_val_16).tag == TAG_Value_Array) {
    NyxVec arr = (*__match_val_16).data.Array.f0;
__match_result_16 = arr.len;
}
 else if ((*__match_val_16).tag == TAG_Value_Object) {
    NyxMap obj = (*__match_val_16).data.Object.f0;
__match_result_16 = obj.len;
}
 else {
__match_result_16 = 0LL;
}
__match_result_16;
});
}

rt_string_t JsonError_to_string(JsonError* self) {
return rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("JSON error at line "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)(*self).line); _buf.ref_count = 1; _buf; })), rt_string_from(", col ")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)(*self).column); _buf.ref_count = 1; _buf; })), rt_string_from(": ")), ({ rt_string_t _c = (*self).message; rt_string_retain(&_c); _c; }));
}

JsonParser JsonParser_new(rt_string_t source) {
return (JsonParser){.source = source, .pos = 0LL, .line = 1LL, .column = 1LL};
}

NyxResult JsonParser_parse(JsonParser* self) {
JsonParser_skip_whitespace(self);
Value result = ({ NyxResult _qres = JsonParser_parse_value(self); if (_qres.tag == RESULT_ERR) { return _qres; } *(Value*)_qres.data.ptr_val; });
JsonParser_skip_whitespace(self);
if (((*self).pos < (*self).source.length)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Unexpected trailing characters"), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Value _stmp_ = result; &_stmp_; }); _tmp; });
}
}

NyxResult JsonParser_parse_value(JsonParser* self) {
JsonParser_skip_whitespace(self);
char ch = JsonParser_peek(self);
({ if ((ch == 'n')) {
return JsonParser_parse_null(self);
}
});
({ if (((ch == 't') || (ch == 'f'))) {
return JsonParser_parse_bool(self);
}
});
({ if ((ch == '\"')) {
return JsonParser_parse_string_value(self);
}
});
({ if ((ch == '[')) {
return JsonParser_parse_array(self);
}
});
({ if ((ch == '{')) {
return JsonParser_parse_object(self);
}
});
({ if (((ch == '-') || ((ch >= '0') && (ch <= '9')))) {
return JsonParser_parse_number(self);
}
});
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_concat(rt_string_from("Unexpected character: "), rt_string_from_char((char)(ch))), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}

NyxResult JsonParser_parse_null(JsonParser* self) {
if (JsonParser_consume(self, rt_string_from("null"))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Value _stmp_ = (Value){ .tag = TAG_Value_Null }; &_stmp_; }); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Expected 'null'"), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}
}

NyxResult JsonParser_parse_bool(JsonParser* self) {
({ if ((JsonParser_peek(self) == 't')) {
({ if (JsonParser_consume(self, rt_string_from("true"))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Value _stmp_ = (Value){ .tag = TAG_Value_Bool, .data.Bool = {.f0 = 1} }; &_stmp_; }); _tmp; });
}
});
}
});
({ if ((JsonParser_peek(self) == 'f')) {
({ if (JsonParser_consume(self, rt_string_from("false"))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Value _stmp_ = (Value){ .tag = TAG_Value_Bool, .data.Bool = {.f0 = 0} }; &_stmp_; }); _tmp; });
}
});
}
});
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Expected 'true' or 'false'"), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}

NyxResult JsonParser_parse_string_value(JsonParser* self) {
rt_string_t s = ({ NyxResult _qres = JsonParser_parse_raw_string(self); if (_qres.tag == RESULT_ERR) { return _qres; } rt_string_from((char*)_qres.data.ptr_val); });
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Value _stmp_ = (Value){ .tag = TAG_Value_String, .data.String = {.f0 = s} }; &_stmp_; }); _tmp; });
}

NyxResult JsonParser_parse_raw_string(JsonParser* self) {
({ if ((JsonParser_peek(self) != '\"')) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Expected '\"'"), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}
});
JsonParser_advance(self);
rt_string_t result = rt_string_from("");
while (1) {
char ch = JsonParser_peek(self);
({ if ((ch == '\0')) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Unterminated string"), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}
});
({ if ((ch == '\"')) {
JsonParser_advance(self);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = result; _stmp_.data; }); _tmp; });
}
});
({ NyxResult __if_res_31; if ((ch == '\\')) {
JsonParser_advance(self);
char esc = JsonParser_peek(self);
JsonParser_advance(self);
__if_res_31 = ({
rt_string_t __match_result_31;
char __match_val_31 = esc;
if (__match_val_31 == '\"') {
__match_result_31 = (result = rt_string_concat(result, rt_string_from("\"")));
}
 else if (__match_val_31 == '\\') {
__match_result_31 = (result = rt_string_concat(result, rt_string_from("\\")));
}
 else if (__match_val_31 == '/') {
__match_result_31 = (result = rt_string_concat(result, rt_string_from("/")));
}
 else if (__match_val_31 == 'n') {
__match_result_31 = (result = rt_string_concat(result, rt_string_from("\n")));
}
 else if (__match_val_31 == 't') {
__match_result_31 = (result = rt_string_concat(result, rt_string_from("\t")));
}
 else if (__match_val_31 == 'r') {
__match_result_31 = (result = rt_string_concat(result, rt_string_from("\r")));
}
 else if (__match_val_31 == 'b') {
__match_result_31 = (result = rt_string_concat(result, rt_string_from("\\x08")));
}
 else if (__match_val_31 == 'f') {
__match_result_31 = (result = rt_string_concat(result, rt_string_from("\\x0C")));
}
 else if (__match_val_31 == 'u') {
int64_t hex1 = ({ NyxResult _qres = JsonParser_parse_hex_digit(self); if (_qres.tag == RESULT_ERR) { return _qres; } (int64_t)_qres.data.int_val; });
int64_t hex2 = ({ NyxResult _qres = JsonParser_parse_hex_digit(self); if (_qres.tag == RESULT_ERR) { return _qres; } (int64_t)_qres.data.int_val; });
int64_t hex3 = ({ NyxResult _qres = JsonParser_parse_hex_digit(self); if (_qres.tag == RESULT_ERR) { return _qres; } (int64_t)_qres.data.int_val; });
int64_t hex4 = ({ NyxResult _qres = JsonParser_parse_hex_digit(self); if (_qres.tag == RESULT_ERR) { return _qres; } (int64_t)_qres.data.int_val; });
int64_t codepoint = ((((hex1 << 12LL) | (hex2 << 8LL)) | (hex3 << 4LL)) | hex4);
__match_result_31 = (result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)Value_from_codepoint(&(String), codepoint)); _s.ref_count = 1; _s; })));
}
 else {
__match_result_31 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Invalid escape character"), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}
__match_result_31;
});
} else {
JsonParser_advance(self);
__if_res_31 = (result = rt_string_concat(result, rt_string_from_char((char)(ch))));
}
__if_res_31; });
}
;
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Unterminated string"), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}

NyxResult JsonParser_parse_hex_digit(JsonParser* self) {
char ch = JsonParser_peek(self);
JsonParser_advance(self);
if (((ch >= '0') && (ch <= '9'))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(((int64_t)(ch) - (int64_t)('0'))); _tmp; });
} else if (((ch >= 'a') && (ch <= 'f'))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)((((int64_t)(ch) - (int64_t)('a')) + 10LL)); _tmp; });
} else if (((ch >= 'A') && (ch <= 'F'))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)((((int64_t)(ch) - (int64_t)('A')) + 10LL)); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Invalid hex digit"), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}
}

NyxResult JsonParser_parse_number(JsonParser* self) {
int64_t start = (*self).pos;
int is_float = 0;
({ if ((JsonParser_peek(self) == '-')) {
JsonParser_advance(self);
}
});
while (((JsonParser_peek(self) >= '0') && (JsonParser_peek(self) <= '9'))) {
JsonParser_advance(self);
}
;
({ if ((JsonParser_peek(self) == '.')) {
is_float = 1;
JsonParser_advance(self);
while (((JsonParser_peek(self) >= '0') && (JsonParser_peek(self) <= '9'))) {
JsonParser_advance(self);
}
;
}
});
({ if (((JsonParser_peek(self) == 'e') || (JsonParser_peek(self) == 'E'))) {
is_float = 1;
JsonParser_advance(self);
({ if (((JsonParser_peek(self) == '+') || (JsonParser_peek(self) == '-'))) {
JsonParser_advance(self);
}
});
while (((JsonParser_peek(self) >= '0') && (JsonParser_peek(self) <= '9'))) {
JsonParser_advance(self);
}
;
}
});
rt_string_t num_str = rt_string_slice((*self).source, start, (*self).pos);
if (is_float) {
return ({
NyxResult __match_result_36;
NyxOption __match_val_36 = rt_string_parse_float(num_str);
if (__match_val_36.tag == OPTION_SOME) {
    void* n = __match_val_36.data.ptr_val;
__match_result_36 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Value _stmp_ = (Value){ .tag = TAG_Value_Number, .data.Number = {.f0 = n} }; &_stmp_; }); _tmp; });
}
 else if (__match_val_36.tag == OPTION_NONE) {
__match_result_36 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Invalid number"), .position = start, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}
__match_result_36;
});
} else {
return ({
NyxResult __match_result_37;
NyxOption __match_val_37 = rt_string_parse_int(num_str);
if (__match_val_37.tag == OPTION_SOME) {
    void* n = __match_val_37.data.ptr_val;
__match_result_37 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Value _stmp_ = (Value){ .tag = TAG_Value_Number, .data.Number = {.f0 = (double)(n)} }; &_stmp_; }); _tmp; });
}
 else if (__match_val_37.tag == OPTION_NONE) {
__match_result_37 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Invalid number"), .position = start, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}
__match_result_37;
});
}
}

NyxResult JsonParser_parse_array(JsonParser* self) {
JsonParser_advance(self);
NyxVec elements = rt_vec_new(sizeof(int64_t));
JsonParser_skip_whitespace(self);
({ if ((JsonParser_peek(self) == ']')) {
JsonParser_advance(self);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Value _stmp_ = (Value){ .tag = TAG_Value_Array, .data.Array = {.f0 = elements} }; &_stmp_; }); _tmp; });
}
});
while (1) {
Value value = ({ NyxResult _qres = JsonParser_parse_value(self); if (_qres.tag == RESULT_ERR) { return _qres; } *(Value*)_qres.data.ptr_val; });
rt_vec_push(&elements, ({ Value _tmp_39 = (int64_t)(value); &_tmp_39; }));
JsonParser_skip_whitespace(self);
({ NyxResult __if_res_41; if ((JsonParser_peek(self) == ',')) {
JsonParser_advance(self);
__if_res_41 = JsonParser_skip_whitespace(self);
} else if ((JsonParser_peek(self) == ']')) {
JsonParser_advance(self);
__if_res_41 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Value _stmp_ = (Value){ .tag = TAG_Value_Array, .data.Array = {.f0 = elements} }; &_stmp_; }); _tmp; });
} else {
__if_res_41 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Expected ',' or ']'"), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}
__if_res_41; });
}
;
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Unterminated array"), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}

NyxResult JsonParser_parse_object(JsonParser* self) {
JsonParser_advance(self);
NyxMap map = rt_map_new();
JsonParser_skip_whitespace(self);
({ if ((JsonParser_peek(self) == '}')) {
JsonParser_advance(self);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Value _stmp_ = (Value){ .tag = TAG_Value_Object, .data.Object = {.f0 = map} }; &_stmp_; }); _tmp; });
}
});
while (1) {
JsonParser_skip_whitespace(self);
rt_string_t key = ({ NyxResult _qres = JsonParser_parse_raw_string(self); if (_qres.tag == RESULT_ERR) { return _qres; } rt_string_from((char*)_qres.data.ptr_val); });
JsonParser_skip_whitespace(self);
({ if ((JsonParser_peek(self) != ':')) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Expected ':'"), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}
});
JsonParser_advance(self);
JsonParser_skip_whitespace(self);
Value value = ({ NyxResult _qres = JsonParser_parse_value(self); if (_qres.tag == RESULT_ERR) { return _qres; } *(Value*)_qres.data.ptr_val; });
rt_map_insert(&(map), key, (int64_t)(({ Value* _mtmp = malloc(sizeof(Value)); *_mtmp = value; (int64_t)_mtmp; })));
JsonParser_skip_whitespace(self);
({ NyxResult __if_res_44; if ((JsonParser_peek(self) == ',')) {
JsonParser_advance(self);
} else if ((JsonParser_peek(self) == '}')) {
JsonParser_advance(self);
__if_res_44 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Value _stmp_ = (Value){ .tag = TAG_Value_Object, .data.Object = {.f0 = map} }; &_stmp_; }); _tmp; });
} else {
__if_res_44 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Expected ',' or '}'"), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}
__if_res_44; });
}
;
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ JsonError _stmp_ = (JsonError){.message = rt_string_from("Unterminated object"), .position = (*self).pos, .line = (*self).line, .column = (*self).column}; &_stmp_; }); _tmp; });
}

void JsonParser_skip_whitespace(JsonParser* self) {
while (((*self).pos < (*self).source.length)) {
char ch = JsonParser_peek(self);
({ if (((((ch == ' ') || (ch == '\t')) || (ch == '\n')) || (ch == '\\'))) {
JsonParser_advance(self);
} else {
break;
}
});
}
;
}

char JsonParser_peek(JsonParser* self) {
if (((*self).pos < (*self).source.length)) {
return rt_string_char_at((*self).source, (*self).pos);
} else {
return '\0';
}
}

void JsonParser_advance(JsonParser* self) {
({ if (((*self).pos < (*self).source.length)) {
char ch = JsonParser_peek(self);
({ if ((ch == '\n')) {
(self->line = ((*self).line + 1LL));
(self->column = 1LL);
} else {
(self->column = ((*self).column + 1LL));
}
});
(self->pos = ((*self).pos + 1LL));
}
});
}

int JsonParser_consume(JsonParser* self, rt_string_t expected) {
for (int64_t _ch = 0; _ch < rt_string_chars(expected).len; _ch++) {
    int64_t ch = *(int64_t*)rt_vec_get(rt_string_chars(expected), _ch);
({ if ((JsonParser_peek(self) != ch)) {
return 0;
}
});
JsonParser_advance(self);
}
;
return 1;
}

NyxResult parse(rt_string_t input) {
Arena* __arena = arena_create();
JsonParser parser = JsonParser_new(input);
arena_destroy(__arena);
return JsonParser_parse(&(parser));
}

rt_string_t stringify(Value* value) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_48;
Value* __match_val_48 = value;
if ((*__match_val_48).tag == TAG_Value_Null) {
__match_result_48 = rt_string_from("null");
}
 else if ((*__match_val_48).tag == TAG_Value_Bool) {
    int b = (*__match_val_48).data.Bool.f0;
if (b) {
__match_result_48 = rt_string_from("true");
} else {
__match_result_48 = rt_string_from("false");
}
}
 else if ((*__match_val_48).tag == TAG_Value_Number) {
    double n = (*__match_val_48).data.Number.f0;
if ((n == (double)((int64_t)(n)))) {
__match_result_48 = ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)(int64_t)(n)); _buf.ref_count = 1; _buf; });
} else {
__match_result_48 = ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", n); _buf.ref_count = 1; _buf; });
}
}
 else if ((*__match_val_48).tag == TAG_Value_String) {
    rt_string_t s = (*__match_val_48).data.String.f0;
__match_result_48 = rt_string_concat(rt_string_concat(rt_string_from("\""), escape_string(({ rt_string_t _c = s; rt_string_retain(&_c); _c; }))), rt_string_from("\""));
}
 else if ((*__match_val_48).tag == TAG_Value_Array) {
    NyxVec items = (*__match_val_48).data.Array.f0;
rt_string_t out = rt_string_from("[");
for (int64_t i = 0LL; i < items.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(","));
}
});
out = rt_string_concat(out, stringify(({ int64_t _atmp_0 = rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(items, (size_t)(i)); _u.data.ptr_val; })); &_atmp_0; })));
}
;
__match_result_48 = rt_string_concat(out, rt_string_from("]"));
}
 else if ((*__match_val_48).tag == TAG_Value_Object) {
    NyxMap fields = (*__match_val_48).data.Object.f0;
rt_string_t out = rt_string_from("{");
int first = 1;
for (int64_t _idx_key_value = 0; _idx_key_value < rt_map_iter(&(fields)).len; _idx_key_value++) {
    NyxTuple2 _tup_key_value = *(NyxTuple2*)rt_vec_get(rt_map_iter(&(fields)), _idx_key_value);
    int64_t key = (int64_t)(_tup_key_value.f0);
    int64_t value = (int64_t)(_tup_key_value.f1);
({ if ((!first)) {
out = rt_string_concat(out, rt_string_from(","));
}
});
first = 0;
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("\"")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)key); _s.ref_count = 1; _s; })), rt_string_from("\":")), stringify(&(value)));
}
;
__match_result_48 = rt_string_concat(out, rt_string_from("}"));
}
__match_result_48;
});
}

rt_string_t escape_string(rt_string_t s) {
Arena* __arena = arena_create();
rt_string_t result = rt_string_from("");
for (int64_t _ch = 0; _ch < rt_string_chars(s).len; _ch++) {
    int64_t ch = *(int64_t*)rt_vec_get(rt_string_chars(s), _ch);
({
rt_string_t __match_result_51;
int64_t __match_val_51 = ch;
if (__match_val_51 == '\"') {
__match_result_51 = (result = rt_string_concat(result, rt_string_from("\\\"")));
}
 else if (__match_val_51 == '\\') {
__match_result_51 = (result = rt_string_concat(result, rt_string_from("\\\\")));
}
 else if (__match_val_51 == '\n') {
__match_result_51 = (result = rt_string_concat(result, rt_string_from("\\n")));
}
 else if (__match_val_51 == '\t') {
__match_result_51 = (result = rt_string_concat(result, rt_string_from("\\t")));
}
 else if (__match_val_51 == '\\') {
__match_result_51 = (result = rt_string_concat(result, rt_string_from("\\r")));
}
 else if (__match_val_51 == '\\') {
__match_result_51 = (result = rt_string_concat(result, rt_string_from("\\b")));
}
 else if (__match_val_51 == '\\') {
__match_result_51 = (result = rt_string_concat(result, rt_string_from("\\f")));
}
 else { int64_t c = __match_val_51;
if (((int64_t)(c) < 32LL)) {
int64_t hex = int64_t_to_hex((int64_t)(c));
__match_result_51 = (result = rt_string_concat(rt_string_concat(result, rt_string_slice(rt_string_from("\\u0000"), 0LL, (6LL - hex.len))), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)hex); _s.ref_count = 1; _s; })));
} else {
__match_result_51 = (result = rt_string_concat(result, rt_string_from_char((char)(c))));
}
}
__match_result_51;
});
}
;
arena_destroy(__arena);
return result;
}

rt_string_t pretty(Value* value) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return pretty_indent(value, 0LL);
}

rt_string_t pretty_indent(Value* value, int64_t indent) {
Arena* __arena = arena_create();
int64_t pad = rt_string_repeat(rt_string_from("    "), indent);
int64_t pad1 = rt_string_repeat(rt_string_from("    "), (indent + 1LL));
arena_destroy(__arena);
return ({
rt_string_t __match_result_52;
Value* __match_val_52 = value;
if ((*__match_val_52).tag == TAG_Value_Null) {
__match_result_52 = rt_string_from("null");
}
 else if ((*__match_val_52).tag == TAG_Value_Bool) {
    int b = (*__match_val_52).data.Bool.f0;
if (b) {
__match_result_52 = rt_string_from("true");
} else {
__match_result_52 = rt_string_from("false");
}
}
 else if ((*__match_val_52).tag == TAG_Value_Number) {
    double n = (*__match_val_52).data.Number.f0;
__match_result_52 = ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", n); _buf.ref_count = 1; _buf; });
}
 else if ((*__match_val_52).tag == TAG_Value_String) {
    rt_string_t s = (*__match_val_52).data.String.f0;
__match_result_52 = rt_string_concat(rt_string_concat(rt_string_from("\""), ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })), rt_string_from("\""));
}
 else if ((*__match_val_52).tag == TAG_Value_Array) {
    NyxVec items = (*__match_val_52).data.Array.f0;
({ if ((items.len == 0LL)) {
return rt_string_from("[]");
}
});
rt_string_t out = rt_string_from("[\n");
for (int64_t i = 0LL; i < items.len; i++) {
out = rt_string_concat(rt_string_concat(out, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)pad1); _s.ref_count = 1; _s; })), pretty_indent(({ int64_t _atmp_0 = rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(items, (size_t)(i)); _u.data.ptr_val; })); &_atmp_0; }), (indent + 1LL)));
({ if ((i < (items.len - 1LL))) {
out = rt_string_concat(out, rt_string_from(","));
}
});
out = rt_string_concat(out, rt_string_from("\n"));
}
;
__match_result_52 = rt_string_concat(rt_string_concat(out, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)pad); _s.ref_count = 1; _s; })), rt_string_from("]"));
}
 else if ((*__match_val_52).tag == TAG_Value_Object) {
    NyxMap fields = (*__match_val_52).data.Object.f0;
({ if ((fields.len == 0LL)) {
return rt_string_from("{}");
}
});
rt_string_t out = rt_string_from("{\n");
int first = 1;
for (int64_t _idx_key_value = 0; _idx_key_value < rt_map_iter(&(fields)).len; _idx_key_value++) {
    NyxTuple2 _tup_key_value = *(NyxTuple2*)rt_vec_get(rt_map_iter(&(fields)), _idx_key_value);
    int64_t key = (int64_t)(_tup_key_value.f0);
    int64_t value = (int64_t)(_tup_key_value.f1);
({ if ((!first)) {
out = rt_string_concat(out, rt_string_from(",\n"));
}
});
first = 0;
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)pad1); _s.ref_count = 1; _s; })), rt_string_from("\"")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)key); _s.ref_count = 1; _s; })), rt_string_from("\": ")), pretty_indent(&(value), (indent + 1LL)));
}
;
__match_result_52 = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("\n")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)pad); _s.ref_count = 1; _s; })), rt_string_from("}"));
}
__match_result_52;
});
}

Value merge(Value* base, Value* overlay) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
Value __match_result_57;
Value* __match_val_57 = base;
if ((*__match_val_57).tag == TAG_Value_Object) {
    NyxMap base_map = (*__match_val_57).data.Object.f0;
__match_result_57 = ({
Value __match_result_58;
Value* __match_val_58 = overlay;
if ((*__match_val_58).tag == TAG_Value_Object) {
    NyxMap overlay_map = (*__match_val_58).data.Object.f0;
NyxMap result = rt_map_new();
for (int64_t _idx_key_value = 0; _idx_key_value < rt_map_iter(&(base_map)).len; _idx_key_value++) {
    NyxTuple2 _tup_key_value = *(NyxTuple2*)rt_vec_get(rt_map_iter(&(base_map)), _idx_key_value);
    int64_t key = (int64_t)(_tup_key_value.f0);
    int64_t value = (int64_t)(_tup_key_value.f1);
rt_map_insert(&(result), key, (int64_t)(value));
}
;
for (int64_t _idx_key_value = 0; _idx_key_value < rt_map_iter(&(overlay_map)).len; _idx_key_value++) {
    NyxTuple2 _tup_key_value = *(NyxTuple2*)rt_vec_get(rt_map_iter(&(overlay_map)), _idx_key_value);
    int64_t key = (int64_t)(_tup_key_value.f0);
    int64_t value = (int64_t)(_tup_key_value.f1);
rt_map_insert(&(result), key, (int64_t)(value));
}
;
__match_result_58 = (Value){ .tag = TAG_Value_Object, .data.Object = {.f0 = result} };
}
 else {
__match_result_58 = overlay /* clone ptr */;
}
__match_result_58;
});
}
 else {
__match_result_57 = overlay /* clone ptr */;
}
__match_result_57;
});
}

int deep_equal(Value* a, Value* b) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
int __match_result_59;
Value* __match_val_59 = a;
if ((*__match_val_59).tag == TAG_Value_Null) {
__match_result_59 = ({
int __match_result_60;
Value* __match_val_60 = b;
if ((*__match_val_60).tag == TAG_Value_Null) {
__match_result_60 = 1;
}
 else {
__match_result_60 = 0;
}
__match_result_60;
});
}
 else if ((*__match_val_59).tag == TAG_Value_Bool) {
    int x = (*__match_val_59).data.Bool.f0;
__match_result_59 = ({
int __match_result_61;
Value* __match_val_61 = b;
if ((*__match_val_61).tag == TAG_Value_Bool) {
    int y = (*__match_val_61).data.Bool.f0;
__match_result_61 = (x == y);
}
 else {
__match_result_61 = 0;
}
__match_result_61;
});
}
 else if ((*__match_val_59).tag == TAG_Value_Number) {
    double x = (*__match_val_59).data.Number.f0;
__match_result_59 = ({
int __match_result_62;
Value* __match_val_62 = b;
if ((*__match_val_62).tag == TAG_Value_Number) {
    double y = (*__match_val_62).data.Number.f0;
__match_result_62 = (x == y);
}
 else {
__match_result_62 = 0;
}
__match_result_62;
});
}
 else if ((*__match_val_59).tag == TAG_Value_String) {
    rt_string_t x = (*__match_val_59).data.String.f0;
__match_result_59 = ({
int __match_result_63;
Value* __match_val_63 = b;
if ((*__match_val_63).tag == TAG_Value_String) {
    rt_string_t y = (*__match_val_63).data.String.f0;
__match_result_63 = rt_string_eq(x, y);
}
 else {
__match_result_63 = 0;
}
__match_result_63;
});
}
 else if ((*__match_val_59).tag == TAG_Value_Array) {
    NyxVec a_arr = (*__match_val_59).data.Array.f0;
__match_result_59 = ({
int __match_result_64;
Value* __match_val_64 = b;
if ((*__match_val_64).tag == TAG_Value_Array) {
    NyxVec b_arr = (*__match_val_64).data.Array.f0;
({ if ((a_arr.len != b_arr.len)) {
return 0;
}
});
for (int64_t i = 0LL; i < a_arr.len; i++) {
({ if ((!deep_equal(({ int64_t _atmp_0 = rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(a_arr, (size_t)(i)); _u.data.ptr_val; })); &_atmp_0; }), ({ int64_t _atmp_1 = rt_string_from((char*)({ NyxOption _u = rt_vec_get_at(b_arr, (size_t)(i)); _u.data.ptr_val; })); &_atmp_1; })))) {
return 0;
}
});
}
;
__match_result_64 = 1;
}
 else {
__match_result_64 = 0;
}
__match_result_64;
});
}
 else if ((*__match_val_59).tag == TAG_Value_Object) {
    NyxMap a_obj = (*__match_val_59).data.Object.f0;
__match_result_59 = ({
int __match_result_67;
Value* __match_val_67 = b;
if ((*__match_val_67).tag == TAG_Value_Object) {
    NyxMap b_obj = (*__match_val_67).data.Object.f0;
({ if ((a_obj.len != b_obj.len)) {
return 0;
}
});
for (int64_t _idx_key_val = 0; _idx_key_val < rt_map_iter(&(a_obj)).len; _idx_key_val++) {
    NyxTuple2 _tup_key_val = *(NyxTuple2*)rt_vec_get(rt_map_iter(&(a_obj)), _idx_key_val);
    int64_t key = (int64_t)(_tup_key_val.f0);
    Expr* val = (Expr*)(_tup_key_val.f1);
({
int64_t __match_result_69;
NyxOption __match_val_69 = rt_map_get(&(b_obj), key);
if (__match_val_69.tag == OPTION_SOME) {
    void* b_val = __match_val_69.data.ptr_val;
({ if ((!deep_equal(val, b_val))) {
return 0;
}
});
}
 else if (__match_val_69.tag == OPTION_NONE) {
return 0;
}
__match_result_69;
});
}
;
__match_result_67 = 1;
}
 else {
__match_result_67 = 0;
}
__match_result_67;
});
}
__match_result_59;
});
}

