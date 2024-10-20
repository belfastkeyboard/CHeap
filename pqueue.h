#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct PriorityQueue PriorityQueue, PQueue;
typedef int (*CompareFunc)(const void*, const void*);

__attribute__((warn_unused_result))
PQueue *create_pqueue(size_t size, CompareFunc comparator);
void destroy_pqueue(PQueue **pqueue);

void push_pqueue(PQueue *pqueue, void *value);

void pop_pqueue(PQueue *pqueue);

void *front_pqueue(PQueue *pqueue);

bool empty_pqueue(PQueue *pqueue);
size_t size_pqueue(PQueue *pqueue);

void set_comparator_pqueue(PQueue *pqueue, CompareFunc comparator);
