#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "range.h"


typedef struct Array Array;


__attribute__((warn_unused_result))
Array *create_array(size_t size);

void destroy_array(Array **array);


void push_back_array(Array *array,
                     void *value);

void insert_array(Array *array,
                  void *value,
                  size_t index);


void push_back_range_array(Array *array,
                           Range *range);

void insert_range_array(Array *array,
                        size_t index,
                        Range *range);

Range get_range_array(Array *array,
                     size_t begin,
                     size_t end);


void pop_back_array(Array *array);

size_t erase_array(Array *array,
                   size_t index);

void clear_array(Array *array);


void *at_array(Array *array,
               size_t index);

void *front_array(Array *array);

void *back_array(Array *array);


bool empty_array(Array *array);

size_t size_array(Array *array);

size_t capacity_array(Array *array);


void reserve_array(Array *array,
                   size_t amount);

void shrink_to_fit_array(Array *array);
