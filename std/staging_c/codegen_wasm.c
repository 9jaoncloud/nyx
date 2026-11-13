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

rt_string_t generate_wasm(NyxVec*);

#line 1 "codegen_wasm.nyx"
// module codegen_wasm
// import ast
// import std::string
// import std::vec
rt_string_t generate_wasm(NyxVec* program) {
Arena* __arena = arena_create();
rt_string_t out = ast.Literal_from(String, rt_string_from("(module\n"));
out = rt_string_concat(out, rt_string_from("  (import \"env\" \"memory\" (memory 256 2048))\n"));
out = rt_string_concat(out, rt_string_from("  (import \"env\" \"clock_now_ms\" (func $clock_now_ms (result f64)))\n"));
out = rt_string_concat(out, rt_string_from("  (import \"nyx\" \"canvas_fill_rect\" (func $canvas_fill_rect (param i32 i32 i32 i32 i32)))\n"));
out = rt_string_concat(out, rt_string_from("  (import \"nyx\" \"canvas_draw_text\" (func $canvas_draw_text (param i32 i32 i32 i32 i32)))\n"));
out = rt_string_concat(out, rt_string_from("  (import \"nyx\" \"canvas_draw_circle\" (func $canvas_draw_circle (param i32 i32 i32 i32)))\n"));
out = rt_string_concat(out, rt_string_from("  (import \"nyx\" \"canvas_draw_gradient_rect\" (func $canvas_draw_gradient_rect (param i32 i32 i32 i32 i32 i32)))\n"));
out = rt_string_concat(out, rt_string_from("  (import \"nyx\" \"canvas_draw_glow\" (func $canvas_draw_glow (param i32 i32 i32 i32 i32 i32)))\n"));
out = rt_string_concat(out, rt_string_from("  (import \"nyx\" \"canvas_draw_ripple\" (func $canvas_draw_ripple (param i32 i32 i32 i32 f32)))\n"));
out = rt_string_concat(out, rt_string_from("\n"));
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    ast.TopLevel item = *(ast.TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
rt_string_t __match_result_0;
ast.TopLevel __match_val_0 = item;
{
    rt_string_t name = __match_val_0.name;
    NyxVec params = __match_val_0.params;
    void* return_type = (void*)(__match_val_0.return_type);
    void* body = (void*)(__match_val_0.body);
out = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(out, rt_string_from("  (func $")), name), rt_string_from(" (export \"")), name), rt_string_from("\")\n"));
out = rt_string_concat(out, rt_string_from("    ;; WebAssembly Function Body\n"));
__match_result_0 = (out = rt_string_concat(out, rt_string_from("  )\n\n")));
}
 else {
}
__match_result_0;
});
}
;
out = rt_string_concat(out, rt_string_from("  (func (export \"nyx_main\")\n"));
out = rt_string_concat(out, rt_string_from("    ;; Entry point for WASM Web Runtime\n"));
out = rt_string_concat(out, rt_string_from("  )\n"));
out = rt_string_concat(out, rt_string_from("  (func (export \"nyx_render_frame\")\n"));
out = rt_string_concat(out, rt_string_from("    ;; 120 FPS Frame Callback\n"));
out = rt_string_concat(out, rt_string_from("  )\n"));
out = rt_string_concat(out, rt_string_from(")\n"));
arena_destroy(__arena);
return out;
}

