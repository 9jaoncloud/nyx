#include "runtime/rt_string.h"
#include <stdint.h>
#include <math.h>
#include "runtime/rt_stdlib.h"
#include "runtime/rt_async.h"
#include <stdio.h>
#include "runtime/rt_option.h"
#include "runtime/rt_json.h"
#include <stdlib.h>
#include <string.h>
#include "runtime/rt_map.h"
#include "runtime/rt_http.h"
#include "runtime/rt_db.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_io.h"
#include "runtime/rt_arena.h"
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
    int64_t id;
    rt_string_t source_file;
    int64_t line;
    int verified;
} Breakpoint;

typedef struct {
    int64_t frame_id;
    rt_string_t function_name;
    rt_string_t source_file;
    int64_t line;
    int64_t column;
} StackFrame;

typedef struct {
    rt_string_t name;
    rt_string_t value;
    rt_string_t type_name;
} Variable;

typedef struct {
    rt_string_t session_id;
    int64_t state;
    int64_t active_thread_id;
    int64_t current_line;
    rt_string_t current_function;
    rt_string_t stop_reason;
    int64_t breakpoint_count;
} DapSession;

DapSession create_session(rt_string_t);
Breakpoint set_breakpoint(DapSession*, rt_string_t, int64_t);
void step_next(DapSession*, int64_t, rt_string_t);
Variable create_variable(rt_string_t, rt_string_t, rt_string_t);
// module dap
// import std::io
// import std::string
// import std::vec
DapSession create_session(rt_string_t session_id) {
return (DapSession){.session_id = session_id, .state = 1LL, .active_thread_id = 1LL, .current_line = 1LL, .current_function = rt_string_from("main"), .stop_reason = rt_string_from("entry"), .breakpoint_count = 0LL};
}

Breakpoint set_breakpoint(DapSession* sess, rt_string_t file, int64_t line) {
(sess->breakpoint_count = ((*sess).breakpoint_count + 1LL));
return (Breakpoint){.id = (*sess).breakpoint_count, .source_file = file, .line = line, .verified = 1};
}

void step_next(DapSession* sess, int64_t next_line, rt_string_t next_fn) {
(sess->state = 4LL);
(sess->current_line = next_line);
(sess->current_function = next_fn);
(sess->stop_reason = rt_string_from("step"));
}

Variable create_variable(rt_string_t name, rt_string_t value, rt_string_t type_name) {
return (Variable){.name = name, .value = value, .type_name = type_name};
}

