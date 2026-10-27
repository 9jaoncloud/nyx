#include "rt_string.h"
#include <stdlib.h>
#include <string.h>

rt_string_t rt_string_from(const char* s) {
    rt_string_t str;
    if (!s) {
        str.length = 0;
        str.data = (char*)calloc(1, 1);
        str.ref_count = 1;
        return str;
    }
    str.length = strlen(s);
    str.data = (char*)malloc(str.length + 1);
    memcpy(str.data, s, str.length);
    str.data[str.length] = '\0';
    str.ref_count = 1;
    return str;
}

rt_string_t rt_string_concat(rt_string_t a, rt_string_t b) {
    size_t len_a = (a.data != NULL) ? a.length : 0;
    size_t len_b = (b.data != NULL) ? b.length : 0;
    rt_string_t result;
    result.length = len_a + len_b;
    result.data = (char*)malloc(result.length + 1);
    if (len_a > 0 && a.data) {
        memcpy(result.data, a.data, len_a);
    }
    if (len_b > 0 && b.data) {
        memcpy(result.data + len_a, b.data, len_b);
    }
    result.data[result.length] = '\0';
    result.ref_count = 1;
    return result;
}

void rt_string_retain(rt_string_t* s) {
    if (s) s->ref_count++;
}

void rt_string_release(rt_string_t* s) {
    if (s && s->data && --s->ref_count <= 0) {
        free(s->data);
        s->data = NULL;
        s->length = 0;
        s->ref_count = 0;
    }
}
