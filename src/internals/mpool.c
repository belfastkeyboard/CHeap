#include "../../internals/mpool.h"
#include "../../internals/base.h"
#include "../../internals/cassert.h"
#include "../../iter.h"
#include <memory.h>

ALLOC static void *mempool_alloc(const size_t nmemb, const size_t size)
{
	void *tmp = malloc(nmemb * size);

	CHEAP_ASSERT(tmp, "Failed to allocate additional memory.");

	return tmp;
}

ALLOC static void *mempool_realloc(void *array, const size_t size)
{
	void *tmp = realloc(array, size);

	CHEAP_ASSERT(tmp, "Failed to reallocate memory.");

	return tmp;
}

ALLOC static void *mempool_resize(void        *array,
                                  size_t      *capacity,
                                  const size_t size)
{
	*capacity = (*capacity > EMPTY) ? *capacity * SEQUENTIAL_GROWTH
	                                : SEQUENTIAL_INIT;

	return mempool_realloc(array, *capacity * size);
}

ALLOC static void *mempool_range_resize(void        *array,
                                        size_t      *capacity,
                                        const size_t size,
                                        const size_t r_nmemb)
{
	*capacity = (*capacity + r_nmemb > *capacity * SEQUENTIAL_GROWTH)
	                ? *capacity + r_nmemb
	                : *capacity * SEQUENTIAL_GROWTH;

	return mempool_realloc(array, *capacity * size);
}

static void mempool_insert(void        *array,
                           const size_t index,
                           const void  *value,
                           const size_t nmemb,
                           const size_t size)
{
	CHEAP_ASSERT(index < nmemb + 1, "Index out of bounds.");

	if (index < nmemb)
	{
		memmove(array + (index + 1) * size,
		        array + index * size,
		        (nmemb - index) * size);
	}

	memcpy(array + index * size, value, size);
}

static void mempool_set(void        *array,
                        const void  *value,
                        const size_t index,
                        const size_t size)
{
	CHEAP_ASSERT(array && value, "Array and value cannot be NULL.");

	memcpy(array + index * size, value, size);
}

static void *mempool_random_access(void        *array,
                                   const size_t index,
                                   const size_t size)
{
	CHEAP_ASSERT(array, "Array cannot be NULL");

	return array + index * size;
}

static void mempool_remove(void        *array,
                           const size_t index,
                           const size_t nmemb,
                           const size_t size)
{
	CHEAP_ASSERT(index < nmemb, "Index out of bounds.");

	memmove(array + index * size,
	        array + (index + 1) * size,
	        (nmemb - index - 1) * size);
}

void *generic_mempool_alloc(size_t nmemb, size_t size)
{
	return mempool_alloc(nmemb, size);
}

void generic_mempool_push_back(void       **array,
                               const void  *value,
                               size_t      *capacity,
                               size_t      *nmemb,
                               const size_t size)
{
	CHEAP_ASSERT(value, "Value cannot be NULL.");

	generic_mempool_insert(array, value, *nmemb, capacity, nmemb, size);
}

void generic_mempool_push_front(void       **array,
                                const void  *value,
                                size_t      *capacity,
                                size_t      *nmemb,
                                const size_t size)
{
	CHEAP_ASSERT(value, "Value cannot be NULL.");

	generic_mempool_insert(array, value, 0, capacity, nmemb, size);
}

void generic_mempool_insert(void       **array,
                            const void  *value,
                            const size_t index,
                            size_t      *capacity,
                            size_t      *nmemb,
                            const size_t size)
{
	CHEAP_ASSERT(value, "Value cannot be NULL.");

	if (*nmemb >= *capacity)
	{
		*array = mempool_resize(*array, capacity, size);
	}

	mempool_insert(*array, index, value, *nmemb, size);

	(*nmemb)++;
}

void generic_mempool_set(void        *array,
                         const void  *value,
                         const size_t index,
                         const size_t nmemb,
                         const size_t size)
{
	CHEAP_ASSERT(index < nmemb, "Index out of bounds.");

	mempool_set(array, value, index, size);
}

void generic_mempool_range_insert(void       **array,
                                  size_t index,
                                  size_t      *capacity,
                                  size_t      *nmemb,
                                  const size_t size,
                                  Range       range)
{
	bool forward = range.begin.type < ITERATOR_REVERSE;
	bool (*done)(Range) = (forward) ? done_range : done_range_r;
	void (*iterate)(Iter*) = (forward) ? next_iter : prev_iter;

	for (; !done(range); iterate(&range.begin))
	{
		void *value = get_range(range);
		generic_mempool_insert(array, value, index, capacity, nmemb, size);
		index++;
	}
}

void generic_mempool_pop_back(size_t *nmemb)
{
	CHEAP_ASSERT(*nmemb, "Cannot pop back empty container.");

	(*nmemb)--;
}

void generic_mempool_pop_front(void **array, size_t *nmemb, const size_t size)
{
	CHEAP_ASSERT(*nmemb, "Cannot pop front empty container.");

	mempool_remove(*array, 0, *nmemb, size);

	(*nmemb)--;
}

void generic_mempool_erase(void       **array,
                           size_t       index,
                           size_t      *nmemb,
                           const size_t size)
{
	CHEAP_ASSERT(*array, "Array cannot be NULL.");

	mempool_remove(*array, index, *nmemb, size);

	(*nmemb)--;
}

void generic_mempool_clear(size_t *nmemb)
{
	*nmemb = 0;
}

void *generic_mempool_access_front(void *array, const size_t size)
{
	return mempool_random_access(array, 0, size);
}

void *generic_mempool_access_back(void        *array,
                                  const size_t nmemb,
                                  const size_t size)
{
	return mempool_random_access(array, nmemb - 1, size);
}

void *generic_mempool_random_access(void        *array,
                                    const size_t index,
                                    const size_t nmemb,
                                    const size_t size)
{
	CHEAP_ASSERT(index < nmemb, "Index out of bounds.");

	return mempool_random_access(array, index, size);
}

void generic_mempool_reserve(void       **array,
                             size_t      *capacity,
                             const size_t size,
                             const size_t new_cap)
{
	if (new_cap > *capacity)
	{
		*array    = mempool_realloc(*array, new_cap * size);
		*capacity = new_cap;
	}
}

void generic_mempool_shrink_to_fit(void       **array,
                                   size_t      *capacity,
                                   const size_t nmemb,
                                   const size_t size)
{
	if (nmemb < *capacity)
	{
		*capacity = nmemb;
		*array    = mempool_realloc(*array, *capacity * size);
	}
}

void next_mempool(Iter *iter)
{
	iter->data.contiguous.array += iter->data.contiguous.size;
}

void prev_mempool(Iter *iter)
{
	iter->data.contiguous.array -= iter->data.contiguous.size;
}

void *get_mempool(const Iter iter)
{
	return iter.data.contiguous.array;
}
