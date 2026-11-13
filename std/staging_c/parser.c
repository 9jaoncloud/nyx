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
    Lexer lexer;
    Token current;
    int had_error;
    NyxVec errors;
} Parser;

Parser new_parser(rt_string_t);
NyxVec Parser_parse_program(Parser*);
void Parser_advance(Parser*);
int Parser_check(Parser*, TokenKind);
void Parser_expect(Parser*, TokenKind);
void Parser_error(Parser*, rt_string_t);
void Parser_skip_to_semicolon(Parser*);
rt_string_t Parser_parse_ident(Parser*);
Type Parser_parse_type(Parser*);
TopLevel Parser_parse_fn_decl(Parser*, int);
TopLevel Parser_parse_struct_def(Parser*);
TopLevel Parser_parse_enum_def(Parser*);
Expr Parser_parse_block_expr(Parser*);
NyxVec Parser_parse_block(Parser*);
Stmt Parser_parse_stmt(Parser*);
Expr Parser_parse_expr(Parser*);
Expr Parser_pratt(Parser*, int64_t);
Expr Parser_parse_prefix(Parser*);
NyxVec Parser_parse_call_args(Parser*);
Expr Parser_parse_if(Parser*);
Expr Parser_parse_while(Parser*);
Expr Parser_parse_for(Parser*);
Expr Parser_parse_match(Parser*);
Pattern Parser_parse_pattern(Parser*);
Expr Parser_parse_lambda(Parser*);

#line 1 "parser.nyx"
// module parser
// import token::*
// import lexer::*
// import ast::*
Parser new_parser(rt_string_t source) {
Arena* __arena = arena_create();
Lexer lex = new_lexer(source);
arena_destroy(__arena);
return (Parser){.lexer = lex, .current = Lexer_next_token(&(lex)), .had_error = 0, .errors = rt_vec_new(sizeof(rt_string_t))};
}

NyxVec Parser_parse_program(Parser* self) {
NyxVec items = rt_vec_new(sizeof(TopLevel));
while (1) {
({
int64_t __match_result_0;
TokenKind* __match_val_0 = (&(((*self).current).kind));
if ((*__match_val_0).tag == TAG_TokenKind_Fn) {
rt_vec_push(&items, ({ TopLevel _tmp_1 = (Parser_parse_fn_decl(self, 0)); &_tmp_1; }));
}
 else if ((*__match_val_0).tag == TAG_TokenKind_Async) {
Parser_advance(self);
rt_vec_push(&items, ({ TopLevel _tmp_2 = (Parser_parse_fn_decl(self, 1)); &_tmp_2; }));
}
 else if ((*__match_val_0).tag == TAG_TokenKind_Struct) {
rt_vec_push(&items, ({ TopLevel _tmp_3 = (Parser_parse_struct_def(self)); &_tmp_3; }));
}
 else if ((*__match_val_0).tag == TAG_TokenKind_Enum) {
rt_vec_push(&items, ({ TopLevel _tmp_4 = (Parser_parse_enum_def(self)); &_tmp_4; }));
}
 else if ((*__match_val_0).tag == TAG_TokenKind_Import) {
Parser_advance(self);
Parser_skip_to_semicolon(self);
}
 else if ((*__match_val_0).tag == TAG_TokenKind_Module) {
Parser_advance(self);
Parser_skip_to_semicolon(self);
}
 else if ((*__match_val_0).tag == TAG_TokenKind_Extern) {
Parser_advance(self);
Parser_skip_to_semicolon(self);
}
 else if ((*__match_val_0).tag == TAG_TokenKind_EOF) {
break;
}
 else {
Parser_error(self, rt_string_from("Unexpected token"));
Parser_advance(self);
}
__match_result_0;
});
}
;
return items;
}

void Parser_advance(Parser* self) {
(self->current = Lexer_next_token(&((*self).lexer)));
}

int Parser_check(Parser* self, TokenKind kind) {
return (((*self).current).kind.tag == kind.tag);
}

