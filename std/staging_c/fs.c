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

typedef struct {
    rt_string_t path;
    int is_dir;
} DirEntry;

int exists(rt_string_t);
int is_file(rt_string_t);
int is_dir(rt_string_t);
NyxResult stat(rt_string_t);
NyxResult file_size(rt_string_t);
NyxResult read_to_string(rt_string_t);
NyxResult read_bytes(rt_string_t);
NyxResult write_string(rt_string_t, rt_string_t);
NyxResult write_bytes(rt_string_t, NyxVec);
NyxResult append(rt_string_t, rt_string_t);
NyxResult create_dir(rt_string_t);
NyxResult create_dir_all(rt_string_t);
NyxResult read_dir(rt_string_t);
NyxResult read_dir_names(rt_string_t);
NyxResult remove_file(rt_string_t);
NyxResult remove_dir(rt_string_t);
NyxResult rename_file(rt_string_t, rt_string_t);
rt_string_t basename(rt_string_t);
NyxResult copy(rt_string_t, rt_string_t);
NyxResult copy_dir(rt_string_t, rt_string_t);
rt_string_t temp_dir();
rt_string_t home_dir();
// module fs
extern FILE* fopen(const char*, const char*);
extern int fclose(FILE*);
extern size_t fread(void*, size_t, size_t, FILE*);
extern size_t fwrite(const void*, size_t, size_t, FILE*);
extern int fseek(FILE*, long, int);
extern long ftell(FILE*);
extern int access(const char*, int);
extern int mkdir(const char*);
extern int rmdir(const char*);
extern int remove(const char*);
extern int rename(const char*, const char*);
extern int64_t stat_size(int64_t* p0);
extern Dir* opendir(int64_t* p0);
extern DirEntry_C* readdir(Dir* p0);
extern int64_t closedir(Dir* p0);
extern int64_t* dirent_name(DirEntry_C* p0);
extern int64_t dirent_is_dir(DirEntry_C* p0);
int exists(rt_string_t path) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (access(((const uint8_t*)(path.data)), 0LL) == 0LL);
}

int is_file(rt_string_t path) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (access(((const uint8_t*)(path.data)), 0LL) == 0LL);
}

int is_dir(rt_string_t path) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (access(((const uint8_t*)(path.data)), 0LL) == 0LL);
}

NyxResult stat(rt_string_t path) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((access(((const uint8_t*)(path.data)), 0LL) == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Not found: "), path); _stmp_.data; }); _tmp; });
}
}

NyxResult file_size(rt_string_t path) {
Arena* __arena = arena_create();
int64_t size = stat_size(((const uint8_t*)(path.data)));
arena_destroy(__arena);
if ((size >= 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(size); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot stat: "), path); _stmp_.data; }); _tmp; });
}
}

NyxResult read_to_string(rt_string_t path) {
Arena* __arena = arena_create();
uint8_t* f = fopen(((const uint8_t*)(path.data)), rt_string_from("rb"));
({ if ((f == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot open: "), path); _stmp_.data; }); _tmp; });
}
});
fseek(f, 0LL, 2LL);
int64_t size = ftell(f);
fseek(f, 0LL, 0LL);
({ if ((size <= 0LL)) {
fclose(f);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from(""); _stmp_.data; }); _tmp; });
}
});
uint8_t buf[65536LL] = { 0LL };
NyxVec data = rt_vec_new(sizeof(int64_t));
int64_t remaining = (int64_t)(size);
while ((remaining > 0LL)) {
int64_t chunk = ({ int64_t __if_res_3; if ((remaining > 65536LL)) {
__if_res_3 = 65536LL;
} else {
__if_res_3 = remaining;
}
__if_res_3; });
int64_t read = fread((&(buf[0LL])), 1LL, chunk, f);
({ if ((read == 0LL)) {
break;
}
});
for (int64_t i = 0LL; i < read; i++) {
rt_vec_push(&data, ({ int64_t _tmp_4 = (int64_t)(buf[(int64_t)(i)]); &_tmp_4; }));
}
;
remaining = (remaining - read);
}
;
fclose(f);
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = ({ NyxVec _v = data; rt_string_t _s; _s.data = (char*)malloc(_v.len + 1); if (_v.data && _v.len > 0) memcpy(_s.data, _v.data, _v.len); _s.data[_v.len] = 0; _s.length = _v.len; _s.ref_count = 1; _s; }); _stmp_.data; }); _tmp; });
}

