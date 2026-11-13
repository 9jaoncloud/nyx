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

void dap_write(rt_string_t);
rt_string_t make_response(int64_t, int64_t, rt_string_t, int, rt_string_t);
rt_string_t make_event(int64_t, rt_string_t, rt_string_t);
rt_string_t extract_str_field(rt_string_t, rt_string_t);
int64_t parse_int_safe(rt_string_t);
int64_t handle_initialize(int64_t, int64_t);
int64_t handle_launch(int64_t, int64_t);
int64_t handle_attach(int64_t, int64_t);
int64_t handle_configuration_done(int64_t, int64_t);
int64_t handle_set_breakpoints(int64_t, int64_t, rt_string_t, void*);
int64_t handle_set_function_breakpoints(int64_t, int64_t);
int64_t handle_set_exception_breakpoints(int64_t, int64_t);
int64_t handle_threads(int64_t, int64_t);
int64_t handle_stack_trace(int64_t, int64_t, void*);
int64_t handle_scopes(int64_t, int64_t);
int64_t handle_variables(int64_t, int64_t, rt_string_t, void*);
int64_t handle_evaluate(int64_t, int64_t, rt_string_t);
int64_t handle_next(int64_t, int64_t, void*);
int64_t handle_step_in(int64_t, int64_t, void*);
int64_t handle_step_out(int64_t, int64_t, void*);
int64_t handle_continue(int64_t, int64_t, void*);
int64_t handle_pause(int64_t, int64_t, void*);
int64_t handle_terminate(int64_t, int64_t, rt_string_t);
int64_t handle_unknown(int64_t, int64_t, rt_string_t);
void nyx_main();

#line 1 "dap_server.nyx"
// module dap_server
// import std::io
// import std::string
// import std::tooling::dap
void dap_write(rt_string_t msg) {
Arena* __arena = arena_create();
int64_t len = msg.length;
rt_print(rt_string_from("Content-Length: "));
rt_print(({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)len); _buf.ref_count = 1; _buf; }));
rt_print(rt_string_from("\r\n\r\n"));
rt_print(msg);
arena_destroy(__arena);
}

rt_string_t make_response(int64_t seq, int64_t req_seq, rt_string_t command, int success, rt_string_t body) {
Arena* __arena = arena_create();
rt_string_t success_str = ({ rt_string_t __if_res_1; if (success) {
__if_res_1 = rt_string_from("true");
} else {
__if_res_1 = rt_string_from("false");
}
__if_res_1; });
arena_destroy(__arena);
return rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("{\"seq\":"), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)seq); _buf.ref_count = 1; _buf; })), rt_string_from(",\"type\":\"response\"")), rt_string_from(",\"request_seq\":")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)req_seq); _buf.ref_count = 1; _buf; })), rt_string_from(",\"success\":")), success_str), rt_string_from(",\"command\":\"")), command), rt_string_from("\"")), rt_string_from(",\"body\":")), body), rt_string_from("}"));
}

rt_string_t make_event(int64_t seq, rt_string_t event, rt_string_t body) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("{\"seq\":"), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)seq); _buf.ref_count = 1; _buf; })), rt_string_from(",\"type\":\"event\"")), rt_string_from(",\"event\":\"")), event), rt_string_from("\"")), rt_string_from(",\"body\":")), body), rt_string_from("}"));
}

rt_string_t extract_str_field(rt_string_t json, rt_string_t key) {
Arena* __arena = arena_create();
rt_string_t search = rt_string_concat(rt_string_concat(rt_string_from("\""), ({ rt_string_t _c = key; rt_string_retain(&_c); _c; })), rt_string_from("\":"));
int64_t search_len = search.length;
int64_t json_len = json.length;
int64_t i = 0LL;
while ((i < (json_len - search_len))) {
rt_string_t slice = rt_string_slice(json, i, (i + search_len));
({ if (rt_string_eq(slice, search)) {
int64_t val_start = (i + search_len);
int64_t vs = val_start;
while (((vs < json_len) && (rt_string_char_at(json, vs) == ' '))) {
vs = (vs + 1LL);
}
;
({ if ((vs >= json_len)) {
return rt_string_from("");
}
});
({ rt_string_t __if_res_4; if ((rt_string_char_at(json, vs) == '\"')) {
int64_t ve = (vs + 1LL);
while (((ve < json_len) && (rt_string_char_at(json, ve) != '\"'))) {
ve = (ve + 1LL);
}
;
__if_res_4 = rt_string_slice(json, (vs + 1LL), ve);
} else {
int64_t ve = vs;
while ((ve < json_len)) {
char c = rt_string_char_at(json, ve);
({ if (((((((c == ',') || (c == '}')) || (c == ']')) || (c == ' ')) || (c == '\n')) || (c == '\\'))) {
break;
}
});
ve = (ve + 1LL);
}
;
__if_res_4 = rt_string_trim(rt_string_slice(json, vs, ve));
}
__if_res_4; });
}
});
i = (i + 1LL);
}
;
arena_destroy(__arena);
return rt_string_from("");
}

