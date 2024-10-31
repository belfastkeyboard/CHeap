#pragma once

#include <stddef.h>

typedef struct Arena Arena;

typedef enum
{
    ARENA_STATIC,
    ARENA_DYNAMIC
} ArenaType;

__attribute__((warn_unused_result))
Arena *create_arena(size_t nmemb, size_t size, ArenaType type);
void destroy_arena(Arena **arena);

__attribute__((warn_unused_result))
void *alloc_arena(Arena *arena, size_t size);
__attribute__((warn_unused_result))
void *calloc_arena(Arena *arena, size_t size);

void free_arena(Arena *arena, void *ptr, size_t size);
void clear_arena(Arena *arena);

size_t memory_remaining_arena(const Arena *arena);
ArenaType get_type_arena(const Arena *arena);
