#pragma once

#include <stddef.h>

typedef struct Arena Arena;

typedef enum
{
    ARENA_STATIC,
    ARENA_DYNAMIC,
    ARENA_GLOBAL
} ArenaType;

__attribute__((warn_unused_result))
Arena *create_arena(size_t size, ArenaType type);
void destroy_arena(Arena **arena);
void clear_arena(Arena *arena);
__attribute__((warn_unused_result))
void *alloc_arena(Arena *arena, size_t size);
void free_arena(Arena *arena, size_t size);

size_t memory_remaining_arena(const Arena *arena);
ArenaType get_type_arena(const Arena *arena);
