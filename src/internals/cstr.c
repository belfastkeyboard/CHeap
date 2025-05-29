#include "../../cstr.h"
#include "../../arena.h"
#include <assert.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#define METADATA_ITEM_SIZE sizeof(uint32_t)
#define NULLTERM_SIZE      1
#define METADATA_SIZE      (METADATA_ITEM_SIZE * 2)
#define NON_STRING_SIZE    (METADATA_SIZE + NULLTERM_SIZE)

#define BUFFER(str)  (str) ? (str - METADATA_SIZE) : NULL
#define LENGTH(str)  (str) ? (str - METADATA_ITEM_SIZE) : NULL
#define STRING(meta) (meta) ? (meta + METADATA_SIZE) : NULL

typedef char       *Buffer;
typedef const char *ConstBuffer;

typedef char *(*AllocationStrategy)(String, uint32_t, Arena *);
typedef String (*WriteStrategy)(String, const void *, uint32_t, void *);

//              SCHEMA             \\
// |----|----|------------|------| \\
// |BUFF|LEN |   STRING   | FREE | \\
// |----|----|------------|------| \\

ALLOC static char *stdlib_alloc(String, const uint32_t sz, Arena *)
{
	return malloc(sz);
}

ALLOC static char *stdlib_realloc(String string, const uint32_t sz, Arena *)
{
	char *ptr = realloc(BUFFER(string), sz);

	assert(ptr);

	return ptr;
}

ALLOC static char *arena_alloc(String, const uint32_t sz, Arena *arena)
{
	return alloc_arena(arena, sz);
}

ALLOC static char *arena_realloc(String string, const uint32_t sz, Arena *arena)
{
	void *ptr = alloc_arena(arena, sz);

	memcpy(ptr, BUFFER(string), sz);

	return ptr;
}

static uint32_t max(uint32_t a, uint32_t b)
{
	return (a > b) ? a : b;
}

static void write_buffer_size(Buffer meta, const uint32_t size)
{
	memcpy(meta, &size, sizeof(size));
}

static void write_string_len(Buffer meta, const uint32_t len)
{
	memcpy(meta, &len, sizeof(len));
}

static uint32_t get_metadata_item(ConstBuffer meta)
{
	uint32_t sz;

	memcpy(&sz, meta, sizeof(uint32_t));

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
	string[n]        = '\0';
	return string;
}

ALLOC static String generic_allocation(String             string,
                                       const uint32_t     sz,
                                       AllocationStrategy strategy,
                                       Arena             *arena)
{
	char *buff = strategy(string, NON_STRING_SIZE + sz, arena);

	write_buffer_size(buff, sz);

	return STRING(buff);
}

ALLOC static String
write_formatted(String string, const void *src, const uint32_t len, void *data)
{
	va_list *args = data;

	vsnprintf(string, len + 1, src, *args);

	return string;
}

ALLOC static String
write_stream(String string, const void *src, const uint32_t len, void *)
{
	fread(string, sizeof(char), len, (FILE *)src);

	return string;
}

ALLOC static String generic_write(String         string,
                                  const void    *src,
                                  const uint32_t len,
                                  void          *data,
                                  WriteStrategy  strategy)
{
	string = strategy(string, src, len, data);

	write_string_len(LENGTH(string), len);

	return null_terminate(string);
}

ALLOC static String string_construct_formatted(const char        *fmt,
                                               va_list            args,
                                               va_list            len_args,
                                               AllocationStrategy strategy,
                                               Arena             *arena)
{
	const size_t len    = vsnprintf(NULL, 0, fmt, len_args);
	String       string = generic_allocation(NULL, len, strategy, arena);

	return generic_write(string, fmt, len, args, write_formatted);
}

ALLOC static String string_construct_from_stream(FILE              *stream,
                                                 AllocationStrategy strategy,
                                                 Arena             *arena)
{
	fseek(stream, 0, SEEK_END);

	long   n      = ftell(stream);
	String string = generic_allocation(NULL, n, strategy, arena);

	rewind(stream);

	string = generic_write(string, stream, n, NULL, write_stream);

	return string;
}

