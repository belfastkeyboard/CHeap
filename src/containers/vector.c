#include "../../vector.h"
#include "../../internals/base.h"
#include "../../internals/mpool.h"
#include "../../span.h"

typedef struct Vector
{
	void  *array;
	size_t capacity;
	size_t nmemb;
	size_t size;
} Vector;

Vector *create_vector(size_t size)
{
	Vector *vector = memory_allocate_container(sizeof(Vector));

	vector->size = size;

	return vector;
}

void destroy_vector(Vector **vector)
{
	memory_free_container_generic((void **)vector, (*vector)->array);
}

void push_back_vector(Vector *vector, const void *value)
{
	generic_mempool_push_back(&vector->array,
	                          value,
	                          &vector->capacity,
	                          &vector->nmemb,
	                          vector->size);
}

void insert_vector(Vector *vector, const void *value, const size_t index)
{
	generic_mempool_insert(&vector->array,
	                       value,
	                       index,
	                       &vector->capacity,
	                       &vector->nmemb,
	                       vector->size);
}

void push_back_range_vector(Vector *vector, const Range range)
{
	generic_mempool_range_insert(&vector->array,
	                             vector->nmemb,
	                             &vector->capacity,
	                             &vector->nmemb,
	                             vector->size,
	                             range);
}

void insert_range_vector(Vector *vector, const size_t index, const Range range)
{
	generic_mempool_range_insert(&vector->array,
	                             index,
	                             &vector->capacity,
	                             &vector->nmemb,
	                             vector->size,
	                             range);
}

void pop_back_vector(Vector *vector)
{
	generic_mempool_pop_back(&vector->nmemb);
}

void erase_vector(Vector *vector, const size_t index)
{
	generic_mempool_erase(&vector->array, index, &vector->nmemb, vector->size);
}

void clear_vector(Vector *vector)
{
	generic_mempool_clear(&vector->nmemb);
}

void *at_vector(const Vector *vector, const size_t index)
{
	return generic_mempool_random_access(vector->array,
	                                     index,
	                                     vector->nmemb,
	                                     vector->size);
}

void *front_vector(const Vector *vector)
{
	return generic_mempool_access_front(vector->array, vector->nmemb);
}

void *back_vector(const Vector *vector)
{
	return generic_mempool_access_back(vector->array,
	                                   vector->nmemb,
	                                   vector->size);
}

Iter begin_vector(const Vector *vector)
{
	const size_t size  = vector->size;
	void        *array = front_vector(vector);

	Iter iter = {
		.type            = ITERATOR_VECTOR,
		.data.contiguous = { .array = array, .size = size }
	};

	return iter;
}

Iter end_vector(const Vector *vector)
{
	const size_t size  = vector->size;
	void        *array = back_vector(vector) + size;

	Iter iter = {
		.type            = ITERATOR_VECTOR,
		.data.contiguous = { .array = array, .size = size }
	};

	return iter;
}

Iter rbegin_vector(const Vector *vector)
{
	const size_t size  = vector->size;
	void        *array = back_vector(vector);

	Iter iter = {
		.type            = ITERATOR_VECTOR_REVERSE,
		.data.contiguous = { .array = array, .size = size }
	};

	return iter;
}

Iter rend_vector(const Vector *vector)
{
	const size_t size  = vector->size;
	void        *array = front_vector(vector) - size;

	Iter iter = {
		.type            = ITERATOR_VECTOR_REVERSE,
		.data.contiguous = { .array = array, .size = size }
	};

	return iter;
}

bool empty_vector(const Vector *vector)
{
	return generic_empty(vector->nmemb);
}

size_t size_vector(const Vector *vector)
{
	return generic_size(vector->nmemb);
}

size_t capacity_vector(const Vector *vector)
{
	return generic_capacity(vector->capacity);
}

void reserve_vector(Vector *vector, const size_t amount)
{
	generic_mempool_reserve(&vector->array,
	                        &vector->capacity,
	                        vector->size,
	                        amount);
}

void shrink_to_fit_vector(Vector *vector)
{
	generic_mempool_shrink_to_fit(&vector->array,
	                              &vector->capacity,
	                              vector->nmemb,
	                              vector->size);
}

Span span_from_vector(Vector *vector)
{
	void *data = vector->array;
	const size_t size = vector->size;
	const size_t nmemb = vector->nmemb;

	return make_span(data, size, nmemb);
}

Span span_from_vector_slice(Vector *vector, size_t start, size_t end)
{
	const size_t size = vector->size;
	void *data = vector->array + start * size;
	const size_t nmemb = end - start;

	return make_span(data, size, nmemb);
}
