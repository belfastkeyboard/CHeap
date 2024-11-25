#pragma once

#include <stddef.h>

typedef struct String String;

String *create_string(const char *text);
void destroy_string(String **string);


void append_string(String *dest,
                   const String *src);

void insert(String *dest,
            const String *src,
            size_t index);

char *replace(String *string,
              const String *find,
              const String *replace);

/*
char *join(char **strings, size_t size, const char *delim);
char *lpad(const char *string, size_t pad);
char *rpad(const char *string, size_t pad);
*/

void print(const String *string);
