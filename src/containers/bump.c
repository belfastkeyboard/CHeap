#include <assert.h>
#include <malloc.h>
#include <string.h>
#include "../../internals/base.h"
#include "../../bump.h"

typedef struct BumpAlloc
{
    void *ptr;
    size_t offset;
    size_t size;
} BumpAlloc;

BumpAlloc *create_bump_allocator(const size_t size)
{
    BumpAlloc *b_alloc = memory_allocate_container(sizeof(BumpAlloc));

    b_alloc->ptr = malloc(size);
    b_alloc->size = size;

    return b_alloc;
}
void destroy_bump_allocator(BumpAlloc **bump)
{
    free((*bump)->ptr);
    free(*bump);
    *bump = NULL;
}

void *bump_alloc(BumpAlloc *bump, size_t size)
{
    assert(bump->offset + size <= bump->size);

    void *ptr = (bump->ptr + bump->offset);

    bump->offset += size;

    return ptr;
}
void *bump_calloc(BumpAlloc *bump, size_t size)
{
    return memset(bump_alloc(bump, size), 0, size);
}
void *bump_salloc(BumpAlloc *bump, const char *string)
{
    size_t len = strlen(string);
    return strncpy(bump_calloc(bump, len + 1), string, len);
}

void bump_clear(BumpAlloc *bump)
{
    bump->offset = 0;
}
