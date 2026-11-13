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

rt_string_t generate_mlir(NyxVec*);
rt_string_t mlir_type(Type*);
rt_string_t emit_mlir_body(Expr*);
rt_string_t emit_mlir_stmt(Stmt*);
rt_string_t emit_mlir_expr(Expr*);
rt_string_t generate_llvm_dialect(NyxVec*);
rt_string_t mlir_llvm_type(Type*);
rt_string_t emit_llvm_body(Expr*);
rt_string_t emit_llvm_stmt(Stmt*);
rt_string_t emit_llvm_expr(Expr*);

#line 1 "codegen_mlir.nyx"
// module codegen_mlir
// import ast::*
rt_string_t generate_mlir(NyxVec* program) {
Arena* __arena = arena_create();
rt_string_t out = rt_string_from("// Nyx MLIR Dialect IR\n");
out = rt_string_concat(out, rt_string_from("// dialects: nyx.func, nyx.struct, nyx.region, nyx.async\n\n"));
out = rt_string_concat(out, rt_string_from("module {\n"));
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
rt_string_t __match_result_0;
TopLevel __match_val_0 = item;
if (__match_val_0.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_0.data.FnDecl.f0;
    NyxVec generics = __match_val_0.data.FnDecl.f1;
    NyxVec params = __match_val_0.data.FnDecl.f2;
    Type ret_type = __match_val_0.data.FnDecl.f3;
    Expr body = __match_val_0.data.FnDecl.f4;
    int is_async = __match_val_0.data.FnDecl.f5;
({ rt_string_t __if_res_2; if (is_async) {
__if_res_2 = (out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  nyx.async.func @")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("(")));
} else {
__if_res_2 = (out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  nyx.func @")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("(")));
}
__if_res_2; });
for (int64_t i = 0LL; i < params.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("%")), ({ rt_string_t _c = ((*(Param*)rt_vec_get(params, i))).name; rt_string_retain(&_c); _c; })), rt_string_from(": ")), mlir_type((&(((*(Param*)rt_vec_get(params, i))).ty))));
}
;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from(") -> ")), mlir_type(&(ret_type))), rt_string_from(" {\n"));
out = rt_string_concat(out, rt_string_from("  ^bb0:\n"));
out = rt_string_concat(out, emit_mlir_body(&(body)));
__match_result_0 = (out = rt_string_concat(out, rt_string_from("  }\n\n")));
}
 else if (__match_val_0.tag == TAG_TopLevel_StructDef) {
    rt_string_t name = __match_val_0.data.StructDef.f0;
    NyxVec generics = __match_val_0.data.StructDef.f1;
    NyxVec fields = __match_val_0.data.StructDef.f2;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  nyx.struct @")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" {\n"));
for (int64_t _idx_fname_fty = 0; _idx_fname_fty < fields.len; _idx_fname_fty++) {
    NyxTuple2 _tup_fname_fty = *(NyxTuple2*)rt_vec_get(fields, _idx_fname_fty);
    rt_string_t fname = *(rt_string_t*)(_tup_fname_fty.f0);
    Type fty = *(Type*)(_tup_fname_fty.f1);
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("    ")), ({ rt_string_t _c = fname; rt_string_retain(&_c); _c; })), rt_string_from(": ")), mlir_type((&(fty)))), rt_string_from(",\n"));
}
;
__match_result_0 = (out = rt_string_concat(out, rt_string_from("  }\n\n")));
}
 else if (__match_val_0.tag == TAG_TopLevel_EnumDef) {
    rt_string_t name = __match_val_0.data.EnumDef.f0;
    NyxVec generics = __match_val_0.data.EnumDef.f1;
    NyxVec variants = __match_val_0.data.EnumDef.f2;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  nyx.enum @")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" {\n"));
for (int64_t _idx_vname_vtypes = 0; _idx_vname_vtypes < variants.len; _idx_vname_vtypes++) {
    NyxTuple2 _tup_vname_vtypes = *(NyxTuple2*)rt_vec_get(variants, _idx_vname_vtypes);
    rt_string_t vname = *(rt_string_t*)(_tup_vname_vtypes.f0);
    NyxVec vtypes = *(NyxVec*)(_tup_vname_vtypes.f1);
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("    ")), ({ rt_string_t _c = vname; rt_string_retain(&_c); _c; })), rt_string_from("("));
for (int64_t i = 0LL; i < vtypes.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(out, mlir_type((&((*(Type*)rt_vec_get(vtypes, i))))));
}
;
out = rt_string_concat(out, rt_string_from(")\n"));
}
;
__match_result_0 = (out = rt_string_concat(out, rt_string_from("  }\n\n")));
}
 else if (__match_val_0.tag == TAG_TopLevel_Module) {
    rt_string_t name = __match_val_0.data.Module.f0;
__match_result_0 = (out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  // module ")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("\n\n")));
}
 else if (__match_val_0.tag == TAG_TopLevel_Import) {
    NyxVec path = __match_val_0.data.Import.f0;
out = rt_string_concat(out, rt_string_from("  // import "));
for (int64_t i = 0LL; i < path.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from("."));
}
});
out = rt_string_concat(out, ({ rt_string_t _c = (*(rt_string_t*)rt_vec_get(path, i)); rt_string_retain(&_c); _c; }));
}
;
__match_result_0 = (out = rt_string_concat(out, rt_string_from("\n\n")));
}
 else {
}
__match_result_0;
});
}
;
out = rt_string_concat(out, rt_string_from("}\n"));
arena_destroy(__arena);
return out;
}

