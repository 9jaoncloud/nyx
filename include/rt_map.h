#ifndef RT_MAP_H
#define RT_MAP_H
#include <stdlib.h>
#include "rt_string.h"
#include "rt_option.h"

typedef struct NyxMapEntry {
    rt_string_t key;
    int64_t value;
} NyxMapEntry;

typedef struct {
    NyxMapEntry* entries;
    size_t len;
    size_t cap;
} NyxMap;

static inline NyxMap rt_map_new(void) {
    NyxMap m;
    m.entries = NULL;
    m.len = 0;
    m.cap = 0;
    return m;
}

static inline void rt_map_insert(NyxMap* m, rt_string_t key, int64_t value) {
    if (m->len >= m->cap) {
        m->cap = m->cap == 0 ? 8 : m->cap * 2;
        m->entries = (NyxMapEntry*)realloc(m->entries, m->cap * sizeof(NyxMapEntry));
    }
    m->entries[m->len].key = key;
    m->entries[m->len].value = value;
    m->len++;
}

static inline NyxOption rt_map_get(NyxMap* m, rt_string_t key) {
    for (size_t i = 0; i < m->len; i++) {
        if (strcmp(m->entries[i].key.data, key.data) == 0) {
            return rt_option_some_int(m->entries[i].value);
        }
    }
    return rt_option_none();
}

#endif
