#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "arena.h"

#define EMPTY 0
#define UNSET EMPTY

#define SEQUENTIAL_GROWTH 2
#define SEQUENTIAL_INIT 1

#define ARENA_CONTAINER_INIT_COUNT 8

void *memory_allocate_container(size_t size);
void memory_free_buffer(void **buffer);
void memory_free_container_mempool(void **container, void *array);
void memory_free_container_hash(void **container, void *buckets, void *keys, void *values);

bool generic_empty(size_t nmemb);
size_t generic_size(size_t nmemb);
size_t generic_capacity(size_t capacity);
