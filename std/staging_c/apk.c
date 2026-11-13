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

#define TAG_CompatTier_ContainerOnly 0
#define TAG_CompatTier_NativeOnly 1
#define TAG_CompatTier_RequiresFullAndroid 2
typedef struct {
    int tag;
    union {
    } data;
} CompatTier;

#define TAG_SigningScheme_V1 0
#define TAG_SigningScheme_V2 1
#define TAG_SigningScheme_V3 2
#define TAG_SigningScheme_V4 3
#define TAG_SigningScheme_Unknown 4
typedef struct {
    int tag;
    union {
    } data;
} SigningScheme;

typedef struct {
    rt_string_t package_name;
    rt_string_t version_name;
    int64_t version_code;
    int64_t min_sdk_version;
    int64_t target_sdk_version;
    rt_string_t app_label;
    NyxVec permissions;
    NyxVec activities;
    NyxVec services;
    NyxVec receivers;
    NyxVec providers;
} ApkManifest;

typedef struct {
    rt_string_t abi;
    rt_string_t name;
    int64_t size_bytes;
} NativeLib;

typedef struct {
    rt_string_t path;
    int64_t file_size_bytes;
    ApkManifest manifest;
    NyxVec dex_files;
    NyxVec native_libs;
    int has_resources;
    int has_assets;
    SigningScheme signing_scheme;
    CompatTier compatibility_tier;
} ApkInfo;

typedef struct {
    rt_string_t name;
    int64_t compressed_size;
    int64_t uncompressed_size;
    int64_t offset;
    int64_t method;
} ZipEntry;

typedef struct {
    NyxVec strings;
} StringPool;

int64_t read_u16_le(NyxVec*, int64_t);
int64_t read_u32_le(NyxVec*, int64_t);
NyxResult find_eocd(NyxVec*);
NyxResult parse_central_directory(NyxVec*);
NyxResult extract_entry_bytes(NyxVec*, ZipEntry*);
NyxResult parse_string_pool(NyxVec*, int64_t);
NyxResult parse_axml_manifest(NyxVec*);
CompatTier assess_compatibility(ApkManifest*, NyxVec*, int64_t);
NyxResult inspect_apk(rt_string_t);
// module apk
// import prelude::*
int64_t read_u16_le(NyxVec* data, int64_t offset) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ((int64_t)((*(int64_t*)rt_vec_get(*data, offset))) | ((int64_t)((*(int64_t*)rt_vec_get(*data, (offset + 1LL)))) << 8LL));
}

int64_t read_u32_le(NyxVec* data, int64_t offset) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return ((((int64_t)((*(int64_t*)rt_vec_get(*data, offset))) | ((int64_t)((*(int64_t*)rt_vec_get(*data, (offset + 1LL)))) << 8LL)) | ((int64_t)((*(int64_t*)rt_vec_get(*data, (offset + 2LL)))) << 16LL)) | ((int64_t)((*(int64_t*)rt_vec_get(*data, (offset + 3LL)))) << 24LL));
}

NyxResult find_eocd(NyxVec* data) {
Arena* __arena = arena_create();
int64_t eocd_sig = 101010256LL;
int64_t min_eocd_size = 22LL;
({ if ((data->len < min_eocd_size)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("File too small to be a valid APK"); _stmp_.data; }); _tmp; });
}
});
int64_t max_comment_len = 65535LL;
int64_t search_start = ({ int64_t __if_res_2; if ((data->len > (max_comment_len + min_eocd_size))) {
__if_res_2 = ((data->len - max_comment_len) - min_eocd_size);
} else {
__if_res_2 = (int64_t)(0LL);
}
__if_res_2; });
int64_t i = (data->len - min_eocd_size);
while ((i >= search_start)) {
({ if ((read_u32_le(data, i) == eocd_sig)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(i); _tmp; });
}
});
({ if ((i == 0LL)) {
break;
}
});
i = (i - 1LL);
}
;
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("No EOCD record found — not a valid ZIP/APK file"); _stmp_.data; }); _tmp; });
}

