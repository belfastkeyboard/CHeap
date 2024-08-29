#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct Array Array;

Array *create_array(size_t size);
void destroy_array(Array **array);

void push_back(Array *array, void *item);
void insert(Array *array, void *item, size_t index);

void pop_back(Array *array);
size_t erase(Array *array, size_t index);
void clear(Array *array);

void *at(Array *array, size_t index);
void *front(Array *array);
void *back(Array *array);

bool empty(Array *array);
size_t size(Array *array);
size_t capacity(Array *array);

void reserve(Array *array, size_t amount);
void shrink(Array *array);
