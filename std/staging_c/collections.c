#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "runtime/rt_arena.h"
#include "runtime/rt_string.h"
#include "runtime/rt_io.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_option.h"
#include "runtime/rt_map.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_async.h"
#include "runtime/rt_json.h"
#include "runtime/rt_db.h"
#include "runtime/rt_http.h"
#include "runtime/rt_net.h"
#include "runtime/rt_crypto.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_time.h"
#include "runtime/rt_nursery.h"
#include "runtime/rt_collections.h"
#include "runtime/rt_math.h"
#include "runtime/rt_format.h"
#include "runtime/rt_thread.h"
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#ifndef NYX_TUPLE_DEFS
#define NYX_TUPLE_DEFS
typedef struct { void* f0; void* f1; } NyxTuple2;
typedef struct { void* f0; void* f1; void* f2; } NyxTuple3;
typedef struct { void* f0; void* f1; void* f2; void* f3; } NyxTuple4;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; } NyxTuple5;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; } NyxTuple6;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; } NyxTuple7;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; void* f7; } NyxTuple8;
#endif

typedef struct {
    uint8_t* raw;
} PriorityQueue;

typedef struct {
    uint8_t* raw;
} Deque;

typedef struct {
    uint8_t* raw;
} LRUCache;

typedef struct {
    NyxVec items;
    int64_t chunk_index;
} ChunkEntry;

typedef struct {
    NyxVec items;
    int64_t cursor;
} LazyCollection;

PriorityQueue PriorityQueue_max_heap(int64_t);
PriorityQueue PriorityQueue_min_heap(int64_t);
int PriorityQueue_push(PriorityQueue*, double, int64_t, rt_string_t);
int64_t PriorityQueue_len(PriorityQueue*);
Deque Deque_new(int64_t);
int Deque_push_back(Deque*, int64_t);
int Deque_push_front(Deque*, int64_t);
LRUCache LRUCache_new(int64_t);
int LRUCache_put(LRUCache*, rt_string_t, rt_string_t);
NyxOption LRUCache_get(LRUCache*, rt_string_t);
LazyCollection LazyCollection_from_vec(NyxVec);
int64_t LazyCollection_len(LazyCollection*);
int LazyCollection_is_empty(LazyCollection*);
NyxVec LazyCollection_chunk_by(LazyCollection*, void* (*)(void*));
NyxVec LazyCollection_chunk_while(LazyCollection*, int (*)(void*, void*));
void LazyCollection_each_chunk(LazyCollection*, void* (*)(void*), void (*)(NyxVec*));
// module collections
// import prelude::*
// extern nyx_pqueue_create
// extern nyx_pqueue_destroy
// extern nyx_pqueue_push
// extern nyx_pqueue_pop
// extern nyx_pqueue_size
// extern nyx_deque_create
// extern nyx_deque_destroy
// extern nyx_deque_push_back
// extern nyx_deque_push_front
// extern nyx_deque_pop_front
// extern nyx_deque_pop_back
// extern nyx_lru_create
// extern nyx_lru_destroy
// extern nyx_lru_put
// extern nyx_lru_get
PriorityQueue PriorityQueue_max_heap(int64_t capacity) {
return (PriorityQueue){.raw = nyx_pqueue_create(capacity, 1)};
}

PriorityQueue PriorityQueue_min_heap(int64_t capacity) {
return (PriorityQueue){.raw = nyx_pqueue_create(capacity, 0)};
}

int PriorityQueue_push(PriorityQueue* self, double priority, int64_t val, rt_string_t tag) {
return nyx_pqueue_push((*self).raw, priority, val, ((const uint8_t*)(tag.data)));
}

int64_t PriorityQueue_len(PriorityQueue* self) {
return nyx_pqueue_size((*self).raw);
}

Deque Deque_new(int64_t capacity) {
return (Deque){.raw = nyx_deque_create(capacity)};
}

int Deque_push_back(Deque* self, int64_t val) {
return nyx_deque_push_back((*self).raw, val);
}

int Deque_push_front(Deque* self, int64_t val) {
return nyx_deque_push_front((*self).raw, val);
}

LRUCache LRUCache_new(int64_t capacity) {
return (LRUCache){.raw = nyx_lru_create(capacity)};
}

