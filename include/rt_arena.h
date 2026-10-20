#ifndef RT_ARENA_H
#define RT_ARENA_H
#include <stddef.h>
typedef struct Arena Arena;
Arena* arena_create(void);
void* arena_alloc(Arena* a, size_t size);
void arena_destroy(Arena* a);
#endif
