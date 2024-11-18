#pragma once

#include <stddef.h>

typedef struct String String;

String *create_string(const char *text);
void destroy_string(String **string);

char *append(const char *dest, const char *src);
char *insert(const char *string, size_t index, const char *value);
char *replace(const char *string, const char *find, const char *replace);
char *join(char **strings, size_t size, const char *delim);
char *lpad(const char *string, size_t pad);
char *rpad(const char *string, size_t pad);

void print(const String *string);
