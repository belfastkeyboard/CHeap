#include <stdbool.h>
#include "../../internals/alloc.h"
#include "../../internals/base.h"
#include "../../internals/cassert.h"

#ifndef CHEAP_ALLOC
    #include <malloc.h>
#endif


void *memory_allocate_container(const size_t size)
{
    void *ptr = CHEAP_CALLOC(1,
                             size);

    CHEAP_ASSERT(ptr,
                 "Failed to allocate container.");

    return ptr;
}

void memory_free_container_mempool(void **container,
                                   void *array)
{
    CHEAP_ASSERT(*container,
                 "Container cannot be NULL.");

    memory_free_buffer(&array);
    memory_free_buffer(container);
}


void memory_free_buffer(void **buffer)
{
    if (*buffer)
    {
        CHEAP_FREE(*buffer);
    }

    *buffer = NULL;
}


void memory_free_container_hash(void **container,
                                void *buckets,
                                void *keys,
                                void *values)
{
    CHEAP_ASSERT(*container,
                 "Container cannot be NULL.");

    memory_free_buffer(&buckets);
    memory_free_buffer(&keys);
    memory_free_buffer(&values);
    memory_free_buffer(container);
}


bool generic_empty(const size_t nmemb)
{
    return (nmemb == EMPTY);
}

size_t generic_size(const size_t nmemb)
{
    return nmemb;
}

size_t generic_capacity(const size_t capacity)
{
    return capacity;
}
