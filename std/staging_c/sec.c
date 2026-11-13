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

#define TAG_Capability_FileSystem 0
#define TAG_Capability_Network 1
#define TAG_Capability_Process 2
#define TAG_Capability_Environment 3
#define TAG_Capability_UI 4
#define TAG_Capability_Hardware 5
#define TAG_Capability_FFI 6
#define TAG_Capability_Unsafe 7
typedef struct {
    int tag;
    union {
    } data;
} Capability;

typedef struct {
    Capability cap;
    int granted;
    int64_t granted_at;
    rt_string_t source_module;
} SecurityToken;

typedef struct {
    int64_t timestamp;
    rt_string_t mod_name;
    Capability cap;
    rt_string_t action;
    int allowed;
} AuditEntry;

typedef struct {
    NyxVec tokens;
    NyxVec audit_log;
    NyxVec manifest_caps;
    int strict_mode;
} SecurityContext;

SecurityContext new_context();
SecurityContext new_context_relaxed();
void SecurityContext_grant(SecurityContext*, Capability, rt_string_t);
void SecurityContext_revoke(SecurityContext*, Capability*, rt_string_t);
int SecurityContext_check(SecurityContext*, Capability*);
NyxResult SecurityContext_require(SecurityContext*, Capability*, rt_string_t);
NyxResult SecurityContext_load_manifest(SecurityContext*, rt_string_t);
rt_string_t SecurityContext_audit_summary(SecurityContext*);
rt_string_t SecurityContext_export_audit_log(SecurityContext*);
NyxResult check_file_access(SecurityContext*, rt_string_t, rt_string_t);
NyxResult check_network_access(SecurityContext*, rt_string_t, int64_t);
NyxResult check_process_access(SecurityContext*, rt_string_t);
NyxResult check_env_access(SecurityContext*, rt_string_t);
NyxResult check_ffi_access(SecurityContext*, rt_string_t);
NyxResult check_unsafe_access(SecurityContext*, rt_string_t);
// module sec
// import std::io
// import std::fs
// import std::string
SecurityContext new_context() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (SecurityContext){.tokens = rt_vec_new(sizeof(int64_t)), .audit_log = rt_vec_new(sizeof(int64_t)), .manifest_caps = rt_vec_new(sizeof(int64_t)), .strict_mode = 1};
}

SecurityContext new_context_relaxed() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (SecurityContext){.tokens = rt_vec_new(sizeof(int64_t)), .audit_log = rt_vec_new(sizeof(int64_t)), .manifest_caps = rt_vec_new(sizeof(int64_t)), .strict_mode = 0};
}

void SecurityContext_grant(SecurityContext* self, Capability cap, rt_string_t mod_name) {
rt_vec_push(&(*self).tokens, ({ SecurityToken _tmp_0 = (int64_t)((SecurityToken){.cap = cap, .granted = 1, .granted_at = 0LL, .source_module = ({ rt_string_t _c = mod_name; rt_string_retain(&_c); _c; })}); &_tmp_0; }));
rt_vec_push(&(*self).audit_log, ({ AuditEntry _tmp_1 = (int64_t)((AuditEntry){.timestamp = 0LL, .mod_name = mod_name, .cap = cap, .action = rt_string_from("grant"), .allowed = 1}); &_tmp_1; }));
}

void SecurityContext_revoke(SecurityContext* self, Capability* cap, rt_string_t* mod_name) {
for (int64_t _token = 0; _token < rt_vec_iter_mut((*self).tokens).len; _token++) {
    int64_t token = *(int64_t*)rt_vec_get(rt_vec_iter_mut((*self).tokens), _token);
({ if (((token.cap.tag == (*(cap))->tag) && rt_string_eq(token.source_module, (*(mod_name))))) {
(token.granted = 0);
}
});
}
;
rt_vec_push(&(*self).audit_log, ({ AuditEntry _tmp_3 = (int64_t)((AuditEntry){.timestamp = 0LL, .mod_name = mod_name /* clone ptr */, .cap = cap /* clone ptr */, .action = rt_string_from("revoke"), .allowed = 1}); &_tmp_3; }));
}

int SecurityContext_check(SecurityContext* self, Capability* cap) {
({ if ((!(*self).strict_mode)) {
return 1;
}
});
return int64_t_any(rt_vec_iter((*self).tokens), ({ int __lambda_5(int64_t t) { return ((t.cap.tag == (*(cap))->tag) && t.granted); } __lambda_5; }));
}

