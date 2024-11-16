#include <malloc.h>
#include <memory.h>
#include <assert.h>
#include "../../internals/base.h"
#include "../../internals/mpool.h"

void *mempool_realloc(void *array,
                      const size_t size)
{
    void *tmp = realloc(array,
                        size);

    assert(tmp);

    return tmp;
}

__attribute__((warn_unused_result()))
static void *mempool_resize(void *array,
                            size_t *capacity,
                            const size_t size)
{
    *capacity = (*capacity > EMPTY) ? *capacity * SEQUENTIAL_GROWTH :
                                      SEQUENTIAL_INIT;

    return mempool_realloc(array,
                           *capacity * size);
}

__attribute__((warn_unused_result()))
static void *mempool_range_resize(void *array,
                                  size_t *capacity,
                                  const size_t size,
                                  const size_t r_nmemb)
{
    *capacity = (*capacity + r_nmemb > *capacity * SEQUENTIAL_GROWTH) ? *capacity + r_nmemb :
                                                                       *capacity * SEQUENTIAL_GROWTH;

    return mempool_realloc(array,
                           *capacity * size);
}

static void mempool_insert(void *array,
                           const size_t index,
                           void *value,
                           const size_t nmemb,
                           const size_t size,
                           const size_t shift)
{
    assert(array && value && index < nmemb + 1 && shift > 0);

    if (index < nmemb)
    {
        memmove(array + (index + shift) * size, // TODO: could 1 here be changed to the number in the range?
                array + index * size,
                (nmemb - index) * size);
    }

    memcpy(array + index * size,
           value,
           shift * size);
}

size_t mempool_remove(void *array,
                      const size_t index,
                      const size_t nmemb,
                      const size_t size)
{
    assert(array && index < nmemb);

    memmove(array + index * size,
            array + (index + 1) * size,
            (nmemb - index - 1) * size);

    return index - 1;
}

// TODO: is this operation necessary or is setting nmemb to 0 enough?
void mempool_clear(void *array,
                   const size_t capacity,
                   const size_t size)
{
    assert(array);

    memset(array,
           UNSET,
           capacity * size);
}

void *mempool_random_access(void *array,
                            const size_t index,
                            const size_t size)
{
    assert(array);

    return array + index * size;
}


void generic_mempool_push_back(void **array,
                               void *value,
                               size_t *capacity,
                               size_t *nmemb,
                               const size_t size)
{
    generic_mempool_insert(array,
                           value,
                           *nmemb,
                           capacity,
                           nmemb,
                           size);
}

void generic_mempool_push_front(void **array,
                                void *value,
                                size_t *capacity,
                                size_t *nmemb,
                                const size_t size)
{
    generic_mempool_insert(array,
                           value,
                           0,
                           capacity,
                           nmemb,
                           size);
}

void generic_mempool_insert(void **array,
                            void *value,
                            const size_t index,
                            size_t *capacity,
                            size_t *nmemb,
                            const size_t size)
{
    if (*nmemb >= *capacity)
    {
        *array = mempool_resize(*array,
                                capacity,
                                size);
    }

    mempool_insert(*array,
                   index,
                   value,
                   *nmemb,
                   size,
                   1);

    (*nmemb)++;
}

void generic_mempool_range_insert(void **array,
                                  const size_t index,
                                  size_t *capacity,
                                  size_t *nmemb,
                                  const size_t size,
                                  const Range range)
{
    assert(size == range.size);

    if (*nmemb + range.nmemb >= *capacity)
    {
        *array = mempool_range_resize(*array,
                                      capacity,
                                      size,
                                      range.nmemb);
    }

    mempool_insert(*array,
                   index,
                   range.array,
                   *nmemb,
                   range.size,
                   range.nmemb);

    *nmemb += range.nmemb;
}

Range generic_mempool_get_range(void *array,
                                const size_t capacity,
                                const size_t size,
                                const size_t start,
                                const size_t end)
{
    assert(array && start < capacity && end <= capacity);

    Range range = {
        .array = array + start * size,
        .nmemb = 1 + end - start,
        .size = size
    };

    return range;
}


void generic_mempool_pop_back(size_t *nmemb)
{
    assert(*nmemb);

    (*nmemb)--;
}

void generic_mempool_pop_front(void **array,
                               size_t *nmemb,
                               const size_t size)
{
    assert(*nmemb);

    mempool_remove(*array,
                   0,
                   *nmemb,
                   size);

    (*nmemb)--;
}

size_t generic_mempool_erase(void **array,
                             size_t index,
                             size_t *nmemb,
                             const size_t size)
{
    assert(*array && index < *nmemb);

    index = mempool_remove(*array,
                           index,
                           *nmemb,
                           size);

    (*nmemb)--;

    return index;
}

void generic_mempool_clear(void **array,
                           const size_t capacity,
                           size_t *nmemb,
                           const size_t size)
{
    mempool_clear(*array,
                  capacity,
                  size);

    *nmemb = 0;
}



void *generic_mempool_access_front(void *array,
                                   size_t size)
{
    return mempool_random_access(array,
                                 0,
                                 size);
}

void *generic_mempool_access_back(void *array,
                                  size_t nmemb,
                                  size_t size)
{
    return mempool_random_access(array,
                                 nmemb - 1,
                                 size);
}

void *generic_mempool_random_access(void *array,
                                    const size_t index,
                                    const size_t nmemb,
                                    const size_t size)
{
    assert(index < nmemb);

    return mempool_random_access(array,
                                 index,
                                 size);
}


void generic_mempool_reserve(void **array,
                             size_t *capacity,
                             const size_t size,
                             const size_t new_cap)
{
    if (new_cap > *capacity)
    {
        *array = mempool_realloc(*array,
                                 new_cap * size);

        *capacity = new_cap;
    }
}

void generic_mempool_shrink_to_fit(void **array,
                                   size_t *capacity,
                                   const size_t nmemb,
                                   const size_t size)
{
    if (nmemb < *capacity)
    {
        *capacity = nmemb;

        *array = mempool_realloc(*array,
                                 *capacity * size);
    }
}
