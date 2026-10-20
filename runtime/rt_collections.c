/*
 * rt_collections.c — Nyx Runtime Advanced Collections Implementation
 */

#include "rt_collections.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Priority Queue Implementation ───────────────────────────────────────── */
NyxPriorityQueue* nyx_pqueue_create(size_t initial_cap, bool is_max_heap) {
    NyxPriorityQueue* pq = (NyxPriorityQueue*)malloc(sizeof(NyxPriorityQueue));
    if (!pq) return NULL;
    pq->capacity = (initial_cap > 4) ? initial_cap : 16;
    pq->size = 0;
    pq->is_max_heap = is_max_heap;
    pq->items = (NyxHeapItem*)malloc(sizeof(NyxHeapItem) * pq->capacity);
    return pq;
}

void nyx_pqueue_destroy(NyxPriorityQueue* pq) {
    if (!pq) return;
    if (pq->items) free(pq->items);
    free(pq);
}

static bool compare_priority(double a, double b, bool is_max) {
    return is_max ? (a > b) : (a < b);
}

bool nyx_pqueue_push(NyxPriorityQueue* pq, double priority, int64_t value, const char* tag) {
    if (!pq) return false;
    if (pq->size >= pq->capacity) {
        size_t new_cap = pq->capacity * 2;
        NyxHeapItem* new_items = (NyxHeapItem*)realloc(pq->items, sizeof(NyxHeapItem) * new_cap);
        if (!new_items) return false;
        pq->items = new_items;
        pq->capacity = new_cap;
    }

    size_t i = pq->size++;
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (compare_priority(priority, pq->items[parent].priority, pq->is_max_heap)) {
            pq->items[i] = pq->items[parent];
            i = parent;
        } else {
            break;
        }
    }

    pq->items[i].priority = priority;
    pq->items[i].value = value;
    if (tag) strncpy(pq->items[i].tag, tag, sizeof(pq->items[i].tag) - 1);
    else pq->items[i].tag[0] = '\0';
    return true;
}

bool nyx_pqueue_pop(NyxPriorityQueue* pq, NyxHeapItem* out_item) {
    if (!pq || pq->size == 0) return false;
    if (out_item) *out_item = pq->items[0];

    NyxHeapItem last = pq->items[--pq->size];
    if (pq->size == 0) return true;

    size_t i = 0;
    while (i * 2 + 1 < pq->size) {
        size_t left = i * 2 + 1;
        size_t right = i * 2 + 2;
        size_t best = left;

        if (right < pq->size && compare_priority(pq->items[right].priority, pq->items[left].priority, pq->is_max_heap)) {
            best = right;
        }

        if (compare_priority(pq->items[best].priority, last.priority, pq->is_max_heap)) {
            pq->items[i] = pq->items[best];
            i = best;
        } else {
            break;
        }
    }
    pq->items[i] = last;
    return true;
}

bool nyx_pqueue_peek(const NyxPriorityQueue* pq, NyxHeapItem* out_item) {
    if (!pq || pq->size == 0) return false;
    if (out_item) *out_item = pq->items[0];
    return true;
}

size_t nyx_pqueue_size(const NyxPriorityQueue* pq) {
    return pq ? pq->size : 0;
}

/* ── Deque Implementation ────────────────────────────────────────────────── */
NyxDeque* nyx_deque_create(size_t capacity) {
    NyxDeque* dq = (NyxDeque*)malloc(sizeof(NyxDeque));
    if (!dq) return NULL;
    dq->capacity = (capacity > 4) ? capacity : 16;
    dq->buffer = (int64_t*)malloc(sizeof(int64_t) * dq->capacity);
    dq->head = 0;
    dq->tail = 0;
    dq->count = 0;
    return dq;
}

void nyx_deque_destroy(NyxDeque* dq) {
    if (!dq) return;
    if (dq->buffer) free(dq->buffer);
    free(dq);
}

bool nyx_deque_push_back(NyxDeque* dq, int64_t val) {
    if (!dq || dq->count >= dq->capacity) return false;
    dq->buffer[dq->tail] = val;
    dq->tail = (dq->tail + 1) % dq->capacity;
    dq->count++;
    return true;
}

bool nyx_deque_push_front(NyxDeque* dq, int64_t val) {
    if (!dq || dq->count >= dq->capacity) return false;
    dq->head = (dq->head + dq->capacity - 1) % dq->capacity;
    dq->buffer[dq->head] = val;
    dq->count++;
    return true;
}

