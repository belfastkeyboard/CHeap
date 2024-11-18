#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "../../internals/base.h"
#include "../../string_t.h"

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

static void string_write(char *dest,
                         const char *src,
                         const size_t len)
{
    strncpy(dest, src, len);
    dest[len] = '\0';
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

    string->len = strlen(text);

    if (string->len < SHORT_STRING_LEN)
    {
        string_write(string->stack, text, string->len);
        string->capacity = SHORT_STRING_LEN;
        string->text = string->stack;
    }
    else
    {
        string->capacity = resize_policy(string->len, string->capacity);
        string->heap = malloc(string->capacity);
        string_write(string->heap, text, string->len);
        string->text = string->heap;
    }

    return string;
}

void destroy_string(String **string)
{
    if ((*string)->capacity >= SHORT_STRING_LEN)
    {
        free((*string)->heap);
    }

    free(*string);
    *string = NULL;
}


void print(const String *string)
{
    printf("%s", string->text);
}
