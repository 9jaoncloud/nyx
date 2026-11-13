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
    rt_string_t pattern;
    int is_literal;
    rt_string_t literal;
} Regex;

typedef struct {
    rt_string_t match_type;
    int64_t next;
    int64_t alt_next;
    int accept;
} NFAState;

NyxResult compile(rt_string_t);
int is_match(Regex*, rt_string_t);
NyxOption find(Regex*, rt_string_t);
NyxVec find_all(Regex*, rt_string_t);
rt_string_t replace(Regex*, rt_string_t, rt_string_t);
NyxVec split(Regex*, rt_string_t);
int match_nfa(rt_string_t, rt_string_t);
NyxOption match_nfa_first(rt_string_t, rt_string_t);
NyxVec build_nfa(rt_string_t);
int matches_char(rt_string_t, char);
NyxVec epsilon_closure(NyxVec*, NyxVec);
// module regex
NyxResult compile(rt_string_t pattern) {
Arena* __arena = arena_create();
int is_literal = 1;
rt_string_t special = rt_string_from(".*+?^$[]{}()|\\");
for (int64_t _ch = 0; _ch < rt_string_chars(pattern).len; _ch++) {
    int64_t ch = *(int64_t*)rt_vec_get(rt_string_chars(pattern), _ch);
for (int64_t _s = 0; _s < rt_string_chars(special).len; _s++) {
    int64_t s = *(int64_t*)rt_vec_get(rt_string_chars(special), _s);
({ if ((ch == s)) {
is_literal = 0;
break;
}
});
}
;
({ if ((!is_literal)) {
break;
}
});
}
;
arena_destroy(__arena);
if (is_literal) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Regex _stmp_ = (Regex){.pattern = ({ rt_string_t _c = pattern; rt_string_retain(&_c); _c; }), .is_literal = 1, .literal = pattern}; &_stmp_; }); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Regex _stmp_ = (Regex){.pattern = ({ rt_string_t _c = pattern; rt_string_retain(&_c); _c; }), .is_literal = 0, .literal = rt_string_from("")}; &_stmp_; }); _tmp; });
}
}

int is_match(Regex* self, rt_string_t text) {
Arena* __arena = arena_create();
({ if ((*self).is_literal) {
return rt_string_contains(text, ({ rt_string_t _c = (*self).literal; rt_string_retain(&_c); _c; }));
}
});
arena_destroy(__arena);
return match_nfa(({ rt_string_t _c = (*self).pattern; rt_string_retain(&_c); _c; }), text);
}

NyxOption find(Regex* self, rt_string_t text) {
Arena* __arena = arena_create();
({ if ((*self).is_literal) {
NyxOption pos = rt_string_find(text, ({ rt_string_t _c = (*self).literal; rt_string_retain(&_c); _c; }));
({ if ((pos >= 0LL)) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_slice(text, (int64_t)(pos), ((int64_t)(pos) + (int64_t)((*self).literal.length))); _stmp_.data; }); _tmp; });
}
});
return rt_option_none();
}
});
arena_destroy(__arena);
return match_nfa_first(({ rt_string_t _c = (*self).pattern; rt_string_retain(&_c); _c; }), text);
}

