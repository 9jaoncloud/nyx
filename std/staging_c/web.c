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

#define TAG_HtmxSwap_InnerHTML 0
#define TAG_HtmxSwap_OuterHTML 1
#define TAG_HtmxSwap_BeforeBegin 2
#define TAG_HtmxSwap_AfterBegin 3
#define TAG_HtmxSwap_BeforeEnd 4
#define TAG_HtmxSwap_AfterEnd 5
#define TAG_HtmxSwap_Delete 6
#define TAG_HtmxSwap_None 7
typedef struct {
    int tag;
    union {
    } data;
} HtmxSwap;

typedef struct {
    rt_string_t id;
    int64_t width;
    int64_t height;
} WebCanvas;

typedef struct {
    rt_string_t shader_code;
    int64_t bind_group_layout_id;
} WebGpuPipeline;

typedef struct {
    int64_t wasm_binary_size_bytes;
    int simd128_enabled;
    double canvas_fps;
} WebReport;

typedef struct {
    rt_string_t field;
    rt_string_t message;
} ValidationError;

typedef struct {
    int is_valid;
    NyxVec errors;
} ValidationResult;

typedef struct {
} Validator;

typedef struct {
    rt_string_t html_content;
    rt_string_t target_selector;
    HtmxSwap swap_style;
    rt_string_t trigger_event;
} HtmxResponse;

WebCanvas WebCanvas_new(rt_string_t, int64_t, int64_t);
int ValidationResult_passes(ValidationResult*);
int ValidationResult_fails(ValidationResult*);
NyxOption ValidationResult_first_error(ValidationResult*, rt_string_t);
rt_string_t ValidationResult_to_json(ValidationResult*);
NyxOption Validator_validate_field(rt_string_t, rt_string_t, rt_string_t);
ValidationResult Validator_make(NyxVec, NyxVec);
rt_string_t HtmxSwap_to_header_val(HtmxSwap*);
HtmxResponse HtmxResponse_new(rt_string_t);
HtmxResponse HtmxResponse_target(HtmxResponse*, rt_string_t);
HtmxResponse HtmxResponse_swap(HtmxResponse*, HtmxSwap);
HtmxResponse HtmxResponse_trigger(HtmxResponse*, rt_string_t);
rt_string_t HtmxResponse_render_html(HtmxResponse*);
// module web
// import std::string
// import std::vec
// import std::option
// extern nyx_wasm_dom_query
// extern nyx_wasm_canvas_get_context2d
// extern nyx_wasm_fetch_get
// extern nyx_wasm_storage_set
// extern nyx_wasm_storage_get
WebCanvas WebCanvas_new(rt_string_t id, int64_t width, int64_t height) {
return (WebCanvas){.id = id, .width = width, .height = height};
}

int ValidationResult_passes(ValidationResult* self) {
return (*self).is_valid;
}

int ValidationResult_fails(ValidationResult* self) {
return (!(*self).is_valid);
}

NyxOption ValidationResult_first_error(ValidationResult* self, rt_string_t field) {
int64_t i = 0LL;
while ((i < (*self).errors.len)) {
({ if (rt_string_eq(((*(int64_t*)rt_vec_get((*self).errors, i))).field, field)) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = ({ rt_string_t _c = ((*(int64_t*)rt_vec_get((*self).errors, i))).message; rt_string_retain(&_c); _c; }); _stmp_.data; }); _tmp; });
}
});
i = (i + 1LL);
}
;
return rt_option_none();
}

rt_string_t ValidationResult_to_json(ValidationResult* self) {
rt_string_t json = rt_string_from("{\"status\":422,\"errors\":{");
int64_t i = 0LL;
while ((i < (*self).errors.len)) {
({ if ((i > 0LL)) {
json = rt_string_concat(json, rt_string_from(","));
}
});
json = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(json, rt_string_from("\"")), ((*(int64_t*)rt_vec_get((*self).errors, i))).field), rt_string_from("\":[\"")), ((*(int64_t*)rt_vec_get((*self).errors, i))).message), rt_string_from("\"]"));
i = (i + 1LL);
}
;
json = rt_string_concat(json, rt_string_from("}}"));
return json;
}

