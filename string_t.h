#pragma once

#include <stddef.h>

typedef struct String
{
    const char *text;
    size_t len;
} String;

// allocate a buffer with an allocator
String create_string(const char *text);

void print_string(String string);
