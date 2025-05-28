#include <malloc.h>
#include <stdarg.h>
#include <string.h>
#include "../../cstr.h"

#define METADATA_LEN 8
#define NULLTERM_LEN 1
#define STR_META_LEN (METADATA_LEN + NULLTERM_LEN)

#define METADATA(str) (str) ? (str - METADATA_LEN)  : NULL
#define STRING(meta) (meta) ? (meta + METADATA_LEN) : NULL


static void write_metadata(char *meta,
                           const size_t data)
{
    memcpy(meta,
           &data,
           sizeof(size_t));
}

__attribute__((warn_unused_result))
static String string_alloc(const size_t sz)
{
    char *buff = malloc(STR_META_LEN + sz);

    write_metadata(buff,
                   sz);

    String string = STRING(buff);

    return string;
}

__attribute__((warn_unused_result))
static String string_resize_buffer(String string,
                                   const size_t sz)
{
    string_free(string);

    return string_alloc(sz);
}


String string_new(const char *fmt,
                  ...)
{
    va_list args;
    va_list len_args;

    va_start(args,
             fmt);

    va_copy(len_args,
            args);

    const size_t len = vsnprintf(NULL,
                                 0,
                                 fmt,
                                 len_args);

    String string = string_alloc(len);

    vsnprintf(string,
              len + 1,
              fmt,
              args);

    va_end(args);

    return string;
}

void string_free(String string)
{
    char *buff = METADATA(string);

    free(buff);
}

String string_from_stream(FILE *stream)
{
    fseek(stream,
          0,
          SEEK_END);

    long n = ftell(stream);
    String string = string_alloc(n);

    rewind(stream);

    fread(string,
          sizeof(char),
          n,
          stream);

    string[n] = '\0';

    return string;
}


size_t string_size(ConstString string)
{
    size_t sz;
    const char *meta = METADATA(string);

    memcpy(&sz,
           meta,
           sizeof(size_t));

    return sz;
}

size_t string_len(ConstString string)
{
    const size_t max = string_size(string);

    return strnlen(string,
                   max);
}


String string_cpy(String restrict dest,
                  ConstString restrict src)
{
    const size_t sz = string_len(src);

    if (string_size(dest) < sz)
    {
        dest = string_resize_buffer(dest,
                                    sz);
    }

    memset(dest,
           0,
           sz + 1);

    const size_t n = strnlen(src, sz);

    strncpy(dest,
            src,
            n);

    return dest;
}

String string_dup(ConstString restrict src)
{
    const size_t sz = string_size(src);

    String dup = string_alloc(sz);

    return string_cpy(dup,
                      src);
}


int string_cmp(ConstString str1,
               ConstString str2)
{
    return strcmp(str1,
                  str2);
}