NyxVec find_all(Regex* self, rt_string_t text) {
Arena* __arena = arena_create();
NyxVec results = rt_vec_new(sizeof(rt_string_t));
({ if ((*self).is_literal) {
int64_t pos = 0LL;
rt_string_t search_text = ({ rt_string_t _c = text; rt_string_retain(&_c); _c; });
while (1) {
int64_t found = rt_string_find_from(search_text, ({ rt_string_t _c = (*self).literal; rt_string_retain(&_c); _c; }), pos);
({ if ((found < 0LL)) {
break;
}
});
rt_vec_push(&results, ({ rt_string_t _tmp_7 = (int64_t)(rt_string_slice(search_text, (int64_t)(found), ((int64_t)(found) + (int64_t)((*self).literal.length)))); &_tmp_7; }));
pos = ((int64_t)(found) + (int64_t)((*self).literal.length));
({ if ((pos >= search_text.length)) {
break;
}
});
}
;
return results;
}
});
rt_string_t remaining = ({ rt_string_t _c = text; rt_string_retain(&_c); _c; });
while (1) {
({
rt_string_t __match_result_9;
NyxOption __match_val_9 = match_nfa_first(({ rt_string_t _c = (*self).pattern; rt_string_retain(&_c); _c; }), ({ rt_string_t _c = remaining; rt_string_retain(&_c); _c; }));
if (__match_val_9.tag == OPTION_SOME) {
    void* m = __match_val_9.data.ptr_val;
rt_vec_push(&results, (void*)(m /* clone ptr */));
int64_t m_len = (int64_t)(m->len);
({ if ((m_len == 0LL)) {
break;
}
});
__match_result_9 = (remaining = rt_string_slice(remaining, m_len, remaining.length));
}
 else if (__match_val_9.tag == OPTION_NONE) {
break;
}
__match_result_9;
});
}
;
arena_destroy(__arena);
return results;
}

rt_string_t replace(Regex* self, rt_string_t text, rt_string_t replacement) {
Arena* __arena = arena_create();
({ if ((*self).is_literal) {
return rt_string_replace(text, ({ rt_string_t _c = (*self).literal; rt_string_retain(&_c); _c; }), replacement);
}
});
rt_string_t result = rt_string_from("");
rt_string_t remaining = ({ rt_string_t _c = text; rt_string_retain(&_c); _c; });
while (1) {
({
rt_string_t __match_result_13;
NyxOption __match_val_13 = match_nfa_first(({ rt_string_t _c = (*self).pattern; rt_string_retain(&_c); _c; }), ({ rt_string_t _c = remaining; rt_string_retain(&_c); _c; }));
if (__match_val_13.tag == OPTION_SOME) {
    void* m = __match_val_13.data.ptr_val;
int64_t m_len = (int64_t)(m->len);
NyxOption pos = rt_string_find(remaining, m /* clone ptr */);
result = rt_string_concat(rt_string_concat(result, rt_string_slice(remaining, 0LL, (int64_t)(pos))), ({ rt_string_t _c = replacement; rt_string_retain(&_c); _c; }));
__match_result_13 = (remaining = rt_string_slice(remaining, ((int64_t)(pos) + m_len), remaining.length));
}
 else if (__match_val_13.tag == OPTION_NONE) {
result = rt_string_concat(result, remaining);
break;
}
__match_result_13;
});
}
;
arena_destroy(__arena);
return result;
}

NyxVec split(Regex* self, rt_string_t text) {
Arena* __arena = arena_create();
NyxVec results = rt_vec_new(sizeof(rt_string_t));
({ if ((*self).is_literal) {
NyxVec parts = rt_string_split(text, ({ rt_string_t _c = (*self).literal; rt_string_retain(&_c); _c; }));
for (int64_t _part = 0; _part < parts.len; _part++) {
    rt_string_t part = *(rt_string_t*)rt_vec_get(parts, _part);
rt_vec_push(&results, ({ rt_string_t _tmp_15 = (int64_t)(part); &_tmp_15; }));
}
;
return results;
}
});
rt_string_t remaining = ({ rt_string_t _c = text; rt_string_retain(&_c); _c; });
while (1) {
({
rt_string_t __match_result_16;
NyxOption __match_val_16 = match_nfa_first(({ rt_string_t _c = (*self).pattern; rt_string_retain(&_c); _c; }), ({ rt_string_t _c = remaining; rt_string_retain(&_c); _c; }));
if (__match_val_16.tag == OPTION_SOME) {
    void* m = __match_val_16.data.ptr_val;
int64_t m_len = (int64_t)(m->len);
NyxOption pos = rt_string_find(remaining, m /* clone ptr */);
rt_vec_push(&results, ({ rt_string_t _tmp_17 = (int64_t)(rt_string_slice(remaining, 0LL, (int64_t)(pos))); &_tmp_17; }));
__match_result_16 = (remaining = rt_string_slice(remaining, ((int64_t)(pos) + m_len), remaining.length));
}
 else if (__match_val_16.tag == OPTION_NONE) {
rt_vec_push(&results, ({ rt_string_t _tmp_18 = (int64_t)(remaining); &_tmp_18; }));
break;
}
__match_result_16;
});
}
;
arena_destroy(__arena);
return results;
}

