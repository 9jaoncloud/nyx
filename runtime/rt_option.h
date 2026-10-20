#ifndef RT_OPTION_H
#define RT_OPTION_H
#include <stdint.h>
#include "rt_string.h"

typedef struct {
    int tag;
    union {
        int64_t int_val;
        double float_val;
        void* ptr_val;
    } data;
} NyxOption;

typedef struct {
    int tag;
    union {
        int64_t int_val;
        double float_val;
        void* ptr_val;
    } data;
} NyxResult;

#define OPTION_SOME 1
#define OPTION_NONE 0
#define OPTION_NONE_TAG 0
#define OPTION_NONE_VAL ((NyxOption){ .tag = 0, .data.int_val = 0 })

/* Cross-Language Null/None/Nil/Nullopt Aliases */
#define OPTION_NULL OPTION_NONE
#define OPTION_NIL OPTION_NONE
#define OPTION_NULLOPT OPTION_NONE
#define OPTION_UNDEFINED OPTION_NONE
#define rt_option_null() rt_option_none()
#define rt_option_nil() rt_option_none()
#define rt_option_nullopt() rt_option_none()
#define rt_option_undefined() rt_option_none()

#define RESULT_OK 1
#define RESULT_ERR 0

static inline NyxOption rt_option_some_int(int64_t val) {
    NyxOption o;
    o.tag = OPTION_SOME;
    o.data.int_val = val;
    return o;
}

static inline NyxOption rt_option_some_ptr(void* val) {
    NyxOption o;
    o.tag = OPTION_SOME;
    o.data.ptr_val = val;
    return o;
}

static inline NyxOption rt_option_none(void) {
    NyxOption o;
    o.tag = OPTION_NONE_TAG;
    o.data.int_val = 0;
    return o;
}

static inline NyxResult rt_result_ok_int(int64_t val) {
    NyxResult r;
    r.tag = RESULT_OK;
    r.data.int_val = val;
    return r;
}

static inline NyxResult rt_result_err(void) {
    NyxResult r;
    r.tag = RESULT_ERR;
    r.data.int_val = 0;
    return r;
}

static inline NyxResult rt_option_ok_or_else(NyxOption o, void* fn) {
    if (o.tag == OPTION_SOME) {
        NyxResult r;
        r.tag = RESULT_OK;
        r.data.ptr_val = o.data.ptr_val;
        return r;
    }
    // Call the error function
    typedef rt_string_t (*ErrFn)();
    ErrFn ef = (ErrFn)fn;
    rt_string_t msg = ef();
    NyxResult r;
    r.tag = RESULT_ERR;
    r.data.ptr_val = (void*)msg.data;
    return r;
}

static inline int64_t int64_t_rev(int64_t x) {
    return -x;
}

static inline int64_t int64_t_unwrap(NyxOption o) {
    return o.data.int_val;
}

#endif
