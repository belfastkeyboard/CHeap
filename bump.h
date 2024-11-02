#pragma once

#include <stddef.h>

#define KB(x) (x * 1024)
#define MB(x) (KB(x) * 1024)
#define GB(x) (MB(x) * 1024)

typedef struct BumpAlloc BumpAlloc;

__attribute__((warn_unused_result))
BumpAlloc *create_bump_allocator(size_t size);
void destroy_bump_allocator(BumpAlloc **bump);

__attribute__((warn_unused_result))
void *bump_alloc(BumpAlloc *bump, size_t size);
__attribute__((warn_unused_result))
void *bump_calloc(BumpAlloc *bump, size_t size);
__attribute__((unused))
void *bump_salloc(BumpAlloc *bump, const char *string);

void bump_clear(BumpAlloc *bump);