int64_t parse_int_safe(rt_string_t s) {
Arena* __arena = arena_create();
({ if ((s.length == 0LL)) {
return 0LL;
}
});
arena_destroy(__arena);
return rt_string_parse_int(s);
}

int64_t handle_initialize(int64_t req_seq, int64_t seq) {
Arena* __arena = arena_create();
rt_string_t caps = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("{\"supportsConfigurationDoneRequest\":true,"), rt_string_from("\"supportsEvaluateForHovers\":true,")), rt_string_from("\"supportsStepBack\":false,")), rt_string_from("\"supportsRestartRequest\":true,")), rt_string_from("\"supportsTerminateRequest\":true,")), rt_string_from("\"supportSuspendDebuggee\":true,")), rt_string_from("\"supportsSetVariable\":true,")), rt_string_from("\"supportsReadMemoryRequest\":false}"));
dap_write(make_response(seq, req_seq, rt_string_from("initialize"), 1, caps));
arena_destroy(__arena);
return (seq + 1LL);
}

int64_t handle_launch(int64_t req_seq, int64_t seq) {
Arena* __arena = arena_create();
dap_write(make_response(seq, req_seq, rt_string_from("launch"), 1, rt_string_from("{}")));
int64_t seq2 = (seq + 1LL);
dap_write(make_event(seq2, rt_string_from("initialized"), rt_string_from("{}")));
arena_destroy(__arena);
return (seq2 + 1LL);
}

int64_t handle_attach(int64_t req_seq, int64_t seq) {
Arena* __arena = arena_create();
dap_write(make_response(seq, req_seq, rt_string_from("attach"), 1, rt_string_from("{}")));
int64_t seq2 = (seq + 1LL);
dap_write(make_event(seq2, rt_string_from("initialized"), rt_string_from("{}")));
arena_destroy(__arena);
return (seq2 + 1LL);
}

int64_t handle_configuration_done(int64_t req_seq, int64_t seq) {
Arena* __arena = arena_create();
dap_write(make_response(seq, req_seq, rt_string_from("configurationDone"), 1, rt_string_from("{}")));
int64_t seq2 = (seq + 1LL);
dap_write(make_event(seq2, rt_string_from("stopped"), rt_string_from("{\"reason\":\"entry\",\"threadId\":1,\"allThreadsStopped\":true}")));
arena_destroy(__arena);
return (seq2 + 1LL);
}

