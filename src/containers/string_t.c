#include <assert.h>
#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "../../vector.h"
#include "../../internals/base.h"
#include "../../string_t.h"

#define SHORT_STRING_LEN 16
#define GROWTH_POLICY    2
#define NOT_FOUND        ((size_t)-1)


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

static size_t l_find(const char *string,
                     const char *value)
{
    size_t index = NOT_FOUND;
    const char *substr = string;

    if (substr)
    {
        if ((substr = strstr(substr, value)))
            index = substr - string;
    }

    return index;
}

static size_t r_find(const String *string,
                     const String *value)
{
    size_t index = NOT_FOUND;
    const char *substr = string->text;

    while ((substr = strstr(substr, value->text)))
    {
        index = substr - string->text;
        substr += value->len;
    }

    return index;
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
    }

    string->text = string->heap;
    string->capacity = new_capacity;
}


void append_string(String *dest,
                   const String *src)
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

String *replace(String *string,
                const String *search,
                const String *replace)
{
    size_t index = NOT_FOUND;
    ssize_t diff = (ssize_t)(replace->len - search->len);

    while ((index = l_find(string->text, search->text)) != NOT_FOUND)
    {
        if (string->len + diff >= string->capacity)
        {
            string_resize(string, string->len + diff);
        }

        memmove(string->text + index + replace->len, string->text + index + search->len, string->len - index);
        strncpy(string->text + index, replace->text, replace->len);
        string->len += diff;
    }

    return string;
}

String *lpad(String *string,
             const size_t pad)
{
    if (string->len + pad > string->capacity)
    {
        string_resize(string, string->len + pad);
    }

    memmove(string->text + pad, string->text, string->len);
    memset(string->text, ' ', pad);

    string->len += pad;

    return string;
}

String *rpad(String *string,
             const size_t pad)
{
    if (string->len + pad > string->capacity)
    {
        string_resize(string, string->len + pad);
    }

    memset(string->text + string->len, ' ', pad);

    string->len += pad;

    return string;
}


String *reverse(String *string)
{
    char *copy = strdup(string->text);

    size_t len = strlen(copy);

    for (size_t i = len - 1, j = 0; i < len; i--, j++)
    {
        string->text[j] = copy[i];
    }

    free(copy);

    return string;
}

String *capitalise(String *string)
{
    char c = string->text[0];
    c = (char)toupper((unsigned char)c);
    string->text[0] = c;

    return string;
}

String *lower(String *string)
{
    for (size_t i = 0; i < string->len; i++)
    {
        char c = string->text[i];
        c = (char)tolower((unsigned char)c);
        string->text[i] = c;
    }

    return string;
}

String *upper(String *string)
{
    for (size_t i = 0; i < string->len; i++)
    {
        char c = string->text[i];
        c = (char)toupper((unsigned char)c);
        string->text[i] = c;
    }

    return string;
}

String *title(String *string)
{
    for (size_t i = 0; i < string->len; i++)
    {
        if (i == 0)
        {
            char c = string->text[i];
            c = (char)toupper((unsigned char)c);
            string->text[i] = c;
        }
        else if (string->text[i] == ' ')
        {
            char c = string->text[i+1];
            c = (char)toupper((unsigned char)c);
            string->text[i+1] = c;
            i++;
        }
        else if (isalpha(string->text[i]))
        {
            char c = string->text[i];
            c = (char)tolower((unsigned char)c);
            string->text[i] = c;
        }
    }

    return string;
}

String *swapcase(String *string)
{
    for (size_t i = 0; i < string->len; i++)
    {
        char c = string->text[i];

        if (isupper(c))
        {
            string->text[i] = (char)tolower((unsigned char)c);
        }
        else if (islower(c))
        {
            string->text[i] = (char)toupper((unsigned char)c);
        }
    }

    return string;
}

String *lstrip(String *string)
{
    size_t count = 0;

    for (; isspace(string->text[count]); count++);

    if (count)
    {
        memmove(string->text, string->text + count, string->len - count);

        string->len -= count;
        string->text[string->len] = '\0';
    }

    return string;
}

