#include "../../internals/base.h"
#include "../../internals/mpool.h"
#include "../../queue.h"

typedef struct Queue
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
} Queue;


Queue *create_queue(const size_t size)
{
    Queue *queue = memory_allocate_container(sizeof(Queue));

    queue->size = size;

    return queue;
}

void destroy_queue(Queue **queue)
{
    memory_free_container_mempool((void **) queue,
                                  (*queue)->array);
}


void push_queue(Queue *queue,
                const void *value)
{
    generic_mempool_push_back(&queue->array,
                              value,
                              &queue->capacity,
                              &queue->nmemb,
                              queue->size);
}

void push_range_queue(Queue *queue,
                      const Range *range)
{
    generic_mempool_range_insert(&queue->array,
                                 queue->nmemb,
                                 &queue->capacity,
                                 &queue->nmemb,
                                 queue->size,
                                 range);
}


void *front_queue(const Queue *queue)
{
    return generic_mempool_access_front(queue->array,
                                        queue->nmemb);
}

void *back_queue(const Queue *queue)
{
    return generic_mempool_access_back(queue->array,
                                       queue->nmemb,
                                       queue->size);
}

void pop_queue(Queue *queue)
{
    generic_mempool_pop_front(&queue->array,
                              &queue->nmemb,
                              queue->size);
}


bool empty_queue(const Queue *queue)
{
    return generic_empty(queue->nmemb);
}

size_t size_queue(const Queue *queue)
{
    return generic_size(queue->nmemb);
}
