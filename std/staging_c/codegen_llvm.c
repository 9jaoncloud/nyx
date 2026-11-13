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
    int64_t val_counter;
    int64_t lbl_counter;
    int64_t str_counter;
    rt_string_t globals;
    rt_string_t body;
} CgState;

CgState cg_new();
int64_t next_val(CgState*);
int64_t next_lbl(CgState*);
int64_t next_str(CgState*);
void emit(CgState*, rt_string_t);
void emit_lbl(CgState*, int64_t);
rt_string_t val_name(int64_t);
rt_string_t lbl_name(int64_t);
rt_string_t llvm_type(Type*);
rt_string_t emit_literal(CgState*, Literal*);
rt_string_t binop_opcode(BinOp*, int);
int is_cmp_op(BinOp*);
rt_string_t emit_expr(CgState*, Expr*);
rt_string_t emit_stmt(CgState*, Stmt*);
rt_string_t emit_params(NyxVec*);
void emit_param_allocas(CgState*, NyxVec*);
rt_string_t emit_function(CgState*, rt_string_t, NyxVec*, Type*, Expr*);
rt_string_t generate_llvm(NyxVec*);

#line 1 "codegen_llvm.nyx"
// module codegen_llvm
// import ast::*
CgState cg_new() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (CgState){.val_counter = 0LL, .lbl_counter = 0LL, .str_counter = 0LL, .globals = rt_string_from(""), .body = rt_string_from("")};
}

int64_t next_val(CgState* state) {
Arena* __arena = arena_create();
int64_t v = (*state).val_counter;
(state->val_counter = ((*state).val_counter + 1LL));
arena_destroy(__arena);
return v;
}

int64_t next_lbl(CgState* state) {
Arena* __arena = arena_create();
int64_t l = (*state).lbl_counter;
(state->lbl_counter = ((*state).lbl_counter + 1LL));
arena_destroy(__arena);
return l;
}

int64_t next_str(CgState* state) {
Arena* __arena = arena_create();
int64_t s = (*state).str_counter;
(state->str_counter = ((*state).str_counter + 1LL));
arena_destroy(__arena);
return s;
}

void emit(CgState* state, rt_string_t line) {
Arena* __arena = arena_create();
(state->body = rt_string_concat(rt_string_concat(rt_string_concat((*state).body, rt_string_from("  ")), line), rt_string_from("\n")));
arena_destroy(__arena);
}

void emit_lbl(CgState* state, int64_t id) {
Arena* __arena = arena_create();
(state->body = rt_string_concat(rt_string_concat(rt_string_concat((*state).body, rt_string_from("lbl")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)id); _buf.ref_count = 1; _buf; })), rt_string_from(":\n")));
arena_destroy(__arena);
}

rt_string_t val_name(int64_t v) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_concat(rt_string_from("%"), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)v); _buf.ref_count = 1; _buf; }));
}

rt_string_t lbl_name(int64_t l) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_concat(rt_string_from("lbl"), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)l); _buf.ref_count = 1; _buf; }));
}

rt_string_t llvm_type(Type* ty) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_0;
Type* __match_val_0 = ty;
if ((*__match_val_0).tag == TAG_Type_Int) {
__match_result_0 = rt_string_from("i64");
}
 else if ((*__match_val_0).tag == TAG_Type_Float) {
__match_result_0 = rt_string_from("double");
}
 else if ((*__match_val_0).tag == TAG_Type_Bool) {
__match_result_0 = rt_string_from("i1");
}
 else if ((*__match_val_0).tag == TAG_Type_String) {
__match_result_0 = rt_string_from("i8*");
}
 else if ((*__match_val_0).tag == TAG_Type_Char) {
__match_result_0 = rt_string_from("i8");
}
 else if ((*__match_val_0).tag == TAG_Type_Void) {
__match_result_0 = rt_string_from("void");
}
 else {
__match_result_0 = rt_string_from("i64");
}
__match_result_0;
});
}