void Parser_expect(Parser* self, TokenKind kind) {
({ if ((((*self).current).kind.tag == kind.tag)) {
Parser_advance(self);
} else {
Parser_error(self, rt_string_from("Unexpected token"));
}
});
}

void Parser_error(Parser* self, rt_string_t msg) {
(self->had_error = 1);
rt_vec_push(&(*self).errors, ({ rt_string_t _tmp_6 = (msg); &_tmp_6; }));
}

void Parser_skip_to_semicolon(Parser* self) {
while ((!(((*self).current).kind.tag == TAG_TokenKind_Semicolon || ((*self).current).kind.tag == TAG_TokenKind_EOF))) {
Parser_advance(self);
}
;
({ if ((((*self).current).kind.tag == TAG_TokenKind_Semicolon)) {
Parser_advance(self);
}
});
}

rt_string_t Parser_parse_ident(Parser* self) {
/* iflet */ 0;
(self->had_error = 1);
return rt_string_from("unknown");
}

Type Parser_parse_type(Parser* self) {
{ TokenKind* __iflet_8 = (&(((*self).current).kind));
if ((*__iflet_8).tag == TAG_TokenKind_Ident) {
    rt_string_t name = (*__iflet_8).data.Ident.f0;
Parser_advance(self);
return ({
Type __match_result_9;
rt_string_t __match_val_9 = name;
if (rt_string_eq(__match_val_9, rt_string_from("Int"))) {
__match_result_9 = (Type){ .tag = TAG_Type_Int };
}
 else if (rt_string_eq(__match_val_9, rt_string_from("Float"))) {
__match_result_9 = (Type){ .tag = TAG_Type_Float };
}
 else if (rt_string_eq(__match_val_9, rt_string_from("Bool"))) {
__match_result_9 = (Type){ .tag = TAG_Type_Bool };
}
 else if (rt_string_eq(__match_val_9, rt_string_from("String"))) {
__match_result_9 = (Type){ .tag = TAG_Type_String };
}
 else if (rt_string_eq(__match_val_9, rt_string_from("Char"))) {
__match_result_9 = (Type){ .tag = TAG_Type_Char };
}
 else if (rt_string_eq(__match_val_9, rt_string_from("Void"))) {
__match_result_9 = (Type){ .tag = TAG_Type_Void };
}
 else {
__match_result_9 = (Type){ .tag = TAG_Type_Custom, .data.Custom = {.f0 = name} };
}
__match_result_9;
});
} else if ((((*self).current).kind.tag == TAG_TokenKind_LBracket)) {
Parser_advance(self);
Type inner = Parser_parse_type(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBracket });
return (Type){ .tag = TAG_Type_Array, .data.Array = {.f0 = ({ Type _addr_tmp_0 = inner; &_addr_tmp_0; })} };
} else if ((((*self).current).kind.tag == TAG_TokenKind_Ampersand)) {
Parser_advance(self);
return ({ Type __if_res_11; if ((((*self).current).kind.tag == TAG_TokenKind_Mut)) {
Parser_advance(self);
__if_res_11 = (Type){ .tag = TAG_Type_MutRef, .data.MutRef = {.f0 = ({ Type _addr_tmp_0 = Parser_parse_type(self); &_addr_tmp_0; })} };
} else {
__if_res_11 = (Type){ .tag = TAG_Type_Ref, .data.Ref = {.f0 = ({ Type _addr_tmp_0 = Parser_parse_type(self); &_addr_tmp_0; })} };
}
__if_res_11; });
} else {
(self->had_error = 1);
return (Type){ .tag = TAG_Type_Void };
}
}
}

