#include "../base.h"
#include "../mpool.h"
#include "../queue.h"

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
    memory_free_container_mempool((void **) queue, (*queue)->array);
}

void push_queue(Queue *queue, void *value)
{
    generic_mempool_push_back(&queue->array, value, &queue->capacity, &queue->nmemb, queue->size);
}
void *front_queue(Queue *queue)
{
    return mempool_random_access(queue->array, 0, queue->size);
}
void *back_queue(Queue *queue)
{
    return mempool_random_access(queue->array, queue->nmemb - 1, queue->size);
}
void pop_queue(Queue *queue)
{
    generic_mempool_pop_front(&queue->array, &queue->nmemb, queue->size);
}

bool empty_queue(Queue *queue)
{
    return generic_empty(queue->nmemb);
}
size_t size_queue(Queue *queue)
{
    return generic_size(queue->nmemb);
}
