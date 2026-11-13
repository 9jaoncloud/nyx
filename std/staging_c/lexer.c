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
#include "runtime/rt_atomic.h"
#include "runtime/nyx_generational.h"
#include "include/nyx_multidialect.h"
#ifndef NYX_CURRENT_CONTEXT_DEFINED
#define NYX_CURRENT_CONTEXT_DEFINED
/* Odin-inspired Native Execution Context */
typedef struct NyxContext {
    void* nursery;
    void* temp_nursery;
    uint64_t thread_id;
    void (*logger)(const char* msg);
    void* user_data;
} NyxContext;
#if defined(_MSC_VER)
__declspec(selectany) __declspec(thread) NyxContext nyx_current_context = {0};
#elif defined(__GNUC__) || defined(__clang__)
__attribute__((weak)) __thread NyxContext nyx_current_context = {0};
#else
static __thread NyxContext nyx_current_context = {0};
#endif
#endif
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
    rt_string_t source;
    int64_t pos;
    int64_t line;
    int64_t col;
} Lexer;

Lexer new_lexer(rt_string_t);
Token Lexer_next_token(Lexer*);
void Lexer_advance(Lexer*);
char Lexer_peek(Lexer*);
void Lexer_skip_whitespace(Lexer*);
Token Lexer_read_number(Lexer*);
Token Lexer_read_string(Lexer*);
Token Lexer_read_ident(Lexer*);

#line 1 "lexer.nyx"
// module lexer
// import token::*
// import std::string
Lexer new_lexer(rt_string_t source) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (Lexer){.source = source, .pos = 0LL, .line = 1LL, .col = 1LL};
}

Token Lexer_next_token(Lexer* self) {
Lexer_skip_whitespace(self);
({ if (((*self).pos >= (*self).source.length)) {
return (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_EOF }, .line = (*self).line, .col = (*self).col};
}
});
char ch = (*self).source.data[(*self).pos];
return ({
Token __match_result_1;
char __match_val_1 = ch;
if (__match_val_1 == '(') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_LParen }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == ')') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_RParen }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '{') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_LBrace }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '}') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_RBrace }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '[') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_LBracket }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == ']') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_RBracket }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == ',') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Comma }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == ';') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Semicolon }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == ':') {
if ((Lexer_peek(self) == ':')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_PathSep }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Colon }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '=') {
if ((Lexer_peek(self) == '=')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_EqEq }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else if ((Lexer_peek(self) == '>')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_FatArrow }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Equals }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '+') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Plus }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '-') {
if ((Lexer_peek(self) == '>')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Arrow }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Minus }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '*') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Star }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '/') {
if ((Lexer_peek(self) == '/')) {
while ((((*self).pos < (*self).source.length) && ((*self).source.data[(*self).pos] != '\n'))) {
Lexer_advance(self);
}
;
__match_result_1 = Lexer_next_token(self);
} else if ((Lexer_peek(self) == '*')) {
Lexer_advance(self);
Lexer_advance(self);
while (((*self).pos < (*self).source.length)) {
({ if ((((*self).source.data[(*self).pos] == '*') && (Lexer_peek(self) == '/'))) {
Lexer_advance(self);
Lexer_advance(self);
break;
}
});
Lexer_advance(self);
}
;
__match_result_1 = Lexer_next_token(self);
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Slash }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '%') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Percent }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '<') {
if ((Lexer_peek(self) == '=')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_LtEq }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Lt }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '>') {
if ((Lexer_peek(self) == '=')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_GtEq }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Gt }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '&') {
Lexer_advance(self);
if ((Lexer_peek(self) == '&')) {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_And }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Ampersand }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '|') {
Lexer_advance(self);
if ((Lexer_peek(self) == '|')) {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Or }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Pipe }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '!') {
if ((Lexer_peek(self) == '=')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_NotEq }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Not }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '.') {
if ((Lexer_peek(self) == '.')) {
Lexer_advance(self);
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_DotDot }, .line = (*self).line, .col = ((*self).col - 2LL)};
} else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Dot }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
}
 else if (__match_val_1 == '?') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Question }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (__match_val_1 == '_') {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Underscore }, .line = (*self).line, .col = ((*self).col - 1LL)};
}
 else if (0) {
/* range pattern */
}
 else if (__match_val_1 == '\"') {
__match_result_1 = Lexer_read_string(self);
}
 else if (0 || 0) {
__match_result_1 = Lexer_read_ident(self);
}
 else {
Lexer_advance(self);
__match_result_1 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_EOF }, .line = (*self).line, .col = (*self).col};
}
__match_result_1;
});
}

void Lexer_advance(Lexer* self) {
({ if (((*self).source.data[(*self).pos] == '\n')) {
(self->line = ((*self).line + 1LL));
(self->col = 1LL);
} else {
(self->col = ((*self).col + 1LL));
}
});
(self->pos = ((*self).pos + 1LL));
}

char Lexer_peek(Lexer* self) {
if ((((*self).pos + 1LL) < (*self).source.length)) {
return (*self).source.data[((*self).pos + 1LL)];
} else {
return '\0';
}
}

