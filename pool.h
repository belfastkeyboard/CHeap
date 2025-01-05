#pragma once

#include <stddef.h>


typedef struct PoolAlloc PoolAlloc;


PoolAlloc *create_pool_allocator(size_t nmemb,
                                 size_t size);

void destroy_pool_allocator(PoolAlloc **pool);


__attribute__((warn_unused_result))
void *alloc_pool_allocator(PoolAlloc *pool);

__attribute__((warn_unused_result))
void *calloc_pool_allocator(PoolAlloc *pool);


void free_pool_allocator(PoolAlloc *pool,
                         void *ptr);

void clear_pool_allocator(PoolAlloc *pool);
