#pragma once

#include <stdbool.h>
#include <stddef.h>

#define SEQ_CONTAINER_INIT(cntr) cntr->nmemb = 0; cntr->capacity = 0; cntr->size = size; cntr->array = NULL;

void *memory_allocate_container(size_t size);
void memory_free_buffer(void *buffer);
void memory_free_container(void **container);

bool container_empty(size_t nmemb);
size_t container_size(size_t nmemb);
size_t container_capacity(size_t capacity);

__attribute__((warn_unused_result()))
void *sequential_realloc(void *array, size_t capacity, size_t size);
__attribute__((warn_unused_result()))
void *sequential_resize(void *array, size_t *capacity, size_t size);
void sequential_insert(void *array, size_t index, void *item, size_t nmemb, size_t size);
size_t sequential_remove(void *array, size_t index, size_t nmemb, size_t size);
void sequential_clear(void *array, size_t capacity, size_t size);
void *sequential_index_access(void *array, size_t index, size_t size);
