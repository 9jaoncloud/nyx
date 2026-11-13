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
    int64_t pid;
} Process;

NyxResult run(rt_string_t, NyxVec);
NyxResult run_sync(rt_string_t, NyxVec);
int64_t Process_wait(Process*);
NyxResult Process_kill(Process*);
int64_t Process_pid(Process*);
void exit(int64_t);
// module process
extern int system(const char*);
// extern nyx_process_spawn
// extern nyx_process_wait
// extern nyx_process_kill
// extern nyx_exit
NyxResult run(rt_string_t command, NyxVec args) {
Arena* __arena = arena_create();
rt_string_t full_cmd = ({ rt_string_t _c = command; rt_string_retain(&_c); _c; });
for (int64_t _arg = 0; _arg < args.len; _arg++) {
    rt_string_t arg = *(rt_string_t*)rt_vec_get(args, _arg);
full_cmd = rt_string_concat(rt_string_concat(full_cmd, rt_string_from(" ")), arg);
}
;
int64_t pid = nyx_process_spawn(((const uint8_t*)(full_cmd.data)));
arena_destroy(__arena);
if ((pid > 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.ptr_val = (void*)({ Process _stmp_ = (Process){.pid = pid}; &_stmp_; }); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Failed to spawn process"); _stmp_.data; }); _tmp; });
}
}

NyxResult run_sync(rt_string_t command, NyxVec args) {
Arena* __arena = arena_create();
rt_string_t full_cmd = ({ rt_string_t _c = command; rt_string_retain(&_c); _c; });
for (int64_t _arg = 0; _arg < args.len; _arg++) {
    rt_string_t arg = *(rt_string_t*)rt_vec_get(args, _arg);
full_cmd = rt_string_concat(rt_string_concat(full_cmd, rt_string_from(" ")), arg);
}
;
int64_t code = system(((const uint8_t*)(full_cmd.data)));
arena_destroy(__arena);
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(code); _tmp; });
}

int64_t Process_wait(Process* self) {
return nyx_process_wait((*self).pid);
}

NyxResult Process_kill(Process* self) {
if ((nyx_process_kill((*self).pid) == 0LL)) {
return ({ NyxResult _tmp; _tmp.tag = RESULT_OK; _tmp.data.int_val = (int64_t)(0LL); _tmp; });
} else {
return ({ NyxResult _tmp; _tmp.tag = RESULT_ERR; _tmp.data.ptr_val = (void*)({ rt_string_t _stmp_ = rt_string_from("Failed to kill process"); _stmp_.data; }); _tmp; });
}
}

int64_t Process_pid(Process* self) {
return (*self).pid;
}

void exit(int64_t code) {
Arena* __arena = arena_create();
nyx_exit(code);
arena_destroy(__arena);
}