NyxResult parse_central_directory(NyxVec* data) {
Arena* __arena = arena_create();
int64_t eocd_offset = ({ NyxResult _qres = find_eocd(data); if (_qres.tag == RESULT_ERR) { return _qres; } (int64_t)_qres.data.int_val; });
int64_t cd_count = (int64_t)(read_u16_le(data, (eocd_offset + 8LL)));
int64_t cd_offset = (int64_t)(read_u32_le(data, (eocd_offset + 16LL)));
NyxVec entries = rt_vec_new(sizeof(int64_t));
int64_t pos = cd_offset;
int64_t cd_sig = 33639248LL;
for (int64_t _ = 0LL; _ < cd_count; _++) {
({ if (((pos + 46LL) > data->len)) {
break;
}
});
({ if ((read_u32_le(data, pos) != cd_sig)) {
break;
}
});
int64_t method = read_u16_le(data, (pos + 10LL));
int64_t compressed = (int64_t)(read_u32_le(data, (pos + 20LL)));
int64_t uncompressed = (int64_t)(read_u32_le(data, (pos + 24LL)));
int64_t name_len = (int64_t)(read_u16_le(data, (pos + 28LL)));
int64_t extra_len = (int64_t)(read_u16_le(data, (pos + 30LL)));
int64_t comment_len = (int64_t)(read_u16_le(data, (pos + 32LL)));
int64_t local_offset = (int64_t)(read_u32_le(data, (pos + 42LL)));
int64_t name_end = ((pos + 46LL) + name_len);
({ if ((name_end > data->len)) {
break;
}
});
rt_string_t name = rt_string_from((const char*)(({ int64_t _utmp_ = /* slice */ 0; &_utmp_; })));
rt_vec_push(&entries, ({ ZipEntry _tmp_7 = (int64_t)((ZipEntry){.name = name, .compressed_size = compressed, .uncompressed_size = uncompressed, .offset = local_offset, .method = method}); &_tmp_7; }));
pos = ((((pos + 46LL) + name_len) + extra_len) + comment_len);
}
;
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = entries; &_stmp_; }); _tmp; });
}

NyxResult extract_entry_bytes(NyxVec* data, ZipEntry* entry) {
Arena* __arena = arena_create();
int64_t lh_sig = 67324752LL;
int64_t offset = (int64_t)((*entry).offset);
({ if (((offset + 30LL) > data->len)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Entry offset out of bounds"); _stmp_.data; }); _tmp; });
}
});
({ if ((read_u32_le(data, offset) != lh_sig)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Invalid local file header"); _stmp_.data; }); _tmp; });
}
});
int64_t name_len = (int64_t)(read_u16_le(data, (offset + 26LL)));
int64_t extra_len = (int64_t)(read_u16_le(data, (offset + 28LL)));
int64_t data_start = (((offset + 30LL) + name_len) + extra_len);
int64_t data_end = (data_start + (int64_t)((*entry).compressed_size));
({ if ((data_end > data->len)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Entry data out of bounds"); _stmp_.data; }); _tmp; });
}
});
arena_destroy(__arena);
if (((*entry).method == (int64_t)(0LL))) {
NyxVec extracted = rt_vec_new(sizeof(int64_t));
for (int64_t i = data_start; i < data_end; i++) {
rt_vec_push(&extracted, ({ int64_t _tmp_11 = (int64_t)((*(int64_t*)rt_vec_get(*data, i))); &_tmp_11; }));
}
;
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = extracted; &_stmp_; }); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("DEFLATE entry requires zlib decompression"); _stmp_.data; }); _tmp; });
}
}

