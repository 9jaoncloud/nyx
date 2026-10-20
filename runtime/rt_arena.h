#ifndef RT_ARENA_H
#define RT_ARENA_H
#include <stddef.h>
#include <stdint.h>
/* Inline helpers below use `bool`. This header previously relied on the
   including translation unit having included <stdbool.h> first, which made it
   uncompilable standalone (e.g. by tests/benchmarks that include it directly).
   Include it here so the header is self-contained. */
#include <stdbool.h>

#define NYX_CACHE_LINE_SIZE 64

typedef struct Arena Arena;

typedef struct ArenaCheckpoint {
    void* block_ptr;
    size_t used_offset;
} ArenaCheckpoint;

/* ── Nyx Fat Slice Invariant for Hardened C FFI ── */
typedef struct NyxFatSlice {
    void* ptr;
    size_t len;
    size_t elem_size;
} NyxFatSlice;

static inline bool nyx_slice_check_bounds(const NyxFatSlice* slice, size_t index) {
    return slice && slice->ptr && (index < slice->len);
}

static inline void* nyx_slice_at(const NyxFatSlice* slice, size_t index) {
    if (!nyx_slice_check_bounds(slice, index)) return NULL;
    return (void*)((uintptr_t)slice->ptr + (index * slice->elem_size));
}

Arena* arena_create(void);
void* arena_alloc(Arena* a, size_t size);
void* arena_alloc_aligned(Arena* a, size_t size, size_t alignment);
void* arena_alloc_pinned(Arena* a, size_t size);
void arena_pin(Arena* a);
void arena_unpin(Arena* a);
bool arena_is_pinned(Arena* a);
ArenaCheckpoint arena_checkpoint(Arena* a);
void arena_rewind(Arena* a, ArenaCheckpoint cp);
void arena_reset(Arena* a);
void arena_destroy(Arena* a);

// Software Prefetch Intrinsics (inspired by täkō hardware research)
void nyx_prefetch_read(const void* addr);
void nyx_prefetch_write(const void* addr);

#endif

