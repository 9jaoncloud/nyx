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
    rt_string_t url;
    rt_string_t checksum;
    NyxVec resolved_deps;
} LockEntry;

typedef struct {
    NyxVec entries;
} Lockfile;

Lockfile new_lockfile();
NyxResult load_lockfile(rt_string_t);
NyxResult parse(rt_string_t);
NyxResult save_lockfile(Lockfile*, rt_string_t);
int contains(Lockfile*, rt_string_t, rt_string_t);
NyxOption find(Lockfile*, rt_string_t);
void nyx_remove(Lockfile*, rt_string_t);
rt_string_t strip_quotes(rt_string_t);

#line 1 "lockfile.nyx"
// module lockfile
// import std::io
// import std::fs
// import std::string
Lockfile new_lockfile() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (Lockfile){.entries = rt_vec_new(sizeof(LockEntry))};
}

NyxResult load_lockfile(rt_string_t path) {
Arena* __arena = arena_create();
({ if ((!exists(path))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Lockfile _stmp_ = new_lockfile(); &_stmp_; }); _tmp; });
}
});
NyxResult content = ({
rt_string_t __match_result_1;
NyxResult __match_val_1 = read_to_string(path);
if (__match_val_1.tag == RESULT_OK) {
    void* c = __match_val_1.data.ptr_val;
__match_result_1 = c;
}
 else if (__match_val_1.tag == RESULT_ERR) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Lockfile _stmp_ = new_lockfile(); &_stmp_; }); _tmp; });
}
__match_result_1;
});
arena_destroy(__arena);
return parse(content);
}

NyxResult parse(rt_string_t content) {
Arena* __arena = arena_create();
Lockfile lockfile = new_lockfile();
rt_string_t current_name = rt_string_from("");
rt_string_t current_version = rt_string_from("");
rt_string_t current_url = rt_string_from("");
rt_string_t current_checksum = rt_string_from("");
int in_package = 0;
NyxVec lines = rt_string_split(content, rt_string_from("\n"));
for (int64_t _line = 0; _line < lines.len; _line++) {
    int64_t line = *(int64_t*)rt_vec_get(lines, _line);
rt_string_t trimmed = int64_t_trim(line);
({ if ((rt_string_eq(trimmed, rt_string_from("")) || rt_string_starts_with(trimmed, rt_string_from("#")))) {
continue;
}
});
({ if (rt_string_eq(trimmed, rt_string_from("[[package]]"))) {
({ if ((in_package && !rt_string_eq(current_name, rt_string_from("")))) {
rt_vec_push(&lockfile.entries, ({ LockEntry _tmp_5 = ((LockEntry){.name = ({ rt_string_t _c = current_name; rt_string_retain(&_c); _c; }), .version = ({ rt_string_t _c = current_version; rt_string_retain(&_c); _c; }), .url = ({ rt_string_t _c = current_url; rt_string_retain(&_c); _c; }), .checksum = ({ rt_string_t _c = current_checksum; rt_string_retain(&_c); _c; }), .resolved_deps = rt_vec_new(sizeof(rt_string_t))}); &_tmp_5; }));
current_name = rt_string_from("");
current_version = rt_string_from("");
current_url = rt_string_from("");
current_checksum = rt_string_from("");
}
});
in_package = 1;
continue;
}
});
({ if (rt_string_contains(trimmed, rt_string_from("="))) {
int64_t eq_pos = rt_string_index_of(trimmed, rt_string_from("="));
rt_string_t key = rt_string_trim(rt_string_slice(trimmed, 0LL, eq_pos));
rt_string_t value_raw = rt_string_trim(rt_string_slice(trimmed, (eq_pos + 1LL), trimmed.length));
rt_string_t value = strip_quotes(value_raw);
({
rt_string_t __match_result_7;
rt_string_t __match_val_7 = key;
if (rt_string_eq(__match_val_7, rt_string_from("name"))) {
__match_result_7 = (current_name = value);
}
 else if (rt_string_eq(__match_val_7, rt_string_from("version"))) {
__match_result_7 = (current_version = value);
}
 else if (rt_string_eq(__match_val_7, rt_string_from("url"))) {
__match_result_7 = (current_url = value);
}
 else if (rt_string_eq(__match_val_7, rt_string_from("checksum"))) {
__match_result_7 = (current_checksum = value);
}
 else {
}
__match_result_7;
});
}
});
}
;
({ if ((in_package && !rt_string_eq(current_name, rt_string_from("")))) {
rt_vec_push(&lockfile.entries, ({ LockEntry _tmp_9 = ((LockEntry){.name = ({ rt_string_t _c = current_name; rt_string_retain(&_c); _c; }), .version = ({ rt_string_t _c = current_version; rt_string_retain(&_c); _c; }), .url = ({ rt_string_t _c = current_url; rt_string_retain(&_c); _c; }), .checksum = ({ rt_string_t _c = current_checksum; rt_string_retain(&_c); _c; }), .resolved_deps = rt_vec_new(sizeof(rt_string_t))}); &_tmp_9; }));
}
});
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Lockfile _stmp_ = lockfile; &_stmp_; }); _tmp; });
}

NyxResult save_lockfile(Lockfile* lockfile, rt_string_t path) {
Arena* __arena = arena_create();
rt_string_t content = rt_string_from("# Nyx lockfile - auto-generated, do not edit\n\n");
for (int64_t _entry = 0; _entry < rt_vec_iter((*lockfile).entries).len; _entry++) {
    int64_t entry = *(int64_t*)rt_vec_get(rt_vec_iter((*lockfile).entries), _entry);
content = rt_string_concat(content, rt_string_from("[[package]]\n"));
content = rt_string_concat(rt_string_concat(rt_string_concat(content, rt_string_from("name = \"")), entry.name), rt_string_from("\"\n"));
content = rt_string_concat(rt_string_concat(rt_string_concat(content, rt_string_from("version = \"")), entry.version), rt_string_from("\"\n"));
content = rt_string_concat(rt_string_concat(rt_string_concat(content, rt_string_from("url = \"")), entry.url), rt_string_from("\"\n"));
content = rt_string_concat(rt_string_concat(rt_string_concat(content, rt_string_from("checksum = \"")), entry.checksum), rt_string_from("\"\n\n"));
}
;
arena_destroy(__arena);
return write(path, content);
}

int contains(Lockfile* lockfile, rt_string_t name, rt_string_t version) {
Arena* __arena = arena_create();
for (int64_t _entry = 0; _entry < rt_vec_iter((*lockfile).entries).len; _entry++) {
    int64_t entry = *(int64_t*)rt_vec_get(rt_vec_iter((*lockfile).entries), _entry);
({ if ((rt_string_eq(entry.name, (*(name))) && rt_string_eq(entry.version, (*(version))))) {
return 1;
}
});
}
;
arena_destroy(__arena);
return 0;
}

NyxOption find(Lockfile* lockfile, rt_string_t name) {
Arena* __arena = arena_create();
for (int64_t _entry = 0; _entry < rt_vec_iter((*lockfile).entries).len; _entry++) {
    int64_t entry = *(int64_t*)rt_vec_get(rt_vec_iter((*lockfile).entries), _entry);
({ if (rt_string_eq(entry.name, (*(name)))) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)(entry); _tmp; });
}
});
}
;
arena_destroy(__arena);
return rt_option_none();
}

void nyx_remove(Lockfile* lockfile, rt_string_t name) {
Arena* __arena = arena_create();
rt_vec_retain((*lockfile).entries, ({ int __lambda_12(int64_t e) { return !rt_string_eq(e.name, (*(name))); } __lambda_12; }));
arena_destroy(__arena);
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

