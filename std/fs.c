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
#include "runtime/rt_concurrency.h"
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
File* fopen(int64_t* p0, int64_t* p1); // extern
int64_t fclose(File* p0); // extern
int64_t fread(int64_t* p0, int64_t p1, int64_t p2, File* p3); // extern
int64_t fwrite(int64_t* p0, int64_t p1, int64_t p2, File* p3); // extern
int64_t fseek(File* p0, int64_t p1, int64_t p2); // extern
int64_t ftell(File* p0); // extern
int64_t access(int64_t* p0, int64_t p1); // extern
int64_t mkdir(int64_t* p0); // extern
int64_t rmdir(int64_t* p0); // extern
int64_t remove(int64_t* p0); // extern
int64_t rename(int64_t* p0, int64_t* p1); // extern
int64_t stat_size(int64_t* p0); // extern
Dir* opendir(int64_t* p0); // extern
DirEntry_C* readdir(Dir* p0); // extern
int64_t closedir(Dir* p0); // extern
int64_t* dirent_name(DirEntry_C* p0); // extern
int64_t dirent_is_dir(DirEntry_C* p0); // extern
int exists(rt_string_t path) {
return (access(((char*)(path.data)), 0LL) == 0LL);
}

int is_file(rt_string_t path) {
return (access(((char*)(path.data)), 0LL) == 0LL);
}

int is_dir(rt_string_t path) {
return (access(((char*)(path.data)), 0LL) == 0LL);
}

NyxResult stat(rt_string_t path) {
if ((access(((char*)(path.data)), 0LL) == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({  })); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Not found: "), path); _stmp_.data; }); _tmp; });
}
}

NyxResult file_size(rt_string_t path) {
int64_t size = stat_size(((char*)(path.data)));
if ((size >= 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(size); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot stat: "), path); _stmp_.data; }); _tmp; });
}
}

NyxResult read_to_string(rt_string_t path) {
void** f = fopen(((char*)(path.data)), rt_string_from("rb"));
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
int64_t buf[65536LL] = { 0LL };
rt_string_t result = rt_string_from("");
int64_t remaining = (int64_t)(size);
while ((remaining > 0LL)) {
int64_t chunk = ({ int64_t __if_res_3; if ((remaining > 65536LL)) {
__if_res_3 = 65536LL;
} else {
__if_res_3 = remaining;
}
__if_res_3; });
int64_t read = fread((&((*(int64_t*)rt_vec_get(buf, 0LL)))), 1LL, chunk, f);
({ if ((read == 0LL)) {
break;
}
});
result = rt_string_concat(result, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)int64_t_from_utf8(String, /* slice */ 0)); _s.ref_count = 1; _s; }));
remaining = (remaining - read);
}
;
fclose(f);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = result; _stmp_.data; }); _tmp; });
}

NyxResult read_bytes(rt_string_t path) {
void** f = fopen(((char*)(path.data)), rt_string_from("rb"));
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
int64_t buf[65536LL] = { 0LL };
int64_t remaining = (int64_t)(size);
while ((remaining > 0LL)) {
int64_t chunk = ({ int64_t __if_res_7; if ((remaining > 65536LL)) {
__if_res_7 = 65536LL;
} else {
__if_res_7 = remaining;
}
__if_res_7; });
int64_t read = fread((&((*(int64_t*)rt_vec_get(buf, 0LL)))), 1LL, chunk, f);
({ if ((read == 0LL)) {
break;
}
});
for (int64_t i = 0LL; i < read; i++) {
rt_vec_push(&data, ({ int64_t _tmp_8 = (*(int64_t*)rt_vec_get(buf, (int64_t)(i))); &_tmp_8; }));
}
;
remaining = (remaining - read);
}
;
fclose(f);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = data; &_stmp_; }); _tmp; });
}

NyxResult write_string(rt_string_t path, rt_string_t content) {
void** f = fopen(((char*)(path.data)), rt_string_from("wb"));
({ if ((f == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot write: "), path); _stmp_.data; }); _tmp; });
}
});
fwrite(((char*)(content.data)), 1LL, (int64_t)(content.length), f);
fclose(f);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({  })); _tmp; });
}

NyxResult write_bytes(rt_string_t path, NyxVec data) {
void** f = fopen(((char*)(path.data)), rt_string_from("wb"));
({ if ((f == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot write: "), path); _stmp_.data; }); _tmp; });
}
});
fwrite(data, 1LL, (int64_t)(data.len), f);
fclose(f);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({  })); _tmp; });
}

NyxResult append(rt_string_t path, rt_string_t content) {
void** f = fopen(((char*)(path.data)), rt_string_from("ab"));
({ if ((f == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot append: "), path); _stmp_.data; }); _tmp; });
}
});
fwrite(((char*)(content.data)), 1LL, (int64_t)(content.length), f);
fclose(f);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({  })); _tmp; });
}

NyxResult create_dir(rt_string_t path) {
if ((mkdir(((char*)(path.data))) == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({  })); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot create dir: "), path); _stmp_.data; }); _tmp; });
}
}

NyxResult create_dir_all(rt_string_t path) {
({ if (exists(({ rt_string_t _c = path; rt_string_retain(&_c); _c; }))) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({  })); _tmp; });
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
({ rt_string_t __if_res_15; if ((current.length > 0LL)) {
__if_res_15 = (current = rt_string_concat(rt_string_concat(current, rt_string_from("/")), ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)part); _s.ref_count = 1; _s; })));
} else {
__if_res_15 = (current = part);
}
__if_res_15; });
({ if ((!exists(({ rt_string_t _c = current; rt_string_retain(&_c); _c; })))) {
({ if ((mkdir(((char*)(current.data))) != 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot create dir: "), current); _stmp_.data; }); _tmp; });
}
});
}
});
}
;
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({  })); _tmp; });
}

