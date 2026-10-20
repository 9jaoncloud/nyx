#ifndef RT_VEC_H
#define RT_VEC_H
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "rt_string.h"
#include "rt_option.h"

typedef struct {
    void* data;
    size_t len;
    size_t cap;
    size_t elem_size;
} NyxVec;

static inline NyxVec rt_vec_new(size_t elem_size) {
    NyxVec v;
    v.data = NULL;
    v.len = 0;
    v.cap = 0;
    v.elem_size = elem_size;
    return v;
}

static inline void rt_vec_push(NyxVec* v, void* elem) {
    if (v->len >= v->cap) {
        v->cap = v->cap == 0 ? 8 : v->cap * 2;
        v->data = realloc(v->data, v->cap * v->elem_size);
    }
    memcpy((char*)v->data + v->len * v->elem_size, elem, v->elem_size);
    v->len++;
}

static inline void* rt_vec_get(NyxVec v, size_t idx) {
    return (char*)v.data + idx * v.elem_size;
}

static inline NyxVec rt_vec_clone(NyxVec v) {
    NyxVec result = rt_vec_new(v.elem_size);
    if (v.len > 0 && v.data) {
        result.data = malloc(v.len * v.elem_size);
        result.len = v.len;
        result.cap = v.len;
        memcpy(result.data, v.data, v.len * v.elem_size);
    }
    return result;
}

// Nyx stdlib Vec methods — used by self-hosted compiler codegen
static inline void rt_vec_pop(NyxVec* v) {
    if (v->len > 0) v->len--;
}

static inline void* rt_vec_last(NyxVec v) {
    if (v.len == 0) return NULL;
    return (char*)v.data + (v.len - 1) * v.elem_size;
}

static inline void* rt_vec_last_mut(NyxVec* v) {
    if (v->len == 0) return NULL;
    return (char*)v->data + (v->len - 1) * v->elem_size;
}

typedef struct {
    NyxVec vec;
    size_t pos;
    size_t len;
} NyxVecIter;

static inline NyxVecIter rt_vec_iter(NyxVec v) {
    NyxVecIter it;
    it.vec = v;
    it.pos = 0;
    return it;
}

static inline NyxVecIter rt_vec_rev(NyxVec v) {
    NyxVecIter it;
    it.vec = v;
    it.pos = v.len;
    return it;
}

static inline int rt_vec_iter_done(NyxVecIter* it) {
    return it->pos >= it->vec.len;
}

static inline int rt_vec_rev_done(NyxVecIter* it) {
    return it->pos == 0;
}

static inline void* rt_vec_iter_next(NyxVecIter* it) {
    void* elem = (char*)it->vec.data + it->pos * it->vec.elem_size;
    it->pos++;
    return elem;
}

static inline void* rt_vec_rev_next(NyxVecIter* it) {
    it->pos--;
    return (char*)it->vec.data + it->pos * it->vec.elem_size;
}

static inline NyxVec rt_vec_collect(NyxVec v) {
    return rt_vec_clone(v);
}

static inline NyxVec int64_t_map(NyxVecIter it, int64_t (*fn)(int64_t)) {
    NyxVec result = rt_vec_new(sizeof(int64_t));
    while (!rt_vec_iter_done(&it)) {
        int64_t val = *(int64_t*)rt_vec_iter_next(&it);
        int64_t mapped = fn(val);
        rt_vec_push(&result, &mapped);
    }
    return result;
}

static inline NyxVecIter _rev(NyxVecIter it) { return rt_vec_rev(it.vec); }
static inline int64_t _get(void* map, void* key) { return 0; }

static inline int rt_vec_contains(NyxVec v, rt_string_t s) {
    for (size_t i = 0; i < v.len; i++) {
        rt_string_t* elem = (rt_string_t*)((char*)v.data + i * v.elem_size);
        if (rt_string_eq(*elem, s)) return 1;
    }
    return 0;
}

static inline NyxOption rt_vec_get_option(NyxVec v, rt_string_t opt) {
    for (size_t i = 0; i < v.len; i++) {
        rt_string_t* elem = (rt_string_t*)((char*)v.data + i * v.elem_size);
        if (rt_string_eq(*elem, opt) && i + 1 < v.len) {
            rt_string_t* val = (rt_string_t*)((char*)v.data + (i + 1) * v.elem_size);
            return rt_option_some_ptr((void*)val->data);
        }
    }
    return rt_option_none();
}

static inline rt_string_t rt_vec_join(NyxVec v, rt_string_t sep) {
    if (v.len == 0) return rt_string_from("");
    rt_string_t res = *(rt_string_t*)rt_vec_get(v, 0);
    res = rt_string_from(res.data);
    for (size_t i = 1; i < v.len; i++) {
        res = rt_string_concat(res, sep);
        res = rt_string_concat(res, *(rt_string_t*)rt_vec_get(v, i));
    }
    return res;
}

static inline NyxOption rt_vec_get_at(NyxVec v, size_t idx) {
    if (idx < v.len) {
        void* ptr = rt_vec_get(v, idx);
        NyxOption o;
        o.tag = OPTION_SOME;
        memset(&o.data, 0, sizeof(o.data));
        if (v.elem_size <= sizeof(o.data)) {
            memcpy(&o.data, ptr, v.elem_size);
        } else {
            o.data.ptr_val = ptr;
        }
        return o;
    }
    return rt_option_none();
}

static inline NyxVec rt_vec_filter(NyxVec v, int (*pred)(int64_t)) {
    NyxVec result = rt_vec_new(v.elem_size);
    for (size_t i = 0; i < v.len; i++) {
        int64_t val = *(int64_t*)((char*)v.data + i * v.elem_size);
        if (pred(val)) {
            rt_vec_push(&result, &val);
        }
    }
    return result;
}

#endif // RT_VEC_H