int LRUCache_put(LRUCache* self, rt_string_t key, rt_string_t val) {
return nyx_lru_put((*self).raw, ((const uint8_t*)(key.data)), ((const uint8_t*)(val.data)));
}

NyxOption LRUCache_get(LRUCache* self, rt_string_t key) {
int64_t buf = int64_t_with_capacity(String, 128LL);
if (nyx_lru_get((*self).raw, ((const uint8_t*)(key.data)), int64_t_as_mut_ptr(buf), 128LL)) {
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.int_val = (int64_t)(buf); _tmp; });
} else {
return rt_option_none();
}
}

LazyCollection LazyCollection_from_vec(NyxVec items) {
return (LazyCollection){.items = items, .cursor = 0LL};
}

int64_t LazyCollection_len(LazyCollection* self) {
return (*self).items.len;
}

int LazyCollection_is_empty(LazyCollection* self) {
return ((*self).items.len == 0LL);
}

NyxVec LazyCollection_chunk_by(LazyCollection* self, void* key_fn) {
NyxVec chunks = rt_vec_new(sizeof(int64_t));
({ if (((*self).items.len == 0LL)) {
return chunks;
}
});
NyxVec current_chunk = rt_vec_new(sizeof(int64_t));
void* current_key = key_fn((&((*(int64_t*)rt_vec_get((*self).items, 0LL)))));
rt_vec_push(&current_chunk, (void*)((*(int64_t*)rt_vec_get((*self).items, 0LL))));
int64_t i = 1LL;
while ((i < (*self).items.len)) {
void* next_key = key_fn((&((*(int64_t*)rt_vec_get((*self).items, i)))));
({ void* __if_res_3; if ((next_key == current_key)) {
rt_vec_push(&current_chunk, (void*)((*(int64_t*)rt_vec_get((*self).items, i))));
} else {
rt_vec_push(&chunks, ({ NyxVec _tmp_4 = (int64_t)(current_chunk); &_tmp_4; }));
current_chunk = rt_vec_new(sizeof(int64_t));
rt_vec_push(&current_chunk, (void*)((*(int64_t*)rt_vec_get((*self).items, i))));
__if_res_3 = (current_key = next_key);
}
__if_res_3; });
i = (i + 1LL);
}
;
({ if ((current_chunk.len > 0LL)) {
rt_vec_push(&chunks, ({ NyxVec _tmp_7 = (int64_t)(current_chunk); &_tmp_7; }));
}
});
return chunks;
}

NyxVec LazyCollection_chunk_while(LazyCollection* self, void* predicate) {
NyxVec chunks = rt_vec_new(sizeof(int64_t));
({ if (((*self).items.len == 0LL)) {
return chunks;
}
});
NyxVec current_chunk = rt_vec_new(sizeof(int64_t));
rt_vec_push(&current_chunk, (void*)((*(int64_t*)rt_vec_get((*self).items, 0LL))));
int64_t i = 1LL;
while ((i < (*self).items.len)) {
({ NyxVec __if_res_11; if (predicate((&((*(int64_t*)rt_vec_get((*self).items, (i - 1LL))))), (&((*(int64_t*)rt_vec_get((*self).items, i)))))) {
rt_vec_push(&current_chunk, (void*)((*(int64_t*)rt_vec_get((*self).items, i))));
} else {
rt_vec_push(&chunks, ({ NyxVec _tmp_12 = (int64_t)(current_chunk); &_tmp_12; }));
current_chunk = rt_vec_new(sizeof(int64_t));
rt_vec_push(&current_chunk, (void*)((*(int64_t*)rt_vec_get((*self).items, i))));
}
__if_res_11; });
i = (i + 1LL);
}
;
({ if ((current_chunk.len > 0LL)) {
rt_vec_push(&chunks, ({ NyxVec _tmp_15 = (int64_t)(current_chunk); &_tmp_15; }));
}
});
return chunks;
}

void LazyCollection_each_chunk(LazyCollection* self, void* key_fn, void* handler) {
int64_t chunks = chunk_by(self, key_fn);
int64_t c = 0LL;
while ((c < chunks.len)) {
handler((&(chunks[c])));
c = (c + 1LL);
}
;
}