NyxOption Validator_validate_field(rt_string_t field, rt_string_t val, rt_string_t rule) {
({ if (rt_string_eq(rule, rt_string_from("required"))) {
({ if ((val.length == 0LL)) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_concat(rt_string_from("The "), field), rt_string_from(" field is required.")); _stmp_.data; }); _tmp; });
}
});
} else if (rt_string_eq(rule, rt_string_from("email"))) {
({ if (((rt_string_find(val, rt_string_from("@")) < 0LL) || (rt_string_find(val, rt_string_from(".")) < 0LL))) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_concat(rt_string_from("The "), field), rt_string_from(" must be a valid email address.")); _stmp_.data; }); _tmp; });
}
});
} else if (rt_string_eq(rule, rt_string_from("numeric"))) {
NyxOption parsed = rt_string_parse_int(val);
({
int64_t __match_result_5;
NyxOption __match_val_5 = parsed;
if (__match_val_5.tag == OPTION_SOME) {
}
 else if (__match_val_5.tag == OPTION_NONE) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_concat(rt_string_from("The "), field), rt_string_from(" must be a number.")); _stmp_.data; }); _tmp; });
}
__match_result_5;
});
} else if (rt_string_starts_with(rule, rt_string_from("min:"))) {
rt_string_t min_len_str = rt_string_slice(rule, 4LL, rule.length);
({
int64_t __match_result_6;
NyxOption __match_val_6 = rt_string_parse_int(min_len_str);
if (__match_val_6.tag == OPTION_SOME) {
    void* min_val = __match_val_6.data.ptr_val;
({ if (((int64_t)(val.length) < min_val)) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("The "), field), rt_string_from(" must be at least ")), min_len_str), rt_string_from(" characters.")); _stmp_.data; }); _tmp; });
}
});
}
 else if (__match_val_6.tag == OPTION_NONE) {
}
__match_result_6;
});
} else if (rt_string_starts_with(rule, rt_string_from("max:"))) {
rt_string_t max_len_str = rt_string_slice(rule, 4LL, rule.length);
({
int64_t __match_result_8;
NyxOption __match_val_8 = rt_string_parse_int(max_len_str);
if (__match_val_8.tag == OPTION_SOME) {
    void* max_val = __match_val_8.data.ptr_val;
({ if (((int64_t)(val.length) > max_val)) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("The "), field), rt_string_from(" cannot exceed ")), max_len_str), rt_string_from(" characters.")); _stmp_.data; }); _tmp; });
}
});
}
 else if (__match_val_8.tag == OPTION_NONE) {
}
__match_result_8;
});
}
});
return rt_option_none();
}

ValidationResult Validator_make(NyxVec data, NyxVec rules) {
NyxVec errors = rt_vec_new(sizeof(int64_t));
int64_t r = 0LL;
while ((r < rules.len)) {
int64_t field_name = (&(((*(void**)rt_vec_get(rules, r))).f0));
int64_t rule_chain = (&(((*(void**)rt_vec_get(rules, r))).f1));
rt_string_t field_val = rt_string_from("");
int64_t d = 0LL;
while ((d < data.len)) {
({ if (((&(((*(void**)rt_vec_get(data, d))).f0)) == field_name)) {
field_val = ((*(void**)rt_vec_get(data, d))).f1;
break;
}
});
d = (d + 1LL);
}
;
NyxVec single_rules = int64_t_split(rule_chain, rt_string_from("|"));
int64_t s = 0LL;
while ((s < single_rules.len)) {
NyxOption err_opt = Validator_validate_field(field_name, ({ rt_string_t _c = field_val; rt_string_retain(&_c); _c; }), (*(int64_t*)rt_vec_get(single_rules, s)));
({
int64_t __match_result_11;
NyxOption __match_val_11 = err_opt;
if (__match_val_11.tag == OPTION_SOME) {
    rt_string_t msg = rt_string_from((char*)__match_val_11.data.ptr_val);
rt_vec_push(&errors, ({ ValidationError _tmp_12 = (int64_t)((ValidationError){.field = field_name, .message = msg}); &_tmp_12; }));
break;
}
 else if (__match_val_11.tag == OPTION_NONE) {
}
__match_result_11;
});
s = (s + 1LL);
}
;
r = (r + 1LL);
}
;
int is_valid = (errors.len == 0LL);
return (ValidationResult){.is_valid = is_valid, .errors = errors};
}

rt_string_t HtmxSwap_to_header_val(HtmxSwap* self) {
return ({
rt_string_t __match_result_13;
void* __match_val_13 = self;
if ((*__match_val_13).tag == TAG_HtmxSwap_InnerHTML) {
__match_result_13 = rt_string_from("innerHTML");
}
 else if ((*__match_val_13).tag == TAG_HtmxSwap_OuterHTML) {
__match_result_13 = rt_string_from("outerHTML");
}
 else if ((*__match_val_13).tag == TAG_HtmxSwap_BeforeBegin) {
__match_result_13 = rt_string_from("beforebegin");
}
 else if ((*__match_val_13).tag == TAG_HtmxSwap_AfterBegin) {
__match_result_13 = rt_string_from("afterbegin");
}
 else if ((*__match_val_13).tag == TAG_HtmxSwap_BeforeEnd) {
__match_result_13 = rt_string_from("beforeend");
}
 else if ((*__match_val_13).tag == TAG_HtmxSwap_AfterEnd) {
__match_result_13 = rt_string_from("afterend");
}
 else if ((*__match_val_13).tag == TAG_HtmxSwap_Delete) {
__match_result_13 = rt_string_from("delete");
}
 else if ((*__match_val_13).tag == TAG_HtmxSwap_None) {
__match_result_13 = rt_string_from("none");
}
__match_result_13;
});
}

HtmxResponse HtmxResponse_new(rt_string_t html) {
return (HtmxResponse){.html_content = html, .target_selector = rt_string_from(""), .swap_style = (HtmxSwap){ .tag = TAG_HtmxSwap_InnerHTML }, .trigger_event = rt_string_from("")};
}

HtmxResponse HtmxResponse_target(HtmxResponse* self, rt_string_t selector) {
(self->target_selector = selector);
return self;
}

HtmxResponse HtmxResponse_swap(HtmxResponse* self, HtmxSwap style) {
(self->swap_style = style);
return self;
}

HtmxResponse HtmxResponse_trigger(HtmxResponse* self, rt_string_t event_name) {
(self->trigger_event = event_name);
return self;
}

rt_string_t HtmxResponse_render_html(HtmxResponse* self) {
return ({ rt_string_t _c = (*self).html_content; rt_string_retain(&_c); _c; });
}