rt_string_t mlir_type(Type* ty) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_5;
Type* __match_val_5 = ty;
if ((*__match_val_5).tag == TAG_Type_Int) {
__match_result_5 = rt_string_from("i64");
}
 else if ((*__match_val_5).tag == TAG_Type_Float) {
__match_result_5 = rt_string_from("f64");
}
 else if ((*__match_val_5).tag == TAG_Type_Bool) {
__match_result_5 = rt_string_from("i1");
}
 else if ((*__match_val_5).tag == TAG_Type_String) {
__match_result_5 = rt_string_from("!nyx.str");
}
 else if ((*__match_val_5).tag == TAG_Type_Char) {
__match_result_5 = rt_string_from("!nyx.char");
}
 else if ((*__match_val_5).tag == TAG_Type_Void) {
__match_result_5 = rt_string_from("none");
}
 else if ((*__match_val_5).tag == TAG_Type_Custom) {
    rt_string_t n = (*__match_val_5).data.Custom.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.struct<@"), ({ rt_string_t _c = n; rt_string_retain(&_c); _c; })), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Generic) {
    rt_string_t n = (*__match_val_5).data.Generic.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.generic<"), ({ rt_string_t _c = n; rt_string_retain(&_c); _c; })), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Ref) {
    Type* inner = (*__match_val_5).data.Ref.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.ref<"), mlir_type(inner)), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_MutRef) {
    Type* inner = (*__match_val_5).data.MutRef.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.mutref<"), mlir_type(inner)), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Array) {
    Type* inner = (*__match_val_5).data.Array.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.array<"), mlir_type(inner)), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Option) {
    Type* inner = (*__match_val_5).data.Option.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.option<"), mlir_type(inner)), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Result) {
    Type* ok = (*__match_val_5).data.Result.f0;
    Type* err = (*__match_val_5).data.Result.f1;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("!nyx.result<"), mlir_type(ok)), rt_string_from(", ")), mlir_type(err)), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Tuple) {
    NyxVec types = (*__match_val_5).data.Tuple.f0;
rt_string_t s = rt_string_from("!nyx.tuple<");
for (int64_t i = 0LL; i < types.len; i++) {
({ if ((i > 0LL)) {
s = rt_string_concat(s, rt_string_from(", "));
}
});
s = rt_string_concat(s, mlir_type((&((*(Type*)rt_vec_get(types, i))))));
}
;
__match_result_5 = rt_string_concat(s, rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Func) {
    NyxVec params = (*__match_val_5).data.Func.f0;
    Type* ret = (*__match_val_5).data.Func.f1;
rt_string_t s = rt_string_from("!nyx.func<(");
for (int64_t i = 0LL; i < params.len; i++) {
({ if ((i > 0LL)) {
s = rt_string_concat(s, rt_string_from(", "));
}
});
s = rt_string_concat(s, mlir_type((&((*(Type*)rt_vec_get(params, i))))));
}
;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_concat(s, rt_string_from(") -> ")), mlir_type(ret)), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Region) {
    rt_string_t name = (*__match_val_5).data.Region.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.region<'"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Recursive) {
    rt_string_t name = (*__match_val_5).data.Recursive.f0;
__match_result_5 = rt_string_concat(rt_string_concat(rt_string_from("!nyx.rec<"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(">"));
}
 else if ((*__match_val_5).tag == TAG_Type_Unknown) {
__match_result_5 = rt_string_from("!nyx.unknown");
}
__match_result_5;
});
}

