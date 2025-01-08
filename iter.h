#pragma once

#include <stddef.h>


#ifndef CHEAP_ITERATOR_AVAILABLE
#define CHEAP_ITERATOR_AVAILABLE
#endif


typedef struct Iter
{
    void *ptr;
    const size_t size;
} Iter, Iterator;


Iter create_iter(void *ptr,
                 size_t size);


Iter next_iter(Iter *iter);

Iter prev_iter(Iter *iter);


#define range(begin, end) ; begin.ptr <= end.ptr; next_iter(&begin)
