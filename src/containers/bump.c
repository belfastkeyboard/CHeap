#include <string.h>
#include "../../internals/alloc.h"
#include "../../internals/base.h"
#include "../../internals/cassert.h"
#include "../../bump.h"

#ifndef CHEAP_ALLOC
    #include <malloc.h>
#endif

typedef struct BumpAlloc
{
    void *ptr;
    size_t offset;
    size_t size;
} BumpAlloc;


BumpAlloc *create_bump_allocator(const size_t size)
{
    BumpAlloc *b_alloc = memory_allocate_container(sizeof(BumpAlloc));

    b_alloc->ptr = CHEAP_MALLOC(size);
    b_alloc->size = size;

    return b_alloc;
}

void destroy_bump_allocator(BumpAlloc **bump)
{
    memory_free_container_mempool((void **)bump,
                                  (*bump)->ptr);
}


void *bump_alloc(BumpAlloc *bump,
                 const size_t size)
{
    CHEAP_ASSERT(bump->offset + size <= bump->size, "Bump allocator has insufficient memory.");

    void *ptr = (bump->ptr + bump->offset);

    bump->offset += size;

    return ptr;
}

void *bump_calloc(BumpAlloc *bump,
                  const size_t size)
{
    void *ptr = bump_alloc(bump,
                           size);

    return memset(ptr,
                  0,
                  size);
}

void *bump_salloc(BumpAlloc *bump,
                  const char *string)
{
    size_t len = strlen(string);

    void *ptr = bump_calloc(bump,
                            len + 1);

    return strncpy(ptr,
                   string,
                   len);
}


void bump_clear(BumpAlloc *bump)
{
    bump->offset = 0;
}