int64_t handle_set_breakpoints(int64_t req_seq, int64_t seq, rt_string_t body, void* sess) {
Arena* __arena = arena_create();
rt_string_t source_path = extract_str_field(({ rt_string_t _c = body; rt_string_retain(&_c); _c; }), rt_string_from("path"));
rt_string_t file = ({ rt_string_t __if_res_7; if ((source_path.length > 0LL)) {
__if_res_7 = source_path;
} else {
__if_res_7 = rt_string_from("src/main.nyx");
}
__if_res_7; });
rt_string_t bp_json = rt_string_from("[");
int64_t bp_count = 0LL;
rt_string_t lines_key = rt_string_from("\"lines\":[");
NyxOption lines_pos = rt_string_find(body, lines_key);
({ if ((lines_pos >= 0LL)) {
int64_t pos = (lines_pos + lines_key.length);
int64_t body_len = body.length;
while (((pos < body_len) && (bp_count < 8LL))) {
while (((pos < body_len) && ((rt_string_char_at(body, pos) == ' ') || (rt_string_char_at(body, pos) == '\n')))) {
pos = (pos + 1LL);
}
;
({ if ((pos >= body_len)) {
break;
}
});
char ch = rt_string_char_at(body, pos);
({ if ((ch == ']')) {
break;
}
});
({ if ((ch == ',')) {
pos = (pos + 1LL);
}
});
int64_t num_start = pos;
while ((pos < body_len)) {
char c = rt_string_char_at(body, pos);
({ if ((((c == ',') || (c == ']')) || (c == ' '))) {
break;
}
});
pos = (pos + 1LL);
}
;
rt_string_t num_str = rt_string_trim(rt_string_slice(body, num_start, pos));
({ if ((num_str.length > 0LL)) {
int64_t line_num = parse_int_safe(num_str);
Breakpoint bp = set_breakpoint(sess, ({ rt_string_t _c = file; rt_string_retain(&_c); _c; }), line_num);
({ if ((bp_count > 0LL)) {
bp_json = rt_string_concat(bp_json, rt_string_from(","));
}
});
bp_json = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(bp_json, rt_string_from("{\"id\":")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)bp.id); _buf.ref_count = 1; _buf; })), rt_string_from(",\"verified\":true")), rt_string_from(",\"line\":")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)bp.line); _buf.ref_count = 1; _buf; })), rt_string_from(",\"source\":{\"path\":\"")), ({ rt_string_t _c = file; rt_string_retain(&_c); _c; })), rt_string_from("\"}}"));
bp_count = (bp_count + 1LL);
}
});
}
;
}
});
({ if ((bp_count == 0LL)) {
Breakpoint bp = set_breakpoint(sess, ({ rt_string_t _c = file; rt_string_retain(&_c); _c; }), 1LL);
bp_json = rt_string_concat(rt_string_concat(rt_string_concat(bp_json, rt_string_from("{\"id\":")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)bp.id); _buf.ref_count = 1; _buf; })), rt_string_from(",\"verified\":true,\"line\":1}"));
}
});
bp_json = rt_string_concat(bp_json, rt_string_from("]"));
dap_write(make_response(seq, req_seq, rt_string_from("setBreakpoints"), 1, rt_string_concat(rt_string_concat(rt_string_from("{\"breakpoints\":"), bp_json), rt_string_from("}"))));
arena_destroy(__arena);
return (seq + 1LL);
}

int64_t handle_set_function_breakpoints(int64_t req_seq, int64_t seq) {
Arena* __arena = arena_create();
dap_write(make_response(seq, req_seq, rt_string_from("setFunctionBreakpoints"), 1, rt_string_from("{\"breakpoints\":[]}")));
arena_destroy(__arena);
return (seq + 1LL);
}

int64_t handle_set_exception_breakpoints(int64_t req_seq, int64_t seq) {
Arena* __arena = arena_create();
dap_write(make_response(seq, req_seq, rt_string_from("setExceptionBreakpoints"), 1, rt_string_from("{\"breakpoints\":[]}")));
arena_destroy(__arena);
return (seq + 1LL);
}

int64_t handle_threads(int64_t req_seq, int64_t seq) {
Arena* __arena = arena_create();
rt_string_t threads = rt_string_from("[{\"id\":1,\"name\":\"main\"}]");
dap_write(make_response(seq, req_seq, rt_string_from("threads"), 1, rt_string_concat(rt_string_concat(rt_string_from("{\"threads\":"), threads), rt_string_from("}"))));
arena_destroy(__arena);
return (seq + 1LL);
}

int64_t handle_stack_trace(int64_t req_seq, int64_t seq, void* sess) {
Arena* __arena = arena_create();
rt_string_t frame = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("{\"id\":0"), rt_string_from(",\"name\":\"")), ({ rt_string_t _c = (*sess).current_function; rt_string_retain(&_c); _c; })), rt_string_from("\"")), rt_string_from(",\"source\":{\"name\":\"main.nyx\",\"path\":\"src/main.nyx\"}")), rt_string_from(",\"line\":")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)(*sess).current_line); _buf.ref_count = 1; _buf; })), rt_string_from(",\"column\":0}"));
dap_write(make_response(seq, req_seq, rt_string_from("stackTrace"), 1, rt_string_concat(rt_string_concat(rt_string_from("{\"stackFrames\":["), frame), rt_string_from("],\"totalFrames\":1}"))));
arena_destroy(__arena);
return (seq + 1LL);
}

