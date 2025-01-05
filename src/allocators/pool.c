#include <memory.h>
#include "../../internals/base.h"
#include "../../internals/cassert.h"
#include "../../pool.h"

struct FreeBlock
{
    struct FreeBlock *prev;
};

struct PoolAlloc
{
    struct FreeBlock *blocks;
    void *ptr;
    size_t offset;
    size_t nmemb;
    size_t size;
};


static size_t min_size(const size_t size)
{
    return (size > sizeof(struct FreeBlock)) ? size :
                                               sizeof(struct FreeBlock);
}


static void *pool_alloc(PoolAlloc *pool)
{
    void *ptr = NULL;

    if (pool->blocks)
    {
        ptr = pool->blocks;

        pool->blocks = pool->blocks->prev;
    }
    else if (pool->offset < pool->nmemb)
    {
        ptr = pool->ptr + pool->offset * pool->size;

        pool->offset++;
    }

    CHEAP_ASSERT(ptr,
                 "Pool allocator could not allocate memory.");

    return ptr;
}

static void *pool_c_alloc(PoolAlloc *pool)
{
    void *ptr = pool_alloc(pool);

    return memset(ptr,
                  0,
                  pool->size);
}


static void pool_free(PoolAlloc *pool,
                      void *ptr)
{
    if (ptr == pool->ptr + (pool->offset - 1) * pool->size)
    {
        pool->offset--;
    }
    else
    {
        struct FreeBlock *block = ptr;

        block->prev = pool->blocks;

        pool->blocks = block;
    }
}

static void pool_clear(PoolAlloc *pool)
{
    pool->blocks = NULL;
    pool->offset = 0;
}


PoolAlloc *create_pool_allocator(size_t nmemb,
                                 size_t size)
{
    const size_t n_size = min_size(size);

    void *memory = memory_allocate_container(sizeof(struct PoolAlloc) + n_size * nmemb);

    PoolAlloc *pool = memory;

    pool->blocks = NULL;
    pool->ptr = memory + sizeof(struct PoolAlloc);
    pool->offset = 0;
    pool->nmemb = nmemb;
    pool->size = n_size;

    return pool;
}

void destroy_pool_allocator(PoolAlloc **pool)
{
    memory_free_buffer((void**)pool);
}


__attribute__((warn_unused_result))
void *alloc_pool_allocator(PoolAlloc *pool)
{
    return pool_alloc(pool);
}

__attribute__((warn_unused_result))
void *calloc_pool_allocator(PoolAlloc *pool)
{
    return pool_c_alloc(pool);
}


void free_pool_allocator(PoolAlloc *pool,
                         void *ptr)
{
    pool_free(pool,
              ptr);
}

void clear_pool_allocator(PoolAlloc *pool)
{
    pool_clear(pool);
}
