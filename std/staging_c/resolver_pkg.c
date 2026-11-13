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
} ResolvedPackage;

typedef struct {
    rt_string_t registry_url;
    NyxVec resolved;
    NyxVec errors;
} Resolver;

Resolver new_resolver();
NyxResult Resolver_resolve_manifest(Resolver*, manifest.Manifest*, lockfile.Lockfile*);
NyxResult Resolver_resolve_package(Resolver*, rt_string_t, rt_string_t, lockfile.Lockfile*, Map*);
NyxResult Resolver_resolve_version_from_registry(Resolver*, rt_string_t, rt_string_t);
NyxVec Resolver_parse_version_list(Resolver*, rt_string_t);
NyxResult Resolver_pick_best_version(Resolver*, NyxVec*, rt_string_t);
int Resolver_satisfies_version(Resolver*, rt_string_t, rt_string_t);
int Resolver_version_gte(Resolver*, rt_string_t, rt_string_t);

#line 1 "resolver_pkg.nyx"
// module resolver_pkg
// import std::io
// import std::string
// import manifest
// import lockfile
Resolver new_resolver() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (Resolver){.registry_url = rt_string_from("https://packages.nyx-lang.org"), .resolved = rt_vec_new(sizeof(ResolvedPackage)), .errors = rt_vec_new(sizeof(rt_string_t))};
}

NyxResult Resolver_resolve_manifest(Resolver* self, manifest.Manifest* pkg, lockfile.Lockfile* lock) {
rt_vec_clear(&((*self).resolved));
rt_vec_clear(&((*self).errors));
NyxMap all_deps = rt_map_new();
for (int64_t _idx_name_version = 0; _idx_name_version < rt_map_iter(&((*pkg).dependencies)).len; _idx_name_version++) {
    NyxTuple2 _tup_name_version = *(NyxTuple2*)rt_vec_get(rt_map_iter(&((*pkg).dependencies)), _idx_name_version);
    rt_string_t name = *(rt_string_t*)(_tup_name_version.f0);
    int64_t version = (int64_t)(_tup_name_version.f1);
rt_map_insert(&(all_deps), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), (int64_t)(version));
}
;
for (int64_t _idx_name_version = 0; _idx_name_version < rt_map_iter(&((*pkg).dev_dependencies)).len; _idx_name_version++) {
    NyxTuple2 _tup_name_version = *(NyxTuple2*)rt_vec_get(rt_map_iter(&((*pkg).dev_dependencies)), _idx_name_version);
    rt_string_t name = *(rt_string_t*)(_tup_name_version.f0);
    int64_t version = (int64_t)(_tup_name_version.f1);
rt_map_insert(&(all_deps), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }), (int64_t)(version));
}
;
NyxMap visited = rt_map_new();
for (int64_t _idx_name_version = 0; _idx_name_version < rt_map_iter(&(all_deps)).len; _idx_name_version++) {
    NyxTuple2 _tup_name_version = *(NyxTuple2*)rt_vec_get(rt_map_iter(&(all_deps)), _idx_name_version);
    rt_string_t name = *(rt_string_t*)(_tup_name_version.f0);
    int64_t version = (int64_t)(_tup_name_version.f1);
({ NyxResult _qres = Resolver_resolve_package(self, &(name), &(version), lock, (&(visited))); if (_qres.tag == RESULT_ERR) { return _qres; } (int64_t)_qres.data.int_val; });
}
;
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = rt_vec_clone((*self).resolved); &_stmp_; }); _tmp; });
}

NyxResult Resolver_resolve_package(Resolver* self, rt_string_t* name, rt_string_t* version_req, lockfile.Lockfile* lock, void* visited) {
({ if (contains_key(visited, name)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
}
});
rt_map_insert(visited, name /* clone ptr */, (int64_t)(1));
/* iflet */ 0;
rt_string_t resolved_version = ({ NyxResult _qres = Resolver_resolve_version_from_registry(self, name, version_req); if (_qres.tag == RESULT_ERR) { return _qres; } rt_string_from((char*)_qres.data.ptr_val); });
rt_vec_push(&(*self).resolved, ({ ResolvedPackage _tmp_1 = ((ResolvedPackage){.name = name /* clone ptr */, .version = ({ rt_string_t _c = resolved_version; rt_string_retain(&_c); _c; }), .url = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat((*self).registry_url, rt_string_from("/packages/")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)name); _s.ref_count = 1; _s; })), rt_string_from("/")), resolved_version), rt_string_from(".tar.gz"))}); &_tmp_1; }));
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
}

