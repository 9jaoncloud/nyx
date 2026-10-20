#include "rt_arena.h"
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 4096

typedef struct Block {
    char* data;
    size_t used;
    struct Block* next;
} Block;

struct Arena {
    Block* first;
    Block* current;
    uint32_t pin_count; /* @pinned reference locks */
};

Arena* arena_create(void) {
    Arena* a = (Arena*)malloc(sizeof(Arena));
    a->first = (Block*)malloc(sizeof(Block));
    a->first->data = (char*)malloc(BLOCK_SIZE);
    a->first->used = 0;
    a->first->next = NULL;
    a->current = a->first;
    a->pin_count = 0;
    return a;
}

void* arena_alloc(Arena* a, size_t size) {
    if (!a) return NULL;
    Block* b = a->current;
    if (b->used + size > BLOCK_SIZE) {
        Block* new_block = (Block*)malloc(sizeof(Block));
        new_block->data = (char*)malloc(BLOCK_SIZE);
        new_block->used = 0;
        new_block->next = NULL;
        b->next = new_block;
        b = new_block;
        a->current = b;
    }
    void* ptr = b->data + b->used;
    b->used += size;
    return ptr;
}

void* arena_alloc_aligned(Arena* a, size_t size, size_t alignment) {
    if (!a) return NULL;
    if (alignment == 0) alignment = 8;
    Block* b = a->current;
    
    // Calculate aligned offset
    uintptr_t current_ptr = (uintptr_t)(b->data + b->used);
    size_t padding = (alignment - (current_ptr % alignment)) % alignment;
    
    if (b->used + padding + size > BLOCK_SIZE) {
        Block* new_block = (Block*)malloc(sizeof(Block));
        new_block->data = (char*)malloc(BLOCK_SIZE);
        new_block->used = 0;
        new_block->next = NULL;
        b->next = new_block;
        b = new_block;
        a->current = b;
        
        current_ptr = (uintptr_t)b->data;
        padding = (alignment - (current_ptr % alignment)) % alignment;
    }
    
    b->used += padding;
    void* ptr = b->data + b->used;
    b->used += size;
    return ptr;
}

void arena_pin(Arena* a) {
    if (a) a->pin_count++;
}

void arena_unpin(Arena* a) {
    if (a && a->pin_count > 0) a->pin_count--;
}

bool arena_is_pinned(Arena* a) {
    return a && (a->pin_count > 0);
}

void* arena_alloc_pinned(Arena* a, size_t size) {
    if (!a) return NULL;
    void* ptr = arena_alloc_aligned(a, size, 64);
    arena_pin(a);
    return ptr;
}

ArenaCheckpoint arena_checkpoint(Arena* a) {
    ArenaCheckpoint cp;
    cp.block_ptr = a ? (void*)a->current : NULL;
    cp.used_offset = a && a->current ? a->current->used : 0;
    return cp;
}

void arena_rewind(Arena* a, ArenaCheckpoint cp) {
    if (!a || arena_is_pinned(a)) {
        // Pinned arena buffers cannot be rewound while foreign references are active
        return;
    }
    if (cp.block_ptr != NULL) {
        a->current = (Block*)cp.block_ptr;
        a->current->used = cp.used_offset;
    }
}

void arena_reset(Arena* a) {
    if (!a || arena_is_pinned(a)) {
        // Pinned arena buffers cannot be wiped while foreign references are active
        return;
    }
    Block* b = a->first;
    while (b) {
        b->used = 0;
        b = b->next;
    }
    a->current = a->first;
}

void arena_destroy(Arena* a) {
    Block* b = a->first;
    while (b) {
        Block* next = b->next;
        free(b->data);
        free(b);
        b = next;
    }
    free(a);
}

void nyx_prefetch_read(const void* addr) {
#if defined(__GNUC__) || defined(__clang__)
    __builtin_prefetch(addr, 0, 3);
#elif defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
    _mm_prefetch((const char*)addr, _MM_HINT_T0);
#else
    (void)addr;
#endif
}

void nyx_prefetch_write(const void* addr) {
#if defined(__GNUC__) || defined(__clang__)
    __builtin_prefetch(addr, 1, 3);
#elif defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
    _mm_prefetch((const char*)addr, _MM_HINT_T0);
#else
    (void)addr;
#endif
}