TopLevel Parser_parse_fn_decl(Parser* self, int is_async) {
Parser_advance(self);
rt_string_t name = Parser_parse_ident(self);
NyxVec generics = rt_vec_new(sizeof(int64_t));
({ if ((((*self).current).kind.tag == TAG_TokenKind_Lt)) {
Parser_advance(self);
while (1) {
rt_vec_push(&generics, ({ rt_string_t _tmp_12 = (Parser_parse_ident(self)); &_tmp_12; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Gt });
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LParen });
NyxVec params = rt_vec_new(sizeof(int64_t));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_RParen))) {
while (1) {
rt_string_t pname = Parser_parse_ident(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Colon });
Type ptype = Parser_parse_type(self);
rt_vec_push(&params, ({ Param _tmp_15 = ((Param){.name = pname, .ty = ptype}); &_tmp_15; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RParen });
Type ret_type = ({ Type __if_res_18; if ((((*self).current).kind.tag == TAG_TokenKind_Arrow)) {
Parser_advance(self);
__if_res_18 = Parser_parse_type(self);
} else {
__if_res_18 = (Type){ .tag = TAG_Type_Void };
}
__if_res_18; });
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
Expr body = Parser_parse_block_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
return (TopLevel){ .tag = TAG_TopLevel_FnDecl, .data.FnDecl = { .f0 = name, .f1 = generics, .f2 = params, .f3 = ret_type, .f4 = body, .f5 = is_async } };
}

TopLevel Parser_parse_struct_def(Parser* self) {
Parser_advance(self);
rt_string_t name = Parser_parse_ident(self);
NyxVec generics = rt_vec_new(sizeof(int64_t));
({ if ((((*self).current).kind.tag == TAG_TokenKind_Lt)) {
Parser_advance(self);
while (1) {
rt_vec_push(&generics, ({ rt_string_t _tmp_19 = (Parser_parse_ident(self)); &_tmp_19; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Gt });
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec fields = rt_vec_new(sizeof(int64_t));
while ((!(((*self).current).kind.tag == TAG_TokenKind_RBrace))) {
rt_string_t fname = Parser_parse_ident(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Colon });
Type ftype = Parser_parse_type(self);
rt_vec_push(&fields, ({ NyxTuple2 _tmp_21 = (({ NyxTuple2 _tuple_22 = {.f0 = (void*)({ rt_string_t _stmp_0 = fname; _stmp_0.data; }), .f1 = (void*)({ Type _stmp_1 = ftype; &_stmp_1; }) }; _tuple_22; })); &_tmp_21; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
return (TopLevel){ .tag = TAG_TopLevel_StructDef, .data.StructDef = { .f0 = name, .f1 = generics, .f2 = fields } };
}

TopLevel Parser_parse_enum_def(Parser* self) {
Parser_advance(self);
rt_string_t name = Parser_parse_ident(self);
NyxVec generics = rt_vec_new(sizeof(int64_t));
({ if ((((*self).current).kind.tag == TAG_TokenKind_Lt)) {
Parser_advance(self);
while (1) {
rt_vec_push(&generics, ({ rt_string_t _tmp_25 = (Parser_parse_ident(self)); &_tmp_25; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Gt });
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec variants = rt_vec_new(sizeof(int64_t));
while ((!(((*self).current).kind.tag == TAG_TokenKind_RBrace))) {
rt_string_t vname = Parser_parse_ident(self);
NyxVec args = rt_vec_new(sizeof(int64_t));
({ if ((((*self).current).kind.tag == TAG_TokenKind_LParen)) {
Parser_advance(self);
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_RParen))) {
while (1) {
rt_vec_push(&args, ({ Type _tmp_29 = (Parser_parse_type(self)); &_tmp_29; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RParen });
}
});
rt_vec_push(&variants, ({ NyxTuple2 _tmp_31 = (({ NyxTuple2 _tuple_32 = {.f0 = (void*)({ rt_string_t _stmp_0 = vname; _stmp_0.data; }), .f1 = (void*)({ NyxVec _stmp_1 = args; &_stmp_1; }) }; _tuple_32; })); &_tmp_31; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
return (TopLevel){ .tag = TAG_TopLevel_EnumDef, .data.EnumDef = { .f0 = name, .f1 = generics, .f2 = variants } };
}

Expr Parser_parse_block_expr(Parser* self) {
NyxVec stmts = rt_vec_new(sizeof(int64_t));
while ((!(((*self).current).kind.tag == TAG_TokenKind_RBrace || ((*self).current).kind.tag == TAG_TokenKind_EOF))) {
rt_vec_push(&stmts, ({ Stmt _tmp_34 = (Parser_parse_stmt(self)); &_tmp_34; }));
}
;
return (Expr){ .tag = TAG_Expr_Block, .data.Block = {.f0 = stmts} };
}

NyxVec Parser_parse_block(Parser* self) {
NyxVec stmts = rt_vec_new(sizeof(Stmt));
while ((!(((*self).current).kind.tag == TAG_TokenKind_RBrace || ((*self).current).kind.tag == TAG_TokenKind_EOF))) {
rt_vec_push(&stmts, ({ Stmt _tmp_35 = (Parser_parse_stmt(self)); &_tmp_35; }));
}
;
return stmts;
}

Stmt Parser_parse_stmt(Parser* self) {
return ({
Stmt __match_result_36;
TokenKind* __match_val_36 = (&(((*self).current).kind));
if ((*__match_val_36).tag == TAG_TokenKind_Let) {
Parser_advance(self);
int64_t mutable = (((*self).current).kind.tag == TAG_TokenKind_Mut);
({ if (mutable) {
Parser_advance(self);
}
});
rt_string_t name = Parser_parse_ident(self);
NyxOption ty = rt_option_none();
({ if ((((*self).current).kind.tag == TAG_TokenKind_Colon)) {
Parser_advance(self);
ty = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ Type _stmp_ = Parser_parse_type(self); &_stmp_; }); _tmp; });
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Equals });
Expr value = Parser_parse_expr(self);
({ if ((((*self).current).kind.tag == TAG_TokenKind_Semicolon)) {
Parser_advance(self);
}
});
__match_result_36 = (Stmt){ .tag = TAG_Stmt_Let, .data.Let = { .f0 = name, .f1 = mutable, .f2 = ty, .f3 = value } };
}
 else if ((*__match_val_36).tag == TAG_TokenKind_Return) {
Parser_advance(self);
NyxOption expr = ({ NyxOption __if_res_41; if ((!(((*self).current).kind.tag == TAG_TokenKind_Semicolon || ((*self).current).kind.tag == TAG_TokenKind_RBrace))) {
__if_res_41 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ Expr _stmp_ = Parser_parse_expr(self); &_stmp_; }); _tmp; });
} else {
__if_res_41 = rt_option_none();
}
__if_res_41; });
({ if ((((*self).current).kind.tag == TAG_TokenKind_Semicolon)) {
Parser_advance(self);
}
});
__match_result_36 = (Stmt){ .tag = TAG_Stmt_Return, .data.Return = {.f0 = expr} };
}
 else {
Expr expr = Parser_parse_expr(self);
({ if ((((*self).current).kind.tag == TAG_TokenKind_Semicolon)) {
Parser_advance(self);
}
});
__match_result_36 = (Stmt){ .tag = TAG_Stmt_Expr, .data.Expr = {.f0 = expr} };
}
__match_result_36;
});
}

Expr Parser_parse_expr(Parser* self) {
return Parser_pratt(self, 0LL);
}

Expr Parser_pratt(Parser* self, int64_t min_bp) {
Expr lhs = Parser_parse_prefix(self);
while (1) {
NyxTuple3 _tmp_bin_op_l_bp_r_bp = ({
NyxTuple3 __match_result_43;
TokenKind* __match_val_43 = (&(((*self).current).kind));
if ((*__match_val_43).tag == TAG_TokenKind_Plus) {
__match_result_43 = ({ NyxTuple3 _tuple_44 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Add }; &_stmp_0; }), .f1 = (void*)(7LL), .f2 = (void*)(8LL) }; _tuple_44; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Minus) {
__match_result_43 = ({ NyxTuple3 _tuple_45 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Sub }; &_stmp_0; }), .f1 = (void*)(7LL), .f2 = (void*)(8LL) }; _tuple_45; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Star) {
__match_result_43 = ({ NyxTuple3 _tuple_46 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Mul }; &_stmp_0; }), .f1 = (void*)(9LL), .f2 = (void*)(10LL) }; _tuple_46; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Slash) {
__match_result_43 = ({ NyxTuple3 _tuple_47 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Div }; &_stmp_0; }), .f1 = (void*)(9LL), .f2 = (void*)(10LL) }; _tuple_47; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Percent) {
__match_result_43 = ({ NyxTuple3 _tuple_48 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Mod }; &_stmp_0; }), .f1 = (void*)(9LL), .f2 = (void*)(10LL) }; _tuple_48; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_EqEq) {
__match_result_43 = ({ NyxTuple3 _tuple_49 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Eq }; &_stmp_0; }), .f1 = (void*)(5LL), .f2 = (void*)(6LL) }; _tuple_49; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_NotEq) {
__match_result_43 = ({ NyxTuple3 _tuple_50 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_NotEq }; &_stmp_0; }), .f1 = (void*)(5LL), .f2 = (void*)(6LL) }; _tuple_50; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Lt) {
__match_result_43 = ({ NyxTuple3 _tuple_51 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Lt }; &_stmp_0; }), .f1 = (void*)(5LL), .f2 = (void*)(6LL) }; _tuple_51; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_LtEq) {
__match_result_43 = ({ NyxTuple3 _tuple_52 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_LtEq }; &_stmp_0; }), .f1 = (void*)(5LL), .f2 = (void*)(6LL) }; _tuple_52; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Gt) {
__match_result_43 = ({ NyxTuple3 _tuple_53 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Gt }; &_stmp_0; }), .f1 = (void*)(5LL), .f2 = (void*)(6LL) }; _tuple_53; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_GtEq) {
__match_result_43 = ({ NyxTuple3 _tuple_54 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_GtEq }; &_stmp_0; }), .f1 = (void*)(5LL), .f2 = (void*)(6LL) }; _tuple_54; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_And) {
__match_result_43 = ({ NyxTuple3 _tuple_55 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_And }; &_stmp_0; }), .f1 = (void*)(3LL), .f2 = (void*)(4LL) }; _tuple_55; });
}
 else if ((*__match_val_43).tag == TAG_TokenKind_Or) {
__match_result_43 = ({ NyxTuple3 _tuple_56 = {.f0 = (void*)({ BinOp _stmp_0 = (BinOp){ .tag = TAG_BinOp_Or }; &_stmp_0; }), .f1 = (void*)(1LL), .f2 = (void*)(2LL) }; _tuple_56; });
}
 else {
break;
}
__match_result_43;
});
int64_t bin_op = (int64_t)_tmp_bin_op_l_bp_r_bp.f0; // tuple destructuring
int64_t l_bp = (int64_t)_tmp_bin_op_l_bp_r_bp.f1; // tuple destructuring
int64_t r_bp = (int64_t)_tmp_bin_op_l_bp_r_bp.f2; // tuple destructuring
({ if ((l_bp < min_bp)) {
break;
}
});
Parser_advance(self);
Expr rhs = Parser_pratt(self, r_bp);
lhs = (Expr){ .tag = TAG_Expr_Binary, .data.Binary = { .f0 = bin_op, .f1 = ({ Expr _stmp_1 = lhs; &_stmp_1; }), .f2 = ({ Expr _stmp_2 = rhs; &_stmp_2; }) } };
while ((((*self).current).kind.tag == TAG_TokenKind_Dot)) {
Parser_advance(self);
rt_string_t field = Parser_parse_ident(self);
({ Expr __if_res_59; if ((((*self).current).kind.tag == TAG_TokenKind_LParen)) {
NyxVec args = Parser_parse_call_args(self);
__if_res_59 = (lhs = (Expr){ .tag = TAG_Expr_MethodCall, .data.MethodCall = { .f0 = ({ Expr _stmp_0 = lhs; &_stmp_0; }), .f1 = field, .f2 = rt_vec_new(sizeof(int64_t)), .f3 = args } });
} else {
__if_res_59 = (lhs = (Expr){ .tag = TAG_Expr_FieldAccess, .data.FieldAccess = { .f0 = ({ Expr _stmp_0 = lhs; &_stmp_0; }), .f1 = field } });
}
__if_res_59; });
}
;
}
;
return lhs;
}

