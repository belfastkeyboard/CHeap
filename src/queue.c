#include <assert.h>
#include <malloc.h>
#include <memory.h>
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
    Queue *queue = malloc(sizeof(Queue));

    queue->capacity = 0;
    queue->nmemb = 0;
    queue->size = size;

    queue->array = malloc(queue->capacity * size);
    memset(queue->array, 0, queue->capacity * size);

    return queue;
}
void destroy_queue(Queue **queue)
{
    if ((*queue)->array)
        free((*queue)->array);

    free(*queue);
    *queue = NULL;
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