rt_string_t emit_literal(CgState* state, Literal* lit) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_1;
Literal* __match_val_1 = lit;
if ((*__match_val_1).tag == TAG_Literal_Int) {
    int64_t n = (*__match_val_1).data.Int.f0;
__match_result_1 = ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)n); _buf.ref_count = 1; _buf; });
}
 else if ((*__match_val_1).tag == TAG_Literal_Float) {
    double f = (*__match_val_1).data.Float.f0;
__match_result_1 = ({ rt_string_t _buf; _buf.data = (char*)malloc(64); _buf.length = snprintf(_buf.data, 64, "%f", f); _buf.ref_count = 1; _buf; });
}
 else if ((*__match_val_1).tag == TAG_Literal_Bool) {
    int b = (*__match_val_1).data.Bool.f0;
if (b) {
__match_result_1 = rt_string_from("1");
} else {
__match_result_1 = rt_string_from("0");
}
}
 else if ((*__match_val_1).tag == TAG_Literal_String) {
    rt_string_t s = (*__match_val_1).data.String.f0;
int64_t sid = next_str(state);
rt_string_t escaped = ({ rt_string_t _c = s; rt_string_retain(&_c); _c; });
int64_t len = (s.length + 1LL);
rt_string_t gname = rt_string_concat(rt_string_from("@str_"), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)sid); _buf.ref_count = 1; _buf; }));
rt_string_t quote = rt_string_from("\"");
rt_string_t gdef = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(({ rt_string_t _c = gname; rt_string_retain(&_c); _c; }), rt_string_from(" = private unnamed_addr constant [")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)len); _buf.ref_count = 1; _buf; })), rt_string_from(" x i8] c")), quote), escaped), rt_string_from("\\00")), quote), rt_string_from(", align 1"));
(state->globals = rt_string_concat(rt_string_concat((*state).globals, gdef), rt_string_from("\n")));
int64_t v = next_val(state);
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(val_name(v), rt_string_from(" = getelementptr inbounds [")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)len); _buf.ref_count = 1; _buf; })), rt_string_from(" x i8], [")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)len); _buf.ref_count = 1; _buf; })), rt_string_from(" x i8]* ")), gname), rt_string_from(", i32 0, i32 0")));
__match_result_1 = val_name(v);
}
 else if ((*__match_val_1).tag == TAG_Literal_Char) {
    char c = (*__match_val_1).data.Char.f0;
__match_result_1 = ({ rt_string_t _s; _s.data = (char*)malloc(2); _s.data[0] = c; _s.data[1] = 0; _s.length = 1; _s.ref_count = 1; _s; });
}
__match_result_1;
});
}

rt_string_t binop_opcode(BinOp* op, int is_float) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if (is_float) {
return ({
rt_string_t __match_result_2;
BinOp* __match_val_2 = op;
if ((*__match_val_2).tag == TAG_BinOp_Add) {
__match_result_2 = rt_string_from("fadd");
}
 else if ((*__match_val_2).tag == TAG_BinOp_Sub) {
__match_result_2 = rt_string_from("fsub");
}
 else if ((*__match_val_2).tag == TAG_BinOp_Mul) {
__match_result_2 = rt_string_from("fmul");
}
 else if ((*__match_val_2).tag == TAG_BinOp_Div) {
__match_result_2 = rt_string_from("fdiv");
}
 else if ((*__match_val_2).tag == TAG_BinOp_Eq) {
__match_result_2 = rt_string_from("fcmp oeq");
}
 else if ((*__match_val_2).tag == TAG_BinOp_NotEq) {
__match_result_2 = rt_string_from("fcmp one");
}
 else if ((*__match_val_2).tag == TAG_BinOp_Lt) {
__match_result_2 = rt_string_from("fcmp olt");
}
 else if ((*__match_val_2).tag == TAG_BinOp_LtEq) {
__match_result_2 = rt_string_from("fcmp ole");
}
 else if ((*__match_val_2).tag == TAG_BinOp_Gt) {
__match_result_2 = rt_string_from("fcmp ogt");
}
 else if ((*__match_val_2).tag == TAG_BinOp_GtEq) {
__match_result_2 = rt_string_from("fcmp oge");
}
 else {
__match_result_2 = rt_string_from("fadd");
}
__match_result_2;
});
} else {
return ({
rt_string_t __match_result_3;
BinOp* __match_val_3 = op;
if ((*__match_val_3).tag == TAG_BinOp_Add) {
__match_result_3 = rt_string_from("add nsw");
}
 else if ((*__match_val_3).tag == TAG_BinOp_Sub) {
__match_result_3 = rt_string_from("sub nsw");
}
 else if ((*__match_val_3).tag == TAG_BinOp_Mul) {
__match_result_3 = rt_string_from("mul nsw");
}
 else if ((*__match_val_3).tag == TAG_BinOp_Div) {
__match_result_3 = rt_string_from("sdiv");
}
 else if ((*__match_val_3).tag == TAG_BinOp_Mod) {
__match_result_3 = rt_string_from("srem");
}
 else if ((*__match_val_3).tag == TAG_BinOp_Eq) {
__match_result_3 = rt_string_from("icmp eq");
}
 else if ((*__match_val_3).tag == TAG_BinOp_NotEq) {
__match_result_3 = rt_string_from("icmp ne");
}
 else if ((*__match_val_3).tag == TAG_BinOp_Lt) {
__match_result_3 = rt_string_from("icmp slt");
}
 else if ((*__match_val_3).tag == TAG_BinOp_LtEq) {
__match_result_3 = rt_string_from("icmp sle");
}
 else if ((*__match_val_3).tag == TAG_BinOp_Gt) {
__match_result_3 = rt_string_from("icmp sgt");
}
 else if ((*__match_val_3).tag == TAG_BinOp_GtEq) {
__match_result_3 = rt_string_from("icmp sge");
}
 else if ((*__match_val_3).tag == TAG_BinOp_And) {
__match_result_3 = rt_string_from("and");
}
 else if ((*__match_val_3).tag == TAG_BinOp_Or) {
__match_result_3 = rt_string_from("or");
}
 else {
__match_result_3 = rt_string_from("add nsw");
}
__match_result_3;
});
}
}

