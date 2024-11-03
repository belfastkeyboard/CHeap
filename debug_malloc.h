#pragma once

#include <stddef.h>

void *debug_malloc(size_t size,
                   const char *text);

void *debug_realloc(void *data,
                    size_t size,
                    const char *text);

void debug_free(void *ptr,
                size_t size,
                const char *text);
