#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct Queue Queue;

__attribute__((warn_unused_result))
Queue *create_queue(size_t size);
void destroy_queue(Queue **queue);

void push_queue(Queue *queue, void *value);
void *front_queue(Queue *queue);
void *back_queue(Queue *queue);
void pop_queue(Queue *queue);

bool empty_queue(Queue *queue);
size_t size_queue(Queue *queue);
