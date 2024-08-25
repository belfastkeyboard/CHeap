#include <stdlib.h>
#include <memory.h>
#include "../pqueue.h"

static void update(void *array, size_t nmemb, size_t size, CompareFunc fnc)
{
    qsort(array, nmemb, size, fnc);
}

typedef struct PriorityQueue
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
    CompareFunc fnc;
} PriorityQueue;

PriorityQueue *create_pqueue(size_t capacity, size_t size, CompareFunc comparator)
{
    PriorityQueue *pqueue = malloc(sizeof(PriorityQueue));

    pqueue->capacity = capacity;
    pqueue->nmemb = 0;
    pqueue->size = size;
    pqueue->fnc = comparator;

    pqueue->array = malloc(capacity * size);
    memset(pqueue->array, 0, capacity * size);

    return pqueue;
}
void destroy_pqueue(PriorityQueue **pqueue)
{
    if ((*pqueue)->array)
        free((*pqueue)->array);

    free(*pqueue);
    *pqueue = NULL;
}

void push(PriorityQueue *pqueue, void* item)
{
    if (pqueue->nmemb < pqueue->capacity)
    {
        memcpy(pqueue->array + (pqueue->nmemb * pqueue->size), item, pqueue->size);
        pqueue->nmemb++;
        update(pqueue->array, pqueue->nmemb, pqueue->size, pqueue->fnc);
    }
}
void *front(PriorityQueue *pqueue)
{
    return (pqueue->nmemb) ? pqueue->array : NULL;
}
void pop(PriorityQueue *pqueue)
{
    if (pqueue->nmemb)
    {
        memmove(pqueue->array, pqueue->array + pqueue->size, (pqueue->nmemb - 1) * pqueue->size);
        pqueue->nmemb--;
        update(pqueue->array, pqueue->nmemb, pqueue->size, pqueue->fnc);
    }
}

bool empty(PriorityQueue *pqueue)
{
    return pqueue->nmemb == 0;
}
size_t size(PriorityQueue *pqueue)
{
    return pqueue->nmemb;
}

void set_comparator(PriorityQueue *pqueue, CompareFunc comparator)
{
    pqueue->fnc = comparator;
    update(pqueue->array, pqueue->nmemb, pqueue->size, pqueue->fnc);
}