int64_t handle_scopes(int64_t req_seq, int64_t seq) {
Arena* __arena = arena_create();
rt_string_t scopes = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("["), rt_string_from("{\"name\":\"Locals\",\"variablesReference\":1,\"expensive\":false},")), rt_string_from("{\"name\":\"Region Memory\",\"variablesReference\":2,\"expensive\":false},")), rt_string_from("{\"name\":\"Globals\",\"variablesReference\":3,\"expensive\":false}")), rt_string_from("]"));
dap_write(make_response(seq, req_seq, rt_string_from("scopes"), 1, rt_string_concat(rt_string_concat(rt_string_from("{\"scopes\":"), scopes), rt_string_from("}"))));
arena_destroy(__arena);
return (seq + 1LL);
}

int64_t handle_variables(int64_t req_seq, int64_t seq, rt_string_t body, void* sess) {
Arena* __arena = arena_create();
rt_string_t vars_ref_str = extract_str_field(body, rt_string_from("variablesReference"));
int64_t vars_ref = parse_int_safe(vars_ref_str);
rt_string_t vars = ({ rt_string_t __if_res_16; if ((vars_ref == 2LL)) {
__if_res_16 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("[{\"name\":\"region_0\",\"value\":\"heap: 4096 bytes (active)\","), rt_string_from("\"type\":\"Region\",\"variablesReference\":0},")), rt_string_from("{\"name\":\"region_1\",\"value\":\"stack: 256 bytes (bump)\",")), rt_string_from("\"type\":\"Region\",\"variablesReference\":0}]"));
} else if ((vars_ref == 3LL)) {
__if_res_16 = rt_string_concat(rt_string_from("[{\"name\":\"NYX_VERSION\",\"value\":\"\\\"0.19.0\\\"\","), rt_string_from("\"type\":\"&str\",\"variablesReference\":0}]"));
} else {
__if_res_16 = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("[{\"name\":\"argc\",\"value\":\"1\","), rt_string_from("\"type\":\"i32\",\"variablesReference\":0},")), rt_string_from("{\"name\":\"current_line\",\"value\":\"")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)(*sess).current_line); _buf.ref_count = 1; _buf; })), rt_string_from("\",")), rt_string_from("\"type\":\"i32\",\"variablesReference\":0},")), rt_string_from("{\"name\":\"current_fn\",\"value\":\"\\\"")), ({ rt_string_t _c = (*sess).current_function; rt_string_retain(&_c); _c; })), rt_string_from("\\\"\",")), rt_string_from("\"type\":\"String\",\"variablesReference\":0}]"));
}
__if_res_16; });
dap_write(make_response(seq, req_seq, rt_string_from("variables"), 1, rt_string_concat(rt_string_concat(rt_string_from("{\"variables\":"), vars), rt_string_from("}"))));
arena_destroy(__arena);
return (seq + 1LL);
}

int64_t handle_evaluate(int64_t req_seq, int64_t seq, rt_string_t body) {
Arena* __arena = arena_create();
rt_string_t expr = extract_str_field(({ rt_string_t _c = body; rt_string_retain(&_c); _c; }), rt_string_from("expression"));
rt_string_t eval_context = extract_str_field(body, rt_string_from("context"));
rt_string_t result = ({ rt_string_t __if_res_17; if (rt_string_eq(eval_context, rt_string_from("hover"))) {
__if_res_17 = rt_string_concat(({ rt_string_t _c = expr; rt_string_retain(&_c); _c; }), rt_string_from(": <type inferred>"));
} else {
__if_res_17 = rt_string_concat(rt_string_from("= "), ({ rt_string_t _c = expr; rt_string_retain(&_c); _c; }));
}
__if_res_17; });
dap_write(make_response(seq, req_seq, rt_string_from("evaluate"), 1, rt_string_concat(rt_string_concat(rt_string_from("{\"result\":\""), result), rt_string_from("\",\"variablesReference\":0}"))));
arena_destroy(__arena);
return (seq + 1LL);
}

int64_t handle_next(int64_t req_seq, int64_t seq, void* sess) {
Arena* __arena = arena_create();
step_next(sess, ((*sess).current_line + 1LL), ({ rt_string_t _c = (*sess).current_function; rt_string_retain(&_c); _c; }));
dap_write(make_response(seq, req_seq, rt_string_from("next"), 1, rt_string_from("{}")));
int64_t seq2 = (seq + 1LL);
dap_write(make_event(seq2, rt_string_from("stopped"), rt_string_from("{\"reason\":\"step\",\"threadId\":1,\"allThreadsStopped\":true}")));
arena_destroy(__arena);
return (seq2 + 1LL);
}