NyxResult parse_string_pool(NyxVec* data, int64_t offset) {
Arena* __arena = arena_create();
({ if (((offset + 8LL) > data->len)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("String pool too short"); _stmp_.data; }); _tmp; });
}
});
int64_t string_count = (int64_t)(read_u32_le(data, (offset + 8LL)));
int64_t strings_start = (int64_t)(read_u32_le(data, (offset + 20LL)));
int64_t flags = read_u32_le(data, (offset + 16LL));
int is_utf8 = ((flags & (1LL << 8LL)) != 0LL);
NyxVec strings = rt_vec_new(sizeof(int64_t));
int64_t offsets_start = (offset + 28LL);
for (int64_t i = 0LL; i < string_count; i++) {
int64_t str_offset_pos = (offsets_start + (i * 4LL));
({ if (((str_offset_pos + 4LL) > data->len)) {
break;
}
});
int64_t str_offset = (int64_t)(read_u32_le(data, str_offset_pos));
int64_t abs_offset = ((offset + strings_start) + str_offset);
({ if (((abs_offset + 2LL) > data->len)) {
rt_vec_push(&strings, ({ rt_string_t _tmp_15 = (int64_t)(rt_string_from("")); &_tmp_15; }));
continue;
}
});
rt_string_t s = ({ rt_string_t __if_res_17; if (is_utf8) {
int64_t byte_len = (int64_t)((*(int64_t*)rt_vec_get(*data, (abs_offset + 1LL))));
if ((((abs_offset + 2LL) + byte_len) > data->len)) {
__if_res_17 = rt_string_from("");
} else {
__if_res_17 = rt_string_from((const char*)(({ int64_t _utmp_ = /* slice */ 0; &_utmp_; })));
}
} else {
int64_t char_len = (int64_t)(read_u16_le(data, abs_offset));
rt_string_t result = rt_string_from("");
for (int64_t c = 0LL; c < char_len; c++) {
int64_t char_pos = ((abs_offset + 2LL) + (c * 2LL));
({ if (((char_pos + 2LL) > data->len)) {
break;
}
});
int64_t code = read_u16_le(data, char_pos);
({ if ((code > (int64_t)(0LL))) {
rt_string_push_char(result, (char)((int64_t)(code)));
}
});
}
;
__if_res_17 = result;
}
__if_res_17; });
rt_vec_push(&strings, ({ rt_string_t _tmp_19 = (int64_t)(s); &_tmp_19; }));
}
;
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ StringPool _stmp_ = (StringPool){.strings = strings}; &_stmp_; }); _tmp; });
}

