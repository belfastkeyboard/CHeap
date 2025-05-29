#pragma once

#include <stdbool.h>
#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

#ifdef CHEAP_RANGE_AVAILABLE
#include "range.h"
#endif

typedef struct PriorityQueue PriorityQueue, PQueue;

typedef int (*CompareFunc)(const void *, const void *);

ALLOC PQueue *create_pqueue(size_t size, CompareFunc comparator);
void destroy_pqueue(PQueue **pqueue);

void push_pqueue(PQueue *pqueue, const void *value);

#ifdef CHEAP_RANGE_AVAILABLE
void push_range_pqueue(PQueue *pqueue, const Range *range);
#endif

void pop_pqueue(PQueue *pqueue);

void *top_pqueue(const PQueue *pqueue);

bool empty_pqueue(const PQueue *pqueue);
size_t size_pqueue(const PQueue *pqueue);

void set_comparator_pqueue(PQueue *pqueue, CompareFunc comparator);
