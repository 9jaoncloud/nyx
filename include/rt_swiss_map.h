/*
 * rt_swiss_map.h — Nyx High-Performance Swiss Tables Hash Map (SIMD Metadata Probing)
 *
 * Implements Google Abseil/SwissTable-style 16-byte metadata control bytes
 * with fast quadratic probing and high load factor capacity (>87.5%).
 */

#ifndef RT_SWISS_MAP_H
#define RT_SWISS_MAP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SWISS_CTRL_EMPTY 0x80
#define SWISS_CTRL_DELETED 0xFE
#define SWISS_CTRL_SENTINEL 0xFF

typedef struct {
    char key[64];
    int64_t value;
} NyxSwissSlot;

typedef struct {
    uint8_t* ctrl;         /* Control byte array: high 7 bits of hash */
    NyxSwissSlot* slots;   /* Key-value slots */
    size_t capacity;       /* Always power of 2 */
    size_t size;           /* Number of live elements */
    size_t growth_left;    /* Remaining slots before rehash */
} NyxSwissMap;

NyxSwissMap* nyx_swiss_map_create(size_t initial_cap);
void nyx_swiss_map_destroy(NyxSwissMap* map);
bool nyx_swiss_map_insert(NyxSwissMap* map, const char* key, int64_t value);
bool nyx_swiss_map_find(const NyxSwissMap* map, const char* key, int64_t* out_val);
bool nyx_swiss_map_erase(NyxSwissMap* map, const char* key);
size_t nyx_swiss_map_size(const NyxSwissMap* map);

#ifdef __cplusplus
}
#endif

#endif /* RT_SWISS_MAP_H */