int is_cmp_op(BinOp* op) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
int __match_result_4;
BinOp* __match_val_4 = op;
if ((*__match_val_4).tag == TAG_BinOp_Eq) {
__match_result_4 = 1;
}
 else if ((*__match_val_4).tag == TAG_BinOp_NotEq) {
__match_result_4 = 1;
}
 else if ((*__match_val_4).tag == TAG_BinOp_Lt) {
__match_result_4 = 1;
}
 else if ((*__match_val_4).tag == TAG_BinOp_LtEq) {
__match_result_4 = 1;
}
 else if ((*__match_val_4).tag == TAG_BinOp_Gt) {
__match_result_4 = 1;
}
 else if ((*__match_val_4).tag == TAG_BinOp_GtEq) {
__match_result_4 = 1;
}
 else {
__match_result_4 = 0;
}
__match_result_4;
});
}

rt_string_t emit_expr(CgState* state, Expr* expr) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_5;
Expr* __match_val_5 = expr;
if ((*__match_val_5).tag == TAG_Expr_Lit) {
    Literal lit = (*__match_val_5).data.Lit.f0;
__match_result_5 = emit_literal(state, &(lit));
}
 else if ((*__match_val_5).tag == TAG_Expr_Ident) {
    rt_string_t name = (*__match_val_5).data.Ident.f0;
int64_t v = next_val(state);
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(val_name(v), rt_string_from(" = load i64, i64* %")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("_ptr, align 8")));
__match_result_5 = val_name(v);
}
 else if ((*__match_val_5).tag == TAG_Expr_Binary) {
    BinOp op = (*__match_val_5).data.Binary.f0;
    Expr* left = (*__match_val_5).data.Binary.f1;
    Expr* right = (*__match_val_5).data.Binary.f2;
rt_string_t lv = emit_expr(state, left);
rt_string_t rv = emit_expr(state, right);
int is_float = rt_string_contains(lv, rt_string_from("."));
rt_string_t opc = binop_opcode(&(op), is_float);
rt_string_t ty = ({ rt_string_t __if_res_7; if (is_float) {
__if_res_7 = rt_string_from("double");
} else {
__if_res_7 = rt_string_from("i64");
}
__if_res_7; });
int64_t v = next_val(state);
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(val_name(v), rt_string_from(" = ")), opc), rt_string_from(" ")), ty), rt_string_from(" ")), lv), rt_string_from(", ")), rv));
if (is_cmp_op(&(op))) {
int64_t v2 = next_val(state);
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(val_name(v2), rt_string_from(" = zext i1 ")), val_name(v)), rt_string_from(" to i64")));
__match_result_5 = val_name(v2);
} else {
__match_result_5 = val_name(v);
}
}
 else if ((*__match_val_5).tag == TAG_Expr_Unary) {
    UnaryOp op = (*__match_val_5).data.Unary.f0;
    Expr* inner = (*__match_val_5).data.Unary.f1;
rt_string_t iv = emit_expr(state, inner);
__match_result_5 = ({
rt_string_t __match_result_7;
UnaryOp __match_val_7 = op;
if (__match_val_7.tag == TAG_UnaryOp_Neg) {
int64_t v = next_val(state);
emit(state, rt_string_concat(rt_string_concat(val_name(v), rt_string_from(" = sub nsw i64 0, ")), iv));
__match_result_7 = val_name(v);
}
 else if (__match_val_7.tag == TAG_UnaryOp_Not) {
int64_t v = next_val(state);
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(val_name(v), rt_string_from(" = xor i64 ")), iv), rt_string_from(", 1")));
__match_result_7 = val_name(v);
}
 else {
__match_result_7 = iv;
}
__match_result_7;
});
}
 else if ((*__match_val_5).tag == TAG_Expr_Let) {
    rt_string_t name = (*__match_val_5).data.Let.f0;
    Expr* value = (*__match_val_5).data.Let.f1;
    NyxOption ty = (*__match_val_5).data.Let.f2;
    int mutable = (*__match_val_5).data.Let.f3;
int64_t alloca_v = next_val(state);
emit(state, rt_string_concat(val_name(alloca_v), rt_string_from(" = alloca i64, align 8")));
(state->body = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat((*state).body, rt_string_from("  %")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("_ptr = bitcast i64* ")), val_name(alloca_v)), rt_string_from(" to i64*\n")));
rt_string_t init_v = emit_expr(state, value);
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("store i64 "), init_v), rt_string_from(", i64* %")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("_ptr, align 8")));
__match_result_5 = init_v;
}
 else if ((*__match_val_5).tag == TAG_Expr_Assign) {
    rt_string_t name = (*__match_val_5).data.Assign.f0;
    Expr* value = (*__match_val_5).data.Assign.f1;
rt_string_t rhs = emit_expr(state, value);
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("store i64 "), rhs), rt_string_from(", i64* %")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("_ptr, align 8")));
__match_result_5 = rhs;
}
 else if ((*__match_val_5).tag == TAG_Expr_If) {
    Expr* cond = (*__match_val_5).data.If.f0;
    NyxVec then = (*__match_val_5).data.If.f1;
    NyxVec else_ifs = (*__match_val_5).data.If.f2;
    NyxOption else_block = (*__match_val_5).data.If.f3;
rt_string_t cond_v = emit_expr(state, cond);
int64_t cond1 = next_val(state);
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(val_name(cond1), rt_string_from(" = trunc i64 ")), cond_v), rt_string_from(" to i1")));
int64_t then_lbl = next_lbl(state);
int64_t else_lbl = next_lbl(state);
int64_t merge_lbl = next_lbl(state);
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("br i1 "), val_name(cond1)), rt_string_from(", label %")), lbl_name(then_lbl)), rt_string_from(", label %")), lbl_name(else_lbl)));
emit_lbl(state, then_lbl);
rt_string_t then_val = rt_string_from("0");
for (int64_t _stmt = 0; _stmt < then.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(then, _stmt);
then_val = emit_stmt(state, &(stmt));
}
;
emit(state, rt_string_concat(rt_string_from("br label %"), lbl_name(merge_lbl)));
int64_t then_exit_lbl = then_lbl;
emit_lbl(state, else_lbl);
rt_string_t else_val = rt_string_from("0");
({
int64_t __match_result_8;
NyxOption __match_val_8 = else_block;
if (__match_val_8.tag == OPTION_SOME) {
    void* stmts = __match_val_8.data.ptr_val;
for (int64_t _stmt = 0; _stmt < ((NyxVec*)(stmts))->len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(*((NyxVec*)(stmts)), _stmt);
else_val = emit_stmt(state, &(stmt));
}
;
}
 else if (__match_val_8.tag == OPTION_NONE) {
}
__match_result_8;
});
emit(state, rt_string_concat(rt_string_from("br label %"), lbl_name(merge_lbl)));
emit_lbl(state, merge_lbl);
__match_result_5 = rt_string_from("0");
}
 else if ((*__match_val_5).tag == TAG_Expr_While) {
    Expr* cond = (*__match_val_5).data.While.f0;
    NyxVec body = (*__match_val_5).data.While.f1;
int64_t header_lbl = next_lbl(state);
int64_t loop_lbl = next_lbl(state);
int64_t exit_lbl = next_lbl(state);
emit(state, rt_string_concat(rt_string_from("br label %"), lbl_name(header_lbl)));
emit_lbl(state, header_lbl);
rt_string_t cond_v = emit_expr(state, cond);
int64_t cond1 = next_val(state);
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(val_name(cond1), rt_string_from(" = trunc i64 ")), cond_v), rt_string_from(" to i1")));
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("br i1 "), val_name(cond1)), rt_string_from(", label %")), lbl_name(loop_lbl)), rt_string_from(", label %")), lbl_name(exit_lbl)));
emit_lbl(state, loop_lbl);
for (int64_t _stmt = 0; _stmt < body.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(body, _stmt);
emit_stmt(state, &(stmt));
}
;
emit(state, rt_string_concat(rt_string_from("br label %"), lbl_name(header_lbl)));
emit_lbl(state, exit_lbl);
__match_result_5 = rt_string_from("0");
}
 else if ((*__match_val_5).tag == TAG_Expr_Call) {
    Expr* func = (*__match_val_5).data.Call.f0;
    NyxVec generic_args = (*__match_val_5).data.Call.f1;
    NyxVec args = (*__match_val_5).data.Call.f2;
rt_string_t fn_name = ({
rt_string_t __match_result_9;
Expr* __match_val_9 = func;
if ((*__match_val_9).tag == TAG_Expr_Ident) {
    rt_string_t n = (*__match_val_9).data.Ident.f0;
__match_result_9 = ({ rt_string_t _c = n; rt_string_retain(&_c); _c; });
}
 else {
__match_result_9 = rt_string_from("__unknown__");
}
__match_result_9;
});
NyxVec arg_vals = rt_vec_new(sizeof(int64_t));
for (int64_t _arg = 0; _arg < args.len; _arg++) {
    Expr arg = *(Expr*)rt_vec_get(args, _arg);
rt_string_t av = emit_expr(state, &(arg));
rt_vec_push(&arg_vals, ({ rt_string_t _tmp_10 = (rt_string_concat(rt_string_from("i64 "), av)); &_tmp_10; }));
}
;
int64_t v = next_val(state);
rt_string_t arg_str = rt_string_from("");
int64_t ai = 0LL;
while ((ai < arg_vals.len)) {
({ if ((ai > 0LL)) {
arg_str = rt_string_concat(arg_str, rt_string_from(", "));
}
});
arg_str = rt_string_concat(arg_str, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)(*(int64_t*)rt_vec_get(arg_vals, ai))); _s.ref_count = 1; _s; }));
ai = (ai + 1LL);
}
;
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(val_name(v), rt_string_from(" = call i64 @")), fn_name), rt_string_from("(")), arg_str), rt_string_from(")")));
__match_result_5 = val_name(v);
}
 else if ((*__match_val_5).tag == TAG_Expr_MethodCall) {
    Expr* receiver = (*__match_val_5).data.MethodCall.f0;
    rt_string_t method = (*__match_val_5).data.MethodCall.f1;
    NyxVec generic_args = (*__match_val_5).data.MethodCall.f2;
    NyxVec args = (*__match_val_5).data.MethodCall.f3;
rt_string_t recv_v = emit_expr(state, receiver);
rt_string_t arg_str = rt_string_concat(rt_string_from("i64 "), recv_v);
for (int64_t _arg = 0; _arg < args.len; _arg++) {
    Expr arg = *(Expr*)rt_vec_get(args, _arg);
rt_string_t av = emit_expr(state, &(arg));
arg_str = rt_string_concat(rt_string_concat(arg_str, rt_string_from(", i64 ")), av);
}
;
int64_t v = next_val(state);
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(val_name(v), rt_string_from(" = call i64 @nyx_method_")), ({ rt_string_t _c = method; rt_string_retain(&_c); _c; })), rt_string_from("(")), arg_str), rt_string_from(")")));
__match_result_5 = val_name(v);
}
 else if ((*__match_val_5).tag == TAG_Expr_FieldAccess) {
    Expr* obj = (*__match_val_5).data.FieldAccess.f0;
    rt_string_t field = (*__match_val_5).data.FieldAccess.f1;
rt_string_t obj_v = emit_expr(state, obj);
int64_t v = next_val(state);
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(val_name(v), rt_string_from(" = extractvalue { i64, i64 } ")), obj_v), rt_string_from(", 0")));
__match_result_5 = val_name(v);
}
 else if ((*__match_val_5).tag == TAG_Expr_Return) {
emit(state, rt_string_from("ret i64 0"));
__match_result_5 = rt_string_from("0");
}
 else if ((*__match_val_5).tag == TAG_Expr_Block) {
    NyxVec stmts = (*__match_val_5).data.Block.f0;
rt_string_t last = rt_string_from("0");
for (int64_t _stmt = 0; _stmt < stmts.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(stmts, _stmt);
last = emit_stmt(state, &(stmt));
}
;
__match_result_5 = last;
}
 else if ((*__match_val_5).tag == TAG_Expr_Await) {
    Expr* inner = (*__match_val_5).data.Await.f0;
__match_result_5 = emit_expr(state, inner);
}
 else {
__match_result_5 = rt_string_from("0");
}
__match_result_5;
});
}

