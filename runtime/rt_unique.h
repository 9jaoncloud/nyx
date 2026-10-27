/*
 * rt_unique.h — Nyx Runtime Value Canonicalization & String Interning
 *
 * Interns immutable strings & symbols into a single canonical instance.
 * Allows instant O(1) pointer equality (ptrA == ptrB) instead of O(N) strcmp.
 */

#ifndef RT_UNIQUE_H
#define RT_UNIQUE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef const char* NyxSymbol;

/* Initializes the global string interner table */
void nyx_interner_init(void);

/* Shuts down and frees the global string interner table */
void nyx_interner_cleanup(void);

/* Interns a string, returning a canonical immortal pointer */
NyxSymbol nyx_intern(const char* str);

/* Instant O(1) equality check between two interned symbols */
static inline bool nyx_symbol_eq(NyxSymbol a, NyxSymbol b) {
    return a == b;
}

/* Returns the total number of unique interned symbols */
size_t nyx_interner_count(void);

#ifdef __cplusplus
}
#endif

#endif /* RT_UNIQUE_H */
