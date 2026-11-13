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
    NyxVec regions;
    int64_t escape_count;
} RegionAnalyzerResult;

typedef struct {
    rt_string_t name;
    rt_string_t kind;
    int has_escapes;
    NyxVec bound_variables;
} RegionInfo;

typedef struct {
    rt_string_t name;
    int escapes;
} VariableInfo;

typedef struct {
    NyxVec specializations;
} MonomorphizeResult;

typedef struct {
    rt_string_t original_name;
    rt_string_t specialized_name;
    NyxVec type_args;
} Specialization;

typedef struct {
    NyxVec closures;
} ClosureConvertResult;

typedef struct {
    rt_string_t name;
    rt_string_t env_struct_name;
    NyxVec captured_vars;
} ClosureInfo;

typedef struct {
    rt_string_t name;
    int requires_heap;
} CapturedVar;

typedef struct {
} RegionAnalyzer;

typedef struct {
} Monomorphizer;

typedef struct {
} ClosureConverter;

int vec_contains(NyxVec*, rt_string_t);
NyxOption get_option_val(NyxVec*, rt_string_t);
void nyx_main();
rt_string_t token_str(Token*);
void print_token(Token*);
void print_top_level(TopLevel*);
rt_string_t type_to_str(Type*);
void print_type(Type*);
RegionAnalyzer RegionAnalyzer_new();
RegionAnalyzerResult RegionAnalyzer_analyze(RegionAnalyzer*, NyxVec*);
Monomorphizer Monomorphizer_new();
MonomorphizeResult Monomorphizer_monomorphize(Monomorphizer*, NyxVec*);
ClosureConverter ClosureConverter_new();
ClosureConvertResult ClosureConverter_convert(ClosureConverter*, NyxVec*);

#line 1 "main.nyx"
// module main
// import lexer
// import token
// import parser
// import ast
// import typecheck
// import codegen_mlir
// import codegen_llvm
int vec_contains(NyxVec* args, rt_string_t flag) {
Arena* __arena = arena_create();
int64_t i = 0LL;
while ((i < args->len)) {
({ if (rt_string_eq((*(rt_string_t*)rt_vec_get(*args, i)), flag)) {
return 1;
}
});
i = (i + 1LL);
}
;
arena_destroy(__arena);
return 0;
}

NyxOption get_option_val(NyxVec* args, rt_string_t flag) {
Arena* __arena = arena_create();
int64_t i = 0LL;
while ((i < args->len)) {
({ if ((rt_string_eq((*(rt_string_t*)rt_vec_get(*args, i)), flag) && ((i + 1LL) < args->len))) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = ({ rt_string_t _c = (*(rt_string_t*)rt_vec_get(*args, (i + 1LL))); rt_string_retain(&_c); _c; }); _stmp_.data; }); _tmp; });
}
});
i = (i + 1LL);
}
;
arena_destroy(__arena);
return rt_option_none();
}