rt_string_t emit_stmt(CgState* state, Stmt* stmt) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_12;
Stmt* __match_val_12 = stmt;
if ((*__match_val_12).tag == TAG_Stmt_Let) {
    rt_string_t name = (*__match_val_12).data.Let.f0;
    int mutable = (*__match_val_12).data.Let.f1;
    NyxOption ty = (*__match_val_12).data.Let.f2;
    Expr value = (*__match_val_12).data.Let.f3;
int64_t v_alloca = next_val(state);
emit(state, rt_string_concat(val_name(v_alloca), rt_string_from(" = alloca i64, align 8")));
(state->body = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat((*state).body, rt_string_from("  %")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("_ptr = bitcast i64* ")), val_name(v_alloca)), rt_string_from(" to i64*\n")));
rt_string_t init_v = emit_expr(state, &(value));
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("store i64 "), init_v), rt_string_from(", i64* %")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("_ptr, align 8")));
__match_result_12 = init_v;
}
 else if ((*__match_val_12).tag == TAG_Stmt_Expr) {
    Expr expr = (*__match_val_12).data.Expr.f0;
__match_result_12 = emit_expr(state, &(expr));
}
 else if ((*__match_val_12).tag == TAG_Stmt_Return) {
emit(state, rt_string_from("ret i64 0"));
__match_result_12 = rt_string_from("0");
}
__match_result_12;
});
}