int match_nfa(rt_string_t pattern, rt_string_t text) {
Arena* __arena = arena_create();
NyxVec states = build_nfa(pattern);
NyxVec current = rt_vec_new(sizeof(int64_t));
rt_vec_push(&current, ({ int64_t _tmp_19 = (int64_t)(0LL); &_tmp_19; }));
current = epsilon_closure(({ NyxVec _atmp_0 = (&(states)); &_atmp_0; }), current);
for (int64_t i = 0LL; i < text.length; i++) {
int64_t ch = rt_string_char_at(text, (int64_t)(i));
NyxVec next = rt_vec_new(sizeof(int64_t));
for (int64_t _state = 0; _state < current.len; _state++) {
    int64_t state = *(int64_t*)rt_vec_get(current, _state);
({ if ((state < (int64_t)(states.len))) {
NFAState transition = (&((*(NFAState*)rt_vec_get(states, (int64_t)(state)))));
({ if (matches_char(({ rt_string_t _c = transition.match_type; rt_string_retain(&_c); _c; }), ch)) {
rt_vec_push(&next, ({ int64_t _tmp_22 = (int64_t)(transition.next); &_tmp_22; }));
}
});
({ if ((transition.alt_next >= 0LL)) {
rt_vec_push(&next, ({ int64_t _tmp_24 = (int64_t)((int64_t)(transition.alt_next)); &_tmp_24; }));
}
});
}
});
}
;
current = epsilon_closure(({ NyxVec _atmp_0 = (&(states)); &_atmp_0; }), next);
}
;
for (int64_t _state = 0; _state < current.len; _state++) {
    int64_t state = *(int64_t*)rt_vec_get(current, _state);
({ if (((state < (int64_t)(states.len)) && ((*(NFAState*)rt_vec_get(states, (int64_t)(state)))).accept)) {
return 1;
}
});
}
;
arena_destroy(__arena);
return 0;
}

NyxOption match_nfa_first(rt_string_t pattern, rt_string_t text) {
Arena* __arena = arena_create();
for (int64_t end = 1LL; end <= text.length; end++) {
rt_string_t sub = rt_string_slice(text, 0LL, (int64_t)(end));
({ if (match_nfa(({ rt_string_t _c = pattern; rt_string_retain(&_c); _c; }), sub)) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = sub; _stmp_.data; }); _tmp; });
}
});
}
;
arena_destroy(__arena);
return rt_option_none();
}