Expr Parser_parse_prefix(Parser* self) {
return ({
Expr __match_result_59;
TokenKind* __match_val_59 = ({ TokenKind _utmp_ = ((*self).current).kind; &_utmp_; });
if ((*__match_val_59).tag == TAG_TokenKind_IntLit) {
    int64_t n = (*__match_val_59).data.IntLit.f0;
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Int, .data.Int = {.f0 = n} }} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_FloatLit) {
    double f = (*__match_val_59).data.FloatLit.f0;
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Float, .data.Float = {.f0 = f} }} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_StringLit) {
    rt_string_t s = (*__match_val_59).data.StringLit.f0;
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_String, .data.String = {.f0 = ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })} }} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_True) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Bool, .data.Bool = {.f0 = 1} }} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_False) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Bool, .data.Bool = {.f0 = 0} }} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Null) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Int, .data.Int = {.f0 = 0LL} }} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Ident) {
    rt_string_t name = (*__match_val_59).data.Ident.f0;
Parser_advance(self);
if ((((*self).current).kind.tag == TAG_TokenKind_LParen)) {
NyxVec args = Parser_parse_call_args(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Call, .data.Call = { .f0 = ({ Expr _stmp_0 = (Expr){ .tag = TAG_Expr_Ident, .data.Ident = {.f0 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })} }; &_stmp_0; }), .f1 = rt_vec_new(sizeof(int64_t)), .f2 = args } };
} else if ((((*self).current).kind.tag == TAG_TokenKind_LBrace)) {
Parser_advance(self);
NyxVec fields = rt_vec_new(sizeof(int64_t));
while ((!(((*self).current).kind.tag == TAG_TokenKind_RBrace))) {
rt_string_t fname = Parser_parse_ident(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Colon });
Expr val = Parser_parse_expr(self);
rt_vec_push(&fields, ({ NyxTuple2 _tmp_60 = (({ NyxTuple2 _tuple_61 = {.f0 = (void*)({ rt_string_t _stmp_0 = fname; _stmp_0.data; }), .f1 = (void*)({ Expr _stmp_1 = val; &_stmp_1; }) }; _tuple_61; })); &_tmp_60; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
__match_result_59 = (Expr){ .tag = TAG_Expr_StructLiteral, .data.StructLiteral = { .f0 = name, .f1 = rt_vec_new(sizeof(int64_t)), .f2 = fields } };
} else {
__match_result_59 = (Expr){ .tag = TAG_Expr_Ident, .data.Ident = {.f0 = name} };
}
}
 else if ((*__match_val_59).tag == TAG_TokenKind_LParen) {
Parser_advance(self);
Expr expr = Parser_parse_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RParen });
__match_result_59 = expr;
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Not) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Unary, .data.Unary = { .f0 = (UnaryOp){ .tag = TAG_UnaryOp_Not }, .f1 = ({ Expr _stmp_1 = Parser_parse_prefix(self); &_stmp_1; }) } };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Minus) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Unary, .data.Unary = { .f0 = (UnaryOp){ .tag = TAG_UnaryOp_Neg }, .f1 = ({ Expr _stmp_1 = Parser_parse_prefix(self); &_stmp_1; }) } };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_If) {
__match_result_59 = Parser_parse_if(self);
}
 else if ((*__match_val_59).tag == TAG_TokenKind_While) {
__match_result_59 = Parser_parse_while(self);
}
 else if ((*__match_val_59).tag == TAG_TokenKind_For) {
__match_result_59 = Parser_parse_for(self);
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Match) {
__match_result_59 = Parser_parse_match(self);
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Fn) {
__match_result_59 = Parser_parse_lambda(self);
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Pipe) {
__match_result_59 = Parser_parse_lambda(self);
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Return) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Return, .data.Return = {.f0 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ Expr _stmp_ = Parser_parse_expr(self); &_stmp_; }); _tmp; })} };
}
 else if ((*__match_val_59).tag == TAG_TokenKind_Await) {
Parser_advance(self);
__match_result_59 = (Expr){ .tag = TAG_Expr_Await, .data.Await = {.f0 = ({ Expr _addr_tmp_0 = Parser_parse_expr(self); &_addr_tmp_0; })} };
}
 else {
Parser_error(self, rt_string_from("Unexpected token in expression"));
__match_result_59 = (Expr){ .tag = TAG_Expr_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Int, .data.Int = {.f0 = 0LL} }} };
}
__match_result_59;
});
}

