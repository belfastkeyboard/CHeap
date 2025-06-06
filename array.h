#pragma once

#include <stdbool.h>
#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

typedef struct Array Array;

ALLOC Array *create_array(size_t size, size_t nmemb);
void         destroy_array(Array **array);

void set_array(Array *array, const void *value, size_t index);

void *at_array(const Array *array, size_t index);
void *front_array(const Array *array);
void *back_array(const Array *array);

#ifdef CHEAP_ITERATOR_AVAILABLE
Iter begin_array(const Array *array);
Iter end_array(const Array *array);

Iter rbegin_array(const Array *array);
Iter rend_array(const Array *array);
#endif

bool   empty_array(const Array *array);
size_t size_array(const Array *array);

#ifdef CHEAP_SPAN_AVAILABLE
/**
 * @brief Derive a @p Span object giving an abstract view onto a contiguous
 * array
 *
 * @param array The Array object
 * @return @p Span object representing the full length of the array
 */
Span span_from_array(Array *array);

/**
 * @brief Derive a @p Span object giving an abstract view onto a contiguous
 * array
 *
 * @param array The Array object
 * @param start The index of the start of the slice
 * @param size_t The index of the end of the slice
 * @return @p Span object representing a slice of the array
 */
Span span_from_array_slice(Array *array, size_t start, size_t end);
#endif