NyxVec build_nfa(rt_string_t pattern) {
Arena* __arena = arena_create();
NyxVec states = rt_vec_new(sizeof(NFAState));
int64_t chars = rt_string_chars(pattern);
int64_t i = 0LL;
rt_string_t p = ({ rt_string_t _c = pattern; rt_string_retain(&_c); _c; });
while ((i < p.length)) {
int64_t ch = rt_string_char_at(p, (int64_t)(i));
int64_t state_idx = (int64_t)(states.len);
({
int64_t __match_result_27;
int64_t __match_val_27 = ch;
if (__match_val_27 == '.') {
rt_vec_push(&states, ({ NFAState _tmp_28 = (int64_t)((NFAState){.match_type = rt_string_from("any"), .next = (state_idx + 1LL), .alt_next = (-1LL), .accept = 0}); &_tmp_28; }));
}
 else if (__match_val_27 == '\\') {
i = (i + 1LL);
({ if ((i < p.length)) {
int64_t esc = rt_string_char_at(p, (int64_t)(i));
NyxVec mt = ({
rt_string_t __match_result_30;
int64_t __match_val_30 = esc;
if (__match_val_30 == 'd') {
__match_result_30 = rt_string_from("digit");
}
 else if (__match_val_30 == 'w') {
__match_result_30 = rt_string_from("word");
}
 else if (__match_val_30 == 's') {
__match_result_30 = rt_string_from("space");
}
 else {
__match_result_30 = rt_string_concat(rt_string_from("literal:"), rt_string_from_char((char)(esc)));
}
__match_result_30;
});
rt_vec_push(&states, ({ NFAState _tmp_31 = (int64_t)((NFAState){.match_type = mt, .next = (state_idx + 1LL), .alt_next = (-1LL), .accept = 0}); &_tmp_31; }));
}
});
}
 else if (__match_val_27 == '*') {
({ if ((states.len > 0LL)) {
int64_t last = (states.len - 1LL);
((*(int64_t*)rt_vec_get(states, last)).next = state_idx);
((*(int64_t*)rt_vec_get(states, last)).alt_next = ((int64_t)(state_idx) + 1LL));
}
});
rt_vec_push(&states, ({ NFAState _tmp_33 = (int64_t)((NFAState){.match_type = rt_string_from("none"), .next = state_idx, .alt_next = (state_idx + 1LL), .accept = 0}); &_tmp_33; }));
}
 else if (__match_val_27 == '+') {
({ if ((states.len > 0LL)) {
int64_t last = (states.len - 1LL);
((*(int64_t*)rt_vec_get(states, last)).next = state_idx);
((*(int64_t*)rt_vec_get(states, last)).alt_next = ((int64_t)(state_idx) + 1LL));
}
});
rt_vec_push(&states, ({ NFAState _tmp_35 = (int64_t)((NFAState){.match_type = rt_string_from("none"), .next = state_idx, .alt_next = (state_idx + 1LL), .accept = 0}); &_tmp_35; }));
}
 else if (__match_val_27 == '?') {
({ if ((states.len > 0LL)) {
int64_t last = (states.len - 1LL);
((*(int64_t*)rt_vec_get(states, last)).alt_next = ((int64_t)(state_idx) + 1LL));
}
});
rt_vec_push(&states, ({ NFAState _tmp_37 = (int64_t)((NFAState){.match_type = rt_string_from("none"), .next = (state_idx + 1LL), .alt_next = (state_idx + 1LL), .accept = 0}); &_tmp_37; }));
}
 else if (__match_val_27 == '^') {
}
 else if (__match_val_27 == '$') {
({ if ((states.len > 0LL)) {
int64_t last = (states.len - 1LL);
((*(int64_t*)rt_vec_get(states, last)).accept = 1);
}
});
}
 else if (__match_val_27 == '|') {
rt_vec_push(&states, ({ NFAState _tmp_39 = (int64_t)((NFAState){.match_type = rt_string_from("any"), .next = (state_idx + 1LL), .alt_next = (-1LL), .accept = 0}); &_tmp_39; }));
}
 else if (__match_val_27 == '[') {
rt_string_t class_str = rt_string_from("");
i = (i + 1LL);
while (((i < p.length) && (rt_string_char_at(p, (int64_t)(i)) != ']'))) {
class_str = rt_string_concat(class_str, rt_string_from_char((char)(rt_string_char_at(p, (int64_t)(i)))));
i = (i + 1LL);
}
;
rt_vec_push(&states, ({ NFAState _tmp_40 = (int64_t)((NFAState){.match_type = rt_string_concat(rt_string_from("class:"), class_str), .next = (state_idx + 1LL), .alt_next = (-1LL), .accept = 0}); &_tmp_40; }));
}
 else if (__match_val_27 == '(' || __match_val_27 == ')') {
}
 else {
rt_vec_push(&states, ({ NFAState _tmp_41 = (int64_t)((NFAState){.match_type = rt_string_concat(rt_string_from("literal:"), rt_string_from_char((char)(ch))), .next = (state_idx + 1LL), .alt_next = (-1LL), .accept = 0}); &_tmp_41; }));
}
__match_result_27;
});
i = (i + 1LL);
}
;
({ if ((states.len > 0LL)) {
int64_t last = (states.len - 1LL);
((*(int64_t*)rt_vec_get(states, last)).accept = 1);
}
});
arena_destroy(__arena);
return states;
}