rt_string_t emit_mlir_body(Expr* body) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_8;
Expr* __match_val_8 = body;
if ((*__match_val_8).tag == TAG_Expr_Block) {
    NyxVec stmts = (*__match_val_8).data.Block.f0;
rt_string_t out = rt_string_from("");
for (int64_t _stmt = 0; _stmt < stmts.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(stmts, _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
__match_result_8 = out;
}
 else {
__match_result_8 = rt_string_concat(rt_string_concat(rt_string_from("    "), emit_mlir_expr(body)), rt_string_from("\n"));
}
__match_result_8;
});
}

rt_string_t emit_mlir_stmt(Stmt* stmt) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_9;
Stmt* __match_val_9 = stmt;
if ((*__match_val_9).tag == TAG_Stmt_Let) {
    rt_string_t name = (*__match_val_9).data.Let.f0;
    Expr value = (*__match_val_9).data.Let.f3;
    NyxOption ty = (*__match_val_9).data.Let.f2;
    int mutable = (*__match_val_9).data.Let.f1;
rt_string_t out = rt_string_concat(rt_string_from("    %"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }));
({ if (mutable) {
out = rt_string_concat(out, rt_string_from(" mut"));
}
});
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from(" = ")), emit_mlir_expr(&(value))), rt_string_from("\n"));
__match_result_9 = out;
}
 else if ((*__match_val_9).tag == TAG_Stmt_Expr) {
    Expr expr = (*__match_val_9).data.Expr.f0;
__match_result_9 = rt_string_concat(rt_string_concat(rt_string_from("    "), emit_mlir_expr(&(expr))), rt_string_from("\n"));
}
 else if ((*__match_val_9).tag == TAG_Stmt_Return) {
    Expr* e = (Expr*)((*__match_val_9).data.Return.f0.data.ptr_val);
__match_result_9 = rt_string_concat(rt_string_concat(rt_string_from("    nyx.return "), emit_mlir_expr(e)), rt_string_from("\n"));
}
 else if ((*__match_val_9).tag == TAG_Stmt_Return) {
__match_result_9 = rt_string_from("    nyx.return\n");
}
__match_result_9;
});
}

rt_string_t emit_mlir_expr(Expr* expr) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_11;
Expr* __match_val_11 = expr;
if ((*__match_val_11).tag == TAG_Expr_Lit) {
    Literal lit = (*__match_val_11).data.Lit.f0;
__match_result_11 = ({
rt_string_t __match_result_12;
Literal __match_val_12 = lit;
if (__match_val_12.tag == TAG_Literal_Int) {
    int64_t n = __match_val_12.data.Int.f0;
__match_result_12 = ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)n); _buf.ref_count = 1; _buf; });
}
 else if (__match_val_12.tag == TAG_Literal_Float) {
    double n = __match_val_12.data.Float.f0;
__match_result_12 = rt_string_concat(rt_string_from("nyx.const "), ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", n); _buf.ref_count = 1; _buf; }));
}
 else if (__match_val_12.tag == TAG_Literal_Bool) {
    int b = __match_val_12.data.Bool.f0;
if (b) {
__match_result_12 = rt_string_from("true");
} else {
__match_result_12 = rt_string_from("false");
}
}
 else if (__match_val_12.tag == TAG_Literal_String) {
    rt_string_t s = __match_val_12.data.String.f0;
__match_result_12 = rt_string_concat(rt_string_concat(rt_string_from("nyx.str \""), ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })), rt_string_from("\""));
}
 else if (__match_val_12.tag == TAG_Literal_Char) {
    char c = __match_val_12.data.Char.f0;
__match_result_12 = rt_string_concat(rt_string_concat(rt_string_from("nyx.char '"), ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = c; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; })), rt_string_from("'"));
}
__match_result_12;
});
}
 else if ((*__match_val_11).tag == TAG_Expr_Ident) {
    rt_string_t name = (*__match_val_11).data.Ident.f0;
__match_result_11 = rt_string_concat(rt_string_from("%"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }));
}
 else if ((*__match_val_11).tag == TAG_Expr_Binary) {
    BinOp op = (*__match_val_11).data.Binary.f0;
    Expr* left = (*__match_val_11).data.Binary.f1;
    Expr* right = (*__match_val_11).data.Binary.f2;
rt_string_t op_str = ({
rt_string_t __match_result_13;
BinOp __match_val_13 = op;
if (__match_val_13.tag == TAG_BinOp_Add) {
__match_result_13 = rt_string_from("nyx.add");
}
 else if (__match_val_13.tag == TAG_BinOp_Sub) {
__match_result_13 = rt_string_from("nyx.sub");
}
 else if (__match_val_13.tag == TAG_BinOp_Mul) {
__match_result_13 = rt_string_from("nyx.mul");
}
 else if (__match_val_13.tag == TAG_BinOp_Div) {
__match_result_13 = rt_string_from("nyx.div");
}
 else if (__match_val_13.tag == TAG_BinOp_Mod) {
__match_result_13 = rt_string_from("nyx.mod");
}
 else if (__match_val_13.tag == TAG_BinOp_Eq) {
__match_result_13 = rt_string_from("nyx.cmp_eq");
}
 else if (__match_val_13.tag == TAG_BinOp_NotEq) {
__match_result_13 = rt_string_from("nyx.cmp_ne");
}
 else if (__match_val_13.tag == TAG_BinOp_Lt) {
__match_result_13 = rt_string_from("nyx.cmp_lt");
}
 else if (__match_val_13.tag == TAG_BinOp_LtEq) {
__match_result_13 = rt_string_from("nyx.cmp_lte");
}
 else if (__match_val_13.tag == TAG_BinOp_Gt) {
__match_result_13 = rt_string_from("nyx.cmp_gt");
}
 else if (__match_val_13.tag == TAG_BinOp_GtEq) {
__match_result_13 = rt_string_from("nyx.cmp_gte");
}
 else if (__match_val_13.tag == TAG_BinOp_And) {
__match_result_13 = rt_string_from("nyx.and");
}
 else if (__match_val_13.tag == TAG_BinOp_Or) {
__match_result_13 = rt_string_from("nyx.or");
}
__match_result_13;
});
__match_result_11 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(op_str, rt_string_from(" ")), emit_mlir_expr(left)), rt_string_from(", ")), emit_mlir_expr(right));
}
 else if ((*__match_val_11).tag == TAG_Expr_Call) {
    Expr* func = (*__match_val_11).data.Call.f0;
    NyxVec args = (*__match_val_11).data.Call.f2;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_from("call "), emit_mlir_expr(func)), rt_string_from("("));
