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
    NyxMap types;
    NyxMap functions;
} Resolver;

typedef struct {
    rt_string_t name;
    NyxVec generics;
    NyxVec params;
    Type ret;
} FnSig;

Resolver new();
void Resolver_resolve_program(Resolver*, NyxVec*);

#line 1 "resolver.nyx"
// module resolver
// import ast::*
Resolver new() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (Resolver){.types = rt_map_new(), .functions = rt_map_new()};
}

void Resolver_resolve_program(Resolver* self, NyxVec* program) {
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
int64_t __match_result_0;
TopLevel __match_val_0 = item;
if (__match_val_0.tag == TAG_TopLevel_StructDef) {
    rt_string_t name = __match_val_0.data.StructDef.f0;
    NyxVec generics = __match_val_0.data.StructDef.f1;
    NyxVec fields = __match_val_0.data.StructDef.f2;
rt_map_insert(&((*self).types), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), (int64_t)(({ StructDef* _mtmp = malloc(sizeof(StructDef)); *_mtmp = (ast.TopLevel){ .tag = TAG_ast.TopLevel_StructDef, .data.StructDef = { .f0 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), .f1 = rt_vec_clone(generics), .f2 = rt_vec_clone(fields) } }; (int64_t)_mtmp; })));
}
 else if (__match_val_0.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_0.data.FnDecl.f0;
    NyxVec generics = __match_val_0.data.FnDecl.f1;
    NyxVec params = __match_val_0.data.FnDecl.f2;
    Type ret_type = __match_val_0.data.FnDecl.f3;
rt_map_insert(&((*self).functions), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), (int64_t)(({ FnSig* _mtmp = malloc(sizeof(FnSig)); *_mtmp = (FnSig){.name = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), .generics = rt_vec_clone(generics), .params = rt_vec_clone(params), .ret = ret_type}; (int64_t)_mtmp; })));
}
 else {
}
__match_result_0;
});
}
;
}

