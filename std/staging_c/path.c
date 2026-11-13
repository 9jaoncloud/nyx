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

rt_string_t dirname(rt_string_t);
rt_string_t basename(rt_string_t);
rt_string_t ext(rt_string_t);
rt_string_t stem(rt_string_t);
rt_string_t join(rt_string_t, rt_string_t);
NyxOption strip_prefix(rt_string_t, rt_string_t);
NyxOption parent(rt_string_t);
rt_string_t normalize(rt_string_t);
rt_string_t absolute(rt_string_t);
rt_string_t with_ext(rt_string_t, rt_string_t);
// module path
rt_string_t dirname(rt_string_t path) {
Arena* __arena = arena_create();
int64_t last = 0LL;
for (int64_t i = 0LL; i < path.length; i++) {
({ if (((path.data[i] == '/') || (path.data[i] == '\\'))) {
last = i;
}
});
}
;
arena_destroy(__arena);
if ((last == 0LL)) {
return rt_string_from(".");
} else {
return rt_string_slice(path, 0LL, last);
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

rt_string_t ext(rt_string_t path) {
Arena* __arena = arena_create();
rt_string_t name = basename(path);
int64_t last_dot = name.length;
for (int64_t _i = 0; _i < int64_t_rev(/* range used outside for */).len; _i++) {
    int64_t i = *(int64_t*)rt_vec_get(int64_t_rev(/* range used outside for */), _i);
({ if ((name.data[i] == '.')) {
last_dot = i;
break;
}
});
}
;
arena_destroy(__arena);
if ((last_dot < name.length)) {
return rt_string_slice(name, (last_dot + 1LL), ((size_t)-1));
} else {
return rt_string_from("");
}
}

rt_string_t stem(rt_string_t path) {
Arena* __arena = arena_create();
rt_string_t name = basename(path);
int64_t last_dot = name.length;
for (int64_t _i = 0; _i < int64_t_rev(/* range used outside for */).len; _i++) {
    int64_t i = *(int64_t*)rt_vec_get(int64_t_rev(/* range used outside for */), _i);
({ if ((name.data[i] == '.')) {
last_dot = i;
break;
}
});
}
;
arena_destroy(__arena);
return rt_string_slice(name, 0LL, last_dot);
}

rt_string_t join(rt_string_t a, rt_string_t b) {
Arena* __arena = arena_create();
({ if ((a.length == 0LL)) {
return b;
}
});
char last_char = a.data[(a.length - 1LL)];
arena_destroy(__arena);
if (((last_char == '/') || (last_char == '\\'))) {
return rt_string_concat(a, b);
} else {
return rt_string_concat(rt_string_concat(a, rt_string_from("/")), b);
}
}

NyxOption strip_prefix(rt_string_t path, rt_string_t prefix) {
Arena* __arena = arena_create();
arena_destroy(__arena);
if (rt_string_starts_with(path, ({ rt_string_t _c = prefix; rt_string_retain(&_c); _c; }))) {
rt_string_t rest = rt_string_slice(path, prefix.length, ((size_t)-1));
return ({ NyxOption __if_res_6; if (((rest.length > 0LL) && (rest.data[0LL] == '/'))) {
__if_res_6 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_slice(rest, 1LL, ((size_t)-1)); _stmp_.data; }); _tmp; });
} else {
__if_res_6 = ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rest; _stmp_.data; }); _tmp; });
}
__if_res_6; });
} else {
return rt_option_none();
}
}

NyxOption parent(rt_string_t path) {
Arena* __arena = arena_create();
rt_string_t dir = dirname(path);
arena_destroy(__arena);
if (rt_string_eq(dir, rt_string_from("."))) {
return rt_option_none();
} else {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = dir; _stmp_.data; }); _tmp; });
}
}

rt_string_t normalize(rt_string_t path) {
Arena* __arena = arena_create();
int64_t parts = rt_string_split(path, rt_string_from("/"));
NyxVec result = rt_vec_new(sizeof(int64_t));
for (int64_t _part = 0; _part < parts.len; _part++) {
    int64_t part = *(int64_t*)rt_vec_get(parts, _part);
({ if (rt_string_eq(part, rt_string_from(".."))) {
({ if ((result.len > 0LL)) {
rt_vec_pop(&(result));
}
});
} else if ((!rt_string_eq(part, rt_string_from(".")) && (part.len > 0LL))) {
rt_vec_push(&result, ({ int64_t _tmp_8 = (int64_t)(part); &_tmp_8; }));
}
});
}
;
rt_string_t out = rt_string_from("");
for (int64_t i = 0LL; i < result.len; i++) {
({ if ((i > 0LL)) {
out = rt_string_concat(out, rt_string_from("/"));
}
});
out = rt_string_concat(out, ({ rt_string_t _s; _s.data = (char*)malloc(32); _s.length = snprintf(_s.data, 32, "%lld", (long long)(*(int64_t*)rt_vec_get(result, i))); _s.ref_count = 1; _s; }));
}
;
arena_destroy(__arena);
if ((out.length == 0LL)) {
return rt_string_from(".");
} else {
return out;
}
}

rt_string_t absolute(rt_string_t path) {
Arena* __arena = arena_create();
({ if ((rt_string_starts_with(path, rt_string_from("/")) || rt_string_starts_with(path, rt_string_from("\\")))) {
return normalize(path);
}
});
int64_t cwd = current_dir();
arena_destroy(__arena);
return normalize(join(cwd, path));
}

rt_string_t with_ext(rt_string_t path, rt_string_t new_ext) {
Arena* __arena = arena_create();
rt_string_t s = stem(path);
rt_string_t dir = dirname(path);
({ if (!rt_string_eq(dir, rt_string_from("."))) {
dir = rt_string_concat(dir, rt_string_from("/"));
}
});
arena_destroy(__arena);
if (((new_ext.length > 0LL) && (new_ext.data[0LL] == '.'))) {
return rt_string_concat(rt_string_concat(dir, s), new_ext);
} else if ((new_ext.length > 0LL)) {
return rt_string_concat(rt_string_concat(rt_string_concat(dir, s), rt_string_from(".")), new_ext);
} else {
return rt_string_concat(dir, s);
}
}