for (int64_t i = 0LL; i < args.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(out, emit_mlir_expr((&((*(Expr*)rt_vec_get(args, i))))));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from(")"));
}
 else if ((*__match_val_11).tag == TAG_Expr_MethodCall) {
    Expr* receiver = (*__match_val_11).data.MethodCall.f0;
    rt_string_t method = (*__match_val_11).data.MethodCall.f1;
    NyxVec args = (*__match_val_11).data.MethodCall.f3;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("call "), emit_mlir_expr(receiver)), rt_string_from(".")), ({ rt_string_t _c = method; rt_string_retain(&_c); _c; })), rt_string_from("("));
for (int64_t i = 0LL; i < args.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(out, emit_mlir_expr((&((*(Expr*)rt_vec_get(args, i))))));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from(")"));
}
 else if ((*__match_val_11).tag == TAG_Expr_If) {
    Expr* cond = (*__match_val_11).data.If.f0;
    NyxVec then = (*__match_val_11).data.If.f1;
    NyxVec else_ifs = (*__match_val_11).data.If.f2;
    NyxOption else_block = (*__match_val_11).data.If.f3;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_from("nyx.if "), emit_mlir_expr(cond)), rt_string_from(" {\n"));
for (int64_t _stmt = 0; _stmt < then.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(then, _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
for (int64_t _idx_elif_cond_elif_body = 0; _idx_elif_cond_elif_body < else_ifs.len; _idx_elif_cond_elif_body++) {
    NyxTuple2 _tup_elif_cond_elif_body = *(NyxTuple2*)rt_vec_get(else_ifs, _idx_elif_cond_elif_body);
    Expr* elif_cond = (Expr*)(_tup_elif_cond_elif_body.f0);
    NyxVec elif_body = *(NyxVec*)(_tup_elif_cond_elif_body.f1);
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  } nyx.if ")), emit_mlir_expr(elif_cond)), rt_string_from(" {\n"));
for (int64_t _stmt = 0; _stmt < elif_body.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(elif_body, _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
}
;
({
int64_t __match_result_16;
NyxOption __match_val_16 = else_block;
if (__match_val_16.tag == OPTION_SOME) {
    void* stmts = __match_val_16.data.ptr_val;
out = rt_string_concat(out, rt_string_from("  } nyx.else {\n"));
for (int64_t _stmt = 0; _stmt < ((NyxVec*)(stmts))->len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(*((NyxVec*)(stmts)), _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
}
 else if (__match_val_16.tag == OPTION_NONE) {
}
__match_result_16;
});
__match_result_11 = rt_string_concat(out, rt_string_from("  }\n"));
}
 else if ((*__match_val_11).tag == TAG_Expr_While) {
    Expr* cond = (*__match_val_11).data.While.f0;
    NyxVec body = (*__match_val_11).data.While.f1;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_from("nyx.while "), emit_mlir_expr(cond)), rt_string_from(" {\n"));
for (int64_t _stmt = 0; _stmt < body.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(body, _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from("  }\n"));
}
 else if ((*__match_val_11).tag == TAG_Expr_For) {
    rt_string_t var = (*__match_val_11).data.For.f0;
    Expr* iter = (*__match_val_11).data.For.f1;
    NyxVec body = (*__match_val_11).data.For.f2;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("nyx.for %"), ({ rt_string_t _c = var; rt_string_retain(&_c); _c; })), rt_string_from(" in ")), emit_mlir_expr(iter)), rt_string_from(" {\n"));
for (int64_t _stmt = 0; _stmt < body.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(body, _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from("  }\n"));
}
 else if ((*__match_val_11).tag == TAG_Expr_StructLiteral) {
    rt_string_t name = (*__match_val_11).data.StructLiteral.f0;
    NyxVec fields = (*__match_val_11).data.StructLiteral.f2;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_from("nyx.struct "), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" {"));