NyxResult read_dir(rt_string_t path) {
void** dir = opendir(((char*)(path.data)));
({ if ((dir == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot open dir: "), path); _stmp_.data; }); _tmp; });
}
});
NyxVec entries = rt_vec_new(sizeof(int64_t));
while (1) {
void** entry = readdir(dir);
({ if ((entry == 0LL)) {
break;
}
});
int64_t* name_ptr = dirent_name(entry);
({ if ((name_ptr == 0LL)) {
break;
}
});
rt_string_t name = int64_t_to_string(name_ptr);
({ if ((rt_string_eq(name, rt_string_from(".")) || rt_string_eq(name, rt_string_from("..")))) {
continue;
}
});
int is_d = (dirent_is_dir(entry) != 0LL);
rt_vec_push(&entries, ({ DirEntry _tmp_21 = (DirEntry){.path = ({ rt_string_t __if_res_23; if (rt_string_ends_with(path, rt_string_from("/"))) {
__if_res_23 = rt_string_concat(({ rt_string_t _c = path; rt_string_retain(&_c); _c; }), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }));
} else {
__if_res_23 = rt_string_concat(rt_string_concat(({ rt_string_t _c = path; rt_string_retain(&_c); _c; }), rt_string_from("/")), ({ rt_string_t _c = name; rt_string_retain(&_c); _c; }));
}
__if_res_23; }), .is_dir = is_d}; &_tmp_21; }));
}
;
closedir(dir);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = entries; &_stmp_; }); _tmp; });
}

NyxResult read_dir_names(rt_string_t path) {
NyxVec entries = ({ NyxResult _qres = read_dir(path); if (_qres.tag == RESULT_ERR) { return _qres; } (NyxVec)_qres.data.int_val; });
NyxVec names = rt_vec_new(sizeof(int64_t));
for (int64_t _entry = 0; _entry < entries.len; _entry++) {
    int64_t entry = *(int64_t*)rt_vec_get(entries, _entry);
rt_vec_push(&names, ({ rt_string_t _tmp_23 = entry.path; &_tmp_23; }));
}
;
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ NyxVec _stmp_ = names; &_stmp_; }); _tmp; });
}

NyxResult remove_file(rt_string_t path) {
if ((remove(((char*)(path.data))) == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({  })); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot remove: "), path); _stmp_.data; }); _tmp; });
}
}

NyxResult remove_dir(rt_string_t path) {
if ((rmdir(((char*)(path.data))) == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({  })); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot remove dir: "), path); _stmp_.data; }); _tmp; });
}
}

NyxResult rename_file(rt_string_t old, rt_string_t new) {
if ((rename(((char*)(old.data)), ((char*)(new.data))) == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({  })); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("Cannot rename: "), old), rt_string_from(" -> ")), new); _stmp_.data; }); _tmp; });
}
}

rt_string_t basename(rt_string_t path) {
int64_t last = 0LL;
for (int64_t i = 0LL; i < path.length; i++) {
({ if (((path.data[i] == '/') || (path.data[i] == '\\'))) {
last = (i + 1LL);
}
});
}
;
return /* slice */ 0;
}

NyxResult copy(rt_string_t src, rt_string_t dst) {
NyxVec data = ({ NyxResult _qres = read_bytes(src); if (_qres.tag == RESULT_ERR) { return _qres; } (NyxVec)_qres.data.int_val; });
return write_bytes(dst, data);
}

NyxResult copy_dir(rt_string_t src, rt_string_t dst) {
({ NyxResult _qres = create_dir(({ rt_string_t _c = dst; rt_string_retain(&_c); _c; })); if (_qres.tag == RESULT_ERR) { return _qres; } (?)_qres.data.int_val; });
NyxVec entries = ({ NyxResult _qres = read_dir(src); if (_qres.tag == RESULT_ERR) { return _qres; } (NyxVec)_qres.data.int_val; });
for (int64_t _entry = 0; _entry < entries.len; _entry++) {
    int64_t entry = *(int64_t*)rt_vec_get(entries, _entry);
({ ? __if_res_26; if (entry.is_dir) {
__if_res_26 = ({ NyxResult _qres = copy_dir(({ rt_string_t _c = entry.path; rt_string_retain(&_c); _c; }), rt_string_concat(rt_string_concat(({ rt_string_t _c = dst; rt_string_retain(&_c); _c; }), rt_string_from("/")), basename(entry.path))); if (_qres.tag == RESULT_ERR) { return _qres; } (?)_qres.data.int_val; });
} else {
__if_res_26 = ({ NyxResult _qres = copy(entry.path, rt_string_concat(rt_string_concat(({ rt_string_t _c = dst; rt_string_retain(&_c); _c; }), rt_string_from("/")), basename(entry.path))); if (_qres.tag == RESULT_ERR) { return _qres; } (?)_qres.data.int_val; });
}
__if_res_26; });
}
;
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({  })); _tmp; });
}

rt_string_t temp_dir() {
return rt_string_from("/tmp");
}

rt_string_t home_dir() {
extern int64_t* getenv(int64_t*);
int64_t* ptr = getenv(((char*)(rt_string_from("HOME").data)));
if ((ptr != 0LL)) {
return int64_t_to_string(ptr);
} else {
return rt_string_from("/tmp");
}
}

