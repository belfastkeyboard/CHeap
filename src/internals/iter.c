#include "../../iter.h"


Iter create_iter(void *ptr,
                  size_t size)
{
    Iterator iter = {
        .ptr = ptr,
        .size = size
    };

    return iter;
}


Iter next_iter(Iter *iter)
{
    iter->ptr = iter->ptr + iter->size;

    return *iter;
}

Iter prev_iter(Iter *iter)
{
    iter->ptr = iter->ptr - iter->size;

    return *iter;
}