NyxResult Resolver_resolve_version_from_registry(Resolver* self, rt_string_t* name, rt_string_t* version_req) {
rt_string_t url = rt_string_concat(rt_string_concat(rt_string_concat((*self).registry_url, rt_string_from("/api/packages/")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)name); _s.ref_count = 1; _s; })), rt_string_from("/versions"));
rt_println(rt_string_concat(rt_string_concat(rt_string_from("  Fetching versions for "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)name); _s.ref_count = 1; _s; })), rt_string_from(" from registry...")));
int64_t client = int64_t_new_client(http);
NyxResult response = ({
void* __match_result_2;
NyxResult __match_val_2 = rt_map_get(&(client), url);
if (__match_val_2.tag == RESULT_OK) {
    void* r = __match_val_2.data.ptr_val;
__match_result_2 = r;
}
 else if (__match_val_2.tag == RESULT_ERR) {
__match_result_2 = ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Failed to fetch package info from registry: "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)name); _s.ref_count = 1; _s; })); _stmp_.data; }); _tmp; });
}
__match_result_2;
});
({ if ((response.status_code == 404LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Package not found in registry: "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)name); _s.ref_count = 1; _s; })); _stmp_.data; }); _tmp; });
}
});
({ if ((response.status_code != 200LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("Registry error for "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)name); _s.ref_count = 1; _s; })), rt_string_from(": HTTP ")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)response.status_code); _buf.ref_count = 1; _buf; })); _stmp_.data; }); _tmp; });
}
});
NyxVec versions = Resolver_parse_version_list(self, (&(response.body)));
rt_string_t best = ({ NyxResult _qres = Resolver_pick_best_version(self, (&(versions)), version_req); if (_qres.tag == RESULT_ERR) { return _qres; } rt_string_from((char*)_qres.data.ptr_val); });
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = best; _stmp_.data; }); _tmp; });
}

NyxVec Resolver_parse_version_list(Resolver* self, rt_string_t* body) {
NyxVec versions = rt_vec_new(sizeof(rt_string_t));
rt_string_t content = rt_string_trim(body);
({ if (rt_string_starts_with(content, rt_string_from("["))) {
rt_string_t inner = rt_string_slice(content, 1LL, (content.length - 1LL));
NyxVec parts = rt_string_split(inner, rt_string_from(","));
for (int64_t _part = 0; _part < parts.len; _part++) {
    int64_t part = *(int64_t*)rt_vec_get(parts, _part);
int64_t trimmed = rt_string_trim_matches(int64_t_trim(part), '\"');
({ if (!rt_string_eq(trimmed, rt_string_from(""))) {
rt_vec_push(&versions, ({ rt_string_t _tmp_7 = (({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)trimmed); _buf.ref_count = 1; _buf; })); &_tmp_7; }));
}
});
}
;
}
});
return versions;
}

NyxResult Resolver_pick_best_version(Resolver* self, NyxVec* versions, rt_string_t* version_req) {
({ if (((versions)->len == 0)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("No versions available for package"); _stmp_.data; }); _tmp; });
}
});
({ if ((rt_string_eq(version_req, rt_string_from("*")) || rt_string_eq(version_req, rt_string_from("latest")))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({ NyxOption _u = ({ void* _ptr = (versions); (_ptr && ((NyxVec*)_ptr)->len > 0) ? rt_option_some_int((int64_t)rt_vec_get((NyxVec*)_ptr, ((NyxVec*)_ptr)->len - 1)) : rt_option_none(); }); (void*)_u.data.int_val; })); _tmp; });
}
});
rt_string_t req = rt_string_trim(version_req);
({ if (rt_string_starts_with(req, rt_string_from(">="))) {
rt_string_t min = rt_string_trim(rt_string_slice(req, 2LL, req.length));
for (int64_t _v = 0; _v < int64_t_rev(rt_vec_iter(versions)).len; _v++) {
    int64_t v = *(int64_t*)rt_vec_get(int64_t_rev(rt_vec_iter(versions)), _v);
({ if (Resolver_version_gte(self, &(v), &(min))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(v); _tmp; });
}
});
}
;
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("No version satisfies >= "), min); _stmp_.data; }); _tmp; });
}
});
({ if (rt_string_starts_with(req, rt_string_from("^"))) {
rt_string_t caret = rt_string_trim(rt_string_slice(req, 1LL, req.length));
NyxVec caret_parts = rt_vec_collect(rt_string_split(caret, rt_string_from(".")));
({ if ((caret_parts.len >= 2LL)) {
void* major = (*(void**)rt_vec_get(caret_parts, 0LL));
void* minor = (*(void**)rt_vec_get(caret_parts, 1LL));
for (int64_t _v = 0; _v < int64_t_rev(rt_vec_iter(versions)).len; _v++) {
    int64_t v = *(int64_t*)rt_vec_get(int64_t_rev(rt_vec_iter(versions)), _v);
NyxVec v_parts = rt_vec_collect(int64_t_split(v, rt_string_from(".")));
({ if ((((v_parts.len >= 2LL) && ((*(void**)rt_vec_get(v_parts, 0LL)) == major)) && ((*(void**)rt_vec_get(v_parts, 1LL)) == minor))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(v); _tmp; });
}
});
}
;
}
});
}
});
({ if (rt_string_starts_with(req, rt_string_from("~"))) {
rt_string_t tilde = rt_string_trim(rt_string_slice(req, 1LL, req.length));
NyxVec tilde_parts = rt_vec_collect(rt_string_split(tilde, rt_string_from(".")));
({ if ((!((tilde_parts).len == 0))) {
void* major = (*(void**)rt_vec_get(tilde_parts, 0LL));
for (int64_t _v = 0; _v < int64_t_rev(rt_vec_iter(versions)).len; _v++) {
    int64_t v = *(int64_t*)rt_vec_get(int64_t_rev(rt_vec_iter(versions)), _v);
NyxVec v_parts = rt_vec_collect(int64_t_split(v, rt_string_from(".")));
({ if (((!((v_parts).len == 0)) && ((*(void**)rt_vec_get(v_parts, 0LL)) == major))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(v); _tmp; });
}
});
}
;
}
});
}
});
for (int64_t _v = 0; _v < int64_t_rev(rt_vec_iter(versions)).len; _v++) {
    int64_t v = *(int64_t*)rt_vec_get(int64_t_rev(rt_vec_iter(versions)), _v);
({ if (rt_string_eq(v, req)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(v); _tmp; });
}
});
}
;
({ if ((!((versions)->len == 0))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({ NyxOption _u = ({ void* _ptr = (versions); (_ptr && ((NyxVec*)_ptr)->len > 0) ? rt_option_some_int((int64_t)rt_vec_get((NyxVec*)_ptr, ((NyxVec*)_ptr)->len - 1)) : rt_option_none(); }); (void*)_u.data.int_val; })); _tmp; });
}
});
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("No matching version found"); _stmp_.data; }); _tmp; });
}

