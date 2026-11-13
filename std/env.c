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

NyxVec args();
NyxOption get_var(rt_string_t);
void set_var(rt_string_t, rt_string_t);
rt_string_t current_dir();
NyxResult set_current_dir(rt_string_t);
// module env
int64_t* getenv(int64_t* p0); // extern
int64_t setenv(int64_t* p0, int64_t* p1, int64_t p2); // extern
int64_t nyx_get_argc(); // extern
int64_t* nyx_get_argv(int64_t p0); // extern
NyxVec args() {
int64_t count = nyx_get_argc();
NyxVec result = rt_vec_new(sizeof(rt_string_t));
for (int64_t i = 0LL; i < count; i++) {
int64_t* ptr = nyx_get_argv(i);
({ if ((ptr != 0LL)) {
rt_vec_push(&result, ({ rt_string_t _tmp_1 = int64_t_to_string(ptr); &_tmp_1; }));
}
});
}
;
return result;
}

NyxOption get_var(rt_string_t name) {
int64_t* ptr = getenv(((char*)(name.data)));
({ if ((ptr == 0LL)) {
return rt_option_none();
}
});
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = int64_t_to_string(ptr); _stmp_.data; }); _tmp; });
}

void set_var(rt_string_t name, rt_string_t value) {
setenv(((char*)(name.data)), ((char*)(value.data)), 1LL);
}

rt_string_t current_dir() {
int64_t buf[4096LL] = { 0LL };
extern int64_t* getcwd(int64_t*, int64_t);
int64_t* ptr = getcwd((&((*(int64_t*)rt_vec_get(buf, 0LL)))), 4096LL);
if ((ptr != 0LL)) {
return int64_t_to_string(ptr);
} else {
return rt_string_from(".");
}
}

NyxResult set_current_dir(rt_string_t path) {
extern int64_t chdir(int64_t*);
if ((chdir(((char*)(path.data))) == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(({  })); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_concat(rt_string_from("Cannot chdir to "), path); _stmp_.data; }); _tmp; });
}
}