bool nyx_deque_pop_front(NyxDeque* dq, int64_t* out_val) {
    if (!dq || dq->count == 0) return false;
    if (out_val) *out_val = dq->buffer[dq->head];
    dq->head = (dq->head + 1) % dq->capacity;
    dq->count--;
    return true;
}

bool nyx_deque_pop_back(NyxDeque* dq, int64_t* out_val) {
    if (!dq || dq->count == 0) return false;
    dq->tail = (dq->tail + dq->capacity - 1) % dq->capacity;
    if (out_val) *out_val = dq->buffer[dq->tail];
    dq->count--;
    return true;
}

size_t nyx_deque_size(const NyxDeque* dq) {
    return dq ? dq->count : 0;
}

/* ── LRU Cache Implementation ────────────────────────────────────────────── */
static uint32_t hash_str(const char* str) {
    uint32_t hash = 5381;
    int c;
    while ((c = *str++)) hash = ((hash << 5) + hash) + c;
    return hash;
}

NyxLRUCache* nyx_lru_create(size_t capacity) {
    NyxLRUCache* lru = (NyxLRUCache*)malloc(sizeof(NyxLRUCache));
    if (!lru) return NULL;
    lru->capacity = (capacity > 0) ? capacity : 16;
    lru->table_size = lru->capacity * 2;
    lru->table = (NyxLRUNode**)calloc(lru->table_size, sizeof(NyxLRUNode*));
    lru->head = NULL;
    lru->tail = NULL;
    lru->count = 0;
    return lru;
}

void nyx_lru_destroy(NyxLRUCache* lru) {
    if (!lru) return;
    NyxLRUNode* cur = lru->head;
    while (cur) {
        NyxLRUNode* next = cur->next;
        free(cur);
        cur = next;
    }
    if (lru->table) free(lru->table);
    free(lru);
}

static void detach_node(NyxLRUCache* lru, NyxLRUNode* node) {
    if (node->prev) node->prev->next = node->next;
    else lru->head = node->next;
    if (node->next) node->next->prev = node->prev;
    else lru->tail = node->prev;
    node->prev = node->next = NULL;
}

static void attach_front(NyxLRUCache* lru, NyxLRUNode* node) {
    node->next = lru->head;
    node->prev = NULL;
    if (lru->head) lru->head->prev = node;
    lru->head = node;
    if (!lru->tail) lru->tail = node;
}

bool nyx_lru_put(NyxLRUCache* lru, const char* key, const char* value) {
    if (!lru || !key || !value) return false;
    uint32_t idx = hash_str(key) % lru->table_size;

    NyxLRUNode* cur = lru->table[idx];
    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            strncpy(cur->value, value, sizeof(cur->value) - 1);
            detach_node(lru, cur);
            attach_front(lru, cur);
            return true;
        }
        cur = cur->next;
    }

    if (lru->count >= lru->capacity && lru->tail) {
        NyxLRUNode* victim = lru->tail;
        uint32_t v_idx = hash_str(victim->key) % lru->table_size;
        
        NyxLRUNode** p = &lru->table[v_idx];
        while (*p && *p != victim) p = &((*p)->next);
        if (*p) *p = victim->next;

        detach_node(lru, victim);
        free(victim);
        lru->count--;
    }

    NyxLRUNode* node = (NyxLRUNode*)malloc(sizeof(NyxLRUNode));
    if (!node) return false;
    strncpy(node->key, key, sizeof(node->key) - 1);
    strncpy(node->value, value, sizeof(node->value) - 1);
    node->prev = node->next = NULL;

    node->next = lru->table[idx];
    lru->table[idx] = node;

    attach_front(lru, node);
    lru->count++;
    return true;
}

bool nyx_lru_get(NyxLRUCache* lru, const char* key, char* out_val, size_t buf_len) {
    if (!lru || !key) return false;
    uint32_t idx = hash_str(key) % lru->table_size;

    NyxLRUNode* cur = lru->table[idx];
    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            if (out_val && buf_len > 0) {
                strncpy(out_val, cur->value, buf_len - 1);
                out_val[buf_len - 1] = '\0';
            }
            detach_node(lru, cur);
            attach_front(lru, cur);
            return true;
        }
        cur = cur->next;
    }
    return false;
}

size_t nyx_lru_size(const NyxLRUCache* lru) {
    return lru ? lru->count : 0;
}