void nyx_main() {
Arena* __arena = arena_create();
NyxVec args = rt_env_args();
({ if ((args.len < 2LL)) {
printf("%s\n", rt_string_from("Nyx Compiler (nyxc) v0.54.0").data);
printf("%s\n", rt_string_from("Usage: nyxc <file.nyx> [options]").data);
printf("%s\n", rt_string_from("").data);
printf("%s\n", rt_string_from("Options:").data);
printf("%s\n", rt_string_from("  --tokens     Print token stream").data);
printf("%s\n", rt_string_from("  --ast        Print AST").data);
printf("%s\n", rt_string_from("  --types      Run type checking").data);
printf("%s\n", rt_string_from("  --regions    Run region inference").data);
printf("%s\n", rt_string_from("  --generics   Run monomorphization").data);
printf("%s\n", rt_string_from("  --closures   Run closure conversion").data);
printf("%s\n", rt_string_from("  --mlir       Generate MLIR code").data);
printf("%s\n", rt_string_from("  --llvm       Generate LLVM IR bitcode").data);
printf("%s\n", rt_string_from("  --wasm       Target WebAssembly (wasm32) module").data);
printf("%s\n", rt_string_from("  --android    Target Android NDK shared library (.so)").data);
printf("%s\n", rt_string_from("  --ios        Target iOS native static bundle (.xcframework)").data);
printf("%s\n", rt_string_from("  --check      Run all checks (type + region + closure)").data);
printf("%s\n", rt_string_from("  --all        Run all phases with output").data);
printf("%s\n", rt_string_from("  --clean-cache Clean incremental compilation cache").data);
printf("%s\n", rt_string_from("  --no-cache   Bypass cache reading and writing").data);
printf("%s\n", rt_string_from("  --output FILE  Output file for compiled code").data);
return;
}
});
rt_string_t filename = ({ rt_string_t _c = (*(rt_string_t*)rt_vec_get(args, 1LL)); rt_string_retain(&_c); _c; });
rt_string_t source = rt_string_from("");
({
int64_t __match_result_3;
NyxResult __match_val_3 = read_to_string(({ rt_string_t _c = filename; rt_string_retain(&_c); _c; }));
if (__match_val_3.tag == RESULT_OK) {
    rt_string_t s = rt_string_from((char*)__match_val_3.data.ptr_val);
source = s;
int64_t _ok = 1LL;
}
 else if (__match_val_3.tag == RESULT_ERR) {
    rt_string_t e = rt_string_from((char*)__match_val_3.data.ptr_val);
printf("%s\n", rt_string_concat(rt_string_from("Error: could not read file: "), filename).data);
return;
}
__match_result_3;
});
({ if ((source.length == 0LL)) {
printf("%s\n", rt_string_concat(rt_string_from("Error: file is empty: "), filename).data);
return;
}
});
int show_tokens = vec_contains((&(args)), rt_string_from("--tokens"));
int show_ast = vec_contains((&(args)), rt_string_from("--ast"));
int show_types = vec_contains((&(args)), rt_string_from("--types"));
int show_regions = vec_contains((&(args)), rt_string_from("--regions"));
int show_generics = vec_contains((&(args)), rt_string_from("--generics"));
int show_closures = vec_contains((&(args)), rt_string_from("--closures"));
int show_mlir = vec_contains((&(args)), rt_string_from("--mlir"));
int show_llvm = vec_contains((&(args)), rt_string_from("--llvm"));
int target_wasm = (vec_contains((&(args)), rt_string_from("--wasm")) || vec_contains((&(args)), rt_string_from("--target=wasm32")));
int target_android = (vec_contains((&(args)), rt_string_from("--android")) || vec_contains((&(args)), rt_string_from("--target=android")));
int target_ios = (vec_contains((&(args)), rt_string_from("--ios")) || vec_contains((&(args)), rt_string_from("--target=ios")));
int run_check = vec_contains((&(args)), rt_string_from("--check"));
int show_all = vec_contains((&(args)), rt_string_from("--all"));
NyxOption output_file = get_option_val((&(args)), rt_string_from("--output"));
Lexer lex = new_lexer(({ rt_string_t _c = source; rt_string_retain(&_c); _c; }));
({ if ((show_tokens || show_all)) {
printf("%s\n", rt_string_from("=== Phase 1: Lexing ===").data);
while (1) {
Token tok = Lexer_next_token(&(lex));
print_token((&(tok)));
printf("%s", rt_string_from(" ").data);
({
int64_t __match_result_6;
TokenKind __match_val_6 = tok.kind;
if (__match_val_6.tag == TAG_TokenKind_EOF) {
break;
}
 else {
}
__match_result_6;
});
}
;
printf("%s\n", rt_string_from("").data);
}
});
Parser parse = new_parser(({ rt_string_t _c = source; rt_string_retain(&_c); _c; }));
NyxVec program = Parser_parse_program(&(parse));
({ if (parse.had_error) {
printf("%s\n", rt_string_from("Parse errors:").data);
for (int64_t _err = 0; _err < parse.errors.len; _err++) {
    rt_string_t err = *(rt_string_t*)rt_vec_get(parse.errors, _err);
printf("%s\n", rt_string_concat(rt_string_from("  - "), err).data);
}
;
return;
}
});
({ if ((show_ast || show_all)) {
printf("%s\n", rt_string_from("=== Phase 2: AST ===").data);
for (int64_t _item = 0; _item < ((NyxVec*)((&(program))))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)((&(program)))), _item);
print_top_level(&(item));
}
;
printf("%s\n", rt_string_from("").data);
}
});
TypeChecker checker = TypeChecker_new();
TypeChecker_check_program(&(checker), (&(program)));
({ if (TypeChecker_has_errors(&(checker))) {
printf("%s\n", rt_string_from("Type errors:").data);
for (int64_t _err = 0; _err < ((NyxVec*)(TypeChecker_get_errors(&(checker))))->len; _err++) {
    rt_string_t err = *(rt_string_t*)rt_vec_get(*((NyxVec*)(TypeChecker_get_errors(&(checker)))), _err);
printf("%s\n", rt_string_concat(rt_string_from("  - "), err).data);
}
;
return;
}
});
({ if (((show_types || show_all) || run_check)) {
printf("%s\n", rt_string_from("=== Phase 3: Type Checking: PASSED ===").data);
printf("%s\n", rt_string_from("").data);
}
});
RegionAnalyzer region_analyzer = RegionAnalyzer_new();
RegionAnalyzerResult region_result = RegionAnalyzer_analyze(&(region_analyzer), (&(program)));
({ if (((show_regions || show_all) || run_check)) {
printf("%s\n", rt_string_from("=== Phase 4: Region Inference ===").data);
for (int64_t _region = 0; _region < ((NyxVec*)((&(region_result.regions))))->len; _region++) {
    RegionInfo region = *(RegionInfo*)rt_vec_get(*((NyxVec*)((&(region_result.regions)))), _region);
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("  Region: "), region.name), rt_string_from(" (kind: ")), region.kind), rt_string_from(")")).data);
({ if (region.has_escapes) {
printf("%s\n", rt_string_from("    Has escaping references - converting to ARC").data);
}
});
for (int64_t _var = 0; _var < ((NyxVec*)((&(region.bound_variables))))->len; _var++) {
    VariableInfo var = *(VariableInfo*)rt_vec_get(*((NyxVec*)((&(region.bound_variables)))), _var);
rt_string_t esc_str = ({ rt_string_t __if_res_14; if (var.escapes) {
__if_res_14 = rt_string_from("true");
} else {
__if_res_14 = rt_string_from("false");
}
__if_res_14; });
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("    Bound: "), var.name), rt_string_from(" (escapes: ")), esc_str), rt_string_from(")")).data);
}
;
}
;
printf("%s\n", rt_string_concat(rt_string_from("  Regions analyzed: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)region_result.regions.len); _buf.ref_count = 1; _buf; })).data);
printf("%s\n", rt_string_concat(rt_string_from("  Escapes detected: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)region_result.escape_count); _buf.ref_count = 1; _buf; })).data);
printf("%s\n", rt_string_from("").data);
}
});
MonomorphizeResult mono_result = Monomorphizer_monomorphize(({ Monomorphizer _stmp_ = Monomorphizer_new(); &_stmp_; }), (&(program)));
({ if ((show_generics || show_all)) {
printf("%s\n", rt_string_from("=== Phase 5: Monomorphization ===").data);
for (int64_t _spec = 0; _spec < ((NyxVec*)((&(mono_result.specializations))))->len; _spec++) {
    Specialization spec = *(Specialization*)rt_vec_get(*((NyxVec*)((&(mono_result.specializations)))), _spec);
rt_string_t ta_str = rt_string_from("");
for (int64_t i = 0LL; i < spec.type_args.len; i++) {
({ if ((i > 0LL)) {
ta_str = rt_string_concat(ta_str, rt_string_from(", "));
}
});
ta_str = rt_string_concat(ta_str, ({ rt_string_t _c = (*(rt_string_t*)rt_vec_get(spec.type_args, i)); rt_string_retain(&_c); _c; }));
}
;
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("  "), spec.original_name), rt_string_from("<")), ta_str), rt_string_from(">")).data);
printf("%s\n", rt_string_concat(rt_string_from("    -> "), spec.specialized_name).data);
}
;
printf("%s\n", rt_string_concat(rt_string_from("  Specializations: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)mono_result.specializations.len); _buf.ref_count = 1; _buf; })).data);
printf("%s\n", rt_string_from("").data);
}
});
ClosureConvertResult closure_result = ClosureConverter_convert(({ ClosureConverter _stmp_ = ClosureConverter_new(); &_stmp_; }), (&(program)));
({ if (((show_closures || show_all) || run_check)) {
printf("%s\n", rt_string_from("=== Phase 6: Closure Conversion ===").data);
for (int64_t _closure = 0; _closure < ((NyxVec*)((&(closure_result.closures))))->len; _closure++) {
    ClosureInfo closure = *(ClosureInfo*)rt_vec_get(*((NyxVec*)((&(closure_result.closures)))), _closure);
printf("%s\n", rt_string_concat(rt_string_from("  Closure: "), closure.name).data);
printf("%s\n", rt_string_concat(rt_string_from("    Env struct: "), closure.env_struct_name).data);
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_from("    Captures: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)closure.captured_vars.len); _buf.ref_count = 1; _buf; })), rt_string_from(" variables")).data);
for (int64_t _cap = 0; _cap < ((NyxVec*)((&(closure.captured_vars))))->len; _cap++) {
    CapturedVar cap = *(CapturedVar*)rt_vec_get(*((NyxVec*)((&(closure.captured_vars)))), _cap);
rt_string_t heap_str = ({ rt_string_t __if_res_18; if (cap.requires_heap) {
__if_res_18 = rt_string_from("true");
} else {
__if_res_18 = rt_string_from("false");
}
__if_res_18; });
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("      - "), cap.name), rt_string_from(" (heap: ")), heap_str), rt_string_from(")")).data);
}
;
}
;
printf("%s\n", rt_string_concat(rt_string_from("  Closures converted: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)closure_result.closures.len); _buf.ref_count = 1; _buf; })).data);
printf("%s\n", rt_string_from("").data);
}
});
({ if ((show_mlir || show_all)) {
printf("%s\n", rt_string_from("=== Phase 7: MLIR Code Generation ===").data);
rt_string_t mlir_code = generate_mlir((&(program)));
printf("%s\n", mlir_code.data);
printf("%s\n", rt_string_from("").data);
}
});
({ if ((target_wasm || show_all)) {
printf("%s\n", rt_string_from("=== Phase 8: WebAssembly WASM Target ===").data);
printf("%s\n", rt_string_from("  Target Architecture: wasm32-unknown-unknown").data);
printf("%s\n", rt_string_from("  Web Runtime: web-runtime/nyx_web_runtime.js").data);
printf("%s\n", rt_string_from("  Canvas Engine: HTML5 2D / WebGL Material 3").data);
printf("%s\n", rt_string_from("").data);
}
});
({ if ((target_android || show_all)) {
printf("%s\n", rt_string_from("=== Phase 9: Android NDK Native Target ===").data);
printf("%s\n", rt_string_from("  Target Architecture: aarch64-linux-android").data);
printf("%s\n", rt_string_from("  Native Runtime: runtime/rt_android.c").data);
printf("%s\n", rt_string_from("  NDK Package: libnyx_app.so").data);
printf("%s\n", rt_string_from("").data);
}
});
({ if ((target_ios || show_all)) {
printf("%s\n", rt_string_from("=== Phase 10: iOS Native Target ===").data);
printf("%s\n", rt_string_from("  Target Architecture: arm64-apple-ios").data);
printf("%s\n", rt_string_from("  Native Runtime: runtime/rt_ios.m").data);
printf("%s\n", rt_string_from("  Framework Bundle: NyxApp.xcframework").data);
printf("%s\n", rt_string_from("").data);
}
});
({ if ((show_llvm || show_all)) {
printf("%s\n", rt_string_from("=== Phase 11: Direct LLVM IR Code Generation ===").data);
rt_string_t llvm_ir = generate_llvm((&(program)));
printf("%s\n", llvm_ir.data);
printf("%s\n", rt_string_from("").data);
}
});
({
int64_t __match_result_23;
NyxOption __match_val_23 = output_file;
if (__match_val_23.tag == OPTION_SOME) {
    rt_string_t path = rt_string_from((char*)__match_val_23.data.ptr_val);
rt_string_t mlir_code = generate_mlir((&(program)));
__match_result_23 = ({
int64_t __match_result_24;
NyxResult __match_val_24 = write_string(path, mlir_code);
if (__match_val_24.tag == RESULT_OK) {
printf("%s\n", rt_string_concat(rt_string_from("Output written to: "), path).data);
}
 else if (__match_val_24.tag == RESULT_ERR) {
    rt_string_t e = rt_string_from((char*)__match_val_24.data.ptr_val);
printf("%s\n", rt_string_concat(rt_string_from("Error writing output: "), e).data);
}
__match_result_24;
});
}
 else if (__match_val_23.tag == OPTION_NONE) {
}
__match_result_23;
});
printf("%s\n", rt_string_concat(rt_string_from("Compilation successful: "), filename).data);
arena_destroy(__arena);
}

