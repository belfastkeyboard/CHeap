#pragma once

#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

typedef struct BumpAlloc BumpAlloc;

ALLOC BumpAlloc *create_bump_allocator(size_t size);
void destroy_bump_allocator(BumpAlloc **bump);

ALLOC void *bump_alloc(BumpAlloc *bump, size_t size);
ALLOC void *bump_calloc(BumpAlloc *bump, size_t size);
ALLOC void *bump_salloc(BumpAlloc *bump, const char *string);

void bump_clear(BumpAlloc *bump);