NyxResult parse_axml_manifest(NyxVec* data) {
Arena* __arena = arena_create();
({ if ((data->len < 8LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("AXML data too short"); _stmp_.data; }); _tmp; });
}
});
int64_t magic = read_u32_le(data, 0LL);
({ if ((magic != AXML_MAGIC)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Invalid AXML magic (expected 0x00080003)"); _stmp_.data; }); _tmp; });
}
});
ApkManifest manifest = (ApkManifest){.package_name = rt_string_from(""), .version_name = rt_string_from(""), .version_code = 0LL, .min_sdk_version = 0LL, .target_sdk_version = 0LL, .app_label = rt_string_from(""), .permissions = rt_vec_new(sizeof(int64_t)), .activities = rt_vec_new(sizeof(int64_t)), .services = rt_vec_new(sizeof(int64_t)), .receivers = rt_vec_new(sizeof(int64_t)), .providers = rt_vec_new(sizeof(int64_t))};
NyxOption string_pool = rt_option_none();
int64_t pos = 8LL;
while (((pos + 8LL) <= data->len)) {
int64_t chunk_type = read_u16_le(data, pos);
int64_t header_size = (int64_t)(read_u16_le(data, (pos + 2LL)));
int64_t chunk_size = (int64_t)(read_u32_le(data, (pos + 4LL)));
({ if (((chunk_size == 0LL) || ((pos + chunk_size) > data->len))) {
break;
}
});
({
NyxResult __match_result_23;
int64_t __match_val_23 = chunk_type;
if (__match_val_23 == 1LL) {
__match_result_23 = ({
NyxOption __match_result_24;
NyxResult __match_val_24 = parse_string_pool(data, pos);
if (__match_val_24.tag == RESULT_OK) {
    void* pool = __match_val_24.data.ptr_val;
__match_result_24 = (string_pool = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)(pool); _tmp; }));
}
 else if (__match_val_24.tag == RESULT_ERR) {
}
__match_result_24;
});
}
 else if (__match_val_23 == 258LL) {
__match_result_23 = ({
int64_t __match_result_25;
NyxOption __match_val_25 = string_pool;
if (__match_val_25.tag == OPTION_SOME) {
    void* pool = __match_val_25.data.ptr_val;
({ if ((((pos + header_size) + 8LL) <= data->len)) {
int64_t name_idx = (int64_t)(read_u32_le(data, (pos + 12LL)));
int64_t attr_count = (int64_t)(read_u16_le(data, (pos + 20LL)));
int64_t attrs_start = (pos + header_size);
int64_t element_name = ({ int64_t __if_res_28; if ((name_idx < (*pool).strings.len)) {
__if_res_28 = (*(int64_t*)rt_vec_get((*pool).strings, name_idx));
} else {
__if_res_28 = rt_string_from("");
}
__if_res_28; });
for (int64_t a = 0LL; a < attr_count; a++) {
int64_t attr_pos = (attrs_start + (a * 20LL));
({ if (((attr_pos + 20LL) > data->len)) {
break;
}
});
int64_t attr_name_idx = (int64_t)(read_u32_le(data, (attr_pos + 4LL)));
int64_t attr_raw_val = (int64_t)(read_u32_le(data, (attr_pos + 8LL)));
int64_t attr_data = read_u32_le(data, (attr_pos + 16LL));
int64_t attr_name = ({ int64_t __if_res_30; if ((attr_name_idx < (*pool).strings.len)) {
__if_res_30 = (*(int64_t*)rt_vec_get((*pool).strings, attr_name_idx));
} else {
__if_res_30 = rt_string_from("");
}
__if_res_30; });
int64_t attr_str_val = ({ int64_t __if_res_31; if ((attr_raw_val < (*pool).strings.len)) {
__if_res_31 = (*(int64_t*)rt_vec_get((*pool).strings, attr_raw_val));
} else {
__if_res_31 = rt_string_from("");
}
__if_res_31; });
({ if (rt_string_eq(element_name, rt_string_from("manifest"))) {
({ if (rt_string_eq(attr_name, rt_string_from("package"))) {
(manifest.package_name = attr_str_val);
} else if (rt_string_eq(attr_name, rt_string_from("versionName"))) {
(manifest.version_name = attr_str_val);
} else if (rt_string_eq(attr_name, rt_string_from("versionCode"))) {
(manifest.version_code = (int64_t)(attr_data));
}
});
} else if (rt_string_eq(element_name, rt_string_from("uses-sdk"))) {
({ if (rt_string_eq(attr_name, rt_string_from("minSdkVersion"))) {
(manifest.min_sdk_version = (int64_t)(attr_data));
} else if (rt_string_eq(attr_name, rt_string_from("targetSdkVersion"))) {
(manifest.target_sdk_version = (int64_t)(attr_data));
}
});
} else if (rt_string_eq(element_name, rt_string_from("uses-permission"))) {
({ if ((rt_string_eq(attr_name, rt_string_from("name")) && (attr_str_val.len > 0LL))) {
rt_vec_push(&manifest.permissions, ({ int64_t _tmp_35 = (int64_t)(attr_str_val); &_tmp_35; }));
}
});
} else if (rt_string_eq(element_name, rt_string_from("application"))) {
({ if ((rt_string_eq(attr_name, rt_string_from("label")) && (attr_str_val.len > 0LL))) {
(manifest.app_label = attr_str_val);
}
});
} else if (rt_string_eq(element_name, rt_string_from("activity"))) {
({ if ((rt_string_eq(attr_name, rt_string_from("name")) && (attr_str_val.len > 0LL))) {
rt_vec_push(&manifest.activities, ({ int64_t _tmp_38 = (int64_t)(attr_str_val); &_tmp_38; }));
}
});
} else if (rt_string_eq(element_name, rt_string_from("service"))) {
({ if ((rt_string_eq(attr_name, rt_string_from("name")) && (attr_str_val.len > 0LL))) {
rt_vec_push(&manifest.services, ({ int64_t _tmp_40 = (int64_t)(attr_str_val); &_tmp_40; }));
}
});
} else if (rt_string_eq(element_name, rt_string_from("receiver"))) {
({ if ((rt_string_eq(attr_name, rt_string_from("name")) && (attr_str_val.len > 0LL))) {
rt_vec_push(&manifest.receivers, ({ int64_t _tmp_42 = (int64_t)(attr_str_val); &_tmp_42; }));
}
});
} else if (rt_string_eq(element_name, rt_string_from("provider"))) {
({ if ((rt_string_eq(attr_name, rt_string_from("name")) && (attr_str_val.len > 0LL))) {
rt_vec_push(&manifest.providers, ({ int64_t _tmp_44 = (int64_t)(attr_str_val); &_tmp_44; }));
}
});
}
});
}
;
}
});
}
 else if (__match_val_25.tag == OPTION_NONE) {
}
__match_result_25;
});
}
 else {
}
__match_result_23;
});
pos = (pos + chunk_size);
}
;
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ ApkManifest _stmp_ = manifest; &_stmp_; }); _tmp; });
}

