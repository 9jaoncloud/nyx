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
    NyxVec env;
    NyxVec errors;
} TypeChecker;

TypeChecker TypeChecker_new();
void TypeChecker_check_program(TypeChecker*, NyxVec*);
int TypeChecker_has_errors(TypeChecker*);
NyxVec* TypeChecker_get_errors(TypeChecker*);
void TypeChecker_push_scope(TypeChecker*);
void TypeChecker_pop_scope(TypeChecker*);
void TypeChecker_add_var(TypeChecker*, rt_string_t, Type);
NyxOption TypeChecker_lookup(TypeChecker*, rt_string_t);
void TypeChecker_check_top_level(TypeChecker*, TopLevel*);
NyxResult TypeChecker_check_expr(TypeChecker*, Expr*);
NyxResult TypeChecker_check_stmt(TypeChecker*, Stmt*);

#line 1 "typecheck.nyx"
// module typecheck
// import ast::*
// import std::vec
TypeChecker TypeChecker_new() {
return (TypeChecker){.env = rt_vec_new(sizeof(int64_t)), .errors = rt_vec_new(sizeof(rt_string_t))};
}

void TypeChecker_check_program(TypeChecker* self, NyxVec* program) {
TypeChecker_push_scope(self);
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
TypeChecker_check_top_level(self, &(item));
}
;
TypeChecker_pop_scope(self);
}

int TypeChecker_has_errors(TypeChecker* self) {
return ((*self).errors.len > 0LL);
}

NyxVec* TypeChecker_get_errors(TypeChecker* self) {
return (&((*self).errors));
}

void TypeChecker_push_scope(TypeChecker* self) {
rt_vec_push(&(*self).env, ({ NyxMap _tmp_0 = (rt_map_new()); &_tmp_0; }));
}

void TypeChecker_pop_scope(TypeChecker* self) {
rt_vec_pop(&((*self).env));
}

void TypeChecker_add_var(TypeChecker* self, rt_string_t name, Type ty) {
({ if (((*self).env.len > 0LL)) {
int64_t last_idx = ((*self).env.len - 1LL);
NyxMap scope = (*(NyxMap*)rt_vec_get((*self).env, last_idx));
rt_map_insert(&(scope), name, (int64_t)(({ Type* _mtmp = malloc(sizeof(Type)); *_mtmp = ty; (int64_t)_mtmp; })));
}
});
}

NyxOption TypeChecker_lookup(TypeChecker* self, rt_string_t name) {
int64_t i = (*self).env.len;
while ((i > 0LL)) {
i = (i - 1LL);
NyxMap scope = (*(NyxMap*)rt_vec_get((*self).env, i));
({
int64_t __match_result_2;
NyxOption __match_val_2 = rt_map_get(&(scope), name);
if (__match_val_2.tag == OPTION_SOME) {
    Type t = *(Type*)__match_val_2.data.ptr_val;
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ Type _stmp_ = t; &_stmp_; }); _tmp; });
}
 else {
}
__match_result_2;
});
}
;
return rt_option_none();
}

void TypeChecker_check_top_level(TypeChecker* self, TopLevel* item) {
({
int64_t __match_result_3;
TopLevel* __match_val_3 = item;
if ((*__match_val_3).tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = (*__match_val_3).data.FnDecl.f0;
    NyxVec generics = (*__match_val_3).data.FnDecl.f1;
    NyxVec params = (*__match_val_3).data.FnDecl.f2;
    Type ret_type = (*__match_val_3).data.FnDecl.f3;
    Expr body = (*__match_val_3).data.FnDecl.f4;
({ if ((generics.len > 0LL)) {
return;
}
});
TypeChecker_push_scope(self);
for (int64_t _p = 0; _p < params.len; _p++) {
    Param p = *(Param*)rt_vec_get(params, _p);
TypeChecker_add_var(self, ({ rt_string_t _c = p.name; rt_string_retain(&_c); _c; }), p.ty);
}
;
({
int64_t __match_result_5;
NyxResult __match_val_5 = TypeChecker_check_expr(self, &(body));
if (__match_val_5.tag == RESULT_OK) {
}
 else if (__match_val_5.tag == RESULT_ERR) {
    rt_string_t e = rt_string_from((char*)__match_val_5.data.ptr_val);
rt_vec_push(&(*self).errors, ({ rt_string_t _tmp_6 = (e); &_tmp_6; }));
}
__match_result_5;
});
TypeChecker_pop_scope(self);
}
 else if ((*__match_val_3).tag == TAG_TopLevel_StructDef) {
    rt_string_t name = (*__match_val_3).data.StructDef.f0;
    NyxVec fields = (*__match_val_3).data.StructDef.f2;
NyxVec field_types = rt_vec_new(sizeof(int64_t));
for (int64_t _idx___ftype = 0; _idx___ftype < fields.len; _idx___ftype++) {
    NyxTuple2 _tup___ftype = *(NyxTuple2*)rt_vec_get(fields, _idx___ftype);
    int64_t _ = (int64_t)(_tup___ftype.f0);
    int64_t ftype = (int64_t)(_tup___ftype.f1);
rt_vec_push(&field_types, ({ int64_t _tmp_7 = (int64_t)(ftype); &_tmp_7; }));
}
;
}
 else if ((*__match_val_3).tag == TAG_TopLevel_EnumDef) {
}
 else if ((*__match_val_3).tag == TAG_TopLevel_ImplBlock) {
}
 else if ((*__match_val_3).tag == TAG_TopLevel_TraitDef) {
}
 else {
}
__match_result_3;
});
}

