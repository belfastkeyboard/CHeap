#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include <stdbool.h>
#include "../base.h"

#define EMPTY 0
#define UNSET EMPTY

#define SEQUENTIAL_GROWTH 2
#define SEQUENTIAL_INIT 1

void *memory_allocate_container(size_t size)
{
    void *ptr = malloc(size);

    assert(ptr);

    return ptr;
}
void memory_free_buffer(void *buffer)
{
    if (buffer)
        free(buffer);
}
void memory_free_container(void **container)
{
    free(*container);
    *container = NULL;
}

bool container_empty(size_t nmemb)
{
    return (nmemb == EMPTY);
}
size_t container_size(size_t nmemb)
{
    return nmemb;
}
size_t container_capacity(size_t capacity)
{
    return capacity;
}

void *sequential_realloc(void *array, size_t capacity, const size_t size)
{
    void *tmp = realloc(array, size * capacity);

    assert(tmp);

    return tmp;
}
void *sequential_resize(void *array, size_t *capacity, const size_t size)
{
    *capacity = (*capacity > EMPTY) ? *capacity * SEQUENTIAL_GROWTH : SEQUENTIAL_INIT;

    return sequential_realloc(array, *capacity, size);
}
void sequential_insert(void *array, size_t index, void *item, size_t nmemb, size_t size)
{
    if (index < nmemb)
        memmove(array + (index + 1) * size,array + index * size,(nmemb - index) * size);

    memcpy(array + index * size, item, size);
}
size_t sequential_remove(void *array, size_t index, size_t nmemb, size_t size)
{
    if (index < nmemb - 1)
        memmove(array + index * size, array + (index + 1) * size, (nmemb - index) * size);

    return index - 1;
}
void sequential_clear(void *array, size_t capacity, size_t size)
{
    memset(array, UNSET, capacity * size);
}
void *sequential_index_access(void *array, size_t index, size_t size)
{
    return array + index * size;
}
