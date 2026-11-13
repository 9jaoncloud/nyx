/*
 * rt_collections.h — Nyx Runtime Advanced Collections (PriorityQueue, Deque, HashSet, LRUCache)
 */

#ifndef RT_COLLECTIONS_H
#define RT_COLLECTIONS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── Priority Queue (Binary Heap) ────────────────────────────────────────── */
typedef struct {
    double priority;
    int64_t value;
    char tag[32];
} NyxHeapItem;

typedef struct {
    NyxHeapItem* items;
    size_t size;
    size_t capacity;
    bool is_max_heap;
} NyxPriorityQueue;

NyxPriorityQueue* nyx_pqueue_create(size_t initial_cap, bool is_max_heap);
void nyx_pqueue_destroy(NyxPriorityQueue* pq);
bool nyx_pqueue_push(NyxPriorityQueue* pq, double priority, int64_t value, const char* tag);
bool nyx_pqueue_pop(NyxPriorityQueue* pq, NyxHeapItem* out_item);
bool nyx_pqueue_peek(const NyxPriorityQueue* pq, NyxHeapItem* out_item);
size_t nyx_pqueue_size(const NyxPriorityQueue* pq);

/* ── Deque (Double-Ended Queue) ──────────────────────────────────────────── */
typedef struct {
    int64_t* buffer;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
} NyxDeque;

NyxDeque* nyx_deque_create(size_t capacity);
void nyx_deque_destroy(NyxDeque* dq);
bool nyx_deque_push_back(NyxDeque* dq, int64_t val);
bool nyx_deque_push_front(NyxDeque* dq, int64_t val);
bool nyx_deque_pop_front(NyxDeque* dq, int64_t* out_val);
bool nyx_deque_pop_back(NyxDeque* dq, int64_t* out_val);
size_t nyx_deque_size(const NyxDeque* dq);

/* ── LRU Cache (O(1) Get/Put) ────────────────────────────────────────────── */
typedef struct NyxLRUNode {
    char key[64];
    char value[128];
    struct NyxLRUNode* prev;
    struct NyxLRUNode* next;
} NyxLRUNode;

typedef struct {
    NyxLRUNode** table;
    size_t table_size;
    NyxLRUNode* head;
    NyxLRUNode* tail;
    size_t count;
    size_t capacity;
} NyxLRUCache;

NyxLRUCache* nyx_lru_create(size_t capacity);
void nyx_lru_destroy(NyxLRUCache* lru);
bool nyx_lru_put(NyxLRUCache* lru, const char* key, const char* value);
bool nyx_lru_get(NyxLRUCache* lru, const char* key, char* out_val, size_t buf_len);
size_t nyx_lru_size(const NyxLRUCache* lru);

#ifdef __cplusplus
}
#endif

#endif /* RT_COLLECTIONS_H */