rt_string_t emit_params(NyxVec* params) {
Arena* __arena = arena_create();
rt_string_t out = rt_string_from("");
int64_t i = 0LL;
while ((i < params->len)) {
Param* p = (&((*(Param*)rt_vec_get(*params, i))));
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from(", "));
}
});
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, llvm_type((&((*p).ty)))), rt_string_from(" %")), ({ rt_string_t _c = (*p).name; rt_string_retain(&_c); _c; })), rt_string_from("_arg"));
i = (i + 1LL);
}
;
arena_destroy(__arena);
return out;
}

void emit_param_allocas(CgState* state, NyxVec* params) {
Arena* __arena = arena_create();
int64_t i = 0LL;
while ((i < params->len)) {
Param* p = (&((*(Param*)rt_vec_get(*params, i))));
rt_string_t ty = llvm_type((&((*p).ty)));
int64_t v = next_val(state);
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(val_name(v), rt_string_from(" = alloca ")), ty), rt_string_from(", align 8")));
(state->body = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat((*state).body, rt_string_from("  %")), ({ rt_string_t _c = (*p).name; rt_string_retain(&_c); _c; })), rt_string_from("_ptr = bitcast ")), ty), rt_string_from("* ")), val_name(v)), rt_string_from(" to ")), ty), rt_string_from("*\n")));
emit(state, rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("store "), ty), rt_string_from(" %")), ({ rt_string_t _c = (*p).name; rt_string_retain(&_c); _c; })), rt_string_from("_arg, ")), ty), rt_string_from("* %")), ({ rt_string_t _c = (*p).name; rt_string_retain(&_c); _c; })), rt_string_from("_ptr, align 8")));
i = (i + 1LL);
}
;
arena_destroy(__arena);
}