int64_t handle_step_in(int64_t req_seq, int64_t seq, void* sess) {
Arena* __arena = arena_create();
step_next(sess, ((*sess).current_line + 1LL), ({ rt_string_t _c = (*sess).current_function; rt_string_retain(&_c); _c; }));
dap_write(make_response(seq, req_seq, rt_string_from("stepIn"), 1, rt_string_from("{}")));
int64_t seq2 = (seq + 1LL);
dap_write(make_event(seq2, rt_string_from("stopped"), rt_string_from("{\"reason\":\"step\",\"threadId\":1,\"allThreadsStopped\":true}")));
arena_destroy(__arena);
return (seq2 + 1LL);
}

int64_t handle_step_out(int64_t req_seq, int64_t seq, void* sess) {
Arena* __arena = arena_create();
dap_write(make_response(seq, req_seq, rt_string_from("stepOut"), 1, rt_string_from("{}")));
int64_t seq2 = (seq + 1LL);
dap_write(make_event(seq2, rt_string_from("stopped"), rt_string_from("{\"reason\":\"step\",\"threadId\":1,\"allThreadsStopped\":true}")));
arena_destroy(__arena);
return (seq2 + 1LL);
}

int64_t handle_continue(int64_t req_seq, int64_t seq, void* sess) {
Arena* __arena = arena_create();
(sess->state = 2LL);
dap_write(make_response(seq, req_seq, rt_string_from("continue"), 1, rt_string_from("{\"allThreadsContinued\":true}")));
int64_t seq2 = (seq + 1LL);
dap_write(make_event(seq2, rt_string_from("terminated"), rt_string_from("{}")));
arena_destroy(__arena);
return (seq2 + 1LL);
}

int64_t handle_pause(int64_t req_seq, int64_t seq, void* sess) {
Arena* __arena = arena_create();
(sess->state = 3LL);
dap_write(make_response(seq, req_seq, rt_string_from("pause"), 1, rt_string_from("{}")));
int64_t seq2 = (seq + 1LL);
dap_write(make_event(seq2, rt_string_from("stopped"), rt_string_from("{\"reason\":\"pause\",\"threadId\":1,\"allThreadsStopped\":true}")));
arena_destroy(__arena);
return (seq2 + 1LL);
}

int64_t handle_terminate(int64_t req_seq, int64_t seq, rt_string_t cmd) {
Arena* __arena = arena_create();
dap_write(make_response(seq, req_seq, cmd, 1, rt_string_from("{}")));
int64_t seq2 = (seq + 1LL);
dap_write(make_event(seq2, rt_string_from("terminated"), rt_string_from("{}")));
int64_t seq3 = (seq2 + 1LL);
dap_write(make_event(seq3, rt_string_from("exited"), rt_string_from("{\"exitCode\":0}")));
arena_destroy(__arena);
return (seq3 + 1LL);
}

int64_t handle_unknown(int64_t req_seq, int64_t seq, rt_string_t cmd) {
Arena* __arena = arena_create();
rt_string_t err = rt_string_concat(rt_string_concat(rt_string_from("{\"error\":{\"id\":1001,\"format\":\"Unknown DAP command: "), ({ rt_string_t _c = cmd; rt_string_retain(&_c); _c; })), rt_string_from("\",\"sendTelemetry\":false}}"));
rt_string_t resp = rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("{\"seq\":"), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)seq); _buf.ref_count = 1; _buf; })), rt_string_from(",\"type\":\"response\"")), rt_string_from(",\"request_seq\":")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)req_seq); _buf.ref_count = 1; _buf; })), rt_string_from(",\"success\":false")), rt_string_from(",\"command\":\"")), cmd), rt_string_from("\"")), rt_string_from(",\"message\":\"unsupported\"")), rt_string_from(",\"body\":")), err), rt_string_from("}"));
dap_write(resp);
arena_destroy(__arena);
return (seq + 1LL);
}

