#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct Queue Queue;

__attribute__((warn_unused_result))
Queue *create_queue(size_t capacity, size_t size);
void destroy_queue(Queue **queue);

void push(Queue *queue, void* item);
void *front(Queue *queue);
void *back(Queue *queue);
void pop(Queue *queue);

bool empty(Queue *queue);
size_t size(Queue *queue);
