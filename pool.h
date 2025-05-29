#pragma once

#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

typedef struct PoolAlloc PoolAlloc;

ALLOC PoolAlloc *create_pool_allocator(size_t nmemb, size_t size);
void             destroy_pool_allocator(PoolAlloc **pool);

ALLOC void *alloc_pool_allocator(PoolAlloc *pool);
ALLOC void *calloc_pool_allocator(PoolAlloc *pool);

void free_pool_allocator(PoolAlloc *pool, void *ptr);
void clear_pool_allocator(PoolAlloc *pool);
