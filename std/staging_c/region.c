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

NyxVec infer_regions(NyxVec);
Expr wrap_body(Expr*);

#line 1 "region.nyx"
// module region
// import ast::*
NyxVec infer_regions(NyxVec program) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_vec_collect(int64_t_map(rt_vec_iter(program), ({ NyxVec __lambda_0(int64_t item) { return ({
TopLevel __match_result_1;
int64_t __match_val_1 = item;
if (__match_val_1.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_1.data.FnDecl.f0;
    NyxVec generics = __match_val_1.data.FnDecl.f1;
    NyxVec params = __match_val_1.data.FnDecl.f2;
    Type ret_type = __match_val_1.data.FnDecl.f3;
    Expr body = __match_val_1.data.FnDecl.f4;
    int is_async = __match_val_1.data.FnDecl.f5;
__match_result_1 = (TopLevel){ .tag = TAG_TopLevel_FnDecl, .data.FnDecl = { .f0 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), .f1 = rt_vec_clone(generics), .f2 = rt_vec_clone(params), .f3 = ret_type, .f4 = wrap_body(&(body)), .f5 = is_async } };
}
 else {
__match_result_1 = item;
}
__match_result_1;
}); } __lambda_0; })));
}

Expr wrap_body(Expr* body) {
Arena* __arena = arena_create();
arena_destroy(__arena);
{ Expr* __iflet_2 = body;
if ((*__iflet_2).tag == TAG_Expr_Block) {
    NyxVec stmts = (*__iflet_2).data.Block.f0;
NyxVec new_stmts = rt_vec_new(sizeof(int64_t));
rt_vec_push(&new_stmts, ({ Stmt _tmp_3 = ((Stmt){ .tag = TAG_Stmt_Let, .data.Let = { .f0 = rt_string_from("__arena"), .f1 = 1, .f2 = rt_option_none(), .f3 = (Expr){ .tag = TAG_Expr_Call, .data.Call = { .f0 = ({ Expr _stmp_0 = (Expr){ .tag = TAG_Expr_Ident, .data.Ident = {.f0 = rt_string_from("arena_create")} }; &_stmp_0; }), .f1 = rt_vec_new(sizeof(int64_t)), .f2 = rt_vec_new(sizeof(int64_t)) } } } }); &_tmp_3; }));
for (int64_t _stmt = 0; _stmt < stmts.len; _stmt++) {
    Stmt stmt = *(Stmt*)rt_vec_get(stmts, _stmt);
rt_vec_push(&new_stmts, ({ Stmt _tmp_4 = (stmt); &_tmp_4; }));
}
;
rt_vec_push(&new_stmts, ({ Stmt _tmp_5 = ((Stmt){ .tag = TAG_Stmt_Expr, .data.Expr = {.f0 = (Expr){ .tag = TAG_Expr_Call, .data.Call = { .f0 = ({ Expr _stmp_0 = (Expr){ .tag = TAG_Expr_Ident, .data.Ident = {.f0 = rt_string_from("arena_destroy")} }; &_stmp_0; }), .f1 = rt_vec_new(sizeof(int64_t)), .f2 = ({
NyxVec a = rt_vec_new(sizeof(int64_t));
rt_vec_push(&a, ({ Expr _tmp_6 = ((Expr){ .tag = TAG_Expr_Ident, .data.Ident = {.f0 = rt_string_from("__arena")} }); &_tmp_6; }));
a;
}) } }} }); &_tmp_5; }));
return (Expr){ .tag = TAG_Expr_Block, .data.Block = {.f0 = new_stmts} };
} else {
return body /* clone ptr */;
}
}
}