rt_string_t emit_function(CgState* state, rt_string_t name, NyxVec* params, Type* ret_type, Expr* body) {
Arena* __arena = arena_create();
rt_string_t param_ir = emit_params(params);
rt_string_t ret_ir = llvm_type(ret_type);
rt_string_t fn_ir = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("define "), ret_ir), rt_string_from(" @")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)name /* clone ptr */); _s.ref_count = 1; _s; })), rt_string_from("(")), param_ir), rt_string_from(") {\n"));
fn_ir = rt_string_concat(fn_ir, rt_string_from("entry:\n"));
rt_string_t saved_body = ({ rt_string_t _c = (*state).body; rt_string_retain(&_c); _c; });
int64_t saved_val = (*state).val_counter;
int64_t saved_lbl = (*state).lbl_counter;
(state->body = rt_string_from(""));
(state->val_counter = 0LL);
(state->lbl_counter = 0LL);
emit_param_allocas(state, params);
emit_expr(state, body);
({ if (((!rt_string_ends_with((*state).body, rt_string_from("ret i64 0\n"))) && (!rt_string_ends_with((*state).body, rt_string_from("ret void\n"))))) {
({ rt_string_t __if_res_16; if ((ret_type.tag == TAG_Type_Void)) {
__if_res_16 = (state->body = rt_string_concat((*state).body, rt_string_from("  ret void\n")));
} else {
__if_res_16 = (state->body = rt_string_concat((*state).body, rt_string_from("  ret i64 0\n")));
}
__if_res_16; });
}
});
fn_ir = rt_string_concat(rt_string_concat(fn_ir, (*state).body), rt_string_from("}\n\n"));
(state->body = saved_body);
(state->val_counter = saved_val);
(state->lbl_counter = saved_lbl);
arena_destroy(__arena);
return fn_ir;
}