int first = 1;
for (int64_t _idx_fname_fval = 0; _idx_fname_fval < fields.len; _idx_fname_fval++) {
    NyxTuple2 _tup_fname_fval = *(NyxTuple2*)rt_vec_get(fields, _idx_fname_fval);
    rt_string_t fname = *(rt_string_t*)(_tup_fname_fval.f0);
    Expr* fval = (Expr*)(_tup_fname_fval.f1);
({ if ((!first)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
first = 0;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, ({ rt_string_t _c = fname; rt_string_retain(&_c); _c; })), rt_string_from(": ")), emit_mlir_expr(fval));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from("}"));
}
 else if ((*__match_val_11).tag == TAG_Expr_FieldAccess) {
    Expr* obj = (*__match_val_11).data.FieldAccess.f0;
    rt_string_t field = (*__match_val_11).data.FieldAccess.f1;
__match_result_11 = rt_string_concat(rt_string_concat(emit_mlir_expr(obj), rt_string_from(".")), ({ rt_string_t _c = field; rt_string_retain(&_c); _c; }));
}
 else if ((*__match_val_11).tag == TAG_Expr_Return) {
    Expr* e = (Expr*)((*__match_val_11).data.Return.f0.data.ptr_val);
__match_result_11 = rt_string_concat(rt_string_from("nyx.return "), emit_mlir_expr(e));
}
 else if ((*__match_val_11).tag == TAG_Expr_Return) {
__match_result_11 = rt_string_from("nyx.return");
}
 else if ((*__match_val_11).tag == TAG_Expr_Block) {
    NyxVec stmts = (*__match_val_11).data.Block.f0;
rt_string_t out = rt_string_from("{\n");
for (int64_t _stmt = 0; _stmt < stmts.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(stmts, _stmt);
out = rt_string_concat(out, emit_mlir_stmt(&(stmt)));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from("}"));
}
 else if ((*__match_val_11).tag == TAG_Expr_Let) {
    rt_string_t name = (*__match_val_11).data.Let.f0;
    Expr* value = (*__match_val_11).data.Let.f1;
__match_result_11 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("%"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" = ")), emit_mlir_expr(value));
}
 else if ((*__match_val_11).tag == TAG_Expr_Unary) {
    UnaryOp op = (*__match_val_11).data.Unary.f0;
    Expr* expr = (*__match_val_11).data.Unary.f1;
__match_result_11 = ({
rt_string_t __match_result_18;
UnaryOp __match_val_18 = op;
if (__match_val_18.tag == TAG_UnaryOp_Not) {
__match_result_18 = rt_string_concat(rt_string_from("nyx.not "), emit_mlir_expr(expr));
}
 else if (__match_val_18.tag == TAG_UnaryOp_Neg) {
__match_result_18 = rt_string_concat(rt_string_from("nyx.neg "), emit_mlir_expr(expr));
}
 else if (__match_val_18.tag == TAG_UnaryOp_Ref) {
__match_result_18 = rt_string_concat(rt_string_from("nyx.ref "), emit_mlir_expr(expr));
}
 else if (__match_val_18.tag == TAG_UnaryOp_Deref) {
__match_result_18 = rt_string_concat(rt_string_from("nyx.deref "), emit_mlir_expr(expr));
}
__match_result_18;
});
}
 else if ((*__match_val_11).tag == TAG_Expr_Match) {
    Expr* expr = (*__match_val_11).data.Match.f0;
    NyxVec arms = (*__match_val_11).data.Match.f1;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_from("nyx.match "), emit_mlir_expr(expr)), rt_string_from(" {\n"));
