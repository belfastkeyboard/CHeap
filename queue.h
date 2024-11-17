#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "range.h"

typedef struct Queue Queue;


__attribute__((warn_unused_result))
Queue *create_queue(size_t size);

void destroy_queue(Queue **queue);


void push_queue(Queue *queue,
                const void *value);

void push_range_queue(Queue *queue,
                      const Range *range);

void *front_queue(const Queue *queue);

void *back_queue(const Queue *queue);

void pop_queue(Queue *queue);


bool empty_queue(const Queue *queue);

size_t size_queue(const Queue *queue);