CompatTier assess_compatibility(ApkManifest* manifest, NyxVec* native_libs, int64_t dex_count) {
Arena* __arena = arena_create();
int has_arm64_native = 0;
for (int64_t i = 0LL; i < native_libs->len; i++) {
({ if (rt_string_eq(((*(NativeLib*)rt_vec_get(*native_libs, i))).abi, rt_string_from("arm64-v8a"))) {
has_arm64_native = 1;
break;
}
});
}
;
({ if (((dex_count == 0LL) && has_arm64_native)) {
return (CompatTier){ .tag = TAG_CompatTier_NativeOnly };
}
});
int uses_framework_perms = 0;
for (int64_t i = 0LL; i < (*manifest).permissions.len; i++) {
int64_t p = (&((*(int64_t*)rt_vec_get((*manifest).permissions, i))));
({ if ((rt_string_starts_with(p, rt_string_from("android.permission.")) && (((((rt_string_contains(p, rt_string_from("CAMERA")) || rt_string_contains(p, rt_string_from("PHONE"))) || rt_string_contains(p, rt_string_from("CONTACTS"))) || rt_string_contains(p, rt_string_from("LOCATION"))) || rt_string_contains(p, rt_string_from("READ_CALL_LOG"))) || rt_string_contains(p, rt_string_from("NFC"))))) {
uses_framework_perms = 1;
break;
}
});
}
;
({ if (((uses_framework_perms || ((*manifest).services.len > 2LL)) || ((*manifest).receivers.len > 2LL))) {
return (CompatTier){ .tag = TAG_CompatTier_RequiresFullAndroid };
}
});
arena_destroy(__arena);
return (CompatTier){ .tag = TAG_CompatTier_ContainerOnly };
}

