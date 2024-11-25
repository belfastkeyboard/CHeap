#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "../../internals/base.h"
#include "../../string_t.h"
#include <assert.h>

#define SHORT_STRING_LEN 16
#define GROWTH_POLICY    2


typedef struct String
{
    char *text;
    union
    {
        char *heap;
        char stack[SHORT_STRING_LEN];
    };
    size_t len;
    size_t capacity;
} String;

static size_t null_terminate(const size_t len)
{
    return len + 1;
}

static size_t string_write(char *dest,
                         const char *src,
                         const size_t len)
{
    strncpy(dest, src, len);
    dest[len] = '\0';

    return len;
}

static size_t resize_policy(size_t capacity,
                            const size_t old_capacity)
{
    if (capacity > old_capacity && capacity < old_capacity * GROWTH_POLICY)
    {
        capacity = old_capacity * GROWTH_POLICY;
    }

    return null_terminate(capacity);
}


String *create_string(const char *text)
{
    String *string = memory_allocate_container(sizeof(String));

    const size_t len = strlen(text);

    if (len < SHORT_STRING_LEN)
    {
        string->len = string_write(string->stack, text, len);
        string->capacity = SHORT_STRING_LEN;
        string->text = string->stack;
    }
    else
    {
        string->capacity = resize_policy(len, string->capacity);
        string->heap = malloc(string->capacity);
        string->len = string_write(string->heap, text, len);
        string->text = string->heap;
    }

    return string;
}

void destroy_string(String **string)
{
    if ((*string)->capacity > SHORT_STRING_LEN)
    {
        free((*string)->heap);
    }

    free(*string);
    *string = NULL;
}

static void string_resize(String *string,
                          const size_t size)
{
    size_t new_capacity = resize_policy(size,
                                        string->capacity);

    if (string->capacity > SHORT_STRING_LEN)
    {
        string->heap = realloc(string->heap,
                             new_capacity);

        assert(string->heap);
    }
    else
    {
        char *tmp = calloc(new_capacity,
                           sizeof(char));

        assert(tmp);

        strncpy(tmp, string->stack, string->len);

        string->heap = tmp;
        string->text = string->heap;
    }

    string->capacity = new_capacity;
}


void append_string(String *dest, const String *src)
{
    if (dest->len + src->len >= dest->capacity)
    {
        string_resize(dest, dest->len + src->len);
    }

    dest->len += string_write(dest->text + dest->len, src->text, src->len);
}

void insert(String *dest,
            const String *src,
            const size_t index)
{
    if (dest->len + src->len >= dest->capacity)
    {
        string_resize(dest, dest->len + src->len);
    }

    memmove(dest->text + index + src->len, dest->text + index, dest->len - index);
    strncpy(dest->text + index, src->text, src->len);
}

char *replace(String *string,
              const String *search,
              const String *replace)
{
    size_t n = count(string, search);
    size_t s = strlen(search);
    size_t r = strlen(replace);
    size_t len = strlen(string) - (s - r) * n;

    if (len + 1 > string_builder->size)
        string_builder->buffer = realloc_string(len + 1);

    char *buffer = calloc(len, sizeof(char));

    strcpy(string_builder->buffer, string);

    for (size_t i = 0; i < n; i++)
    {
        size_t index = find(string_builder->buffer, search);

        if (index == NOT_FOUND)
            continue;

        char *ptr = NULL;

        ptr = stpncpy(buffer, string_builder->buffer, index);
        ptr = stpncpy(ptr, replace, r);
        strcat(ptr, &string_builder->buffer[index + s]);

        strcpy(string_builder->buffer, buffer);
        memset(buffer, 0, len);
    }

    free(buffer);

    return string_builder->buffer;
}

/*
char *join(char **strings, size_t size, const char *delim)
{
    char *string = strings[0];

    for (size_t i = 1; i < size; i++)
    {
        string = append(string, delim);
        string = append(string, strings[i]);
    }

    return  string;
}
char *lpad(const char *string, size_t pad)
{
    assert(string_builder && "String builder is not initialised. Call create_string_builder() first.");

    size_t len = strlen(string);

    char *copy = strdup(string);

    if (len + pad + 1 > string_builder->size)
        string_builder->buffer = realloc_string(len + pad + 1);

    memset(string_builder->buffer, ' ', pad);
    string_builder->buffer[pad] = '\0';
    strcat(string_builder->buffer, copy);

    free(copy);

    return string_builder->buffer;
}
char *rpad(const char *string, size_t pad)
{
    assert(string_builder && "String builder is not initialised. Call create_string_builder() first.");

    size_t len = strlen(string);

    if (len + pad + 1 > string_builder->size)
        string_builder->buffer = realloc_string(len + pad + 1);

    memmove(string_builder->buffer, string, len);
    memset(string_builder->buffer + len, ' ', pad);
    string_builder->buffer[len + pad] = '\0';

    return string_builder->buffer;
}
*/

void print(const String *string)
{
    printf("%s", string->text);
}
