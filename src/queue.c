#include <stdlib.h>
#include <memory.h>
#include "../queue.h"

typedef struct Queue
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
} Queue;

static void resize(Queue *queue)
{
    queue->capacity = (queue->capacity > 0) ? queue->capacity * 2 : 1;

    void *tmp = realloc(queue->array, queue->size * queue->capacity);
    if (tmp)
        queue->array = tmp;
}

Queue *create_pqueue(size_t capacity, size_t size)
{
    Queue *queue = malloc(sizeof(Queue));

    queue->capacity = capacity;
    queue->nmemb = 0;
    queue->size = size;

    queue->array = malloc(capacity * size);
    memset(queue->array, 0, capacity * size);

    return queue;
}
void destroy_pqueue(Queue **queue)
{
    if ((*queue)->array)
        free((*queue)->array);

    free(*queue);
    *queue = NULL;
}

void push(Queue *queue, void* item)
{
    if (queue->nmemb >= queue->capacity)
        resize(queue);

    memcpy(queue->array + (queue->nmemb * queue->size), item, queue->size);
    queue->nmemb++;
}
void *front(Queue *queue)
{
    return (queue->nmemb) ? queue->array : NULL;
}
void pop(Queue *queue)
{
    if (queue->nmemb)
    {
        memmove(queue->array, queue->array + queue->size, (queue->nmemb - 1) * queue->size);
        queue->nmemb--;
    }
}

bool empty(Queue *queue)
{
    return queue->nmemb == 0;
}
size_t size(Queue *queue)
{
    return queue->nmemb;
}
