#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "internals/iter.h"

#ifdef CHEAP_RANGE_AVAILABLE
#include "range.h"
#endif


typedef struct Vector Vector;


__attribute__((warn_unused_result))
Vector *create_vector(size_t size);

void destroy_vector(Vector **vector);


void push_back_vector(Vector *vector,
                      const void *value);

void insert_vector(Vector *vector,
                   const void *value,
                   size_t index);


#ifdef CHEAP_RANGE_AVAILABLE
void push_back_range_vector(Vector *vector,
                            const Range *range);

void insert_range_vector(Vector *vector,
                         size_t index,
                         const Range *range);

Range get_range_vector(const Vector *vector,
                       size_t begin,
                       size_t end);
#endif

void pop_back_vector(Vector *vector);

size_t erase_vector(Vector *vector,
                    size_t index);

void clear_vector(Vector *vector);


void *at_vector(const Vector *vector,
                size_t index);

void *front_vector(const Vector *vector);

void *back_vector(const Vector *vector);


Iter begin_vector(const Vector *vector);

Iter end_vector(const Vector *vector);


bool empty_vector(const Vector *vector);

size_t size_vector(const Vector *vector);

size_t capacity_vector(const Vector *vector);


void reserve_vector(Vector *vector,
                    size_t amount);

void shrink_to_fit_vector(Vector *vector);