NyxResult TypeChecker_check_expr(TypeChecker* self, Expr* expr) {
return ({
NyxResult __match_result_8;
Expr* __match_val_8 = expr;
if ((*__match_val_8).tag == TAG_Expr_Lit) {
    Literal lit = (*__match_val_8).data.Lit.f0;
__match_result_8 = ({
NyxResult __match_result_9;
Literal __match_val_9 = lit;
if (__match_val_9.tag == TAG_Literal_Int) {
__match_result_9 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Int }; &_stmp_; }); _tmp; });
}
 else if (__match_val_9.tag == TAG_Literal_Float) {
__match_result_9 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Float }; &_stmp_; }); _tmp; });
}
 else if (__match_val_9.tag == TAG_Literal_Bool) {
__match_result_9 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Bool }; &_stmp_; }); _tmp; });
}
 else if (__match_val_9.tag == TAG_Literal_String) {
__match_result_9 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_String }; &_stmp_; }); _tmp; });
}
 else if (__match_val_9.tag == TAG_Literal_Char) {
__match_result_9 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Char }; &_stmp_; }); _tmp; });
}
__match_result_9;
});
}
 else if ((*__match_val_8).tag == TAG_Expr_Ident) {
    rt_string_t name = (*__match_val_8).data.Ident.f0;
__match_result_8 = rt_option_ok_or_else(TypeChecker_lookup(self, name), ({ rt_string_t __lambda_10() { return rt_string_concat(rt_string_from("Undefined variable: "), name); } __lambda_10; }));
}
 else if ((*__match_val_8).tag == TAG_Expr_Binary) {
    BinOp op = (*__match_val_8).data.Binary.f0;
    Expr* left = (*__match_val_8).data.Binary.f1;
    Expr* right = (*__match_val_8).data.Binary.f2;
Type lt = ({ NyxResult _qres = TypeChecker_check_expr(self, left); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
Type rt = ({ NyxResult _qres = TypeChecker_check_expr(self, right); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
__match_result_8 = ({
NyxResult __match_result_11;
BinOp __match_val_11 = op;
if (0 || 0 || 0 || 0 || 0) {
if (((lt.tag == (Type){ .tag = TAG_Type_Int }.tag) && (rt.tag == (Type){ .tag = TAG_Type_Int }.tag))) {
__match_result_11 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Int }; &_stmp_; }); _tmp; });
} else if (((lt.tag == (Type){ .tag = TAG_Type_Float }.tag) && (rt.tag == (Type){ .tag = TAG_Type_Float }.tag))) {
__match_result_11 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Float }; &_stmp_; }); _tmp; });
} else if ((((lt.tag == (Type){ .tag = TAG_Type_String }.tag) && (rt.tag == (Type){ .tag = TAG_Type_String }.tag)) && (op.tag == (BinOp){ .tag = TAG_BinOp_Add }.tag))) {
__match_result_11 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_String }; &_stmp_; }); _tmp; });
} else {
__match_result_11 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Incompatible types for arithmetic"); _stmp_.data; }); _tmp; });
}
}
 else if (0 || 0 || 0 || 0 || 0 || 0) {
__match_result_11 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Bool }; &_stmp_; }); _tmp; });
}
 else if (0 || 0) {
__match_result_11 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Bool }; &_stmp_; }); _tmp; });
}
__match_result_11;
});
}
 else if ((*__match_val_8).tag == TAG_Expr_Unary) {
    UnaryOp op = (*__match_val_8).data.Unary.f0;
    Expr* expr = (*__match_val_8).data.Unary.f1;
Type t = ({ NyxResult _qres = TypeChecker_check_expr(self, expr); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
__match_result_8 = ({
NyxResult __match_result_12;
UnaryOp __match_val_12 = op;
if (__match_val_12.tag == TAG_UnaryOp_Neg) {
__match_result_12 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = t; &_stmp_; }); _tmp; });
}
 else if (__match_val_12.tag == TAG_UnaryOp_Not) {
__match_result_12 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Bool }; &_stmp_; }); _tmp; });
}
 else if (__match_val_12.tag == TAG_UnaryOp_Ref) {
__match_result_12 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Ref, .data.Ref = {.f0 = ({ Type _addr_tmp_0 = t; &_addr_tmp_0; })} }; &_stmp_; }); _tmp; });
}
 else if (__match_val_12.tag == TAG_UnaryOp_Deref) {
__match_result_12 = ({
NyxResult __match_result_13;
Type __match_val_13 = t;
if (__match_val_13.tag == TAG_Type_Ref) {
    Type* inner = __match_val_13.data.Ref.f0;
__match_result_13 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)(inner); _tmp; });
}
 else {
__match_result_13 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Deref requires reference type"); _stmp_.data; }); _tmp; });
}
__match_result_13;
});
}
__match_result_12;
});
}
 else if ((*__match_val_8).tag == TAG_Expr_Call) {
    Expr* func = (*__match_val_8).data.Call.f0;
    NyxVec args = (*__match_val_8).data.Call.f2;
({ NyxResult _qres = TypeChecker_check_expr(self, func); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
for (int64_t _arg = 0; _arg < args.len; _arg++) {
    Expr arg = *(Expr*)rt_vec_get(args, _arg);
({ NyxResult _qres = TypeChecker_check_expr(self, &(arg)); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
}
;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_MethodCall) {
    Expr* receiver = (*__match_val_8).data.MethodCall.f0;
    NyxVec args = (*__match_val_8).data.MethodCall.f3;
({ NyxResult _qres = TypeChecker_check_expr(self, receiver); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
for (int64_t _arg = 0; _arg < args.len; _arg++) {
    Expr arg = *(Expr*)rt_vec_get(args, _arg);
({ NyxResult _qres = TypeChecker_check_expr(self, &(arg)); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
}
;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_If) {
    Expr* cond = (*__match_val_8).data.If.f0;
    NyxVec then = (*__match_val_8).data.If.f1;
    NyxVec else_ifs = (*__match_val_8).data.If.f2;
    NyxOption else_block = (*__match_val_8).data.If.f3;
Type ct = ({ NyxResult _qres = TypeChecker_check_expr(self, cond); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
({ if ((ct.tag != (Type){ .tag = TAG_Type_Bool }.tag)) {
rt_vec_push(&(*self).errors, ({ rt_string_t _tmp_15 = (rt_string_from("If condition must be bool")); &_tmp_15; }));
}
});
Type result = (Type){ .tag = TAG_Type_Void };
({
Type __match_result_16;
NyxResult __match_val_16 = TypeChecker_check_expr(self, ({ Expr _utmp_ = (Expr){ .tag = TAG_Expr_Block, .data.Block = {.f0 = rt_vec_clone(then)} }; &_utmp_; }));
if (__match_val_16.tag == RESULT_OK) {
    Type t = *(Type*)__match_val_16.data.ptr_val;
__match_result_16 = (result = t);
}
 else if (__match_val_16.tag == RESULT_ERR) {
    Type e = *(Type*)__match_val_16.data.ptr_val;
rt_vec_push(&(*self).errors, ({ Type _tmp_17 = (e); &_tmp_17; }));
}
__match_result_16;
});
for (int64_t _idx_elif_cond_elif_body = 0; _idx_elif_cond_elif_body < else_ifs.len; _idx_elif_cond_elif_body++) {
    NyxTuple2 _tup_elif_cond_elif_body = *(NyxTuple2*)rt_vec_get(else_ifs, _idx_elif_cond_elif_body);
    Expr* elif_cond = (Expr*)(_tup_elif_cond_elif_body.f0);
    NyxVec elif_body = *(NyxVec*)(_tup_elif_cond_elif_body.f1);
({ NyxResult _qres = TypeChecker_check_expr(self, elif_cond); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
({ NyxResult _qres = TypeChecker_check_expr(self, ({ Expr _utmp_ = (Expr){ .tag = TAG_Expr_Block, .data.Block = {.f0 = rt_vec_clone(elif_body)} }; &_utmp_; })); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
}
;
/* iflet */ 0;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = result; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_While) {
    Expr* cond = (*__match_val_8).data.While.f0;
    NyxVec body = (*__match_val_8).data.While.f1;
({ NyxResult _qres = TypeChecker_check_expr(self, cond); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
({ NyxResult _qres = TypeChecker_check_expr(self, ({ Expr _utmp_ = (Expr){ .tag = TAG_Expr_Block, .data.Block = {.f0 = rt_vec_clone(body)} }; &_utmp_; })); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_For) {
    rt_string_t var = (*__match_val_8).data.For.f0;
    Expr* iter = (*__match_val_8).data.For.f1;
    NyxVec body = (*__match_val_8).data.For.f2;
({ NyxResult _qres = TypeChecker_check_expr(self, iter); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
TypeChecker_push_scope(self);
TypeChecker_add_var(self, ({ rt_string_t _c = var; rt_string_retain(&_c); _c; }), (Type){ .tag = TAG_Type_Int });
({ NyxResult _qres = TypeChecker_check_expr(self, ({ Expr _utmp_ = (Expr){ .tag = TAG_Expr_Block, .data.Block = {.f0 = rt_vec_clone(body)} }; &_utmp_; })); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
TypeChecker_pop_scope(self);
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_Match) {
    Expr* expr = (*__match_val_8).data.Match.f0;
    NyxVec arms = (*__match_val_8).data.Match.f1;
({ NyxResult _qres = TypeChecker_check_expr(self, expr); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
Type result = (Type){ .tag = TAG_Type_Void };
for (int64_t _arm = 0; _arm < arms.len; _arm++) {
    MatchArm arm = *(MatchArm*)rt_vec_get(arms, _arm);
TypeChecker_push_scope(self);
({
Type __match_result_18;
NyxResult __match_val_18 = TypeChecker_check_expr(self, (&(arm.body)));
if (__match_val_18.tag == RESULT_OK) {
    Type t = *(Type*)__match_val_18.data.ptr_val;
__match_result_18 = (result = t);
}
 else if (__match_val_18.tag == RESULT_ERR) {
    Type e = *(Type*)__match_val_18.data.ptr_val;
rt_vec_push(&(*self).errors, ({ Type _tmp_19 = (e); &_tmp_19; }));
}
__match_result_18;
});
TypeChecker_pop_scope(self);
}
;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = result; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_Block) {
    NyxVec stmts = (*__match_val_8).data.Block.f0;
Type last = (Type){ .tag = TAG_Type_Void };
for (int64_t _s = 0; _s < stmts.len; _s++) {
    Stmt s = *(Stmt*)rt_vec_get(stmts, _s);
last = ({ NyxResult _qres = TypeChecker_check_stmt(self, &(s)); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
}
;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = last; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_Let) {
    rt_string_t name = (*__match_val_8).data.Let.f0;
    Expr* value = (*__match_val_8).data.Let.f1;
    NyxOption ty = (*__match_val_8).data.Let.f2;
Type vt = ({ NyxResult _qres = TypeChecker_check_expr(self, value); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
/* iflet */ 0;
TypeChecker_add_var(self, ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), vt);
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = vt; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_Assign) {
    rt_string_t name = (*__match_val_8).data.Assign.f0;
    Expr* value = (*__match_val_8).data.Assign.f1;
Type vt = ({ NyxResult _qres = TypeChecker_check_expr(self, value); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
__match_result_8 = ({
NyxResult __match_result_20;
NyxOption __match_val_20 = TypeChecker_lookup(self, name);
if (__match_val_20.tag == OPTION_SOME) {
__match_result_20 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = vt; &_stmp_; }); _tmp; });
}
 else {
__match_result_20 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Undefined variable: "), name); _stmp_.data; }); _tmp; });
}
__match_result_20;
});
}
 else if ((*__match_val_8).tag == TAG_Expr_Return) {
    Expr* e = (Expr*)((*__match_val_8).data.Return.f0.data.ptr_val);
__match_result_8 = TypeChecker_check_expr(self, e);
}
 else if ((*__match_val_8).tag == TAG_Expr_Return) {
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_StructLiteral) {
    rt_string_t name = (*__match_val_8).data.StructLiteral.f0;
    NyxVec fields = (*__match_val_8).data.StructLiteral.f2;
for (int64_t _idx___val = 0; _idx___val < fields.len; _idx___val++) {
    NyxTuple2 _tup___val = *(NyxTuple2*)rt_vec_get(fields, _idx___val);
    int64_t _ = (int64_t)(_tup___val.f0);
    Expr* val = (Expr*)(_tup___val.f1);
({ NyxResult _qres = TypeChecker_check_expr(self, val); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
}
;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Custom, .data.Custom = {.f0 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })} }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_FieldAccess) {
    Expr* obj = (*__match_val_8).data.FieldAccess.f0;
    rt_string_t field = (*__match_val_8).data.FieldAccess.f1;
({ NyxResult _qres = TypeChecker_check_expr(self, obj); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_Lambda) {
    NyxVec params = (*__match_val_8).data.Lambda.f0;
    Expr* body = (*__match_val_8).data.Lambda.f1;
TypeChecker_push_scope(self);
for (int64_t _p = 0; _p < params.len; _p++) {
    Param p = *(Param*)rt_vec_get(params, _p);
TypeChecker_add_var(self, ({ rt_string_t _c = p.name; rt_string_retain(&_c); _c; }), p.ty);
}
;
Type bt = ({ NyxResult _qres = TypeChecker_check_expr(self, body); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
TypeChecker_pop_scope(self);
NyxVec param_types = rt_vec_new(sizeof(Type));
for (int64_t _p = 0; _p < params.len; _p++) {
    Param p = *(Param*)rt_vec_get(params, _p);
rt_vec_push(&param_types, ({ Type _tmp_21 = (p.ty); &_tmp_21; }));
}
;
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Func, .data.Func = {.f0 = param_types, .f1 = ({ Type _addr_tmp_1 = bt; &_addr_tmp_1; })} }; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_8).tag == TAG_Expr_Await) {
    Expr* e = (*__match_val_8).data.Await.f0;
__match_result_8 = TypeChecker_check_expr(self, e);
}
 else if ((*__match_val_8).tag == TAG_Expr_Question) {
    Expr* e = (*__match_val_8).data.Question.f0;
__match_result_8 = TypeChecker_check_expr(self, e);
}
 else {
__match_result_8 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
__match_result_8;
});
}

NyxResult TypeChecker_check_stmt(TypeChecker* self, Stmt* stmt) {
return ({
NyxResult __match_result_22;
Stmt* __match_val_22 = stmt;
if ((*__match_val_22).tag == TAG_Stmt_Let) {
    rt_string_t name = (*__match_val_22).data.Let.f0;
    NyxOption ty = (*__match_val_22).data.Let.f2;
    Expr value = (*__match_val_22).data.Let.f3;
Type vt = ({ NyxResult _qres = TypeChecker_check_expr(self, &(value)); if (_qres.tag == RESULT_ERR) { return _qres; } *(Type*)_qres.data.ptr_val; });
/* iflet */ 0;
TypeChecker_add_var(self, ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), vt);
__match_result_22 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = vt; &_stmp_; }); _tmp; });
}
 else if ((*__match_val_22).tag == TAG_Stmt_Expr) {
    Expr e = (*__match_val_22).data.Expr.f0;
__match_result_22 = TypeChecker_check_expr(self, &(e));
}
 else if ((*__match_val_22).tag == TAG_Stmt_Return) {
    Expr* e = (Expr*)((*__match_val_22).data.Return.f0.data.ptr_val);
__match_result_22 = TypeChecker_check_expr(self, e);
}
 else if ((*__match_val_22).tag == TAG_Stmt_Return) {
__match_result_22 = ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Type _stmp_ = (Type){ .tag = TAG_Type_Void }; &_stmp_; }); _tmp; });
}
__match_result_22;
});
}