NyxVec Parser_parse_call_args(Parser* self) {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LParen });
NyxVec args = rt_vec_new(sizeof(Expr));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_RParen))) {
while (1) {
rt_vec_push(&args, ({ Expr _tmp_64 = (Parser_parse_expr(self)); &_tmp_64; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RParen });
return args;
}

Expr Parser_parse_if(Parser* self) {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_If });
Expr cond = Parser_parse_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec then = Parser_parse_block(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
NyxVec else_ifs = rt_vec_new(sizeof(int64_t));
NyxOption else_block = rt_option_none();
while ((((*self).current).kind.tag == TAG_TokenKind_Else)) {
Parser_advance(self);
({ Expr __if_res_67; if ((((*self).current).kind.tag == TAG_TokenKind_If)) {
Parser_advance(self);
Expr elif_cond = Parser_parse_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec elif_body = Parser_parse_block(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
rt_vec_push(&else_ifs, ({ NyxTuple2 _tmp_67 = (({ NyxTuple2 _tuple_68 = {.f0 = (void*)({ Expr _stmp_0 = elif_cond; &_stmp_0; }), .f1 = (void*)({ NyxVec _stmp_1 = elif_body; &_stmp_1; }) }; _tuple_68; })); &_tmp_67; }));
} else {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec body = Parser_parse_block(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
else_block = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = body; &_stmp_; }); _tmp; });
break;
}
__if_res_67; });
}
;
return (Expr){ .tag = TAG_Expr_If, .data.If = { .f0 = ({ Expr _stmp_0 = cond; &_stmp_0; }), .f1 = then, .f2 = else_ifs, .f3 = else_block } };
}

