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
    rt_string_t name;
    rt_string_t version;
    rt_string_t edition;
    rt_string_t description;
    rt_string_t author;
    NyxMap dependencies;
    NyxMap dev_dependencies;
    rt_string_t build_command;
    NyxVec capabilities;
} Manifest;

Manifest new_manifest();
NyxResult load_manifest(rt_string_t);
NyxResult parse(rt_string_t);
NyxResult save_manifest(Manifest*, rt_string_t);
rt_string_t strip_quotes(rt_string_t);

#line 1 "manifest.nyx"
// module manifest
// import std::io
// import std::fs
// import std::string
Manifest new_manifest() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (Manifest){.name = rt_string_from(""), .version = rt_string_from("0.1.0"), .edition = rt_string_from("2024"), .description = rt_string_from(""), .author = rt_string_from(""), .dependencies = rt_map_new(), .dev_dependencies = rt_map_new(), .build_command = rt_string_from(""), .capabilities = rt_vec_new(sizeof(rt_string_t))};
}

NyxResult load_manifest(rt_string_t path) {
Arena* __arena = arena_create();
NyxResult content = ({
rt_string_t __match_result_0;
NyxResult __match_val_0 = read_to_string(path);
if (__match_val_0.tag == RESULT_OK) {
    void* c = __match_val_0.data.ptr_val;
__match_result_0 = c;
}
 else if (__match_val_0.tag == RESULT_ERR) {
    rt_string_t e = rt_string_from((char*)__match_val_0.data.ptr_val);
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("Failed to read "), path), rt_string_from(": ")), e); _stmp_.data; }); _tmp; });
}
__match_result_0;
});
arena_destroy(__arena);
return parse(content);
}

NyxResult parse(rt_string_t content) {
Arena* __arena = arena_create();
Manifest manifest = new_manifest();
rt_string_t current_section = rt_string_from("");
NyxVec lines = rt_string_split(content, rt_string_from("\n"));
for (int64_t _line = 0; _line < lines.len; _line++) {
    int64_t line = *(int64_t*)rt_vec_get(lines, _line);
rt_string_t trimmed = int64_t_trim(line);
({ if ((rt_string_eq(trimmed, rt_string_from("")) || rt_string_starts_with(trimmed, rt_string_from("#")))) {
continue;
}
});
({ if ((rt_string_starts_with(trimmed, rt_string_from("[")) && rt_string_ends_with(trimmed, rt_string_from("]")))) {
current_section = rt_string_slice(trimmed, 1LL, (trimmed.length - 1LL));
continue;
}
});
({ if (rt_string_contains(trimmed, rt_string_from("="))) {
int64_t eq_pos = rt_string_index_of(trimmed, rt_string_from("="));
rt_string_t key = rt_string_trim(rt_string_slice(trimmed, 0LL, eq_pos));
rt_string_t value_raw = rt_string_trim(rt_string_slice(trimmed, (eq_pos + 1LL), trimmed.length));
rt_string_t value = strip_quotes(value_raw);
({
NyxResult __match_result_4;
rt_string_t __match_val_4 = current_section;
if (rt_string_eq(__match_val_4, rt_string_from("package"))) {
__match_result_4 = ({
int64_t __match_result_5;
rt_string_t __match_val_5 = key;
if (rt_string_eq(__match_val_5, rt_string_from("name"))) {
__match_result_5 = (manifest.name = value);
}
 else if (rt_string_eq(__match_val_5, rt_string_from("version"))) {
__match_result_5 = (manifest.version = value);
}
 else if (rt_string_eq(__match_val_5, rt_string_from("edition"))) {
__match_result_5 = (manifest.edition = value);
}
 else if (rt_string_eq(__match_val_5, rt_string_from("description"))) {
__match_result_5 = (manifest.description = value);
}
 else if (rt_string_eq(__match_val_5, rt_string_from("author"))) {
__match_result_5 = (manifest.author = value);
}
 else if (rt_string_eq(__match_val_5, rt_string_from("build"))) {
__match_result_5 = (manifest.build_command = value);
}
 else {
}
__match_result_5;
});
}
 else if (rt_string_eq(__match_val_4, rt_string_from("dependencies"))) {
rt_map_insert(&(manifest.dependencies), key, (int64_t)(({ rt_string_t* _mtmp = malloc(sizeof(rt_string_t)); *_mtmp = value; (int64_t)_mtmp; })));
}
 else if (rt_string_eq(__match_val_4, rt_string_from("dev-dependencies"))) {
rt_map_insert(&(manifest.dev_dependencies), key, (int64_t)(({ rt_string_t* _mtmp = malloc(sizeof(rt_string_t)); *_mtmp = value; (int64_t)_mtmp; })));
}
 else if (rt_string_eq(__match_val_4, rt_string_from("capabilities"))) {
rt_vec_push(&manifest.capabilities, ({ rt_string_t _tmp_6 = (value); &_tmp_6; }));
}
 else {
}
__match_result_4;
});
}
});
}
;
({ if (rt_string_eq(manifest.name, rt_string_from(""))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Manifest missing [package] name"); _stmp_.data; }); _tmp; });
}
});
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Manifest _stmp_ = manifest; &_stmp_; }); _tmp; });
}

