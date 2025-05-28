#include "../../iter.h"

extern Iter *next_vector(Iter *iter);
extern Iter *prev_vector(Iter *iter);
extern void *get_vector(const Iter *iter);

extern Iter *next_hash_table(Iter *iter);
extern Iter *prev_hash_table(Iter *iter);
extern void *get_hash_table(const Iter *iter);


Iter *next_iter(Iter *iter)
{
    switch (iter->type)
    {
        case ITERATOR_VECTOR:
            return next_vector(iter);
        case ITERATOR_HASH_TABLE:
            return next_hash_table(iter);
    }
}

Iter *prev_iter(Iter *iter)
{
    switch (iter->type)
    {
        case ITERATOR_VECTOR:
            return prev_vector(iter);
        case ITERATOR_HASH_TABLE:
            return prev_hash_table(iter);
    }
}

void *get_iter(const Iter *iter)
{
    switch (iter->type)
    {
        case ITERATOR_VECTOR:
            return get_vector(iter);
        case ITERATOR_HASH_TABLE:
            return get_hash_table(iter);
    }
}

bool done_iter(const Iter *begin,
               const Iter *end)
{
    return begin->ptr <= end->ptr;
}

bool done_iter_r(const Iter *begin,
                 const Iter *end)
{
    return begin->ptr >= end->ptr;
}