Expr Parser_parse_while(Parser* self) {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_While });
Expr cond = Parser_parse_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec body = Parser_parse_block(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
return (Expr){ .tag = TAG_Expr_While, .data.While = { .f0 = ({ Expr _stmp_0 = cond; &_stmp_0; }), .f1 = body } };
}

Expr Parser_parse_for(Parser* self) {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_For });
rt_string_t var = Parser_parse_ident(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_In });
Expr iter = Parser_parse_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec body = Parser_parse_block(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
return (Expr){ .tag = TAG_Expr_For, .data.For = { .f0 = var, .f1 = ({ Expr _stmp_1 = iter; &_stmp_1; }), .f2 = body } };
}

Expr Parser_parse_match(Parser* self) {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Match });
Expr expr = Parser_parse_expr(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LBrace });
NyxVec arms = rt_vec_new(sizeof(int64_t));
while ((!(((*self).current).kind.tag == TAG_TokenKind_RBrace))) {
Pattern pattern = Parser_parse_pattern(self);
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_FatArrow });
Expr body = Parser_parse_expr(self);
rt_vec_push(&arms, ({ MatchArm _tmp_69 = ((MatchArm){.pattern = pattern, .guard = rt_option_none(), .body = body}); &_tmp_69; }));
({ if ((((*self).current).kind.tag == TAG_TokenKind_Comma)) {
Parser_advance(self);
}
});
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RBrace });
return (Expr){ .tag = TAG_Expr_Match, .data.Match = { .f0 = ({ Expr _stmp_0 = expr; &_stmp_0; }), .f1 = arms } };
}

