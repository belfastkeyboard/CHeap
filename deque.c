#include <stdlib.h>
#include <memory.h>
#include "deque.h"

typedef struct DoubleEndedQueue
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
} Deque;

static void resize(Deque *deque)
{
    deque->capacity = (deque->capacity > 0) ? deque->capacity * 2 : 1;

    void *tmp = realloc(deque->array, deque->size * deque->capacity);
    if (tmp)
        deque->array = tmp;
}

Deque *create_deque(size_t size)
{
    Deque *deque = malloc(sizeof(Deque));

    deque->capacity = 0;
    deque->nmemb = 0;
    deque->size = size;

    deque->array = malloc(deque->capacity * size);
    memset(deque->array, 0, deque->capacity * size);

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
        resize(deque);

    memmove(deque->array + deque->size, deque->array, deque->size * deque->nmemb);
    memcpy(deque->array, item, deque->size);
    deque->nmemb++;
}
void push_back(Deque *deque, void* item)
{
    if (deque->nmemb >= deque->capacity)
        resize(deque);

    memcpy(deque->array + (deque->nmemb * deque->size), item, deque->size);
    deque->nmemb++;
}
void *front(Deque *deque)
{
    return (deque->nmemb) ? deque->array : NULL;
}
void *back(Deque *deque)
{
    return (deque->nmemb) ? deque->array + (deque->nmemb - 1) * deque->size : NULL;
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
    if (deque->nmemb)
        deque->nmemb--;
}

bool empty(Deque *deque)
{
    return deque->nmemb == 0;
}
size_t size(Deque *deque)
{
    return deque->nmemb;
}