rt_string_t generate_llvm(NyxVec* program) {
Arena* __arena = arena_create();
CgState state = cg_new();
rt_string_t mod_ir = rt_string_from("; ModuleID = 'nyx_module'\n");
mod_ir = rt_string_concat(mod_ir, rt_string_from("source_filename = \"nyx_main.nyx\"\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("target datalayout = \"e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128\"\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("target triple = \"x86_64-pc-windows-msvc\"\n\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("; LTO: whole-program link-time optimization enabled\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("; PGO: profile-guided basic-block reordering enabled\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("!llvm.module.flags = !{!0, !1}\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("!0 = !{i32 7, !\"PIC Level\", i32 2}\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("!1 = !{i32 1, !\"ProfileSummary\", !2}\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("!2 = !{!\"TotalCount\", i64 0}\n\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("; --- External C runtime ---\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("declare i32 @printf(i8*, ...)\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("declare i8* @malloc(i64)\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("declare void @free(i8*)\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("declare i64 @strlen(i8*)\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("declare i8* @strcpy(i8*, i8*)\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("declare void @rt_ui_init()\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("declare void @rt_ui_poll_events()\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("declare void @rt_arena_alloc(i64)\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("declare void @rt_arena_reset()\n"));
mod_ir = rt_string_concat(mod_ir, rt_string_from("declare i64 @nyx_clock_us()\n\n"));
rt_string_t functions_ir = rt_string_from("");
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
rt_string_t __match_result_16;
TopLevel __match_val_16 = item;
if (__match_val_16.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_16.data.FnDecl.f0;
    NyxVec generics = __match_val_16.data.FnDecl.f1;
    NyxVec params = __match_val_16.data.FnDecl.f2;
    Type ret_type = __match_val_16.data.FnDecl.f3;
    Expr body = __match_val_16.data.FnDecl.f4;
    int is_async = __match_val_16.data.FnDecl.f5;
if (rt_string_eq(name, rt_string_from("main"))) {
rt_string_t fn_ir = emit_function((&(state)), ({ rt_string_t _atmp_1 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }); &_atmp_1; }), (&(params)), (&(ret_type)), (&(body)));
functions_ir = rt_string_concat(functions_ir, fn_ir);
__match_result_16 = (functions_ir = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(functions_ir, rt_string_from("define i32 @main(i32 %argc, i8** %argv) {\n")), rt_string_from("entry:\n")), rt_string_from("  call void @rt_ui_init()\n")), rt_string_from("  call i64 @nyx_main()\n")), rt_string_from("  ret i32 0\n")), rt_string_from("}\n\n")));
} else {
rt_string_t fn_ir = emit_function((&(state)), ({ rt_string_t _atmp_1 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }); &_atmp_1; }), (&(params)), (&(ret_type)), (&(body)));
__match_result_16 = (functions_ir = rt_string_concat(functions_ir, fn_ir));
}
}
 else if (__match_val_16.tag == TAG_TopLevel_StructDef) {
    rt_string_t name = __match_val_16.data.StructDef.f0;
    NyxVec generics = __match_val_16.data.StructDef.f1;
    NyxVec fields = __match_val_16.data.StructDef.f2;
mod_ir = rt_string_concat(rt_string_concat(rt_string_concat(mod_ir, rt_string_from("%")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" = type { "));
rt_string_t field_types = rt_string_from("");
int64_t fi = 0LL;
for (int64_t _idx_fname_ftype = 0; _idx_fname_ftype < fields.len; _idx_fname_ftype++) {
    NyxTuple2 _tup_fname_ftype = *(NyxTuple2*)rt_vec_get(fields, _idx_fname_ftype);
    rt_string_t fname = *(rt_string_t*)(_tup_fname_ftype.f0);
    int64_t ftype = (int64_t)(_tup_fname_ftype.f1);
({ if ((fi > 0LL)) {
field_types = rt_string_concat(field_types, rt_string_from(", "));
}
});
field_types = rt_string_concat(field_types, llvm_type((&(ftype))));
fi = (fi + 1LL);
}
;
__match_result_16 = (mod_ir = rt_string_concat(rt_string_concat(mod_ir, field_types), rt_string_from(" }\n")));
}
 else if (__match_val_16.tag == TAG_TopLevel_ExternBlock) {
    rt_string_t lib = __match_val_16.data.ExternBlock.f0;
    NyxVec items = __match_val_16.data.ExternBlock.f1;
mod_ir = rt_string_concat(rt_string_concat(rt_string_concat(mod_ir, rt_string_from("; extern block from lib ")), ({ rt_string_t _c = lib; rt_string_retain(&_c); _c; })), rt_string_from("\n"));
for (int64_t _ext_item = 0; _ext_item < items.len; _ext_item++) {
    TopLevel ext_item = *(TopLevel*)rt_vec_get(items, _ext_item);
({
rt_string_t __match_result_18;
TopLevel __match_val_18 = ext_item;
if (__match_val_18.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_18.data.FnDecl.f0;
    NyxVec params = __match_val_18.data.FnDecl.f2;
    Type ret_type = __match_val_18.data.FnDecl.f3;
rt_string_t param_ir = emit_params((&(params)));
__match_result_18 = (mod_ir = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(mod_ir, rt_string_from("declare ")), llvm_type((&(ret_type)))), rt_string_from(" @")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from("(")), param_ir), rt_string_from(")\n")));
}
 else {
}
__match_result_18;
});
}
;
}
 else if (__match_val_16.tag == TAG_TopLevel_ConstDecl) {
    rt_string_t name = __match_val_16.data.ConstDecl.f0;
    Type ty = __match_val_16.data.ConstDecl.f1;
    Expr value = __match_val_16.data.ConstDecl.f2;
__match_result_16 = (mod_ir = rt_string_concat(rt_string_concat(rt_string_concat(mod_ir, rt_string_from("; const ")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" (TODO: global constant)\n")));
}
 else {
}
__match_result_16;
});
}
;
rt_string_t out = rt_string_concat(mod_ir, rt_string_from("\n"));
({ if ((state.globals.length > 0LL)) {
out = rt_string_concat(out, rt_string_from("; --- String constants ---\n"));
out = rt_string_concat(rt_string_concat(out, state.globals), rt_string_from("\n"));
}
});
arena_destroy(__arena);
return rt_string_concat(out, functions_ir);
}