Pattern Parser_parse_pattern(Parser* self) {
return ({
Pattern __match_result_71;
TokenKind* __match_val_71 = ({ TokenKind _utmp_ = ((*self).current).kind; &_utmp_; });
if ((*__match_val_71).tag == TAG_TokenKind_Ident) {
    rt_string_t name = (*__match_val_71).data.Ident.f0;
Parser_advance(self);
if ((((*self).current).kind.tag == TAG_TokenKind_LParen)) {
Parser_advance(self);
NyxVec args = rt_vec_new(sizeof(int64_t));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_RParen))) {
while (1) {
rt_vec_push(&args, ({ Pattern _tmp_73 = (Parser_parse_pattern(self)); &_tmp_73; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RParen });
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Enum, .data.Enum = { .f0 = Literal_from(&(String), rt_string_from("")), .f1 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), .f2 = args } };
} else {
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Ident, .data.Ident = {.f0 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })} };
}
}
 else if ((*__match_val_71).tag == TAG_TokenKind_Underscore) {
Parser_advance(self);
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Wildcard };
}
 else if ((*__match_val_71).tag == TAG_TokenKind_IntLit) {
    int64_t n = (*__match_val_71).data.IntLit.f0;
Parser_advance(self);
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Int, .data.Int = {.f0 = n} }} };
}
 else if ((*__match_val_71).tag == TAG_TokenKind_StringLit) {
    rt_string_t s = (*__match_val_71).data.StringLit.f0;
Parser_advance(self);
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_String, .data.String = {.f0 = ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })} }} };
}
 else if ((*__match_val_71).tag == TAG_TokenKind_True) {
Parser_advance(self);
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Bool, .data.Bool = {.f0 = 1} }} };
}
 else if ((*__match_val_71).tag == TAG_TokenKind_False) {
Parser_advance(self);
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Lit, .data.Lit = {.f0 = (Literal){ .tag = TAG_Literal_Bool, .data.Bool = {.f0 = 0} }} };
}
 else {
Parser_error(self, rt_string_from("Expected pattern"));
__match_result_71 = (Pattern){ .tag = TAG_Pattern_Wildcard };
}
__match_result_71;
});
}

