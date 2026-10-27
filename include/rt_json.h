#ifndef RT_JSON_H
#define RT_JSON_H
#include "rt_string.h"
#include "rt_option.h"

typedef enum { JSON_NULL, JSON_BOOL, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT } JsonType;

typedef struct JsonVal {
    JsonType type;
    union {
        int bool_val;
        double num_val;
        rt_string_t str_val;
        struct { struct JsonVal** items; size_t len; } arr;
        struct { rt_string_t* keys; struct JsonVal** vals; size_t len; } obj;
    } data;
} JsonVal;

// Called by codegen: json.parse(input) → parse(input)
// Called by codegen: json.pretty(val) → pretty(val)  
// Called by codegen: json.stringify(val) → stringify(val)
NyxResult parse(rt_string_t input);
rt_string_t pretty(rt_string_t json_val);
rt_string_t stringify(void* json_val);
void json_free(JsonVal* v);

#endif