for (int64_t _arm = 0; _arm < arms.len; _arm++) {
    MatchArm arm = *(MatchArm*)rt_vec_get(arms, _arm);
out = rt_string_concat(out, rt_string_from("  "));
({
rt_string_t __match_result_19;
Pattern* __match_val_19 = (&(arm.pattern));
if ((*__match_val_19).tag == TAG_Pattern_Ident) {
    rt_string_t s = (*__match_val_19).data.Ident.f0;
__match_result_19 = (out = rt_string_concat(rt_string_concat(out, rt_string_from("%")), ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })));
}
 else if ((*__match_val_19).tag == TAG_Pattern_Wildcard) {
__match_result_19 = (out = rt_string_concat(out, rt_string_from("_")));
}
 else if ((*__match_val_19).tag == TAG_Pattern_Lit) {
    Literal lit = (*__match_val_19).data.Lit.f0;
__match_result_19 = ({
rt_string_t __match_result_20;
Literal __match_val_20 = lit;
if (__match_val_20.tag == TAG_Literal_Int) {
    int64_t n = __match_val_20.data.Int.f0;
__match_result_20 = (out = rt_string_concat(out, ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)n); _buf.ref_count = 1; _buf; })));
}
 else if (__match_val_20.tag == TAG_Literal_String) {
    rt_string_t s = __match_val_20.data.String.f0;
__match_result_20 = (out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("\"")), ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })), rt_string_from("\"")));
}
 else if (__match_val_20.tag == TAG_Literal_Bool) {
    int b = __match_val_20.data.Bool.f0;
__match_result_20 = (out = rt_string_concat(out, ({ rt_string_t __if_res_22; if (b) {
__if_res_22 = rt_string_from("true");
} else {
__if_res_22 = rt_string_from("false");
}
__if_res_22; })));
}
 else {
__match_result_20 = (out = rt_string_concat(out, rt_string_from("?")));
}
__match_result_20;
});
}
 else if ((*__match_val_19).tag == TAG_Pattern_Enum) {
    rt_string_t variant = (*__match_val_19).data.Enum.f1;
__match_result_19 = (out = rt_string_concat(out, ({ rt_string_t _c = variant; rt_string_retain(&_c); _c; })));
}
__match_result_19;
});
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from(" => ")), emit_mlir_expr((&(arm.body)))), rt_string_from("\n"));
}
;
__match_result_11 = rt_string_concat(out, rt_string_from("}\n"));
}
 else if ((*__match_val_11).tag == TAG_Expr_Lambda) {
    NyxVec params = (*__match_val_11).data.Lambda.f0;
    Expr* body = (*__match_val_11).data.Lambda.f1;
rt_string_t out = rt_string_from("nyx.lambda (");
for (int64_t i = 0LL; i < params.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("%")), ({ rt_string_t _c = ((*(Param*)rt_vec_get(params, i))).name; rt_string_retain(&_c); _c; })), rt_string_from(": ")), mlir_type((&(((*(Param*)rt_vec_get(params, i))).ty))));
}
;
__match_result_11 = rt_string_concat(rt_string_concat(out, rt_string_from(") => ")), emit_mlir_expr(body));
}
 else if ((*__match_val_11).tag == TAG_Expr_Await) {
    Expr* e = (*__match_val_11).data.Await.f0;
__match_result_11 = rt_string_concat(rt_string_from("nyx.await "), emit_mlir_expr(e));
}
 else if ((*__match_val_11).tag == TAG_Expr_Assign) {
    rt_string_t name = (*__match_val_11).data.Assign.f0;
    Expr* value = (*__match_val_11).data.Assign.f1;
__match_result_11 = rt_string_concat(rt_string_concat(({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), rt_string_from(" = ")), emit_mlir_expr(value));
}
 else if ((*__match_val_11).tag == TAG_Expr_Question) {
    Expr* e = (*__match_val_11).data.Question.f0;
__match_result_11 = rt_string_concat(emit_mlir_expr(e), rt_string_from("?"));
}
 else {
__match_result_11 = rt_string_from("");
}
__match_result_11;
});
}