Expr Parser_parse_lambda(Parser* self) {
if ((((*self).current).kind.tag == TAG_TokenKind_Pipe)) {
Parser_advance(self);
NyxVec params = rt_vec_new(sizeof(int64_t));
while ((!(((*self).current).kind.tag == TAG_TokenKind_Pipe))) {
rt_string_t name = Parser_parse_ident(self);
Type ty = ({ Type __if_res_76; if ((((*self).current).kind.tag == TAG_TokenKind_Colon)) {
Parser_advance(self);
__if_res_76 = Parser_parse_type(self);
} else {
__if_res_76 = (Type){ .tag = TAG_Type_Unknown };
}
__if_res_76; });
rt_vec_push(&params, ({ Param _tmp_76 = ((Param){.name = name, .ty = ty}); &_tmp_76; }));
({ if ((((*self).current).kind.tag == TAG_TokenKind_Comma)) {
Parser_advance(self);
}
});
}
;
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Pipe });
Expr body = Parser_parse_expr(self);
return (Expr){ .tag = TAG_Expr_Lambda, .data.Lambda = { .f0 = params, .f1 = ({ Expr _stmp_1 = body; &_stmp_1; }) } };
} else {
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_Fn });
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_LParen });
NyxVec params = rt_vec_new(sizeof(int64_t));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_RParen))) {
while (1) {
rt_string_t pname = Parser_parse_ident(self);
Type ty = ({ Type __if_res_80; if ((((*self).current).kind.tag == TAG_TokenKind_Colon)) {
Parser_advance(self);
__if_res_80 = Parser_parse_type(self);
} else {
__if_res_80 = (Type){ .tag = TAG_Type_Unknown };
}
__if_res_80; });
rt_vec_push(&params, ({ Param _tmp_80 = ((Param){.name = pname, .ty = ty}); &_tmp_80; }));
({ if ((!(((*self).current).kind.tag == TAG_TokenKind_Comma))) {
break;
}
});
Parser_advance(self);
}
;
}
});
Parser_expect(self, (TokenKind){ .tag = TAG_TokenKind_RParen });
Expr body = Parser_parse_expr(self);
return (Expr){ .tag = TAG_Expr_Lambda, .data.Lambda = { .f0 = params, .f1 = ({ Expr _stmp_1 = body; &_stmp_1; }) } };
}
}

