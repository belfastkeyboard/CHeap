#include <stdlib.h>
#include "../../internals/base.h"
#include "../../internals/mpool.h"
#include "../../pqueue.h"

typedef struct PriorityQueue
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
    CompareFunc fnc;
} PriorityQueue;


static void update(void *array,
                   const size_t nmemb,
                   const size_t size,
                   const CompareFunc fnc)
{
    qsort(array,
          nmemb,
          size,
          fnc);
}


PriorityQueue *create_pqueue(const size_t size,
                             const CompareFunc comparator)
{
    PriorityQueue *pqueue = memory_allocate_container(sizeof(PriorityQueue));

    pqueue->fnc = comparator;
    pqueue->size = size;

    return pqueue;
}

void destroy_pqueue(PriorityQueue **pqueue)
{
    memory_free_container_mempool((void **) pqueue,
                                  (*pqueue)->array);
}


void push_pqueue(PriorityQueue *pqueue,
                 const void *value)
{
    generic_mempool_push_back(&pqueue->array,
                              value,
                              &pqueue->capacity,
                              &pqueue->nmemb,
                              pqueue->size);

    update(pqueue->array,
           pqueue->nmemb,
           pqueue->size,
           pqueue->fnc);
}


void *front_pqueue(const PriorityQueue *pqueue)
{
    return generic_mempool_access_front(pqueue->array,
                                        pqueue->nmemb);
}


void pop_pqueue(PriorityQueue *pqueue)
{
    generic_mempool_pop_front(&pqueue->array,
                              &pqueue->nmemb,
                              pqueue->size);

    update(pqueue->array,
           pqueue->nmemb,
           pqueue->size,
           pqueue->fnc);
}


bool empty_pqueue(const PriorityQueue *pqueue)
{
    return generic_empty(pqueue->nmemb);
}

size_t size_pqueue(const PriorityQueue *pqueue)
{
    return generic_size(pqueue->nmemb);
}


void set_comparator_pqueue(PriorityQueue *pqueue,
                           const CompareFunc comparator)
{
    pqueue->fnc = comparator;

    update(pqueue->array,
           pqueue->nmemb,
           pqueue->size,
           pqueue->fnc);
}
