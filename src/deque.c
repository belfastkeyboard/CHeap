#include <stdlib.h>
#include <memory.h>
#include "../deque.h"

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
void insert(Deque *deque, void *const item, const size_t index)
{
    if (index <= deque->nmemb)
    {
        if (deque->nmemb >= deque->capacity)
            resize(deque);

        if (index == deque->nmemb)
            push_back(deque, item);
        else
        {
            memmove(
                deque->array + (index + 1) * deque->size,
                deque->array + index * deque->size,
                (deque->nmemb - index) * deque->size
            );
            memcpy(deque->array + index * deque->size, item, deque->size);
            deque->nmemb++;
        }
    }
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
size_t erase(Deque *deque, size_t index)
{
    if (deque->nmemb)
    {
        if (index == deque->nmemb - 1)
        {
            pop_back(deque);
            index--;
        }
        else
        {
            memmove(deque->array + index, deque->array + index + 1, (deque->nmemb - index) * deque->size);
            deque->nmemb--;
            index--;
        }
    }

    return index;
}
void clear(Deque *deque)
{
    deque->capacity = 0;
    deque->nmemb = 0;

    deque->array = malloc(deque->capacity * deque->size);
    memset(deque->array, 0, deque->capacity * deque->size);
}

bool empty(Deque *deque)
{
    return deque->nmemb == 0;
}
size_t size(Deque *deque)
{
    return deque->nmemb;
}
