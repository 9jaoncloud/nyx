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
    rt_string_t base_url;
    rt_string_t cache_dir;
} RegistryClient;

RegistryClient new_client(rt_string_t);
NyxResult RegistryClient_fetch_package_info(RegistryClient*, rt_string_t);
NyxResult RegistryClient_download_tarball(RegistryClient*, rt_string_t, rt_string_t);
NyxResult RegistryClient_extract_package(RegistryClient*, rt_string_t, rt_string_t);
rt_string_t RegistryClient_compute_checksum(RegistryClient*, rt_string_t);
int RegistryClient_verify_checksum(RegistryClient*, rt_string_t, rt_string_t);

#line 1 "registry.nyx"
// module registry
// import std::io
// import std::fs
// import std::string
// import std::crypto
RegistryClient new_client(rt_string_t base_url) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (RegistryClient){.base_url = base_url, .cache_dir = rt_string_from(".nyx/cache")};
}

NyxResult RegistryClient_fetch_package_info(RegistryClient* self, rt_string_t* name) {
rt_string_t url = rt_string_concat(rt_string_concat((*self).base_url, rt_string_from("/api/packages/")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)name); _s.ref_count = 1; _s; }));
RegistryClient client = int64_t_new_client(http);
NyxResult response = ({
void* __match_result_0;
NyxResult __match_val_0 = rt_map_get(&(client), url);
if (__match_val_0.tag == RESULT_OK) {
    void* r = __match_val_0.data.ptr_val;
__match_result_0 = r;
}
 else if (__match_val_0.tag == RESULT_ERR) {
    rt_string_t e = rt_string_from((char*)__match_val_0.data.ptr_val);
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Network error: "), e); _stmp_.data; }); _tmp; });
}
__match_result_0;
});
({ if ((response.status_code == 404LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Package not found: "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)name); _s.ref_count = 1; _s; })); _stmp_.data; }); _tmp; });
}
});
({ if ((response.status_code != 200LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Registry returned HTTP "), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)response.status_code); _buf.ref_count = 1; _buf; })); _stmp_.data; }); _tmp; });
}
});
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(response.body); _tmp; });
}

NyxResult RegistryClient_download_tarball(RegistryClient* self, rt_string_t* name, rt_string_t* version) {
rt_string_t cache_key = rt_string_concat(rt_string_concat(rt_string_concat(({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)name); _s.ref_count = 1; _s; }), rt_string_from("-")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)version); _s.ref_count = 1; _s; })), rt_string_from(".tar.gz"));
rt_string_t cached_path = rt_string_concat(rt_string_concat((*self).cache_dir, rt_string_from("/")), cache_key);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = cached_path; _stmp_.data; }); _tmp; });
}

NyxResult RegistryClient_extract_package(RegistryClient* self, rt_string_t* tarball, rt_string_t* dest) {
rt_process_run(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("tar -xzf "), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)tarball); _s.ref_count = 1; _s; })), rt_string_from(" -C ")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)dest); _s.ref_count = 1; _s; })));
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
}

rt_string_t RegistryClient_compute_checksum(RegistryClient* self, rt_string_t* data) {
return rt_string_from("verified_hash");
}

int RegistryClient_verify_checksum(RegistryClient* self, rt_string_t* data, rt_string_t* expected) {
return 1;
}

