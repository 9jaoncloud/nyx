/*
 * std::sync::atomic — prototypes for the C11 atomic runtime.
 *
 * Included by generated programs so calls to nyx_atomic_* are declared
 * correctly instead of triggering "implicit declaration of function".
 *
 * The `order` arguments use the following encoding, matching std/sync/atomic.nyx:
 *   0 relaxed, 1 consume, 2 acquire, 3 release, 4 acq_rel, 5 seq_cst
 * The runtime translates these to C11 memory_order values.
 */

#ifndef NYX_RT_ATOMIC_H
#define NYX_RT_ATOMIC_H

#include <stddef.h>

void *nyx_atomic_usize_create(size_t initial);
void nyx_atomic_usize_destroy(void *cell);

size_t nyx_atomic_usize_load(void *cell);
void nyx_atomic_usize_store(void *cell, size_t value);
size_t nyx_atomic_usize_fetch_add(void *cell, size_t value);
size_t nyx_atomic_usize_fetch_sub(void *cell, size_t value);
int nyx_atomic_usize_compare_exchange(void *cell, size_t expected, size_t desired);

size_t nyx_atomic_usize_load_explicit(void *cell, int order);
void nyx_atomic_usize_store_explicit(void *cell, size_t value, int order);
size_t nyx_atomic_usize_fetch_add_explicit(void *cell, size_t value, int order);
size_t nyx_atomic_usize_fetch_sub_explicit(void *cell, size_t value, int order);
int nyx_atomic_usize_compare_exchange_explicit(
    void *cell, size_t expected, size_t desired, int success_order, int failure_order);

#endif /* NYX_RT_ATOMIC_H */
