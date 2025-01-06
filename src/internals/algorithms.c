#include <stdlib.h>
#include <stdio.h>
#include "../../internals/cassert.h"
#include "../../algorithms.h"


void sort(Iter *begin,
          Iter *end,
          Comp comp)
{
    CHEAP_ASSERT(begin->size == end->size,
                 "Cannot sort different types.");

    const size_t size = begin->size;

    const ptrdiff_t diff = (ptrdiff_t)(end->ptr - begin->ptr + size);
    const size_t nmemb = diff / size;

    qsort(begin->ptr,
          nmemb,
          size,
          comp);
}