rt_string_t generate_llvm_dialect(NyxVec* program) {
Arena* __arena = arena_create();
rt_string_t out = rt_string_from("; Nyx -> LLVM Dialect\n");
out = rt_string_concat(out, rt_string_from("module {\n"));
out = rt_string_concat(out, rt_string_from("  llvm.func @nyx_println(i64) -> none\n"));
out = rt_string_concat(out, rt_string_from("  llvm.func @nyx_print_int(i64) -> none\n"));
out = rt_string_concat(out, rt_string_from("  llvm.func @nyx_print_f64(f64) -> none\n\n"));
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
rt_string_t __match_result_23;
TopLevel __match_val_23 = item;
if (__match_val_23.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_23.data.FnDecl.f0;
    NyxVec params = __match_val_23.data.FnDecl.f2;
    Type ret_type = __match_val_23.data.FnDecl.f3;
    Expr body = __match_val_23.data.FnDecl.f4;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  llvm.func @")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("("));
for (int64_t i = 0LL; i < params.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("%")), ({ rt_string_t _c = ((*(Param*)rt_vec_get(params, i))).name; rt_string_retain(&_c); _c; })), rt_string_from(": ")), mlir_llvm_type((&(((*(Param*)rt_vec_get(params, i))).ty))));
}
;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from(") -> ")), mlir_llvm_type(&(ret_type))), rt_string_from(" {\n"));
out = rt_string_concat(out, rt_string_from("  ^bb0:\n"));
out = rt_string_concat(out, emit_llvm_body(&(body)));
__match_result_23 = (out = rt_string_concat(out, rt_string_from("  }\n\n")));
}
 else if (__match_val_23.tag == TAG_TopLevel_StructDef) {
    rt_string_t name = __match_val_23.data.StructDef.f0;
    NyxVec fields = __match_val_23.data.StructDef.f2;
out = rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  ; struct ")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" {\n"));
for (int64_t _idx_fname_fty = 0; _idx_fname_fty < fields.len; _idx_fname_fty++) {
    NyxTuple2 _tup_fname_fty = *(NyxTuple2*)rt_vec_get(fields, _idx_fname_fty);
    rt_string_t fname = *(rt_string_t*)(_tup_fname_fty.f0);
    Type fty = *(Type*)(_tup_fname_fty.f1);
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  ;   ")), ({ rt_string_t _c = fname; rt_string_retain(&_c); _c; })), rt_string_from(": ")), mlir_llvm_type((&(fty)))), rt_string_from("\n"));
}
;
__match_result_23 = (out = rt_string_concat(out, rt_string_from("  ; }\n\n")));
}
 else {
}
__match_result_23;
});
}
;
out = rt_string_concat(out, rt_string_from("}\n"));
arena_destroy(__arena);
return out;
}

rt_string_t mlir_llvm_type(Type* ty) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_25;
Type* __match_val_25 = ty;
if ((*__match_val_25).tag == TAG_Type_Int) {
__match_result_25 = rt_string_from("i64");
}
 else if ((*__match_val_25).tag == TAG_Type_Float) {
__match_result_25 = rt_string_from("f64");
}
 else if ((*__match_val_25).tag == TAG_Type_Bool) {
__match_result_25 = rt_string_from("i1");
}
 else if ((*__match_val_25).tag == TAG_Type_String) {
__match_result_25 = rt_string_from("i8*");
}
 else if ((*__match_val_25).tag == TAG_Type_Char) {
__match_result_25 = rt_string_from("i8");
}
 else if ((*__match_val_25).tag == TAG_Type_Void) {
__match_result_25 = rt_string_from("void");
}
 else {
__match_result_25 = rt_string_from("i8*");
}
__match_result_25;
});
}

rt_string_t emit_llvm_body(Expr* body) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_26;
Expr* __match_val_26 = body;
if ((*__match_val_26).tag == TAG_Expr_Block) {
    NyxVec stmts = (*__match_val_26).data.Block.f0;
rt_string_t out = rt_string_from("");
for (int64_t _stmt = 0; _stmt < stmts.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(stmts, _stmt);
out = rt_string_concat(out, emit_llvm_stmt(&(stmt)));
}
;
__match_result_26 = out;
}
 else {
__match_result_26 = rt_string_from("");
}
__match_result_26;
});
}

rt_string_t emit_llvm_stmt(Stmt* stmt) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_27;
Stmt* __match_val_27 = stmt;
if ((*__match_val_27).tag == TAG_Stmt_Let) {
    rt_string_t name = (*__match_val_27).data.Let.f0;
    Expr value = (*__match_val_27).data.Let.f3;
__match_result_27 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("    %"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" = ")), emit_llvm_expr(&(value))), rt_string_from("\n"));
}
 else if ((*__match_val_27).tag == TAG_Stmt_Expr) {
    Expr expr = (*__match_val_27).data.Expr.f0;
__match_result_27 = rt_string_concat(rt_string_concat(rt_string_from("    "), emit_llvm_expr(&(expr))), rt_string_from("\n"));
}
 else if ((*__match_val_27).tag == TAG_Stmt_Return) {
    Expr* e = (Expr*)((*__match_val_27).data.Return.f0.data.ptr_val);
__match_result_27 = rt_string_concat(rt_string_concat(rt_string_from("    llvm.return "), emit_llvm_expr(e)), rt_string_from("\n"));
}
 else if ((*__match_val_27).tag == TAG_Stmt_Return) {
__match_result_27 = rt_string_from("    llvm.return\n");
}
__match_result_27;
});
}

