/*
 * rt_unique.c — Nyx Runtime Value Canonicalization & String Interning Implementation
 */

#include "rt_unique.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INTERN_TABLE_CAPACITY 4096

typedef struct InternNode {
    char* str;
    uint32_t hash;
    struct InternNode* next;
} InternNode;

static InternNode* g_intern_table[INTERN_TABLE_CAPACITY];
static size_t g_intern_count = 0;
static bool g_intern_initialized = false;

static uint32_t intern_hash(const char* str) {
    uint32_t hash = 2166136261u;
    while (*str) {
        hash ^= (uint8_t)*str++;
        hash *= 16777619u;
    }
    return hash;
}

void nyx_interner_init(void) {
    if (g_intern_initialized) return;
    memset(g_intern_table, 0, sizeof(g_intern_table));
    g_intern_count = 0;
    g_intern_initialized = true;
}

void nyx_interner_cleanup(void) {
    for (size_t i = 0; i < INTERN_TABLE_CAPACITY; i++) {
        InternNode* node = g_intern_table[i];
        while (node) {
            InternNode* next = node->next;
            if (node->str) free(node->str);
            free(node);
            node = next;
        }
        g_intern_table[i] = NULL;
    }
    g_intern_count = 0;
    g_intern_initialized = false;
}

NyxSymbol nyx_intern(const char* str) {
    if (!str) return NULL;
    if (!g_intern_initialized) nyx_interner_init();

    uint32_t h = intern_hash(str);
    size_t bucket = h % INTERN_TABLE_CAPACITY;

    InternNode* node = g_intern_table[bucket];
    while (node) {
        if (node->hash == h && strcmp(node->str, str) == 0) {
            return node->str; /* Found existing canonical pointer */
        }
        node = node->next;
    }

    /* Not found: allocate canonical instance */
    InternNode* new_node = (InternNode*)malloc(sizeof(InternNode));
    if (!new_node) return NULL;

    size_t len = strlen(str);
    new_node->str = (char*)malloc(len + 1);
    if (!new_node->str) {
        free(new_node);
        return NULL;
    }
    memcpy(new_node->str, str, len + 1);
    new_node->hash = h;
    new_node->next = g_intern_table[bucket];
    g_intern_table[bucket] = new_node;

    g_intern_count++;
    return new_node->str;
}

size_t nyx_interner_count(void) {
    return g_intern_count;
}