NyxResult read_bytes(rt_string_t path) {
Arena* __arena = arena_create();
uint8_t* f = fopen(((const uint8_t*)(path.data)), rt_string_from("rb"));
({ if ((f == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot open: "), path); _stmp_.data; }); _tmp; });
}
});
fseek(f, 0LL, 2LL);
int64_t size = ftell(f);
fseek(f, 0LL, 0LL);
({ if ((size <= 0LL)) {
fclose(f);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = rt_vec_new(sizeof(int64_t)); &_stmp_; }); _tmp; });
}
});
NyxVec data = rt_vec_new(sizeof(int64_t));
uint8_t buf[65536LL] = { 0LL };
int64_t remaining = (int64_t)(size);
while ((remaining > 0LL)) {
int64_t chunk = ({ int64_t __if_res_8; if ((remaining > 65536LL)) {
__if_res_8 = 65536LL;
} else {
__if_res_8 = remaining;
}
__if_res_8; });
int64_t read = fread((&(buf[0LL])), 1LL, chunk, f);
({ if ((read == 0LL)) {
break;
}
});
for (int64_t i = 0LL; i < read; i++) {
rt_vec_push(&data, ({ int64_t _tmp_9 = (int64_t)(buf[(int64_t)(i)]); &_tmp_9; }));
}
;
remaining = (remaining - read);
}
;
fclose(f);
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = data; &_stmp_; }); _tmp; });
}

NyxResult write_string(rt_string_t path, rt_string_t content) {
Arena* __arena = arena_create();
uint8_t* f = fopen(((const uint8_t*)(path.data)), rt_string_from("wb"));
({ if ((f == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot write: "), path); _stmp_.data; }); _tmp; });
}
});
fwrite(((const uint8_t*)(content.data)), 1LL, (int64_t)(content.length), f);
fclose(f);
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
}

NyxResult write_bytes(rt_string_t path, NyxVec data) {
Arena* __arena = arena_create();
uint8_t* f = fopen(((const uint8_t*)(path.data)), rt_string_from("wb"));
({ if ((f == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot write: "), path); _stmp_.data; }); _tmp; });
}
});
fwrite(((const uint8_t*)(data.data)), 1LL, (int64_t)(data.len), f);
fclose(f);
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
}

NyxResult append(rt_string_t path, rt_string_t content) {
Arena* __arena = arena_create();
uint8_t* f = fopen(((const uint8_t*)(path.data)), rt_string_from("ab"));
({ if ((f == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot append: "), path); _stmp_.data; }); _tmp; });
}
});
fwrite(((const uint8_t*)(content.data)), 1LL, (int64_t)(content.length), f);
fclose(f);
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
}

NyxResult create_dir(rt_string_t path) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((mkdir(((const uint8_t*)(path.data))) == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot create dir: "), path); _stmp_.data; }); _tmp; });
}
}

NyxResult create_dir_all(rt_string_t path) {
Arena* __arena = arena_create();
({ if (exists(({ rt_string_t _c = path; rt_string_retain(&_c); _c; }))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
}
});
int64_t parts = rt_string_split(path, rt_string_from("/"));
rt_string_t current = rt_string_from("");
for (int64_t _part = 0; _part < parts.len; _part++) {
    int64_t part = *(int64_t*)rt_vec_get(parts, _part);
({ if ((part.len == 0LL)) {
continue;
}
});
({ rt_string_t __if_res_16; if ((current.length > 0LL)) {
__if_res_16 = (current = rt_string_concat(rt_string_concat(current, rt_string_from("/")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)part); _s.ref_count = 1; _s; })));
} else {
__if_res_16 = (current = part);
}
__if_res_16; });
({ if ((!exists(({ rt_string_t _c = current; rt_string_retain(&_c); _c; })))) {
({ if ((mkdir(((const uint8_t*)(current.data))) != 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot create dir: "), current); _stmp_.data; }); _tmp; });
}
});
}
});
}
;
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
}

NyxResult read_dir(rt_string_t path) {
Arena* __arena = arena_create();
uint8_t* dir = opendir(((const uint8_t*)(path.data)));
({ if ((dir == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot open dir: "), path); _stmp_.data; }); _tmp; });
}
});
NyxVec entries = rt_vec_new(sizeof(int64_t));
while (1) {
uint8_t* entry = readdir(dir);
({ if ((entry == 0LL)) {
break;
}
});
uint8_t* name_ptr = dirent_name(entry);
({ if ((name_ptr == 0LL)) {
break;
}
});
rt_string_t name = uint8_t_to_string(name_ptr);
({ if ((rt_string_eq(name, rt_string_from(".")) || rt_string_eq(name, rt_string_from("..")))) {
continue;
}
});
int is_d = (dirent_is_dir(entry) != 0LL);
rt_vec_push(&entries, ({ DirEntry _tmp_22 = (int64_t)((DirEntry){.path = ({ rt_string_t __if_res_24; if (rt_string_ends_with(path, rt_string_from("/"))) {
__if_res_24 = rt_string_concat(({ rt_string_t _c = path; rt_string_retain(&_c); _c; }), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }));
} else {
__if_res_24 = rt_string_concat(rt_string_concat(({ rt_string_t _c = path; rt_string_retain(&_c); _c; }), rt_string_from("/")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }));
}
__if_res_24; }), .is_dir = is_d}); &_tmp_22; }));
}
;
closedir(dir);
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = entries; &_stmp_; }); _tmp; });
}

