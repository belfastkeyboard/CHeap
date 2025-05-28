#pragma once

#include <stddef.h>
#include <stdbool.h>


#ifndef CHEAP_ITERATOR_AVAILABLE
#define CHEAP_ITERATOR_AVAILABLE
#endif


typedef enum IteratorType
{
    ITERATOR_VECTOR,
    ITERATOR_HASH_TABLE
} IteratorType;

typedef struct Iter
{
    const IteratorType type;
    void *ptr;
    const void *end;
    const size_t size;
} Iter, Iterator;


Iter *next_iter(Iter *iter);

Iter *prev_iter(Iter *iter);

void *get_iter(const Iter *iter);

bool done_iter(const Iter *begin,
               const Iter *end);

bool done_iter_r(const Iter *begin,
                 const Iter *end);


#define range(begin, end) ; done_iter(&begin, &end); next_iter(&begin)

#define range_r(begin, end) ; done_iter_r(&begin, &end); prev_iter(&begin)