rt_string_t token_str(Token* tok) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_25;
TokenKind* __match_val_25 = (&((*tok).kind));
if ((*__match_val_25).tag == TAG_TokenKind_Ident) {
    rt_string_t s = (*__match_val_25).data.Ident.f0;
__match_result_25 = rt_string_concat(rt_string_concat(rt_string_from("Ident("), ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })), rt_string_from(")"));
}
 else if ((*__match_val_25).tag == TAG_TokenKind_IntLit) {
    int64_t n = (*__match_val_25).data.IntLit.f0;
__match_result_25 = rt_string_concat(rt_string_concat(rt_string_from("Int("), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)n); _buf.ref_count = 1; _buf; })), rt_string_from(")"));
}
 else if ((*__match_val_25).tag == TAG_TokenKind_FloatLit) {
    double f = (*__match_val_25).data.FloatLit.f0;
__match_result_25 = rt_string_from("Float");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_StringLit) {
    rt_string_t s = (*__match_val_25).data.StringLit.f0;
__match_result_25 = rt_string_concat(rt_string_concat(rt_string_from("String(\""), ({ rt_string_t _c = s; rt_string_retain(&_c); _c; })), rt_string_from("\")"));
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Fn) {
__match_result_25 = rt_string_from("Fn");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Let) {
__match_result_25 = rt_string_from("Let");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Mut) {
__match_result_25 = rt_string_from("Mut");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_If) {
__match_result_25 = rt_string_from("If");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Else) {
__match_result_25 = rt_string_from("Else");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_While) {
__match_result_25 = rt_string_from("While");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_For) {
__match_result_25 = rt_string_from("For");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Return) {
__match_result_25 = rt_string_from("Return");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Struct) {
__match_result_25 = rt_string_from("Struct");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Enum) {
__match_result_25 = rt_string_from("Enum");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Match) {
__match_result_25 = rt_string_from("Match");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Async) {
__match_result_25 = rt_string_from("Async");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Await) {
__match_result_25 = rt_string_from("Await");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Import) {
__match_result_25 = rt_string_from("Import");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Module) {
__match_result_25 = rt_string_from("Module");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Pub) {
__match_result_25 = rt_string_from("Pub");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Plus) {
__match_result_25 = rt_string_from("+");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Minus) {
__match_result_25 = rt_string_from("-");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Star) {
__match_result_25 = rt_string_from("*");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Slash) {
__match_result_25 = rt_string_from("/");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Equals) {
__match_result_25 = rt_string_from("=");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_EqEq) {
__match_result_25 = rt_string_from("==");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_NotEq) {
__match_result_25 = rt_string_from("!=");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Lt) {
__match_result_25 = rt_string_from("<");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Gt) {
__match_result_25 = rt_string_from(">");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_LtEq) {
__match_result_25 = rt_string_from("<=");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_GtEq) {
__match_result_25 = rt_string_from(">=");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_And) {
__match_result_25 = rt_string_from("&&");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Or) {
__match_result_25 = rt_string_from("||");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Not) {
__match_result_25 = rt_string_from("!");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Arrow) {
__match_result_25 = rt_string_from("->");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_FatArrow) {
__match_result_25 = rt_string_from("=>");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_LParen) {
__match_result_25 = rt_string_from("(");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_RParen) {
__match_result_25 = rt_string_from(")");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_LBrace) {
__match_result_25 = rt_string_from("{");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_RBrace) {
__match_result_25 = rt_string_from("}");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_LBracket) {
__match_result_25 = rt_string_from("[");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_RBracket) {
__match_result_25 = rt_string_from("]");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Comma) {
__match_result_25 = rt_string_from(",");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Semicolon) {
__match_result_25 = rt_string_from(";");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Colon) {
__match_result_25 = rt_string_from(":");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Dot) {
__match_result_25 = rt_string_from(".");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_True) {
__match_result_25 = rt_string_from("true");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_False) {
__match_result_25 = rt_string_from("false");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_Null) {
__match_result_25 = rt_string_from("null");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_SelfKw) {
__match_result_25 = rt_string_from("self");
}
 else if ((*__match_val_25).tag == TAG_TokenKind_EOF) {
__match_result_25 = rt_string_from("EOF");
}
 else {
__match_result_25 = rt_string_from("Token");
}
__match_result_25;
});
}