void nyx_main() {
Arena* __arena = arena_create();
DapSession sess = create_session(rt_string_from("nyx-dap-1"));
int64_t seq = 1LL;
int running = 1;
rt_eprintln(rt_string_from("[nyx-dap] Nyx DAP Server v1.0 ready"));
while (running) {
int64_t content_length = (-1LL);
int header_done = 0;
while ((!header_done)) {
rt_string_t line = read_line();
({ if ((line.length == 0LL)) {
running = 0;
header_done = 1;
} else if (rt_string_starts_with(line, rt_string_from("Content-Length:"))) {
rt_string_t val = rt_string_trim(rt_string_slice(line, 15LL, line.length));
content_length = parse_int_safe(val);
} else if ((rt_string_trim(line).length == 0LL)) {
header_done = 1;
}
});
}
;
({ if (((!running) || (content_length <= 0LL))) {
break;
}
});
int64_t body = read_exact(content_length);
rt_string_t command = extract_str_field(body, rt_string_from("command"));
rt_string_t req_seq_s = extract_str_field(body, rt_string_from("seq"));
int64_t req_seq = parse_int_safe(req_seq_s);
rt_eprintln(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_concat(rt_string_from("[nyx-dap] <- "), ({ rt_string_t _c = command; rt_string_retain(&_c); _c; })), rt_string_from(" (req=")), ({ rt_string_t _buf; _buf.data = (char*)malloc(32); _buf.length = snprintf(_buf.data, 32, "%lld", (long long)req_seq); _buf.ref_count = 1; _buf; })), rt_string_from(")")));
({ int64_t __if_res_20; if (rt_string_eq(command, rt_string_from("initialize"))) {
__if_res_20 = (seq = handle_initialize(req_seq, seq));
} else if (rt_string_eq(command, rt_string_from("launch"))) {
__if_res_20 = (seq = handle_launch(req_seq, seq));
} else if (rt_string_eq(command, rt_string_from("attach"))) {
__if_res_20 = (seq = handle_attach(req_seq, seq));
} else if (rt_string_eq(command, rt_string_from("configurationDone"))) {
__if_res_20 = (seq = handle_configuration_done(req_seq, seq));
} else if (rt_string_eq(command, rt_string_from("setBreakpoints"))) {
__if_res_20 = (seq = handle_set_breakpoints(req_seq, seq, body, (&(sess))));
} else if (rt_string_eq(command, rt_string_from("setFunctionBreakpoints"))) {
__if_res_20 = (seq = handle_set_function_breakpoints(req_seq, seq));
} else if (rt_string_eq(command, rt_string_from("setExceptionBreakpoints"))) {
__if_res_20 = (seq = handle_set_exception_breakpoints(req_seq, seq));
} else if (rt_string_eq(command, rt_string_from("threads"))) {
__if_res_20 = (seq = handle_threads(req_seq, seq));
} else if (rt_string_eq(command, rt_string_from("stackTrace"))) {
__if_res_20 = (seq = handle_stack_trace(req_seq, seq, (&(sess))));
} else if (rt_string_eq(command, rt_string_from("scopes"))) {
__if_res_20 = (seq = handle_scopes(req_seq, seq));
} else if (rt_string_eq(command, rt_string_from("variables"))) {
__if_res_20 = (seq = handle_variables(req_seq, seq, body, (&(sess))));
} else if (rt_string_eq(command, rt_string_from("evaluate"))) {
__if_res_20 = (seq = handle_evaluate(req_seq, seq, body));
} else if (rt_string_eq(command, rt_string_from("next"))) {
__if_res_20 = (seq = handle_next(req_seq, seq, (&(sess))));
} else if (rt_string_eq(command, rt_string_from("stepIn"))) {
__if_res_20 = (seq = handle_step_in(req_seq, seq, (&(sess))));
} else if (rt_string_eq(command, rt_string_from("stepOut"))) {
__if_res_20 = (seq = handle_step_out(req_seq, seq, (&(sess))));
} else if (rt_string_eq(command, rt_string_from("continue"))) {
__if_res_20 = (seq = handle_continue(req_seq, seq, (&(sess))));
} else if (rt_string_eq(command, rt_string_from("pause"))) {
__if_res_20 = (seq = handle_pause(req_seq, seq, (&(sess))));
} else if (rt_string_eq(command, rt_string_from("restart"))) {
sess = create_session(rt_string_from("nyx-dap-restart"));
__if_res_20 = (seq = handle_launch(req_seq, seq));
} else if ((rt_string_eq(command, rt_string_from("terminate")) || rt_string_eq(command, rt_string_from("disconnect")))) {
seq = handle_terminate(req_seq, seq, command);
__if_res_20 = (running = 0);
} else {
__if_res_20 = (seq = handle_unknown(req_seq, seq, command));
}
__if_res_20; });
}
;
rt_eprintln(rt_string_from("[nyx-dap] Session terminated. Goodbye."));
arena_destroy(__arena);
}

#include "runtime/rt_async.h"

int main(int argc, char** argv) {
    nyx_main();
    return 0;
}
