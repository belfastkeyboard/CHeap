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
    size_t n = count(string,
                     search);

    size_t s = search->len;
    size_t r = replace->len;
    size_t len = string->len - (s - r) * n;

    if (len + 1 > string->capacity)
    {
        string_resize(string,
                      len + 1);
    }

    char *buffer = calloc(len,
                          sizeof(char));

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

void reverse(char *string)
{
    char *copy = strdup(string);

    size_t len = strlen(copy);

    for (size_t i = len - 1, j = 0; i < len; i--, j++)
        string[j] = copy[i];

    free(copy);
}

void capitalise(char *string)
{
    char c = string[0];
    c = (char)toupper((unsigned char)c);
    string[0] = c;
}

void lower(char *string)
{
    size_t len = strlen(string);
    for (size_t i = 0; i < len; i++)
    {
        char c = string[i];
        c = (char)tolower((unsigned char)c);
        string[i] = c;
    }
}

void upper(char *string)
{
    size_t len = strlen(string);
    for (size_t i = 0; i < len; i++)
    {
        char c = string[i];
        c = (char)toupper((unsigned char)c);
        string[i] = c;
    }
}

void title(char *string)
{
    size_t len = strlen(string);
    for (size_t i = 0; i < len; i++)
    {
        if (i == 0)
        {
            char c = string[i];
            c = (char)toupper((unsigned char)c);
            string[i] = c;
        }
        else if (string[i] == ' ')
        {
            char c = string[i+1];
            c = (char)toupper((unsigned char)c);
            string[i+1] = c;
        }
    }
}

void swapcase(char *string)
{
    size_t len = strlen(string);
    for (size_t i = 0; i < len; i++)
    {
        char c = string[i];
        if (isupper(c))
            string[i] = (char)tolower((unsigned char)c);
        else if (islower(c))
            string[i] = (char)toupper((unsigned char)c);
    }
}

void lstrip(char *string)
{
    size_t count = 0;
    size_t len = strlen(string);

    for (size_t i = 0; isspace(string[i]); i++)
        count++;

    if (count)
    {
        memmove(string, string+count, len-count);
        string[len-count] = '\0';
    }
}

void rstrip(char *string)
{
    size_t count = 0;
    size_t len = strlen(string);

    for (size_t i = len-1; isspace(string[i]) || i > len; i--)
        count++;

    if (count)
    {
        string[len-count] = '\0';
    }
}

void strip(char *string)
{
    rstrip(string);
    lstrip(string);
}

void truncate(char *string, size_t size)
{
    if (strlen(string) > size)
    {
        string[size] = '\0';

        size_t len = strlen(string);

        for (size_t i = len-1; i < len && i > len - 4; i--)
            string[i] = '.';
    }
}

bool is_lower(const char *string)
{
    bool result = true;

    while (result && *string)
    {
        char c = *string;
        if (isalpha(c))
            result = islower(c);
        string++;
    }

    return result;
}
bool is_upper(const char *string)
{
    bool result = true;

    while (result && *string)
    {
        char c = *string;
        if (isalpha(c))
            result = isupper(c);
        string++;
    }

    return result;
}
bool is_title(const char *string)
{
    bool result = true;

    size_t len = strlen(string);
    bool check_upper = true;

    for (size_t i = 0; result && i < len; i++)
    {
        char c = string[i];

        if (isalpha(c))
        {
            result = (check_upper) ? isupper(c) : islower(c);
            check_upper = false;
        }
        else if (isblank(c))
        {
            check_upper = true;
        }
    }

    return result;
}
bool starts_with(const char *string, const char *prefix)
{
    return strncmp(string, prefix, strlen(prefix)) == 0;
}
bool ends_with(const char *string, const char *suffix)
{
    return strcmp(&string[strlen(string) - strlen(suffix)], suffix) == 0;
}
bool is_alnum(const char *string)
{
    bool result = true;
    size_t len = strlen(string);

    for (size_t i = 0; result && i < len; i++)
    {
        result = isalnum(string[i]);
    }

    return result;
}
bool is_alpha(const char *string)
{
    bool result = true;
    size_t len = strlen(string);

    for (size_t i = 0; result && i < len; i++)
    {
        result = isalpha(string[i]);
    }

    return result;
}
bool is_ascii(const char *string)
{
    bool result = true;
    size_t len = strlen(string);

    for (size_t i = 0; result && i < len; i++)
    {
        result = isascii(string[i]);
    }

    return result;
}
bool is_decimal(const char *string)
{
    bool result = true;
    size_t len = strlen(string);

    for (size_t i = 0; result && i < len; i++)
        result = isdigit(string[i]);

    return result;
}
bool is_printable(const char *string)
{
    bool result = true;
    size_t len = strlen(string);

    for (size_t i = 0; result && i < len; i++)
        result = isprint(string[i]);

    return result;
}
bool is_space(const char *string)
{
    bool result = true;
    size_t len = strlen(string);

    for (size_t i = 0; result && i < len; i++)
        result = isspace(string[i]);

    return result;
}

*/

size_t count(const String *string,
             const String *value)
{
    size_t count = 0;
    size_t len = value->len;

    const char *substr = string->text;

    while ((substr = strstr(substr, value->text)))
    {
        count++;
        substr += len;
    }

    return count;
}

size_t find(String *string,
            const String *value)
{
    size_t count = NOT_FOUND;
    char *substr = NULL;

    if (string)
    {
        if ((substr = strstr(string, value)))
            count = substr - string;
    }

    return count;
}

/*
size_t rfind(const char *string, const char *value)
{
    size_t count = NOT_FOUND;
    size_t len = strlen(value);
    const char *substr = string;

    while ((substr = strstr(substr, value)))
    {
        count = substr - string;
        substr += len;
    }

    return count;
}
size_t word_count(const char *string)
{
    size_t count = 0;
    size_t len = strlen(string);
    bool on_word = false;

    for (size_t i = 0; i < len; i++)
    {
        char c = string[i];

        if (isspace(c))
        {
            if (on_word)
                on_word = false;
        }
        else
        {
            if (!on_word)
            {
                on_word = true;
                count++;
            }
        }
    }

    return count;
}*/