NyxResult inspect_apk(rt_string_t path) {
Arena* __arena = arena_create();
NyxResult data = ({
void* __match_result_49;
NyxResult __match_val_49 = read_bytes(({ rt_string_t _c = path; rt_string_retain(&_c); _c; }));
if (__match_val_49.tag == RESULT_OK) {
    void* d = __match_val_49.data.ptr_val;
__match_result_49 = d;
}
 else if (__match_val_49.tag == RESULT_ERR) {
    rt_string_t e = rt_string_from((char*)__match_val_49.data.ptr_val);
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot read APK: "), e); _stmp_.data; }); _tmp; });
}
__match_result_49;
});
int64_t file_size = (int64_t)(data.len);
NyxVec entries = ({ NyxResult _qres = parse_central_directory(({ NyxResult _atmp_0 = (&(data)); &_atmp_0; })); if (_qres.tag == RESULT_ERR) { return _qres; } *((NyxVec*)_qres.data.ptr_val); });
int64_t manifest_idx = 0LL;
int found = 0;
for (int64_t i = 0LL; i < entries.len; i++) {
({ if (rt_string_eq(((*(int64_t*)rt_vec_get(entries, i))).name, rt_string_from("AndroidManifest.xml"))) {
manifest_idx = i;
found = 1;
break;
}
});
}
;
({ if ((!found)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("AndroidManifest.xml not found in APK"); _stmp_.data; }); _tmp; });
}
});
NyxVec manifest_bytes = ({ NyxResult _qres = extract_entry_bytes(({ NyxResult _atmp_0 = (&(data)); &_atmp_0; }), ({ int64_t _atmp_1 = (&((*(int64_t*)rt_vec_get(entries, manifest_idx)))); &_atmp_1; })); if (_qres.tag == RESULT_ERR) { return _qres; } *((NyxVec*)_qres.data.ptr_val); });
ApkManifest manifest = ({ NyxResult _qres = parse_axml_manifest(({ NyxVec _atmp_0 = (&(manifest_bytes)); &_atmp_0; })); if (_qres.tag == RESULT_ERR) { return _qres; } *(ApkManifest*)_qres.data.ptr_val; });
NyxVec dex_files = rt_vec_new(sizeof(int64_t));
for (int64_t i = 0LL; i < entries.len; i++) {
({ if (rt_string_ends_with(((*(int64_t*)rt_vec_get(entries, i))).name, rt_string_from(".dex"))) {
rt_vec_push(&dex_files, ({ rt_string_t _tmp_53 = (int64_t)(({ rt_string_t _c = ((*(int64_t*)rt_vec_get(entries, i))).name; rt_string_retain(&_c); _c; })); &_tmp_53; }));
}
});
}
;
NyxVec native_libs = rt_vec_new(sizeof(int64_t));
for (int64_t i = 0LL; i < entries.len; i++) {
rt_string_t ename = (&(((*(int64_t*)rt_vec_get(entries, i))).name));
({ if ((rt_string_starts_with(ename, rt_string_from("lib/")) && rt_string_ends_with(ename, rt_string_from(".so")))) {
int64_t parts = rt_string_split(ename, rt_string_from("/"));
({ if ((parts.len == 3LL)) {
rt_vec_push(&native_libs, ({ NativeLib _tmp_56 = (int64_t)((NativeLib){.abi = parts[1LL], .name = parts[2LL], .size_bytes = ((*(int64_t*)rt_vec_get(entries, i))).uncompressed_size}); &_tmp_56; }));
}
});
}
});
}
;
int has_resources = 0;
int has_assets = 0;
for (int64_t i = 0LL; i < entries.len; i++) {
({ if (rt_string_eq(((*(int64_t*)rt_vec_get(entries, i))).name, rt_string_from("resources.arsc"))) {
has_resources = 1;
}
});
({ if (rt_string_starts_with(((*(int64_t*)rt_vec_get(entries, i))).name, rt_string_from("assets/"))) {
has_assets = 1;
}
});
}
;
CompatTier compat = assess_compatibility(({ ApkManifest _atmp_0 = (&(manifest)); &_atmp_0; }), ({ NyxVec _atmp_1 = (&(native_libs)); &_atmp_1; }), dex_files.len);
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ ApkInfo _stmp_ = (ApkInfo){.path = path, .file_size_bytes = file_size, .manifest = manifest, .dex_files = dex_files, .native_libs = native_libs, .has_resources = has_resources, .has_assets = has_assets, .signing_scheme = (SigningScheme){ .tag = TAG_SigningScheme_V1 }, .compatibility_tier = compat}; &_stmp_; }); _tmp; });
}

