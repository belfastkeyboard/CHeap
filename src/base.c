#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include <stdbool.h>
#include "../arena.h"
#include "../base.h"

void *memory_allocate_container(size_t size)
{
    void *ptr = calloc(1, size);

    assert(ptr);

    return ptr;
}
void memory_free_buffer(void **buffer)
{
    if (*buffer)
        free(*buffer);
    *buffer = NULL;
}
void memory_free_container_mempool(void **container, void *array)
{
    assert(*container);

    memory_free_buffer(&array);
    memory_free_buffer(container);
}
void memory_free_container_arena(void **container, Arena *arena)
{
    assert(*container);

    destroy_arena(&arena);
    memory_free_buffer(container);
}
void memory_free_container_hash(void **container, void *buckets, void *keys, void *values)
{
    memory_free_buffer(&buckets);
    memory_free_buffer(&keys);
    memory_free_buffer(container);
}

bool generic_empty(size_t nmemb)
{
    return (nmemb == EMPTY);
}
size_t generic_size(size_t nmemb)
{
    return nmemb;
}
size_t generic_capacity(size_t capacity)
{
    return capacity;
}
