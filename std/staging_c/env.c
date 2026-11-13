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

NyxVec args();
NyxOption get_var(rt_string_t);
void set_var(rt_string_t, rt_string_t);
rt_string_t current_dir();
NyxResult set_current_dir(rt_string_t);
// module env
extern char* getenv(const char*);
extern int setenv(const char*, const char*, int);
// extern nyx_get_argc
// extern nyx_get_argv
NyxVec args() {
Arena* __arena = arena_create();
int64_t count = nyx_get_argc();
NyxVec result = rt_vec_new(sizeof(rt_string_t));
for (int64_t i = 0LL; i < count; i++) {
uint8_t* ptr = nyx_get_argv(i);
({ if ((ptr != 0LL)) {
rt_vec_push(&result, ({ rt_string_t _tmp_1 = (int64_t)(uint8_t_to_string(ptr)); &_tmp_1; }));
}
});
}
;
arena_destroy(__arena);
return result;
}

NyxOption get_var(rt_string_t name) {
Arena* __arena = arena_create();
uint8_t* ptr = getenv(((const uint8_t*)(name.data)));
({ if ((ptr == 0LL)) {
return rt_option_none();
}
});
arena_destroy(__arena);
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = uint8_t_to_string(ptr); _stmp_.data; }); _tmp; });
}

void set_var(rt_string_t name, rt_string_t value) {
Arena* __arena = arena_create();
setenv(((const uint8_t*)(name.data)), ((const uint8_t*)(value.data)), 1LL);
arena_destroy(__arena);
}

rt_string_t current_dir() {
Arena* __arena = arena_create();
uint8_t buf[4096LL] = { 0LL };
extern uint8_t* getcwd(uint8_t*, int64_t);
uint8_t* ptr = getcwd((&(buf[0LL])), 4096LL);
arena_destroy(__arena);
if ((ptr != 0LL)) {
return uint8_t_to_string(ptr);
} else {
return rt_string_from(".");
}
}

NyxResult set_current_dir(rt_string_t path) {
Arena* __arena = arena_create();
extern int64_t chdir(uint8_t*);
arena_destroy(__arena);
if ((chdir(((const uint8_t*)(path.data))) == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot chdir to "), path); _stmp_.data; }); _tmp; });
}
}