NyxResult SecurityContext_require(SecurityContext* self, Capability* cap, rt_string_t* operation) {
({ if ((!SecurityContext_check(self, cap))) {
rt_vec_push(&(*self).audit_log, ({ AuditEntry _tmp_7 = (int64_t)((AuditEntry){.timestamp = 0LL, .mod_name = rt_string_from(""), .cap = cap /* clone ptr */, .action = operation /* clone ptr */, .allowed = 0}); &_tmp_7; }));
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Security violation: capability not granted for "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)operation); _s.ref_count = 1; _s; })); _stmp_.data; }); _tmp; });
}
});
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
}

NyxResult SecurityContext_load_manifest(SecurityContext* self, rt_string_t path) {
NyxResult content = ({
rt_string_t __match_result_8;
NyxResult __match_val_8 = read_to_string(path);
if (__match_val_8.tag == RESULT_OK) {
    void* c = __match_val_8.data.ptr_val;
__match_result_8 = c;
}
 else if (__match_val_8.tag == RESULT_ERR) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
}
__match_result_8;
});
NyxVec lines = rt_result_split(content, rt_string_from("\n"));
for (int64_t _line = 0; _line < lines.len; _line++) {
    int64_t line = *(int64_t*)rt_vec_get(lines, _line);
rt_string_t trimmed = int64_t_trim(line);
({ if (rt_string_eq(trimmed, rt_string_from("[capabilities]"))) {
continue;
}
});
({ if (rt_string_starts_with(trimmed, rt_string_from("\""))) {
int64_t cap_name = int64_t_trim_matches(rt_string_trim_matches(trimmed, '\"'), ',');
NyxResult cap = ({
Capability __match_result_11;
int64_t __match_val_11 = cap_name;
if (__match_val_11 == rt_string_from("filesystem")) {
__match_result_11 = (Capability){ .tag = TAG_Capability_FileSystem };
}
 else if (__match_val_11 == rt_string_from("network")) {
__match_result_11 = (Capability){ .tag = TAG_Capability_Network };
}
 else if (__match_val_11 == rt_string_from("process")) {
__match_result_11 = (Capability){ .tag = TAG_Capability_Process };
}
 else if (__match_val_11 == rt_string_from("environment")) {
__match_result_11 = (Capability){ .tag = TAG_Capability_Environment };
}
 else if (__match_val_11 == rt_string_from("ui")) {
__match_result_11 = (Capability){ .tag = TAG_Capability_UI };
}
 else if (__match_val_11 == rt_string_from("hardware")) {
__match_result_11 = (Capability){ .tag = TAG_Capability_Hardware };
}
 else if (__match_val_11 == rt_string_from("ffi")) {
__match_result_11 = (Capability){ .tag = TAG_Capability_FFI };
}
 else if (__match_val_11 == rt_string_from("unsafe")) {
__match_result_11 = (Capability){ .tag = TAG_Capability_Unsafe };
}
 else {
continue;
}
__match_result_11;
});
rt_vec_push(&(*self).manifest_caps, ({ NyxResult _tmp_12 = (int64_t)(cap); &_tmp_12; }));
SecurityContext_grant(self, cap, rt_string_from("manifest"));
}
});
}
;
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
}

rt_string_t SecurityContext_audit_summary(SecurityContext* self) {
int64_t denied = 0LL;
int64_t allowed = 0LL;
for (int64_t _entry = 0; _entry < rt_vec_iter((*self).audit_log).len; _entry++) {
    int64_t entry = *(int64_t*)rt_vec_get(rt_vec_iter((*self).audit_log), _entry);
({ int64_t __if_res_14; if (entry.allowed) {
__if_res_14 = (allowed = (allowed + 1LL));
} else {
__if_res_14 = (denied = (denied + 1LL));
}
__if_res_14; });
}
;
return rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("Audit: "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)allowed); _buf.ref_count = 1; _buf; })), rt_string_from(" allowed, ")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)denied); _buf.ref_count = 1; _buf; })), rt_string_from(" denied"));
}

