#include "../include/nyx_generational.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NYX_GEN_SLAB_SLOT_SIZE 256
#define NYX_GEN_SLAB_CAPACITY  4096

typedef struct NyxGenSlot {
    uint32_t generation;
    bool is_allocated;
    size_t size;
    struct NyxGenSlot* next_free;
    char data[NYX_GEN_SLAB_SLOT_SIZE];
} NyxGenSlot;

typedef struct {
    NyxGenSlot slots[NYX_GEN_SLAB_CAPACITY];
    NyxGenSlot* free_list;
    size_t next_virgin;
} NyxGenSlab;

static NyxGenSlab g_slab = { .free_list = NULL, .next_virgin = 0 };
static bool g_slab_initialized = false;

static void nyx_gen_init_slab(void) {
    if (g_slab_initialized) return;
    for (size_t i = 0; i < NYX_GEN_SLAB_CAPACITY; i++) {
        g_slab.slots[i].generation = 1;
        g_slab.slots[i].is_allocated = false;
        g_slab.slots[i].size = 0;
        g_slab.slots[i].next_free = NULL;
    }
    g_slab.free_list = NULL;
    g_slab.next_virgin = 0;
    g_slab_initialized = true;
}

nyx_gen_ptr_t nyx_gen_alloc(size_t size) {
    nyx_gen_init_slab();
    if (size > NYX_GEN_SLAB_SLOT_SIZE) {
        fprintf(stderr, "[Nyx Generational Error] Allocation size %zu exceeds generational slab slot size %d\n",
                size, NYX_GEN_SLAB_SLOT_SIZE);
        abort();
    }

    NyxGenSlot* slot = NULL;
    if (g_slab.free_list) {
        slot = g_slab.free_list;
        g_slab.free_list = slot->next_free;
    } else if (g_slab.next_virgin < NYX_GEN_SLAB_CAPACITY) {
        slot = &g_slab.slots[g_slab.next_virgin++];
    } else {
        fprintf(stderr, "[Nyx Generational Error] Generational slab capacity exhausted (%d slots)\n",
                NYX_GEN_SLAB_CAPACITY);
        abort();
    }

    slot->is_allocated = true;
    slot->size = size;
    slot->next_free = NULL;

    nyx_gen_ptr_t ptr;
    ptr.target = slot->data;
    ptr.expected_gen = slot->generation;
    ptr.size = size;
    return ptr;
}

static inline NyxGenSlot* nyx_gen_get_slot(void* target) {
    if (!target) return NULL;
    return (NyxGenSlot*)((char*)target - offsetof(NyxGenSlot, data));
}

void* nyx_gen_deref(nyx_gen_ptr_t ref) {
    if (!ref.target) {
        fprintf(stderr, "[Nyx Generational Panic G001] Null pointer dereference in @gen pointer\n");
        abort();
    }

    NyxGenSlot* slot = nyx_gen_get_slot(ref.target);
    if (!slot->is_allocated || slot->generation != ref.expected_gen) {
        fprintf(stderr, "[Nyx Generational Panic G002] Use-After-Free detected! Expected gen %u, current gen %u (slot is %s)\n",
                ref.expected_gen, slot->generation, slot->is_allocated ? "reallocated" : "freed");
        abort();
    }

    return ref.target;
}

void* nyx_gen_try_deref(nyx_gen_ptr_t ref) {
    if (!ref.target) return NULL;
    NyxGenSlot* slot = nyx_gen_get_slot(ref.target);
    if (!slot->is_allocated || slot->generation != ref.expected_gen) {
        return NULL; // Stale or freed reference
    }
    return ref.target;
}

void nyx_gen_free(nyx_gen_ptr_t* ref) {
    if (!ref || !ref->target) return;

    NyxGenSlot* slot = nyx_gen_get_slot(ref->target);
    if (slot->is_allocated && slot->generation == ref->expected_gen) {
        slot->generation++; // Invalidate all aliases
        slot->is_allocated = false;
        slot->size = 0;
        slot->next_free = g_slab.free_list;
        g_slab.free_list = slot;
    }

    ref->target = NULL;
    ref->expected_gen = 0;
    ref->size = 0;
}

nyx_gen_cache_t* nyx_gen_cache_create(size_t capacity) {
    nyx_gen_cache_t* cache = (nyx_gen_cache_t*)malloc(sizeof(nyx_gen_cache_t));
    if (!cache) return NULL;
    cache->capacity = capacity > 0 ? capacity : 16;
    cache->count = 0;
    cache->entries = (nyx_gen_ptr_t*)calloc(cache->capacity, sizeof(nyx_gen_ptr_t));
    return cache;
}

void nyx_gen_cache_put(nyx_gen_cache_t* cache, size_t index, nyx_gen_ptr_t item) {
    if (!cache) return;
    if (index >= cache->capacity) {
        size_t new_cap = (index + 1) * 2;
        nyx_gen_ptr_t* new_entries = (nyx_gen_ptr_t*)realloc(cache->entries, new_cap * sizeof(nyx_gen_ptr_t));
        if (!new_entries) return;
        memset(new_entries + cache->capacity, 0, (new_cap - cache->capacity) * sizeof(nyx_gen_ptr_t));
        cache->entries = new_entries;
        cache->capacity = new_cap;
    }
    cache->entries[index] = item;
    if (index >= cache->count) {
        cache->count = index + 1;
    }
}

nyx_gen_ptr_t nyx_gen_cache_get(nyx_gen_cache_t* cache, size_t index) {
    nyx_gen_ptr_t empty = { NULL, 0, 0 };
    if (!cache || index >= cache->capacity) return empty;
    return cache->entries[index];
}

void nyx_gen_cache_destroy(nyx_gen_cache_t* cache) {
    if (!cache) return;
    if (cache->entries) {
        free(cache->entries);
    }
    free(cache);
}
