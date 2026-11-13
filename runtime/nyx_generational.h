#ifndef NYX_GENERATIONAL_H
#define NYX_GENERATIONAL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// NYX OPTIONAL GENERATIONAL POINTERS (@gen &T / nyx_gen_ptr_t)
// ============================================================================
// For unbounded long-lived dynamic caches where static lifetimes cannot be
// inferred at compile time, @gen provides safe use-after-free protection
// without falling back to a tracing garbage collector.

// 16-byte Fat Reference (target pointer + expected generation)
typedef struct {
    void* target;
    uint32_t expected_gen;
    size_t size;
} nyx_gen_ptr_t;

// Per-allocation metadata header (placed 4 bytes before target pointer)
typedef struct {
    uint32_t generation;
} nyx_gen_header_t;

// Allocate memory with an initial generation counter
nyx_gen_ptr_t nyx_gen_alloc(size_t size);

// Dereference with 1 CPU cycle generation validation; halts on use-after-free
void* nyx_gen_deref(nyx_gen_ptr_t ref);

// Try dereferencing without aborting (returns NULL if generation is stale/freed)
void* nyx_gen_try_deref(nyx_gen_ptr_t ref);

// Free generational memory and increment generation counter to invalidate all aliases
void nyx_gen_free(nyx_gen_ptr_t* ref);

// ============================================================================
// DYNAMIC GENERATIONAL CACHE
// ============================================================================
typedef struct {
    nyx_gen_ptr_t* entries;
    size_t capacity;
    size_t count;
} nyx_gen_cache_t;

nyx_gen_cache_t* nyx_gen_cache_create(size_t capacity);
void nyx_gen_cache_put(nyx_gen_cache_t* cache, size_t index, nyx_gen_ptr_t item);
nyx_gen_ptr_t nyx_gen_cache_get(nyx_gen_cache_t* cache, size_t index);
void nyx_gen_cache_destroy(nyx_gen_cache_t* cache);

#ifdef __cplusplus
}
#endif

#endif // NYX_GENERATIONAL_H