int matches_char(rt_string_t match_type, char ch) {
Arena* __arena = arena_create();
({ if (rt_string_eq(match_type, rt_string_from("any"))) {
return (ch != '\n');
}
});
({ if (rt_string_eq(match_type, rt_string_from("digit"))) {
return ((ch >= '0') && (ch <= '9'));
}
});
({ if (rt_string_eq(match_type, rt_string_from("word"))) {
return (((((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'))) || ((ch >= '0') && (ch <= '9'))) || (ch == '_'));
}
});
({ if (rt_string_eq(match_type, rt_string_from("space"))) {
return ((((ch == ' ') || (ch == '\t')) || (ch == '\n')) || (ch == '\\'));
}
});
({ if (rt_string_starts_with(match_type, rt_string_from("literal:"))) {
rt_string_t expected = rt_string_slice(match_type, 7LL, match_type.length);
return rt_string_eq(({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = ch; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; }), expected);
}
});
({ if (rt_string_starts_with(match_type, rt_string_from("class:"))) {
rt_string_t class_str = rt_string_slice(match_type, 6LL, match_type.length);
for (int64_t _class_ch = 0; _class_ch < rt_string_chars(class_str).len; _class_ch++) {
    int64_t class_ch = *(int64_t*)rt_vec_get(rt_string_chars(class_str), _class_ch);
({ if ((ch == class_ch)) {
return 1;
}
});
}
;
return 0;
}
});
arena_destroy(__arena);
return 0;
}

NyxVec epsilon_closure(NyxVec* states, NyxVec initial) {
Arena* __arena = arena_create();
NyxVec result = rt_vec_new(sizeof(int64_t));
NyxVec stack = rt_vec_new(sizeof(int64_t));
for (int64_t _s = 0; _s < initial.len; _s++) {
    int64_t s = *(int64_t*)rt_vec_get(initial, _s);
rt_vec_push(&stack, ({ int64_t _tmp_50 = (int64_t)(s); &_tmp_50; }));
}
;
while ((stack.len > 0LL)) {
rt_string_t s = rt_string_from((char*)({ NyxOption _u = rt_vec_pop(&(stack)); _u.data.ptr_val; }));
({ if ((s < (int64_t)(states->len))) {
rt_vec_push(&result, ({ rt_string_t _tmp_52 = (int64_t)(s); &_tmp_52; }));
NFAState state = (&((*(NFAState*)rt_vec_get(*states, (int64_t)(s)))));
({ if (rt_string_eq(state.match_type, rt_string_from("none"))) {
({ if ((state.next < (int64_t)(states->len))) {
rt_vec_push(&stack, ({ int64_t _tmp_55 = (int64_t)(state.next); &_tmp_55; }));
}
});
({ if (((state.alt_next >= 0LL) && ((int64_t)(state.alt_next) < (int64_t)(states->len)))) {
rt_vec_push(&stack, ({ int64_t _tmp_57 = (int64_t)((int64_t)(state.alt_next)); &_tmp_57; }));
}
});
}
});
}
});
}
;
arena_destroy(__arena);
return result;
}