NyxResult read_dir_names(rt_string_t path) {
Arena* __arena = arena_create();
NyxVec entries = ({ NyxResult _qres = read_dir(path); if (_qres.tag == RESULT_ERR) { return _qres; } *((NyxVec*)_qres.data.ptr_val); });
NyxVec names = rt_vec_new(sizeof(int64_t));
for (int64_t _entry = 0; _entry < entries.len; _entry++) {
    int64_t entry = *(int64_t*)rt_vec_get(entries, _entry);
rt_vec_push(&names, ({ rt_string_t _tmp_24 = (int64_t)(entry.path); &_tmp_24; }));
}
;
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = names; &_stmp_; }); _tmp; });
}

NyxResult remove_file(rt_string_t path) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((nyx_remove(((const uint8_t*)(path.data))) == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot remove: "), path); _stmp_.data; }); _tmp; });
}
}

NyxResult remove_dir(rt_string_t path) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((rmdir(((const uint8_t*)(path.data))) == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot remove dir: "), path); _stmp_.data; }); _tmp; });
}
}

NyxResult rename_file(rt_string_t old, rt_string_t new) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if ((rename(((const uint8_t*)(old.data)), ((const uint8_t*)(new.data))) == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("Cannot rename: "), old), rt_string_from(" -> ")), new); _stmp_.data; }); _tmp; });
}
}

rt_string_t basename(rt_string_t path) {
Arena* __arena = arena_create();
int64_t last = 0LL;
for (int64_t i = 0LL; i < path.length; i++) {
({ if (((path.data[i] == '/') || (path.data[i] == '\\'))) {
last = (i + 1LL);
}
});
}
;
arena_destroy(__arena);
return rt_string_slice(path, last, ((size_t)-1));
}

NyxResult copy(rt_string_t src, rt_string_t dst) {
Arena* __arena = arena_create();
NyxVec data = ({ NyxResult _qres = read_bytes(src); if (_qres.tag == RESULT_ERR) { return _qres; } *((NyxVec*)_qres.data.ptr_val); });
arena_destroy(__arena);
return write_bytes(dst, data);
}

NyxResult copy_dir(rt_string_t src, rt_string_t dst) {
Arena* __arena = arena_create();
({ NyxResult _qres = create_dir(({ rt_string_t _c = dst; rt_string_retain(&_c); _c; })); if (_qres.tag == RESULT_ERR) { return _qres; } (int64_t)_qres.data.int_val; });
NyxVec entries = ({ NyxResult _qres = read_dir(src); if (_qres.tag == RESULT_ERR) { return _qres; } *((NyxVec*)_qres.data.ptr_val); });
for (int64_t _entry = 0; _entry < entries.len; _entry++) {
    int64_t entry = *(int64_t*)rt_vec_get(entries, _entry);
({ ? __if_res_27; if (entry.is_dir) {
__if_res_27 = ({ NyxResult _qres = copy_dir(({ rt_string_t _c = entry.path; rt_string_retain(&_c); _c; }), rt_string_concat(rt_string_concat(({ rt_string_t _c = dst; rt_string_retain(&_c); _c; }), rt_string_from("/")), basename(entry.path))); if (_qres.tag == RESULT_ERR) { return _qres; } (int64_t)_qres.data.int_val; });
} else {
__if_res_27 = ({ NyxResult _qres = copy(entry.path, rt_string_concat(rt_string_concat(({ rt_string_t _c = dst; rt_string_retain(&_c); _c; }), rt_string_from("/")), basename(entry.path))); if (_qres.tag == RESULT_ERR) { return _qres; } (int64_t)_qres.data.int_val; });
}
__if_res_27; });
}
;
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
}

rt_string_t temp_dir() {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_from("/tmp");
}

rt_string_t home_dir() {
Arena* __arena = arena_create();
// extern getenv
uint8_t* ptr = getenv(((const uint8_t*)(rt_string_from("HOME").data)));
arena_destroy(__arena);
if ((ptr != 0LL)) {
return uint8_t_to_string(ptr);
} else {
return rt_string_from("/tmp");
}
}

