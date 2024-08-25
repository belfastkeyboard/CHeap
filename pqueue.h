#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct PriorityQueue PriorityQueue;
typedef int (*CompareFunc)(const void*, const void*);

__attribute__((warn_unused_result))
PriorityQueue *create_pqueue(size_t capacity, size_t size, CompareFunc comparator);
void destroy_pqueue(PriorityQueue **pqueue);

void push(PriorityQueue *pqueue, void* item);
void *front(PriorityQueue *pqueue);
void pop(PriorityQueue *pqueue);

bool empty(PriorityQueue *pqueue);
size_t size(PriorityQueue *pqueue);

void set_comparator(PriorityQueue *pqueue, CompareFunc comparator);
