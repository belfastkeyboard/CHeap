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

bool   empty_array(const Array *array);
size_t size_array(const Array *array);

#ifdef CHEAP_ITERATOR_AVAILABLE
Iter begin_array(const Array *array);
Iter end_array(const Array *array);
#endif
