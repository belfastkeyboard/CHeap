#include <assert.h>
#include <malloc.h>
#include <string.h>
#include "../../bump.h"

typedef struct BumpAlloc
{
    void *ptr;
    size_t offset;
    size_t size;
} BumpAlloc;

BumpAlloc *create_bump_allocator(const size_t size)
{
    BumpAlloc *b_alloc = malloc(sizeof(BumpAlloc));

    b_alloc->size = size;

    return b_alloc;
}
void destroy_bump_allocator(BumpAlloc **b_alloc)
{
    free((*b_alloc)->ptr);
    free(*b_alloc);
    *b_alloc = NULL;
}

void *bump_allocator_alloc(BumpAlloc *b_alloc, const size_t size)
{
    assert(b_alloc->offset + size <= b_alloc->size);

    void *ptr = (b_alloc->ptr + b_alloc->offset);

    b_alloc->offset += size;

    return ptr;
}
void *bump_allocator_calloc(BumpAlloc *b_alloc, const size_t size)
{
    return memset(bump_allocator_alloc(b_alloc, size), 0, size);
}
void *bump_allocator_salloc(BumpAlloc *b_alloc, const char *string)
{
    size_t len = strlen(string);
    return strncpy(bump_allocator_calloc(b_alloc, len + 1), string, len);
}

void bump_allocator_clear(BumpAlloc *b_alloc)
{
    b_alloc->offset = 0;
}