NyxResult save_manifest(Manifest* manifest, rt_string_t path) {
Arena* __arena = arena_create();
rt_string_t content = rt_string_from("# Nyx package manifest - auto-generated\n\n");
content = rt_string_concat(content, rt_string_from("[package]\n"));
content = rt_string_concat(rt_string_concat(rt_string_concat(content, rt_string_from("name = \"")), (*manifest).name), rt_string_from("\"\n"));
content = rt_string_concat(rt_string_concat(rt_string_concat(content, rt_string_from("version = \"")), (*manifest).version), rt_string_from("\"\n"));
content = rt_string_concat(rt_string_concat(rt_string_concat(content, rt_string_from("edition = \"")), (*manifest).edition), rt_string_from("\"\n"));
({ if (!rt_string_eq((*manifest).description, rt_string_from(""))) {
content = rt_string_concat(rt_string_concat(rt_string_concat(content, rt_string_from("description = \"")), (*manifest).description), rt_string_from("\"\n"));
}
});
({ if (!rt_string_eq((*manifest).author, rt_string_from(""))) {
content = rt_string_concat(rt_string_concat(rt_string_concat(content, rt_string_from("author = \"")), (*manifest).author), rt_string_from("\"\n"));
}
});
({ if (!rt_string_eq((*manifest).build_command, rt_string_from(""))) {
content = rt_string_concat(rt_string_concat(rt_string_concat(content, rt_string_from("build = \"")), (*manifest).build_command), rt_string_from("\"\n"));
}
});
({ if (((*manifest).dependencies.len > 0LL)) {
content = rt_string_concat(content, rt_string_from("\n[dependencies]\n"));
for (int64_t _idx_name_version = 0; _idx_name_version < rt_map_iter(&((*manifest).dependencies)).len; _idx_name_version++) {
    NyxTuple2 _tup_name_version = *(NyxTuple2*)rt_vec_get(rt_map_iter(&((*manifest).dependencies)), _idx_name_version);
    rt_string_t name = *(rt_string_t*)(_tup_name_version.f0);
    int64_t version = (int64_t)(_tup_name_version.f1);
content = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(content, name), rt_string_from(" = \"")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)version); _s.ref_count = 1; _s; })), rt_string_from("\"\n"));
}
;
}
});
({ if (((*manifest).dev_dependencies.len > 0LL)) {
content = rt_string_concat(content, rt_string_from("\n[dev-dependencies]\n"));
for (int64_t _idx_name_version = 0; _idx_name_version < rt_map_iter(&((*manifest).dev_dependencies)).len; _idx_name_version++) {
    NyxTuple2 _tup_name_version = *(NyxTuple2*)rt_vec_get(rt_map_iter(&((*manifest).dev_dependencies)), _idx_name_version);
    rt_string_t name = *(rt_string_t*)(_tup_name_version.f0);
    int64_t version = (int64_t)(_tup_name_version.f1);
content = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(content, name), rt_string_from(" = \"")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)version); _s.ref_count = 1; _s; })), rt_string_from("\"\n"));
}
;
}
});
({ if (((*manifest).capabilities.len > 0LL)) {
content = rt_string_concat(content, rt_string_from("\n[capabilities]\n"));
for (int64_t _cap = 0; _cap < rt_vec_iter((*manifest).capabilities).len; _cap++) {
    int64_t cap = *(int64_t*)rt_vec_get(rt_vec_iter((*manifest).capabilities), _cap);
content = rt_string_concat(rt_string_concat(rt_string_concat(content, rt_string_from("\"")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)cap); _s.ref_count = 1; _s; })), rt_string_from("\"\n"));
}
;
}
});
arena_destroy(__arena);
return write(path, content);
}

rt_string_t strip_quotes(rt_string_t s) {
Arena* __arena = arena_create();
({ if ((rt_string_starts_with(s, rt_string_from("\"")) && rt_string_ends_with(s, rt_string_from("\"")))) {
return rt_string_slice(s, 1LL, (s.length - 1LL));
}
});
({ if ((rt_string_starts_with(s, rt_string_from("'")) && rt_string_ends_with(s, rt_string_from("'")))) {
return rt_string_slice(s, 1LL, (s.length - 1LL));
}
});
arena_destroy(__arena);
return s;
}

