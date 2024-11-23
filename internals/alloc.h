#pragma once

#include <stddef.h>

void open_cheap_context(size_t size);

void close_cheap_context(void);


void *cheap_malloc_impl(size_t size);

void *cheap_calloc_impl(size_t size);

void *cheap_realloc_impl(void *ptr, size_t size);

void cheap_free_impl(void *ptr);

void cheap_clear_impl(void);