String *rstrip(String *string)
{
    size_t count = string->len - 1;

    for (; isspace(string->text[count]); count--);

    if (count < string->len - 1)
    {
        string->len = count;
        string->text[count + 1] = '\0';
    }

    return string;
}

String *strip(String *string)
{
    size_t count = 0;

    for (; isspace(string->text[count]); count++);

    if (count)
    {
        memmove(string->text, string->text + count, string->len - count);

        string->len -= count;
        string->text[string->len] = '\0';
    }

    count = string->len - 1;

    for (; isspace(string->text[count]); count--);

    if (count < string->len - 1)
    {
        string->len = count;
        string->text[count + 1] = '\0';
    }

    return string;
}

String *truncate(String *string,
                 const size_t size)
{
    if (string->len > size)
    {
        string->len = size;
        string->text[size + 1] = '\0';

        if (size > 3)
        {
            for (size_t i = string->len; i > 0 && i > string->len - 3; i--)
            {
                string->text[i] = '.';
            }
        }
    }

    return string;
}


bool is_lower(const String *string)
{
    bool result = true;

    const char *text = string->text;

    while (result && *text)
    {
        char c = *text;

        if (isalpha(c))
        {
            result = islower(c);
        }

        text++;
    }

    return result;
}

bool is_upper(const String *string)
{
    bool result = true;

    const char *text = string->text;

    while (result && *text)
    {
        char c = *text;

        if (isalpha(c))
        {
            result = isupper(c);
        }

        text++;
    }

    return result;
}

bool is_title(const String *string)
{
    bool result = true;
    bool check_upper = true;

    const char *text = string->text;

    while(result && *text)
    {
        char c = *text;

        if (isalpha(c))
        {
            result = (check_upper) ? isupper(c) :
                                     islower(c);

            check_upper = false;
        }
        else if (isblank(c))
        {
            check_upper = true;
        }

        text++;
    }

    return result;
}

bool starts_with(const String *string,
                 const String *prefix)
{
    return strncmp(string->text,
                   prefix->text,
                   prefix->len) == 0;
}

bool ends_with(const String *string,
               const String *suffix)
{
    return strcmp(&string->text[string->len - suffix->len],
                  suffix->text) == 0;
}

bool is_alnum(const String *string)
{
    bool result = true;

    for (size_t i = 0; result && i < string->len; i++)
    {
        result = isalnum(string->text[i]);
    }

    return result;
}

bool is_alpha(const String *string)
{
    bool result = true;

    for (size_t i = 0; result && i < string->len; i++)
    {
        result = isalpha(string->text[i]);
    }

    return result;
}

bool is_ascii(const String *string)
{
    bool result = true;

    for (size_t i = 0; result && i < string->len; i++)
    {
        result = isascii(string->text[i]);
    }

    return result;
}

bool is_decimal(const String *string)
{
    bool result = true;

    for (size_t i = 0; result && i < string->len; i++)
    {
        result = isdigit(string->text[i]);
    }

    return result;
}

bool is_printable(const String *string)
{
    bool result = true;

    for (size_t i = 0; result && i < string->len; i++)
    {
        result = isprint(string->text[i]);
    }

    return result;
}

bool is_space(const String *string)
{
    bool result = true;

    for (size_t i = 0; result && i < string->len; i++)
    {
        result = isspace(string->text[i]);
    }

    return result;
}

String *join(Vector *strings,
             const String *delim)
{
    String *string = create_string("");

    for (int i = 0; i < size_vector(strings); i++)
    {
        String *part = at_vector(strings, i);

        append_string(string, part);

        if (i < size_vector(strings) - 1)
        {
            append_string(string, delim);
        }
    }

    return string;
}


void print(const String *string)
{
    printf("%s", string->text);
}


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
    return l_find(string->text, value->text);
}

size_t rfind(const String *string,
             const String *value)
{
    return r_find(string, value);
}

size_t word_count(const String *string)
{
    size_t count = 0;
    bool on_word = false;

    for (size_t i = 0; i < string->len; i++)
    {
        char c = string->text[i];

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
}