void print_token(Token* tok) {
Arena* __arena = arena_create();
printf("%s", token_str(tok).data);
arena_destroy(__arena);
}

void print_top_level(TopLevel* item) {
Arena* __arena = arena_create();
({
int64_t __match_result_26;
TopLevel* __match_val_26 = item;
if ((*__match_val_26).tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = (*__match_val_26).data.FnDecl.f0;
    NyxVec generics = (*__match_val_26).data.FnDecl.f1;
    NyxVec params = (*__match_val_26).data.FnDecl.f2;
    Type ret_type = (*__match_val_26).data.FnDecl.f3;
    int is_async = (*__match_val_26).data.FnDecl.f5;
rt_string_t async_str = ({ rt_string_t __if_res_28; if (is_async) {
__if_res_28 = rt_string_from("async ");
} else {
__if_res_28 = rt_string_from("");
}
__if_res_28; });
printf("%s", rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("  "), async_str), rt_string_from("fn ")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })).data);
({ if ((generics.len > 0LL)) {
printf("%s", rt_string_from("<").data);
for (int64_t i = 0LL; i < generics.len; i++) {
({ if ((i > 0LL)) {
printf("%s", rt_string_from(", ").data);
}
});
printf("%s", ({ rt_string_t _c = (*(rt_string_t*)rt_vec_get(generics, i)); rt_string_retain(&_c); _c; }).data);
}
;
printf("%s", rt_string_from(">").data);
}
});
printf("%s", rt_string_from("(").data);
for (int64_t i = 0LL; i < params.len; i++) {
({ if ((i > 0LL)) {
printf("%s", rt_string_from(", ").data);
}
});
printf("%s", rt_string_concat(({ rt_string_t _c = ((*(Param*)rt_vec_get(params, i))).name; rt_string_retain(&_c); _c; }), rt_string_from(": ")).data);
print_type((&(((*(Param*)rt_vec_get(params, i))).ty)));
}
;
printf("%s", rt_string_from(") -> ").data);
print_type(&(ret_type));
printf("%s\n", rt_string_from("").data);
}
 else if ((*__match_val_26).tag == TAG_TopLevel_StructDef) {
    rt_string_t name = (*__match_val_26).data.StructDef.f0;
    NyxVec generics = (*__match_val_26).data.StructDef.f1;
printf("%s", rt_string_concat(rt_string_from("  struct "), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })).data);
({ if ((generics.len > 0LL)) {
printf("%s", rt_string_from("<").data);
for (int64_t i = 0LL; i < generics.len; i++) {
({ if ((i > 0LL)) {
printf("%s", rt_string_from(", ").data);
}
});
printf("%s", ({ rt_string_t _c = (*(rt_string_t*)rt_vec_get(generics, i)); rt_string_retain(&_c); _c; }).data);
}
;
printf("%s", rt_string_from(">").data);
}
});
printf("%s\n", rt_string_from("").data);
}
 else if ((*__match_val_26).tag == TAG_TopLevel_EnumDef) {
    rt_string_t name = (*__match_val_26).data.EnumDef.f0;
    NyxVec generics = (*__match_val_26).data.EnumDef.f1;
printf("%s", rt_string_concat(rt_string_from("  enum "), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })).data);
({ if ((generics.len > 0LL)) {
printf("%s", rt_string_from("<").data);
for (int64_t i = 0LL; i < generics.len; i++) {
({ if ((i > 0LL)) {
printf("%s", rt_string_from(", ").data);
}
});
printf("%s", ({ rt_string_t _c = (*(rt_string_t*)rt_vec_get(generics, i)); rt_string_retain(&_c); _c; }).data);
}
;
printf("%s", rt_string_from(">").data);
}
});
printf("%s\n", rt_string_from("").data);
}
 else if ((*__match_val_26).tag == TAG_TopLevel_ImplBlock) {
    rt_string_t type_name = (*__match_val_26).data.ImplBlock.f0;
    NyxVec generics = (*__match_val_26).data.ImplBlock.f1;
printf("%s", rt_string_concat(rt_string_from("  impl "), ({ rt_string_t _c = type_name; rt_string_retain(&_c); _c; })).data);
({ if ((generics.len > 0LL)) {
printf("%s", rt_string_from("<").data);
for (int64_t i = 0LL; i < generics.len; i++) {
({ if ((i > 0LL)) {
printf("%s", rt_string_from(", ").data);
}
});
printf("%s", ({ rt_string_t _c = (*(rt_string_t*)rt_vec_get(generics, i)); rt_string_retain(&_c); _c; }).data);
}
;
printf("%s", rt_string_from(">").data);
}
});
printf("%s\n", rt_string_from(" {").data);
printf("%s\n", rt_string_from("    ...").data);
printf("%s\n", rt_string_from("  }").data);
}
 else if ((*__match_val_26).tag == TAG_TopLevel_TraitDef) {
    rt_string_t name = (*__match_val_26).data.TraitDef.f0;
printf("%s\n", rt_string_concat(rt_string_from("  trait "), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })).data);
}
 else if ((*__match_val_26).tag == TAG_TopLevel_TypeAlias) {
    rt_string_t name = (*__match_val_26).data.TypeAlias.f0;
printf("%s\n", rt_string_concat(rt_string_concat(rt_string_from("  type "), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(" = ...")).data);
}
 else if ((*__match_val_26).tag == TAG_TopLevel_ConstDecl) {
    rt_string_t name = (*__match_val_26).data.ConstDecl.f0;
    Type ty = (*__match_val_26).data.ConstDecl.f1;
printf("%s", rt_string_concat(rt_string_concat(rt_string_from("  const "), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })), rt_string_from(": ")).data);
print_type(&(ty));
printf("%s\n", rt_string_from("").data);
}
 else if ((*__match_val_26).tag == TAG_TopLevel_ModuleDecl) {
    rt_string_t name = (*__match_val_26).data.ModuleDecl.f0;
printf("%s\n", rt_string_concat(rt_string_from("  module "), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; })).data);
}
 else if ((*__match_val_26).tag == TAG_TopLevel_UseDecl) {
    NyxVec path = (*__match_val_26).data.UseDecl.f0;
rt_string_t path_str = rt_string_from("");
for (int64_t i = 0LL; i < path.len; i++) {
({ if ((i > 0LL)) {
path_str = rt_string_concat(path_str, rt_string_from("."));
}
});
path_str = rt_string_concat(path_str, ({ rt_string_t _c = (*(rt_string_t*)rt_vec_get(path, i)); rt_string_retain(&_c); _c; }));
}
;
printf("%s\n", rt_string_concat(rt_string_from("  use "), path_str).data);
}
 else {
printf("%s\n", rt_string_from("  TopLevel(...)").data);
}
__match_result_26;
});
arena_destroy(__arena);
}

