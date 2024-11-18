#pragma once

#include <stddef.h>

typedef struct String String;

String *create_string(const char *text);
void destroy_string(String **string);

void print_string(String string);
