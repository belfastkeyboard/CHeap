#pragma once

#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

#ifndef CHEAP_ARENA_AVAILABLE
#define CHEAP_ARENA_AVAILABLE
#endif

typedef struct Arena Arena;

ALLOC Arena *create_arena(size_t size);
void destroy_arena(Arena **arena);

ALLOC void *alloc_arena(Arena *arena, size_t size);
ALLOC void *calloc_arena(Arena *arena, size_t size);

void clear_arena(Arena *arena);
