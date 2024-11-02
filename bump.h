#pragma once

#include <stddef.h>

#define KB(x) (x * 1024)
#define MB(x) (KB(x) * 1024)
#define GB(x) (MB(x) * 1024)

typedef struct BumpAlloc BumpAlloc;

__attribute__((warn_unused_result))
BumpAlloc *create_bump_allocator(size_t size);
void destroy_bump_allocator(BumpAlloc **bump_allocator);

__attribute__((warn_unused_result))
void *bump_allocator_alloc(BumpAlloc *bump_allocator, size_t size);
__attribute__((warn_unused_result))
void *bump_allocator_calloc(BumpAlloc *bump_allocator, size_t size);
__attribute__((unused))
void *bump_allocator_salloc(BumpAlloc *bump_allocator, const char *string);

void bump_allocator_clear(BumpAlloc *bump_allocator);