void Lexer_skip_whitespace(Lexer* self) {
while (((*self).pos < (*self).source.length)) {
char ch = (*self).source.data[(*self).pos];
({ if (((((ch == ' ') || (ch == '\t')) || (ch == '\\')) || (ch == '\n'))) {
Lexer_advance(self);
} else {
break;
}
});
}
;
}

Token Lexer_read_number(Lexer* self) {
int64_t line = (*self).line;
int64_t col = (*self).col;
rt_string_t num_str = rt_string_from("");
int is_float = 0;
while (((*self).pos < (*self).source.length)) {
char ch = (*self).source.data[(*self).pos];
({ Token __if_res_6; if (((ch >= '0') && (ch <= '9'))) {
num_str = rt_string_concat(num_str, ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = ch; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; }));
Lexer_advance(self);
} else if (((ch == '.') && (!is_float))) {
is_float = 1;
num_str = rt_string_concat(num_str, ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = ch; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; }));
Lexer_advance(self);
} else {
break;
}
__if_res_6; });
}
;
if (is_float) {
return (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_FloatLit, .data.FloatLit = {.f0 = 0} }, .line = line, .col = col};
} else {
int64_t val = (int64_t)(0LL);
for (int64_t i = 0LL; i < num_str.length; i++) {
char ch = num_str.data[i];
val = ((val * 10LL) + ((int64_t)(ch) - (int64_t)('0')));
}
;
return (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_IntLit, .data.IntLit = {.f0 = val} }, .line = line, .col = col};
}
}

Token Lexer_read_string(Lexer* self) {
int64_t line = (*self).line;
int64_t col = (*self).col;
Lexer_advance(self);
rt_string_t content = rt_string_from("");
while ((((*self).pos < (*self).source.length) && ((*self).source.data[(*self).pos] != '\"'))) {
({ Token __if_res_7; if (((*self).source.data[(*self).pos] == '\\')) {
Lexer_advance(self);
({ if (((*self).pos < (*self).source.length)) {
char escaped = (*self).source.data[(*self).pos];
({
rt_string_t __match_result_8;
char __match_val_8 = escaped;
if (__match_val_8 == 'n') {
__match_result_8 = (content = rt_string_concat(content, rt_string_from("\n")));
}
 else if (__match_val_8 == 't') {
__match_result_8 = (content = rt_string_concat(content, rt_string_from("\t")));
}
 else if (__match_val_8 == 'r') {
__match_result_8 = (content = rt_string_concat(content, rt_string_from("\r")));
}
 else if (__match_val_8 == '\\') {
__match_result_8 = (content = rt_string_concat(content, rt_string_from("\\")));
}
 else if (__match_val_8 == '\"') {
__match_result_8 = (content = rt_string_concat(content, rt_string_from("\"")));
}
 else if (__match_val_8 == '0') {
__match_result_8 = (content = rt_string_concat(content, rt_string_from("\0")));
}
 else {
__match_result_8 = (content = rt_string_concat(content, ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = escaped; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; })));
}
__match_result_8;
});
Lexer_advance(self);
}
});
} else {
content = rt_string_concat(content, ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = (*self).source.data[(*self).pos]; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; }));
Lexer_advance(self);
}
__if_res_7; });
}
;
({ if (((*self).pos < (*self).source.length)) {
Lexer_advance(self);
}
});
return (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_StringLit, .data.StringLit = {.f0 = content} }, .line = line, .col = col};
}

Token Lexer_read_ident(Lexer* self) {
int64_t line = (*self).line;
int64_t col = (*self).col;
rt_string_t word = rt_string_from("");
while (((*self).pos < (*self).source.length)) {
char ch = (*self).source.data[(*self).pos];
({ Token __if_res_11; if ((((((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'))) || ((ch >= '0') && (ch <= '9'))) || (ch == '_'))) {
word = rt_string_concat(word, ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = ch; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; }));
Lexer_advance(self);
} else {
break;
}
__if_res_11; });
}
;
return ({
Token __match_result_11;
rt_string_t __match_val_11 = word;
if (rt_string_eq(__match_val_11, rt_string_from("fn"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Fn }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("let"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Let }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("mut"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Mut }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("if"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_If }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("else"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Else }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("while"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_While }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("for"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_For }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("in"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_In }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("return"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Return }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("struct"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Struct }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("enum"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Enum }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("match"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Match }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("trait"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Trait }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("impl"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Impl }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("import"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Import }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("module"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Module }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("pub"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Pub }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("async"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Async }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("await"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Await }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("true"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_True }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("false"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_False }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("self"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_SelfKw }, .line = line, .col = col};
}
 else if (rt_string_eq(__match_val_11, rt_string_from("null"))) {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Null }, .line = line, .col = col};
}
 else {
__match_result_11 = (Token){.kind = (TokenKind){ .tag = TAG_TokenKind_Ident, .data.Ident = {.f0 = word} }, .line = line, .col = col};
}
__match_result_11;
});
}

