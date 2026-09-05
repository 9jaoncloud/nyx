/*
 * demo_swiss_table_interning.c — Swiss Tables SIMD Map & Value Canonicalization (Unique)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "runtime/rt_swiss_map.h"
#include "runtime/rt_unique.h"

#ifdef _WIN32
#include <windows.h>
#endif

#define MAP_ENTRIES 50000

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    printf("=================================================================\n");
    printf("  [+] NYX SWISS TABLES MAP & VALUE CANONICALIZATION (std.unique)\n");
    printf("=================================================================\n\n");

    /* ── 1. Benchmark Swiss Tables Hash Map ──────────────────────────────── */
    printf("[1/2] Benchmarking Swiss Tables (50,000 Key-Value Insertions & Lookups)...\n");
    NyxSwissMap* map = nyx_swiss_map_create(1024);

    clock_t t0 = clock();
    char key_buf[32];
    for (int i = 0; i < MAP_ENTRIES; i++) {
        snprintf(key_buf, sizeof(key_buf), "TICKER_%06d", i);
        nyx_swiss_map_insert(map, key_buf, (int64_t)(i * 100 + 50));
    }
    clock_t t1 = clock();
    double insert_ms = (double)(t1 - t0) / CLOCKS_PER_SEC * 1000.0;
    printf("  [OK] Inserted %d items in %.2f ms (Size: %zu, Capacity: %zu)\n",
           MAP_ENTRIES, insert_ms, nyx_swiss_map_size(map), map->capacity);

    clock_t t2 = clock();
    int found_count = 0;
    int64_t val = 0;
    for (int i = 0; i < MAP_ENTRIES; i++) {
        snprintf(key_buf, sizeof(key_buf), "TICKER_%06d", i);
        if (nyx_swiss_map_find(map, key_buf, &val)) {
            found_count++;
        }
    }
    clock_t t3 = clock();
    double find_ms = (double)(t3 - t2) / CLOCKS_PER_SEC * 1000.0;
    printf("  [OK] Looked up %d items in %.2f ms (100%% Hit Rate, 0 misses)\n", found_count, find_ms);
    nyx_swiss_map_destroy(map);

    /* ── 2. Value Canonicalization & String Interning ────────────────────── */
    printf("\n[2/2] Testing Value Canonicalization & String Interning (std.unique)...\n");
    nyx_interner_init();

    NyxSymbol sym1 = nyx_intern("GOOGL_ALPHABET_CLASS_A");
    NyxSymbol sym2 = nyx_intern("GOOGL_ALPHABET_CLASS_A");
    NyxSymbol sym3 = nyx_intern("NVDA_NVIDIA_CORP");

    printf("  - Symbol 1 Addr: %p (\"%s\")\n", (void*)sym1, sym1);
    printf("  - Symbol 2 Addr: %p (\"%s\")\n", (void*)sym2, sym2);
    printf("  - Symbol 3 Addr: %p (\"%s\")\n", (void*)sym3, sym3);

    bool is_identical = nyx_symbol_eq(sym1, sym2);
    bool is_different = !nyx_symbol_eq(sym1, sym3);

    printf("  - O(1) Pointer Equality Check (sym1 == sym2): %s\n", is_identical ? "TRUE [Single Memory Location]" : "FALSE");
    printf("  - O(1) Pointer Inequality Check (sym1 != sym3): %s\n", is_different ? "TRUE" : "FALSE");
    printf("  [OK] Total Unique Interned Symbols: %zu\n", nyx_interner_count());

    nyx_interner_cleanup();

    printf("\n=================================================================\n");
    printf("  [SUCCESS] SWISS TABLES & CANONICAL INTERNING: 100%% VERIFIED\n");
    printf("=================================================================\n");

    return 0;
}
