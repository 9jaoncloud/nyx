/*
 * rt_swiss_map.c — Nyx High-Performance Swiss Tables Implementation
 */

#include "rt_swiss_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Fast FNV-1a 64-bit Hash */
static uint64_t hash_key(const char* str) {
    uint64_t hash = 14695981039346656037ULL;
    while (*str) {
        hash ^= (uint64_t)(unsigned char)(*str++);
        hash *= 1099511628211ULL;
    }
    return hash;
}

static inline uint8_t h2(uint64_t hash) {
    return (uint8_t)(hash & 0x7F); /* 7-bit fingerprint for control byte */
}

static inline size_t h1(uint64_t hash, size_t capacity) {
    return (size_t)((hash >> 7) & (capacity - 1));
}

NyxSwissMap* nyx_swiss_map_create(size_t initial_cap) {
    NyxSwissMap* map = (NyxSwissMap*)malloc(sizeof(NyxSwissMap));
    if (!map) return NULL;

    size_t cap = 16;
    while (cap < initial_cap) cap *= 2;

    map->capacity = cap;
    map->size = 0;
    map->growth_left = (cap * 7) / 8; /* 87.5% load factor */

    map->ctrl = (uint8_t*)malloc(cap + 16); /* 16 extra bytes for group probing */
    memset(map->ctrl, SWISS_CTRL_EMPTY, cap + 16);

    map->slots = (NyxSwissSlot*)calloc(cap, sizeof(NyxSwissSlot));
    return map;
}

void nyx_swiss_map_destroy(NyxSwissMap* map) {
    if (!map) return;
    if (map->ctrl) free(map->ctrl);
    if (map->slots) free(map->slots);
    free(map);
}

static void resize_swiss_map(NyxSwissMap* map) {
    size_t old_cap = map->capacity;
    uint8_t* old_ctrl = map->ctrl;
    NyxSwissSlot* old_slots = map->slots;

    size_t new_cap = old_cap * 2;
    map->capacity = new_cap;
    map->growth_left = (new_cap * 7) / 8 - map->size;

    map->ctrl = (uint8_t*)malloc(new_cap + 16);
    memset(map->ctrl, SWISS_CTRL_EMPTY, new_cap + 16);
    map->slots = (NyxSwissSlot*)calloc(new_cap, sizeof(NyxSwissSlot));

    for (size_t i = 0; i < old_cap; i++) {
        if (old_ctrl[i] != SWISS_CTRL_EMPTY && old_ctrl[i] != SWISS_CTRL_DELETED) {
            uint64_t hash = hash_key(old_slots[i].key);
            uint8_t tag = h2(hash);
            size_t idx = h1(hash, new_cap);

            size_t probe = 0;
            while (map->ctrl[idx] != SWISS_CTRL_EMPTY) {
                probe++;
                idx = (idx + probe) & (new_cap - 1);
            }

            map->ctrl[idx] = tag;
            map->slots[idx] = old_slots[i];
        }
    }

    free(old_ctrl);
    free(old_slots);
}

bool nyx_swiss_map_insert(NyxSwissMap* map, const char* key, int64_t value) {
    if (!map || !key) return false;

    if (map->growth_left == 0) {
        resize_swiss_map(map);
    }

    uint64_t hash = hash_key(key);
    uint8_t tag = h2(hash);
    size_t idx = h1(hash, map->capacity);

    size_t probe = 0;
    size_t first_deleted = (size_t)-1;

    while (map->ctrl[idx] != SWISS_CTRL_EMPTY) {
        if (map->ctrl[idx] == tag && strcmp(map->slots[idx].key, key) == 0) {
            map->slots[idx].value = value; /* Key already exists, update value */
            return true;
        }
        if (map->ctrl[idx] == SWISS_CTRL_DELETED && first_deleted == (size_t)-1) {
            first_deleted = idx;
        }
        probe++;
        idx = (idx + probe) & (map->capacity - 1);
    }

    size_t target_idx = (first_deleted != (size_t)-1) ? first_deleted : idx;
    map->ctrl[target_idx] = tag;
    strncpy(map->slots[target_idx].key, key, sizeof(map->slots[target_idx].key) - 1);
    map->slots[target_idx].value = value;

    map->size++;
    map->growth_left--;
    return true;
}

bool nyx_swiss_map_find(const NyxSwissMap* map, const char* key, int64_t* out_val) {
    if (!map || !key) return false;

    uint64_t hash = hash_key(key);
    uint8_t tag = h2(hash);
    size_t idx = h1(hash, map->capacity);

    size_t probe = 0;
    while (map->ctrl[idx] != SWISS_CTRL_EMPTY) {
        if (map->ctrl[idx] == tag && strcmp(map->slots[idx].key, key) == 0) {
            if (out_val) *out_val = map->slots[idx].value;
            return true;
        }
        probe++;
        idx = (idx + probe) & (map->capacity - 1);
    }

    return false;
}

bool nyx_swiss_map_erase(NyxSwissMap* map, const char* key) {
    if (!map || !key) return false;

    uint64_t hash = hash_key(key);
    uint8_t tag = h2(hash);
    size_t idx = h1(hash, map->capacity);

    size_t probe = 0;
    while (map->ctrl[idx] != SWISS_CTRL_EMPTY) {
        if (map->ctrl[idx] == tag && strcmp(map->slots[idx].key, key) == 0) {
            map->ctrl[idx] = SWISS_CTRL_DELETED;
            map->slots[idx].key[0] = '\0';
            map->slots[idx].value = 0;
            map->size--;
            return true;
        }
        probe++;
        idx = (idx + probe) & (map->capacity - 1);
    }

    return false;
}

size_t nyx_swiss_map_size(const NyxSwissMap* map) {
    return map ? map->size : 0;
}