ALLOC static String string_copy(restrict String      dest,
                                restrict ConstString src,
                                AllocationStrategy   strategy,
                                Arena               *arena)
{
	const size_t n = string_len(src);

	if (string_buffer(dest) < n) {
		dest = generic_allocation(dest, n, strategy, arena);
	}

	memset(dest, 0, n + 1);
	memcpy(dest, src, n);

	return dest;
}

ALLOC static String string_concatenate(restrict String      dest,
                                       restrict ConstString src,
                                       AllocationStrategy   strategy,
                                       Arena               *arena)
{
	const uint32_t req    = string_len(src);
	const uint32_t buffsz = string_buffer(dest);
	const uint32_t len    = string_len(dest);

	if (buffsz - len < req) {
		const uint32_t sz = max(req, buffsz * 2);
		dest              = generic_allocation(dest, sz, strategy, arena);
	}

	const uint32_t new_len = req + len;

	memcpy(dest + len, src, req);
	write_string_len(LENGTH(dest), new_len);

	return null_terminate(dest);
}

ALLOC static String string_duplicate(ConstString        src,
                                     AllocationStrategy strategy,
                                     Arena             *arena)
{
	const size_t sz  = string_buffer(src);
	String       dup = generic_allocation(NULL, sz, strategy, arena);

	memcpy(BUFFER(dup), BUFFER(src), NON_STRING_SIZE + sz);

	return dup;
}

String string_new(const char *fmt, ...)
{
	va_list args;
	va_list len_args;
	va_start(args, fmt);
	va_copy(len_args, args);

	String string = string_construct_formatted(fmt,
	                                           args,
	                                           len_args,
	                                           stdlib_alloc,
	                                           NULL);

	va_end(args);

	return string;
}

String string_from_stream(FILE *stream)
{
	return string_construct_from_stream(stream, stdlib_alloc, NULL);
}

void string_free(String string)
{
	char *buff = BUFFER(string);

	free(buff);
}

String string_cpy(String restrict dest, ConstString restrict src)
{
	return string_copy(dest, src, stdlib_realloc, NULL);
}

String string_cat(String restrict dest, ConstString restrict src)
{
	return string_concatenate(dest, src, stdlib_realloc, NULL);
}

String string_dup(ConstString restrict src)
{
	return string_duplicate(src, stdlib_alloc, NULL);
}

int string_cmp(ConstString str1, ConstString str2)
{
	return strcmp(str1, str2);
}

StringView string_chr(ConstString string, int c)
{
	return strchr(string, c);
}

StringView string_rchr(ConstString string, int c)
{
	return strrchr(string, c);
}

StringView string_pbrk(ConstString string, ConstString accept)
{
	return strpbrk(string, accept);
}

StringView string_str(ConstString haystack, ConstString needle)
{
	return strstr(haystack, needle);
}

uint32_t string_cspn(ConstString string, ConstString reject)
{
	return strcspn(string, reject);
}

uint32_t string_spn(ConstString string, ConstString accept)
{
	return strspn(string, accept);
}

/* ARENA VERSIONS */
ALLOC FORMAT_EXT String arena_string_new(Arena *arena, const char *fmt, ...)
{
	va_list args;
	va_list len_args;
	va_start(args, fmt);
	va_copy(len_args, args);

	String string = string_construct_formatted(fmt,
	                                           args,
	                                           len_args,
	                                           arena_alloc,
	                                           arena);

	va_end(args);

	return string;
}

ALLOC String arena_string_from_stream(Arena *arena, FILE *stream)
{
	return string_construct_from_stream(stream, arena_alloc, arena);
}

ALLOC String arena_string_cpy(Arena *arena, String dest, ConstString src)
{
	return string_copy(dest, src, arena_realloc, arena);
}

ALLOC String arena_string_cat(Arena *arena, String dest, ConstString src)
{
	return string_concatenate(dest, src, arena_realloc, arena);
}

ALLOC String arena_string_dup(Arena *arena, ConstString src)
{
	return string_duplicate(src, arena_alloc, arena);
}
