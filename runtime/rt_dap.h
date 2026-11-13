#ifndef RT_DAP_H
#define RT_DAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RT_DAP_STATE_UNINITIALIZED = 0,
    RT_DAP_STATE_INITIALIZED,
    RT_DAP_STATE_RUNNING,
    RT_DAP_STATE_PAUSED,
    RT_DAP_STATE_STEPPING,
    RT_DAP_STATE_TERMINATED
} rt_dap_state_t;

typedef struct {
    uint32_t id;
    const char* source_path;
    uint32_t line;
    bool verified;
    uint64_t hit_count;
} rt_dap_breakpoint_t;

typedef struct {
    uint32_t frame_id;
    const char* function_name;
    const char* source_path;
    uint32_t line;
    uint32_t column;
} rt_dap_stack_frame_t;

typedef struct {
    const char* name;
    const char* value;
    const char* type_name;
    uint32_t variables_reference;
} rt_dap_variable_t;

typedef struct {
    rt_dap_state_t state;
    uint32_t active_thread_id;
    uint32_t breakpoint_count;
    uint32_t current_line;
    const char* current_function;
    const char* stop_reason;
} rt_dap_session_t;

static inline rt_dap_session_t rt_dap_create_session(void) {
    rt_dap_session_t session;
    session.state = RT_DAP_STATE_INITIALIZED;
    session.active_thread_id = 1;
    session.breakpoint_count = 0;
    session.current_line = 1;
    session.current_function = "main";
    session.stop_reason = "entry";
    return session;
}

static inline bool rt_dap_set_breakpoint(rt_dap_session_t* session, const char* file, uint32_t line) {
    if (!session) return false;
    session->breakpoint_count++;
    return true;
}

static inline bool rt_dap_step_next(rt_dap_session_t* session, uint32_t next_line, const char* next_fn) {
    if (!session) return false;
    session->state = RT_DAP_STATE_STEPPING;
    session->current_line = next_line;
    session->current_function = next_fn;
    session->stop_reason = "step";
    return true;
}

#ifdef __cplusplus
}
#endif

#endif // RT_DAP_H