rt_string_t SecurityContext_export_audit_log(SecurityContext* self) {
rt_string_t output = rt_string_from("# Security Audit Log\n\n");
for (int64_t _entry = 0; _entry < rt_vec_iter((*self).audit_log).len; _entry++) {
    int64_t entry = *(int64_t*)rt_vec_get(rt_vec_iter((*self).audit_log), _entry);
rt_string_t cap_name = ({
rt_string_t __match_result_14;
Capability __match_val_14 = entry.cap;
if (__match_val_14.tag == TAG_Capability_FileSystem) {
__match_result_14 = rt_string_from("filesystem");
}
 else if (__match_val_14.tag == TAG_Capability_Network) {
__match_result_14 = rt_string_from("network");
}
 else if (__match_val_14.tag == TAG_Capability_Process) {
__match_result_14 = rt_string_from("process");
}
 else if (__match_val_14.tag == TAG_Capability_Environment) {
__match_result_14 = rt_string_from("environment");
}
 else if (__match_val_14.tag == TAG_Capability_UI) {
__match_result_14 = rt_string_from("ui");
}
 else if (__match_val_14.tag == TAG_Capability_Hardware) {
__match_result_14 = rt_string_from("hardware");
}
 else if (__match_val_14.tag == TAG_Capability_FFI) {
__match_result_14 = rt_string_from("ffi");
}
 else if (__match_val_14.tag == TAG_Capability_Unsafe) {
__match_result_14 = rt_string_from("unsafe");
}
__match_result_14;
});
rt_string_t status = ({ rt_string_t __if_res_16; if (entry.allowed) {
__if_res_16 = rt_string_from("ALLOWED");
} else {
__if_res_16 = rt_string_from("DENIED");
}
__if_res_16; });
output = rt_string_concat(output, ({ rt_string_t _utmp_ = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(entry.mod_name, rt_string_from(" ")), cap_name), rt_string_from(" ")), ({ rt_string_t _utmp_ = rt_string_concat(rt_string_concat(rt_string_concat(entry.action, rt_string_from(" [")), status), rt_string_from("]\n")); &_utmp_; })); &_utmp_; }));
}
;
return output;
}

NyxResult check_file_access(SecurityContext* ctx, rt_string_t path, rt_string_t operation) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return SecurityContext_require(ctx, ({ Capability _matmp_0 = (&((Capability){ .tag = TAG_Capability_FileSystem })); &_matmp_0; }), ({ rt_string_t _matmp_1 = ({ rt_string_t _utmp_ = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("file "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)operation); _s.ref_count = 1; _s; })), rt_string_from(" on ")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)path); _s.ref_count = 1; _s; })); &_utmp_; }); &_matmp_1; }));
}

NyxResult check_network_access(SecurityContext* ctx, rt_string_t host, int64_t port) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return SecurityContext_require(ctx, ({ Capability _matmp_0 = (&((Capability){ .tag = TAG_Capability_Network })); &_matmp_0; }), ({ rt_string_t _matmp_1 = ({ rt_string_t _utmp_ = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("network connect to "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)host); _s.ref_count = 1; _s; })), rt_string_from(":")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)port); _buf.ref_count = 1; _buf; })); &_utmp_; }); &_matmp_1; }));
}

NyxResult check_process_access(SecurityContext* ctx, rt_string_t command) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return SecurityContext_require(ctx, ({ Capability _matmp_0 = (&((Capability){ .tag = TAG_Capability_Process })); &_matmp_0; }), ({ rt_string_t _matmp_1 = ({ rt_string_t _utmp_ = rt_string_concat(rt_string_from("process spawn: "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)command); _s.ref_count = 1; _s; })); &_utmp_; }); &_matmp_1; }));
}

NyxResult check_env_access(SecurityContext* ctx, rt_string_t var) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return SecurityContext_require(ctx, ({ Capability _matmp_0 = (&((Capability){ .tag = TAG_Capability_Environment })); &_matmp_0; }), ({ rt_string_t _matmp_1 = ({ rt_string_t _utmp_ = rt_string_concat(rt_string_from("env var access: "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)var); _s.ref_count = 1; _s; })); &_utmp_; }); &_matmp_1; }));
}

NyxResult check_ffi_access(SecurityContext* ctx, rt_string_t lib) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return SecurityContext_require(ctx, ({ Capability _matmp_0 = (&((Capability){ .tag = TAG_Capability_FFI })); &_matmp_0; }), ({ rt_string_t _matmp_1 = ({ rt_string_t _utmp_ = rt_string_concat(rt_string_from("FFI call to "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)lib); _s.ref_count = 1; _s; })); &_utmp_; }); &_matmp_1; }));
}

NyxResult check_unsafe_access(SecurityContext* ctx, rt_string_t operation) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return SecurityContext_require(ctx, ({ Capability _matmp_0 = (&((Capability){ .tag = TAG_Capability_Unsafe })); &_matmp_0; }), ({ rt_string_t _matmp_1 = ({ rt_string_t _utmp_ = rt_string_concat(rt_string_from("unsafe operation: "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)operation); _s.ref_count = 1; _s; })); &_utmp_; }); &_matmp_1; }));
}

