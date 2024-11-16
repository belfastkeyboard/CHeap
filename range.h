#pragma once

#include <stddef.h>

typedef struct Range
{
    const void *array;
    const size_t nmemb;
    const size_t size;
} Range;

Range create_range(const void *array,
                   size_t nmemb,
                   size_t size);
