#include <assert.h>
#include <stdlib.h>
#include "../base.h"
#include "../pqueue.h"

typedef struct PriorityQueue
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
    CompareFunc fnc;
} PriorityQueue;

static void update(void *array, size_t nmemb, size_t size, CompareFunc fnc)
{
    qsort(array, nmemb, size, fnc);
}

PriorityQueue *create_pqueue(size_t size, CompareFunc comparator)
{
    PriorityQueue *pqueue = memory_allocate_container(sizeof(PriorityQueue));

    SEQ_CONTAINER_INIT(pqueue, size);
    pqueue->fnc = comparator;

    return pqueue;
}
void destroy_pqueue(PriorityQueue *pqueue)
{
    assert(pqueue);

    memory_free_buffer(pqueue->array);
    memory_free_container((void**)&pqueue);
}

void push(PriorityQueue *pqueue, void* item)
{
    if (pqueue->nmemb >= pqueue->capacity)
        pqueue->array = sequential_resize(pqueue->array, &pqueue->capacity, pqueue->size);

    sequential_insert(pqueue->array, pqueue->nmemb, item, pqueue->nmemb, pqueue->size);
    pqueue->nmemb++;

    update(pqueue->array, pqueue->nmemb, pqueue->size, pqueue->fnc);
}

void *front(PriorityQueue *pqueue)
{
    return sequential_index_access(pqueue->array, 0, pqueue->size);
}

void pop(PriorityQueue *pqueue)
{
    assert(pqueue->nmemb);

    sequential_remove(pqueue->array, 0, pqueue->nmemb, pqueue->size);
    pqueue->nmemb--;

    update(pqueue->array, pqueue->nmemb, pqueue->size, pqueue->fnc);
}

bool empty(PriorityQueue *pqueue)
{
    return container_empty(pqueue->nmemb);
}
size_t size(PriorityQueue *pqueue)
{
    return container_size(pqueue->nmemb);
}

void set_comparator(PriorityQueue *pqueue, CompareFunc comparator)
{
    pqueue->fnc = comparator;
    update(pqueue->array, pqueue->nmemb, pqueue->size, pqueue->fnc);
}