rt_string_t type_to_str(Type* ty) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ({
rt_string_t __match_result_38;
Type* __match_val_38 = ty;
if ((*__match_val_38).tag == TAG_Type_Int) {
__match_result_38 = rt_string_from("Int");
}
 else if ((*__match_val_38).tag == TAG_Type_Float) {
__match_result_38 = rt_string_from("Float");
}
 else if ((*__match_val_38).tag == TAG_Type_Bool) {
__match_result_38 = rt_string_from("Bool");
}
 else if ((*__match_val_38).tag == TAG_Type_String) {
__match_result_38 = rt_string_from("String");
}
 else if ((*__match_val_38).tag == TAG_Type_Char) {
__match_result_38 = rt_string_from("Char");
}
 else if ((*__match_val_38).tag == TAG_Type_Void) {
__match_result_38 = rt_string_from("Void");
}
 else if ((*__match_val_38).tag == TAG_Type_Custom) {
    rt_string_t name = (*__match_val_38).data.Custom.f0;
__match_result_38 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; });
}
 else if ((*__match_val_38).tag == TAG_Type_Generic) {
    rt_string_t name = (*__match_val_38).data.Generic.f0;
__match_result_38 = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; });
}
 else if ((*__match_val_38).tag == TAG_Type_Array) {
    Type* inner = (*__match_val_38).data.Array.f0;
__match_result_38 = rt_string_concat(rt_string_concat(rt_string_from("["), type_to_str(inner)), rt_string_from("]"));
}
 else if ((*__match_val_38).tag == TAG_Type_Ref) {
    Type* inner = (*__match_val_38).data.Ref.f0;
__match_result_38 = rt_string_concat(rt_string_from("&"), type_to_str(inner));
}
 else if ((*__match_val_38).tag == TAG_Type_MutRef) {
    Type* inner = (*__match_val_38).data.MutRef.f0;
__match_result_38 = rt_string_concat(rt_string_from("&mut "), type_to_str(inner));
}
 else if ((*__match_val_38).tag == TAG_Type_Option) {
    Type* inner = (*__match_val_38).data.Option.f0;
__match_result_38 = rt_string_concat(rt_string_concat(rt_string_from("Option<"), type_to_str(inner)), rt_string_from(">"));
}
 else if ((*__match_val_38).tag == TAG_Type_Result) {
    Type* inner = (*__match_val_38).data.Result.f0;
    Type* err = (*__match_val_38).data.Result.f1;
__match_result_38 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("Result<"), type_to_str(inner)), rt_string_from(", ")), type_to_str(err)), rt_string_from(">"));
}
 else if ((*__match_val_38).tag == TAG_Type_Tuple) {
    NyxVec types = (*__match_val_38).data.Tuple.f0;
rt_string_t s = rt_string_from("(");
for (int64_t i = 0LL; i < types.len; i++) {
({ if ((i > 0LL)) {
s = rt_string_concat(s, rt_string_from(", "));
}
});
s = rt_string_concat(s, type_to_str((&((*(Type*)rt_vec_get(types, i))))));
}
;
__match_result_38 = rt_string_concat(s, rt_string_from(")"));
}
 else if ((*__match_val_38).tag == TAG_Type_Func) {
    NyxVec params = (*__match_val_38).data.Func.f0;
    Type* ret = (*__match_val_38).data.Func.f1;
rt_string_t s = rt_string_from("fn(");
for (int64_t i = 0LL; i < params.len; i++) {
({ if ((i > 0LL)) {
s = rt_string_concat(s, rt_string_from(", "));
}
});
s = rt_string_concat(s, type_to_str((&((*(Type*)rt_vec_get(params, i))))));
}
;
__match_result_38 = rt_string_concat(rt_string_concat(s, rt_string_from(") -> ")), type_to_str(ret));
}
 else if ((*__match_val_38).tag == TAG_Type_Region) {
    rt_string_t name = (*__match_val_38).data.Region.f0;
__match_result_38 = rt_string_concat(rt_string_from("'"), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }));
}
 else if ((*__match_val_38).tag == TAG_Type_Recursive) {
    rt_string_t name = (*__match_val_38).data.Recursive.f0;
__match_result_38 = rt_string_concat(rt_string_from("rec "), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }));
}
 else {
__match_result_38 = rt_string_from("Type");
}
__match_result_38;
});
}

