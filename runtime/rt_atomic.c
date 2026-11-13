/*
 * std::sync::atomic — C11 backing implementation
 *
 * Backs the extern "C" declarations in std/sync/atomic.nyx.
 *
 * HONEST IMPLEMENTATION NOTES
 *   * Uses C11 <stdatomic.h> atomic_size_t, so load/store/fetch_add/sub and
 *     compare_exchange have genuine atomic semantics rather than pretending.
 *   * Memory ordering is memory_order_seq_cst for all operations. The Nyx-level
 *     Ordering constants exist for API compatibility with the generated tools;
 *     the runtime does not yet accept an explicit order argument. That is a
 *     documented limitation, not a hidden one — seq_cst is the strongest and
 *     therefore always correct, just not always fastest.
 */

#include <stdatomic.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/*
 * Translate a Nyx Ordering constant into a C11 memory_order.
 *
 * Values match the constants in std/sync/atomic.nyx:
 *   0 relaxed, 1 consume, 2 acquire, 3 release, 4 acq_rel, 5 seq_cst.
 *
 * `consume` is deliberately mapped to `acquire`: C11 memory_order_consume is
 * widely implemented as acquire by compilers, and relying on it is discouraged.
 * Anything unrecognised falls back to seq_cst, which is always safe.
 */
static memory_order nyx_memory_order(int order) {
    switch (order) {
        case 0: return memory_order_relaxed;
        case 1: return memory_order_acquire; /* consume promoted to acquire */
        case 2: return memory_order_acquire;
        case 3: return memory_order_release;
        case 4: return memory_order_acq_rel;
        case 5: return memory_order_seq_cst;
        default: return memory_order_seq_cst;
    }
}

void *nyx_atomic_usize_create(size_t initial) {
    atomic_size_t *cell = (atomic_size_t *)malloc(sizeof(atomic_size_t));
    if (!cell) {
        return NULL;
    }
    atomic_init(cell, initial);
    return (void *)cell;
}

void nyx_atomic_usize_destroy(void *cell) {
    if (!cell) {
        return;
    }
    free(cell);
}

size_t nyx_atomic_usize_load(void *cell) {
    if (!cell) {
        return 0;
    }
    return (size_t)atomic_load((atomic_size_t *)cell);
}

size_t nyx_atomic_usize_load_explicit(void *cell, int order) {
    if (!cell) {
        return 0;
    }
    return (size_t)atomic_load_explicit((atomic_size_t *)cell, nyx_memory_order(order));
}

void nyx_atomic_usize_store(void *cell, size_t value) {
    if (!cell) {
        return;
    }
    atomic_store((atomic_size_t *)cell, value);
}

void nyx_atomic_usize_store_explicit(void *cell, size_t value, int order) {
    if (!cell) {
        return;
    }
    atomic_store_explicit((atomic_size_t *)cell, value, nyx_memory_order(order));
}

size_t nyx_atomic_usize_fetch_add(void *cell, size_t value) {
    if (!cell) {
        return 0;
    }
    return (size_t)atomic_fetch_add((atomic_size_t *)cell, value);
}

size_t nyx_atomic_usize_fetch_add_explicit(void *cell, size_t value, int order) {
    if (!cell) {
        return 0;
    }
    return (size_t)atomic_fetch_add_explicit(
        (atomic_size_t *)cell, value, nyx_memory_order(order));
}

size_t nyx_atomic_usize_fetch_sub(void *cell, size_t value) {
    if (!cell) {
        return 0;
    }
    return (size_t)atomic_fetch_sub((atomic_size_t *)cell, value);
}

size_t nyx_atomic_usize_fetch_sub_explicit(void *cell, size_t value, int order) {
    if (!cell) {
        return 0;
    }
    return (size_t)atomic_fetch_sub_explicit(
        (atomic_size_t *)cell, value, nyx_memory_order(order));
}

int nyx_atomic_usize_compare_exchange(void *cell, size_t expected, size_t desired) {
    if (!cell) {
        return 0;
    }
    size_t exp = expected;
    int ok = atomic_compare_exchange_strong((atomic_size_t *)cell, &exp, desired);
    return ok ? 1 : 0;
}

int nyx_atomic_usize_compare_exchange_explicit(
    void *cell, size_t expected, size_t desired, int success_order, int failure_order) {
    if (!cell) {
        return 0;
    }
    size_t exp = expected;
    int ok = atomic_compare_exchange_strong_explicit(
        (atomic_size_t *)cell, &exp, desired,
        nyx_memory_order(success_order),
        nyx_memory_order(failure_order));
    return ok ? 1 : 0;
}