int Resolver_satisfies_version(Resolver* self, rt_string_t* actual, rt_string_t* requirement) {
({ if ((rt_string_eq(requirement, rt_string_from("*")) || rt_string_eq(requirement, rt_string_from("latest")))) {
return 1;
}
});
rt_string_t req = rt_string_trim(requirement);
({ if (rt_string_starts_with(req, rt_string_from(">="))) {
rt_string_t min = rt_string_trim(rt_string_slice(req, 2LL, req.length));
return Resolver_version_gte(self, actual, &(min));
}
});
({ if (rt_string_starts_with(req, rt_string_from("^"))) {
rt_string_t caret = rt_string_trim(rt_string_slice(req, 1LL, req.length));
NyxVec caret_parts = rt_vec_collect(rt_string_split(caret, rt_string_from(".")));
NyxVec actual_parts = rt_vec_collect(rt_string_split(actual, rt_string_from(".")));
({ if (((caret_parts.len >= 2LL) && (actual_parts.len >= 2LL))) {
return (((*(void**)rt_vec_get(caret_parts, 0LL)) == (*(void**)rt_vec_get(actual_parts, 0LL))) && ((*(void**)rt_vec_get(caret_parts, 1LL)) == (*(void**)rt_vec_get(actual_parts, 1LL))));
}
});
}
});
return rt_string_eq((*(actual)), req);
}

int Resolver_version_gte(Resolver* self, rt_string_t* a, rt_string_t* b) {
NyxVec pa = rt_vec_collect(rt_vec_map(rt_string_split(a, rt_string_from(".")), ({ int64_t __lambda_24(int64_t s) { return rt_result_unwrap_or(int64_t_parse(s), 0LL); } __lambda_24; })));
NyxVec pb = rt_vec_collect(rt_vec_map(rt_string_split(b, rt_string_from(".")), ({ int64_t __lambda_25(int64_t s) { return rt_result_unwrap_or(int64_t_parse(s), 0LL); } __lambda_25; })));
int64_t max_len = ({ int64_t __if_res_27; if ((pa.len > pb.len)) {
__if_res_27 = pa.len;
} else {
__if_res_27 = pb.len;
}
__if_res_27; });
for (int64_t i = 0LL; i < max_len; i++) {
int64_t va = ({ int64_t __if_res_28; if ((i < pa.len)) {
__if_res_28 = (*(int64_t*)rt_vec_get(pa, i));
} else {
__if_res_28 = 0LL;
}
__if_res_28; });
int64_t vb = ({ int64_t __if_res_29; if ((i < pb.len)) {
__if_res_29 = (*(int64_t*)rt_vec_get(pb, i));
} else {
__if_res_29 = 0LL;
}
__if_res_29; });
({ if ((va > vb)) {
return 1;
}
});
({ if ((va < vb)) {
return 0;
}
});
}
;
return 1;
}

