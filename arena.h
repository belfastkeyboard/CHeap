#pragma once

#include <stddef.h>

#define KB(x) (x * 1024)
#define MB(x) (KB(x) * 1024)
#define GB(x) (MB(x) * 1024)

typedef struct Arena Arena;


__attribute__((warn_unused_result))
Arena *create_arena(size_t nmemb,
                    size_t size);

void destroy_arena(Arena **arena);


__attribute__((warn_unused_result))
void *alloc_arena(Arena *arena,
                  size_t size);

__attribute__((warn_unused_result))
void *calloc_arena(Arena *arena,
                   size_t size);


void free_arena(Arena *arena,
                const void *ptr,
                size_t size);

void clear_arena(Arena *arena);