void print_type(Type* ty) {
Arena* __arena = arena_create();
printf("%s", type_to_str(ty).data);
arena_destroy(__arena);
}

RegionAnalyzer RegionAnalyzer_new() {
return (RegionAnalyzer){};
}

RegionAnalyzerResult RegionAnalyzer_analyze(RegionAnalyzer* self, NyxVec* program) {
NyxVec regions = rt_vec_new(sizeof(RegionInfo));
int64_t escape_count = 0LL;
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
int64_t __match_result_41;
TopLevel __match_val_41 = item;
if (__match_val_41.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_41.data.FnDecl.f0;
NyxVec bvars = rt_vec_new(sizeof(VariableInfo));
RegionInfo region = (RegionInfo){.name = rt_string_concat(({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), rt_string_from("_region")), .kind = rt_string_from("function"), .has_escapes = 0, .bound_variables = bvars};
({ RegionInfo _tmp_42 = (region); rt_vec_push(&regions, &_tmp_42); });
}
 else {
}
__match_result_41;
});
}
;
return (RegionAnalyzerResult){.regions = regions, .escape_count = escape_count};
}

Monomorphizer Monomorphizer_new() {
return (Monomorphizer){};
}

MonomorphizeResult Monomorphizer_monomorphize(Monomorphizer* self, NyxVec* program) {
NyxVec specializations = rt_vec_new(sizeof(Specialization));
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
int64_t __match_result_43;
TopLevel __match_val_43 = item;
if (__match_val_43.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_43.data.FnDecl.f0;
    NyxVec generics = __match_val_43.data.FnDecl.f1;
({ if ((generics.len > 0LL)) {
NyxVec targs = rt_vec_new(sizeof(rt_string_t));
({ rt_string_t _tmp_45 = (rt_string_from("i32")); rt_vec_push(&targs, &_tmp_45); });
({ Specialization _tmp_46 = ((Specialization){.original_name = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), .specialized_name = rt_string_concat(({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), rt_string_from("_i32")), .type_args = targs}); rt_vec_push(&specializations, &_tmp_46); });
}
});
}
 else {
}
__match_result_43;
});
}
;
return (MonomorphizeResult){.specializations = specializations};
}

