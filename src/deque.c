#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include "../base.h"
#include "../deque.h"

typedef struct DoubleEndedQueue
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
} Deque;

Deque *create_deque(size_t size)
{
    Deque *deque = malloc(sizeof(Deque));

    deque->size = size;
    clear(deque);

    return deque;
}
void destroy_deque(Deque **deque)
{
    if ((*deque)->array)
        free((*deque)->array);

    free(*deque);
    *deque = NULL;
}

void push_front(Deque *deque, void* item)
{
    if (deque->nmemb >= deque->capacity)
        deque->array = sequential_resize(deque->array, &deque->capacity, deque->size);

    sequential_insert(deque->array, 0, item, deque->nmemb, deque->size);
    deque->nmemb++;
}
void push_back(Deque *deque, void* item)
{
    if (deque->nmemb >= deque->capacity)
        deque->array = sequential_resize(deque->array, &deque->capacity, deque->size);

    sequential_insert(deque->array, deque->nmemb, item, deque->nmemb, deque->size);
    deque->nmemb++;
}
void insert(Deque *deque, void *const item, const size_t index)
{
    if (deque->nmemb >= deque->capacity)
        deque->array = sequential_resize(deque->array, &deque->capacity, deque->size);

    sequential_insert(deque->array, index, item, deque->nmemb, deque->size);
    deque->nmemb++;
}

void *front(Deque *deque)
{
    return sequential_index_access(deque->array, 0, deque->size);
}
void *back(Deque *deque)
{
    return sequential_index_access(deque->array, deque->nmemb - 1, deque->size);
}

void pop_front(Deque *deque)
{
    if (deque->nmemb)
    {
        memmove(deque->array, deque->array + deque->size, (deque->nmemb - 1) * deque->size);
        deque->nmemb--;
    }
}
void pop_back(Deque *deque)
{
    assert(deque->nmemb);

    deque->nmemb--;
}
size_t erase(Deque *deque, size_t index)
{
    index = sequential_remove(deque->array, index, deque->nmemb, deque->size);
    deque->nmemb--;

    return index;
}
void clear(Deque *deque)
{
    sequential_clear(deque->array, deque->capacity, deque->size);
    deque->nmemb = 0;
}

bool empty(Deque *deque)
{
    return container_empty(deque->nmemb);
}
size_t size(Deque *deque)
{
    return container_size(deque->nmemb);
}