rt_string_t emit_llvm_expr(Expr* expr) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_28;
Expr* __match_val_28 = expr;
if ((*__match_val_28).tag == TAG_Expr_Lit) {
    Literal lit = (*__match_val_28).data.Lit.f0;
__match_result_28 = ({
rt_string_t __match_result_29;
Literal __match_val_29 = lit;
if (__match_val_29.tag == TAG_Literal_Int) {
    int64_t n = __match_val_29.data.Int.f0;
__match_result_29 = ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)n); _buf.ref_count = 1; _buf; });
}
 else if (__match_val_29.tag == TAG_Literal_Float) {
    double n = __match_val_29.data.Float.f0;
__match_result_29 = ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", n); _buf.ref_count = 1; _buf; });
}
 else if (__match_val_29.tag == TAG_Literal_Bool) {
    int b = __match_val_29.data.Bool.f0;
if (b) {
__match_result_29 = rt_string_from("1");
} else {
__match_result_29 = rt_string_from("0");
}
}
 else if (__match_val_29.tag == TAG_Literal_String) {
    rt_string_t s = __match_val_29.data.String.f0;
__match_result_29 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("getelementptr(["), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)s.length); _buf.ref_count = 1; _buf; })), rt_string_from(" x i8], [")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)s.length); _buf.ref_count = 1; _buf; })), rt_string_from(" x i8]* @str_")), ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })), rt_string_from(", i32 0, i32 0)"));
}
 else if (__match_val_29.tag == TAG_Literal_Char) {
    char c = __match_val_29.data.Char.f0;
__match_result_29 = ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = c; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; });
}
__match_result_29;
});
}
 else if ((*__match_val_28).tag == TAG_Expr_Ident) {
    rt_string_t name = (*__match_val_28).data.Ident.f0;
__match_result_28 = rt_string_concat(rt_string_from("%"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }));
}
 else if ((*__match_val_28).tag == TAG_Expr_Binary) {
    BinOp op = (*__match_val_28).data.Binary.f0;
    Expr* left = (*__match_val_28).data.Binary.f1;
    Expr* right = (*__match_val_28).data.Binary.f2;
rt_string_t op_str = ({
rt_string_t __match_result_30;
BinOp __match_val_30 = op;
if (__match_val_30.tag == TAG_BinOp_Add) {
__match_result_30 = rt_string_from("add i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Sub) {
__match_result_30 = rt_string_from("sub i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Mul) {
__match_result_30 = rt_string_from("mul i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Div) {
__match_result_30 = rt_string_from("sdiv i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Mod) {
__match_result_30 = rt_string_from("srem i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Eq) {
__match_result_30 = rt_string_from("icmp eq i64");
}
 else if (__match_val_30.tag == TAG_BinOp_NotEq) {
__match_result_30 = rt_string_from("icmp ne i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Lt) {
__match_result_30 = rt_string_from("icmp slt i64");
}
 else if (__match_val_30.tag == TAG_BinOp_LtEq) {
__match_result_30 = rt_string_from("icmp sle i64");
}
 else if (__match_val_30.tag == TAG_BinOp_Gt) {
__match_result_30 = rt_string_from("icmp sgt i64");
}
 else if (__match_val_30.tag == TAG_BinOp_GtEq) {
__match_result_30 = rt_string_from("icmp sge i64");
}
 else if (__match_val_30.tag == TAG_BinOp_And) {
__match_result_30 = rt_string_from("and i1");
}
 else if (__match_val_30.tag == TAG_BinOp_Or) {
__match_result_30 = rt_string_from("or i1");
}
__match_result_30;
});
__match_result_28 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(op_str, rt_string_from(" ")), emit_llvm_expr(left)), rt_string_from(", ")), emit_llvm_expr(right));
}
 else if ((*__match_val_28).tag == TAG_Expr_Call) {
    Expr* func = (*__match_val_28).data.Call.f0;
    NyxVec args = (*__match_val_28).data.Call.f2;
rt_string_t out = rt_string_concat(rt_string_concat(rt_string_from("call "), emit_llvm_expr(func)), rt_string_from("("));
for (int64_t i = 0LL; i < args.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(rt_string_concat(out, rt_string_from("i64 ")), emit_llvm_expr((&((*(Expr*)rt_vec_get(args, i))))));
}
;
__match_result_28 = rt_string_concat(out, rt_string_from(")"));
}
 else {
__match_result_28 = rt_string_from("/* unsupported */");
}
__match_result_28;
});
}

