#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../builder.h"

#define INIT_BUFFER_SZ 256
#define INVALID (-1)
#define BUFFER_INCREASE_FACTOR 2

struct StringBuilder
{
    char *buffer;
    size_t size;

    char **multi;
    size_t m_size;
};

static struct StringBuilder *string_builder = NULL;

static char *realloc_string(size_t size)
{
    assert(string_builder && "String builder is not initialised. Call create_string_builder() first.");

    if (string_builder->size * BUFFER_INCREASE_FACTOR > size)
        string_builder->size = string_builder->size * BUFFER_INCREASE_FACTOR;
    else
        string_builder->size = size + (unsigned long)((double)size * 0.1);

    return realloc(string_builder->buffer, string_builder->size);
}
static char **realloc_string_array(size_t size)
{
    if (string_builder->multi)
    {
        for (size_t i = 0; i < string_builder->m_size; i++)
            free(string_builder->multi[i]);

        return realloc(string_builder->multi, size * sizeof(char*));
    }
    else
        return malloc(size * sizeof(char*));
}

void create_string_builder(void)
{
    string_builder = malloc(sizeof(*string_builder));

    string_builder->buffer = calloc(INIT_BUFFER_SZ, sizeof(char));
    string_builder->size = INIT_BUFFER_SZ;

    string_builder->multi = NULL;
    string_builder->m_size = 0;
}
void destroy_string_builder(void)
{
    assert(string_builder && "String builder is not initialised. Call create_string_builder() first.");

    if (string_builder->buffer)
        free(string_builder->buffer);

    if (string_builder->multi)
    {
        for (size_t i = 0; i < string_builder->m_size; i++)
            free(string_builder->multi[i]);

        free(string_builder->multi);
    }

    string_builder->size = INVALID;
    string_builder->m_size = INVALID;

    free(string_builder);
    string_builder = NULL;
}

char *make_writable(const char *string)
{
    assert(string_builder && "String builder is not initialised. Call create_string_builder() first.");

    if (strlen(string) > string_builder->size)
        string_builder->buffer = realloc_string(strlen(string) + 1);

    strcpy(string_builder->buffer, string);

    return string_builder->buffer;
}
char *get_buffer(void)
{
    assert(string_builder && "String builder is not initialised. Call create_string_builder() first.");

    return string_builder->buffer;
}

char **split(const char *string, const char *delim, size_t *size)
{
    char *tokens = strdup(string);
    if (!string_builder->multi || strlen(string) > string_builder->m_size)
        string_builder->multi = realloc_string_array(strlen(string));

    *size = 0;
    char *tok = strtok(tokens, delim);
    while (tok)
    {
        string_builder->multi[*size] = calloc(strlen(tok) + 1, sizeof(char));
        strcpy(string_builder->multi[*size], tok);

        (*size)++;
        tok = strtok(NULL, delim);
    }
    string_builder->m_size = *size;

    free(tokens);

    return string_builder->multi;
}

char *append(const char *dest, const char *src)
{
    assert(string_builder && "String builder is not initialised. Call create_string_builder() first.");

    size_t len_d = strlen(dest);
    size_t len_s = strlen(src);

    if (len_d + len_s > string_builder->size)
        string_builder->buffer = realloc_string(len_d + len_s + 1);

    memmove(string_builder->buffer, dest, strlen(dest));
    strcat(string_builder->buffer, src);

    return string_builder->buffer;
}
char *insert(const char *string, size_t index, const char *value)
{
    assert(string_builder && "String builder is not initialised. Call create_string_builder() first.");

    size_t len_d = strlen(string);
    size_t len_s = strlen(value);

    if (len_d + len_s > string_builder->size)
        string_builder->buffer = realloc_string(len_d + len_s + 1);

    char *ptr = NULL;

    ptr = stpncpy(string_builder->buffer, string, index);
    ptr = stpcpy(ptr, value);
    strcpy(ptr, &string[index]);

    return string_builder->buffer;
}
char *replace(const char *string, const char *search, const char *replace)
{
    assert(string_builder && "String builder is not initialised. Call create_string_builder() first.");

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

size_t count(const char *string, const char *value)
{
    size_t count = 0;
    size_t len = strlen(value);

    while ((string = strstr(string, value)))
    {
        count++;
        string += len;
    }

    return count;
}
size_t find(const char *string, const char *value)
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
}
