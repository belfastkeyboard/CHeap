#include "../../array.h"
#include "../../internals/base.h"
#include "../../internals/mpool.h"
#include "../../iter.h"

typedef struct Array
{
	void  *array;
	size_t nmemb;
	size_t size;
} Array;

Array *create_array(const size_t size, const size_t nmemb)
{
	Array *array = memory_allocate_container(sizeof(Array));

	array->array = generic_mempool_alloc(nmemb, size);
	array->nmemb = nmemb;
	array->size  = size;

	return array;
}

void destroy_array(Array **array)
{
	memory_free_container_generic((void **)array, (*array)->array);
}

void set_array(Array *array, const void *value, const size_t index)
{
	generic_mempool_set(array->array, value, index, array->nmemb, array->size);
}

void *at_array(const Array *array, size_t index)
{
	return generic_mempool_random_access(array->array,
	                                     index,
	                                     array->nmemb,
	                                     array->size);
}

void *front_array(const Array *array)
{
	return generic_mempool_access_front(array->array, array->nmemb);
}

void *back_array(const Array *array)
{
	return generic_mempool_access_back(array->array, array->nmemb, array->size);
}

bool empty_array(const Array *array)
{
	return generic_empty(array->nmemb);
}

size_t size_array(const Array *array)
{
	return generic_size(array->nmemb);
}

Iter begin_array(const Array *array)
{
	void        *ptr  = front_array(array);
	const size_t size = array->size;

	Iter iter = {
		.type            = ITERATOR_ARRAY,
		.data.contiguous = { .array = ptr, .size = size }
	};

	return iter;
}

Iter end_array(const Array *array)
{
	const size_t size = array->size;
	void        *arr  = back_array(array) + size;

	Iter iter = {
		.type            = ITERATOR_ARRAY,
		.data.contiguous = { .array = arr, .size = size }
	};

	return iter;
}

Iter rbegin_array(const Array *array)
{
	const size_t size  = array->size;
	void        *arr = back_array(array);

	Iter iter = {
		.type            = ITERATOR_VECTOR,
		.data.contiguous = { .array = arr, .size = size }
	};

	return iter;
}

Iter rend_array(const Array *array)
{
	const size_t size  = array->size;
	void        *arr = front_array(array) - size;

	Iter iter = {
		.type            = ITERATOR_VECTOR,
		.data.contiguous = { .array = arr, .size = size }
	};

	return iter;
}
