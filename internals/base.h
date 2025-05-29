#pragma once

#include <stdbool.h>
#include <stddef.h>

#define EMPTY 0
#define UNSET EMPTY

#define SEQUENTIAL_GROWTH 2
#define SEQUENTIAL_INIT 1

#define ALLOC __attribute__((warn_unused_result))

ALLOC void *memory_allocate_container(size_t size);

void memory_free_buffer(void **buffer);
void memory_free_container_generic(void **container, void *array);

bool generic_empty(size_t nmemb);
size_t generic_size(size_t nmemb);
size_t generic_capacity(size_t capacity);