ClosureConverter ClosureConverter_new() {
return (ClosureConverter){};
}

ClosureConvertResult ClosureConverter_convert(ClosureConverter* self, NyxVec* program) {
NyxVec closures = rt_vec_new(sizeof(ClosureInfo));
for (int64_t _item = 0; _item < ((NyxVec*)(program))->len; _item++) {
    TopLevel item = *(TopLevel*)rt_vec_get(*((NyxVec*)(program)), _item);
({
int64_t __match_result_47;
TopLevel __match_val_47 = item;
if (__match_val_47.tag == TAG_TopLevel_FnDecl) {
    rt_string_t name = __match_val_47.data.FnDecl.f0;
    NyxVec params = __match_val_47.data.FnDecl.f2;
for (int64_t _param = 0; _param < params.len; _param++) {
    Param param = *(Param*)rt_vec_get(params, _param);
({
int64_t __match_result_48;
Type* __match_val_48 = (&(param.ty));
if ((*__match_val_48).tag == TAG_Type_Func) {
NyxVec cvars = rt_vec_new(sizeof(rt_string_t));
({ ClosureInfo _tmp_49 = ((ClosureInfo){.name = ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), .env_struct_name = rt_string_concat(rt_string_from("Env_"), name), .captured_vars = cvars}); rt_vec_push(&closures, &_tmp_49); });
}
 else {
}
__match_result_48;
});
}
;
}
 else {
}
__match_result_47;
});
}
;
return (ClosureConvertResult){.closures = closures};
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    nyx_main();
    return 0;
}
