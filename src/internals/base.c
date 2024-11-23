#include "../../internals/cassert.h"
#include <malloc.h>
#include <stdbool.h>
#include "../../internals/base.h"

void *memory_allocate_container(const size_t size)
{
    void *ptr = calloc(1,
                       size);

    CHEAP_ASSERT(ptr,
                 "Failed to allocate container.");

    return ptr;
}

void memory_free_buffer(void **buffer)
{
    if (*buffer)
        free(*buffer);

    *buffer = NULL;
}

void memory_free_container_mempool(void **container,
                                   void *array)
{
    CHEAP_ASSERT(*container,
                 "Container cannot be NULL.");

    memory_free_buffer(&array);
    memory_free_buffer(container);
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
