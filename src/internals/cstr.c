#include <assert.h>
#include <stdint.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>
#include "../../cstr.h"


#define METADATA_ITEM_SIZE sizeof(uint32_t)
#define NULLTERM_SIZE      1
#define METADATA_SIZE      (METADATA_ITEM_SIZE * 2)
#define NON_STRING_SIZE    (METADATA_SIZE + NULLTERM_SIZE)

#define BUFFER(str)  (str)  ? (str - METADATA_SIZE)      : NULL
#define LENGTH(str)  (str)  ? (str - METADATA_ITEM_SIZE) : NULL
#define STRING(meta) (meta) ? (meta + METADATA_SIZE)     : NULL


typedef char *Buffer;
typedef const char *ConstBuffer;

typedef char *(*AllocationStrategy)(String, uint32_t);
typedef String (*WriteStrategy)(String, const void*, uint32_t, void*);


//              SCHEMA             \\
// |----|----|------------|------| \\
// |BUFF|LEN |   STRING   | FREE | \\
// |----|----|------------|------| \\


static void *stdlib_allocate(void*,
                             uint32_t size)
{
    return malloc(size);
}

static void *stdlib_realloc(void*,
                            void *ptr,
                            uint32_t size)
{
    void *tmp = realloc(ptr, size);

    assert(tmp);

    return tmp;
}

void *arena_string_alloc(void *arena, uint32_t size)
{
    return alloc_arena(arena,
                       size);
}

void *arena_string_realloc(void *arena, void *src, uint32_t size)
{
    void *ptr = alloc_arena(arena,
                            size);

    memcpy(ptr,
           src,
           size);

    return ptr;
}


static uint32_t max(uint32_t a,
                    uint32_t b)
{
    return (a > b) ? a :
                     b;
}


static void write_buffer_size(Buffer meta,
                              const uint32_t size)
{
    memcpy(meta,
           &size,
           sizeof(size));
}

static void write_string_len(Buffer meta,
                             const uint32_t len)
{
    memcpy(meta,
           &len,
           sizeof(len));
}


static uint32_t get_metadata_item(ConstBuffer meta)
{
    uint32_t sz;

    memcpy(&sz,
           meta,
           sizeof(uint32_t));

    return sz;
}

uint32_t string_buffer(ConstString string)
{
    return get_metadata_item(BUFFER(string));
}

uint32_t string_len(ConstString string)
{
    return get_metadata_item(LENGTH(string));
}


static String null_terminate(String string)
{
    const uint32_t n = string_len(string);
    string[n] = '\0';
    return string;
}


__attribute__((warn_unused_result))
static char *allocate(String string,
                      const uint32_t sz)
{
    return allocator_callback(string_allocator,
                              NON_STRING_SIZE + sz);
}

__attribute__((warn_unused_result))
static char *reallocate(String string,
                        const uint32_t sz)
{
    char *ptr = reallocator_callback(string_allocator,
                                     BUFFER(string),
                                     NON_STRING_SIZE + sz);

    assert(ptr);

    return ptr;
}


__attribute__((warn_unused_result))
static String generic_allocation(String string,
                                 const uint32_t sz,
                                 AllocationStrategy strategy)
{
    char *buff = strategy(string,
                          sz);

    write_buffer_size(buff,
                      sz);

    return STRING(buff);
}


__attribute__((warn_unused_result))
static String write_formatted(String string,
                              const void *src,
                              const uint32_t len,
                              void* data)
{
    va_list *args = data;

    vsnprintf(string,
              len + 1,
              src,
              *args);

    return string;
}

__attribute__((warn_unused_result()))
static String write_stream(String string,
                           const void *src,
                           const uint32_t len,
                           void *data)
{
    fread(string,
          sizeof(char),
          len,
          (FILE*)src);

    return string;
}

__attribute__((warn_unused_result))
static String generic_write(String string,
                            const void *src,
                            const uint32_t len,
                            void *data,
                            WriteStrategy strategy)
{
    string = strategy(string,
                      src,
                      len,
                      data);

    write_string_len(LENGTH(string),
                     len);

    return null_terminate(string);
}



void set_string_allocator(void *allocator,
                          AllocatorCallback cb_allocate,
                          ReallocatorCallback cb_reallocate)
{
    string_allocator = allocator;
    allocator_callback = cb_allocate;
    reallocator_callback = cb_reallocate;
}

void reset_string_allocator(void)
{
    string_allocator = NULL;
    allocator_callback = stdlib_allocate;
    reallocator_callback = stdlib_realloc;
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

    String string = generic_allocation(NULL,
                                       len,
                                       allocate);

    string = generic_write(string,
                           fmt,
                           len,
                           args,
                           write_formatted);

    va_end(args);

    return string;
}

void string_free(String string)
{
    char *buff = BUFFER(string);

    free(buff);
}

String string_from_stream(FILE *stream)
{
    fseek(stream,
          0,
          SEEK_END);

    long n = ftell(stream);
    String string = generic_allocation(NULL,
                                       n,
                                       allocate);

    rewind(stream);

    string = generic_write(string,
                           stream,
                           n,
                           NULL,
                           write_stream);

    return string;
}


String string_cpy(String restrict dest,
                  ConstString restrict src)
{
    const size_t sz = string_len(src);

    if (string_buffer(dest) < sz)
    {
        dest = generic_allocation(dest,
                                  sz,
                                  reallocate);
    }

    memset(dest,
           0,
           sz + 1);

    const size_t n = string_len(src);

    memcpy(dest,
           src,
           n);

    return dest;
}

String string_cat(String restrict dest,
                  ConstString restrict src)
{
    const uint32_t req = string_len(src);
    const uint32_t buffsz = string_buffer(dest);
    const uint32_t len = string_len(dest);

    if (buffsz - len < req)
    {
        const uint32_t sz = max(req,
                                buffsz * 2);

        dest = generic_allocation(dest,
                                  sz,
                                  reallocate);
    }

    const uint32_t new_len = req + len;

    memcpy(dest + len,
           src,
           req);

    write_string_len(LENGTH(dest),
                     new_len);

    return null_terminate(dest);
}

String string_dup(ConstString restrict src)
{
    const size_t sz = string_buffer(src);

    String dup = generic_allocation(NULL,
                                    sz,
                                    allocate);

    return string_cpy(dup,
                      src);
}


int string_cmp(ConstString str1,
               ConstString str2)
{
    return strcmp(str1,
                  str2);
}


StringView string_chr(ConstString string,
                      int c)
{
    return strchr(string,
                  c);
}

StringView string_rchr(ConstString string,
                       int c)
{
    return strrchr(string,
                   c);
}

StringView string_pbrk(ConstString string,
                       ConstString accept)
{
    return strpbrk(string,
                   accept);
}

StringView string_str(ConstString haystack,
                      ConstString needle)
{
    return strstr(haystack,
                  needle);
}

uint32_t string_cspn(ConstString string,
                   ConstString reject)
{
    return strcspn(string,
                   reject);
}

uint32_t string_spn(ConstString string,
                  ConstString accept)
{
    return strspn(string,
                  accept);
}
