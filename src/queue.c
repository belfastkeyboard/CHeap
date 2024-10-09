#include <assert.h>
#include <malloc.h>
#include "../base.h"
#include "../queue.h"

typedef struct Queue
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
} Queue;

Queue *create_queue(size_t size)
{
    Queue *queue = memory_allocate_container(sizeof(Queue));

    SEQ_CONTAINER_INIT(queue, size);

    return queue;
}
void destroy_queue(Queue *queue)
{
    assert(queue);

    memory_free_buffer(queue->array);
    memory_free_container((void**)&queue);
}

void push(Queue *queue, void* item)
{
    if (queue->nmemb >= queue->capacity)
        queue->array = sequential_resize(queue->array, &queue->capacity, queue->size);

    sequential_insert(queue->array, queue->nmemb, item, queue->nmemb, queue->size);
    queue->nmemb++;
}
void *front(Queue *queue)
{
    return sequential_index_access(queue->array, 0, queue->size);
}
void *back(Queue *queue)
{
    return sequential_index_access(queue->array, queue->nmemb - 1, queue->size);
}
void pop(Queue *queue)
{
    assert(queue->nmemb);

    queue->nmemb--;
}

bool empty(Queue *queue)
{
    return container_empty(queue->nmemb);
}
size_t size(Queue *queue)
{
    return container_size(queue->nmemb);
}
