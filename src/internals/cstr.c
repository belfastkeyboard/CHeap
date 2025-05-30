#include "../../cstr.h"
#include "../../arena.h"
#include <assert.h>
#include <ctype.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdbool.h>
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

typedef char *(*AllocationStrategy)(String, uint32_t, uint32_t, Arena *);
typedef String (*WriteStrategy)(String, const void *, uint32_t, void *);

//              SCHEMA             \\
// |----|----|------------|------| \\
// |BUFF|LEN |   STRING   | FREE | \\
// |----|----|------------|------| \\

ALLOC static char *stdlib_alloc(String, const uint32_t sz, uint32_t, Arena *)
{
	return calloc(sz, sizeof(char));
}

ALLOC static char *
stdlib_realloc(String string, const uint32_t sz, uint32_t old, Arena *)
{
	char *ptr = realloc(BUFFER(string), sz);

	assert(ptr);

	memset(ptr + old + METADATA_SIZE, 0, sz - old - METADATA_SIZE);

	return ptr;
}

ALLOC static char *
arena_alloc(String, const uint32_t sz, uint32_t, Arena *arena)
{
	return calloc_arena(arena, sz);
}

ALLOC static char *
arena_realloc(String string, const uint32_t sz, uint32_t, Arena *arena)
{
	void *ptr = calloc_arena(arena, sz);

	memcpy(ptr, BUFFER(string), sz);

	return ptr;
}

static uint32_t max(uint32_t a, uint32_t b)
{
	return (a > b) ? a : b;
}

static void write_buffer(Buffer meta, const uint32_t sz)
{
	memcpy(meta, &sz, sizeof(sz));
}

static void write_string_len(String string, const uint32_t len)
{
	write_buffer(LENGTH(string), len);
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

static void null_terminate(String string, const uint32_t len)
{
	string[len] = '\0';
}

static String clean_up(String string, const uint32_t len)
{
	write_string_len(string, len);
	null_terminate(string, len);

	return string;
}

ALLOC static String generic_allocation(String             string,
                                       const uint32_t     sz,
                                       const uint32_t     old,
                                       AllocationStrategy strategy,
                                       Arena             *arena)
{
	char *buff = strategy(string, NON_STRING_SIZE + sz, old, arena);

	write_buffer(buff, sz);

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

	return clean_up(string, len);
}

ALLOC static String string_construct_formatted(const char        *fmt,
                                               va_list            args,
                                               va_list            len_args,
                                               AllocationStrategy strategy,
                                               Arena             *arena)
{
	const size_t len    = vsnprintf(NULL, 0, fmt, len_args);
	String       string = generic_allocation(NULL, len, 0, strategy, arena);

	return generic_write(string, fmt, len, args, write_formatted);
}

static long get_stream_buffer_size(FILE *stream)
{
	const long pos = ftell(stream);
	fseek(stream, 0, SEEK_END);
	const long new_pos = ftell(stream);
	fseek(stream, pos, SEEK_SET);
	return new_pos - pos;
}

ALLOC static String string_construct_from_stream(FILE              *stream,
                                                 AllocationStrategy strategy,
                                                 Arena             *arena)
{
	const long n      = get_stream_buffer_size(stream);
	String     string = generic_allocation(NULL, n, 0, strategy, arena);
	return generic_write(string, stream, n, NULL, write_stream);
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
		dest = generic_allocation(dest, sz, buffsz, strategy, arena);
	}

	const uint32_t new_len = req + len;

	memcpy(dest + len, src, req);

	return clean_up(dest, new_len);
}

ALLOC static String string_duplicate(ConstString        src,
                                     AllocationStrategy strategy,
                                     Arena             *arena)
{
	const size_t sz  = string_buffer(src);
	String       dup = generic_allocation(NULL, sz, 0, strategy, arena);

	memcpy(BUFFER(dup), BUFFER(src), NON_STRING_SIZE + sz);

	return dup;
}

ALLOC static String string_replace(String      str,
                                   const char *old,
                                   const char *new,
                                   AllocationStrategy strategy,
                                   Arena             *arena)
{
	uint32_t      len    = string_len(str);
	const size_t  n      = strlen(new);
	const size_t  o      = strlen(old);
	const int32_t diff   = (int32_t)(n - o);
	char         *substr = str;

	if (diff > 0) {
		uint32_t count = 0;
		while ((substr = strstr(substr, old))) {
			count++;
			substr += o;
		}

		const uint32_t buffsz = string_buffer(str);

		if (buffsz - len < count * diff) {
			const uint32_t sz = max(buffsz + count * diff, buffsz * 2);
			str = generic_allocation(str, sz, buffsz, strategy, arena);
		}

		substr = str;
	}

	while ((substr = strstr(substr, old))) {
		const ptrdiff_t pos = substr - str;
		char           *dest;
		char           *src;
		size_t          amount;

		if (diff < 0) {
			dest   = substr;
			src    = substr + (diff * -1);
			amount = len - pos - (diff * -1);
		} else {
			dest   = substr + diff;
			src    = substr;
			amount = len - pos;
		}

		memmove(dest, src, amount);
		memcpy(substr, new, n);
		len += diff;
		substr += n;
		null_terminate(str, len);
	}

	return clean_up(str, len);
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

String string_cat(String restrict dest, ConstString restrict src)
{
	return string_concatenate(dest, src, stdlib_realloc, NULL);
}

String string_dup(ConstString restrict src)
{
	return string_duplicate(src, stdlib_alloc, NULL);
}

String string_sub(String str, const char *old, const char *new)
{
	return string_replace(str, old, new, stdlib_realloc, NULL);
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

void string_tolower(String str)
{
	const uint32_t len = string_len(str);

	for (uint32_t i = 0; i < len; ++i) {
		char c = str[i];
		str[i] = (char)tolower(c);
	}
}

void string_toupper(String str)
{
	const uint32_t len = string_len(str);

	for (uint32_t i = 0; i < len; ++i) {
		char c = str[i];
		str[i] = (char)toupper(c);
	}
}

void string_clear(String str)
{
	const uint32_t len = 0;
	const uint32_t buff = string_buffer(str);

	memset(str, 0, buff);
	clean_up(str, len);
}

void string_slice(String str, uint32_t start, uint32_t end)
{
	assert(start < end);

	const uint32_t len = end - start;

	memmove(str, str + start, len);
	clean_up(str, len);
}

void string_strip(String str, const char *reject)
{
	char *end = str + string_len(str) - 1;
	char *sp  = str;
	char *ep  = end;

	while (sp <= end && strchr(reject, *sp)) {
		sp++;
	}

	while (ep > sp && strchr(reject, *ep)) {
		ep--;
	}

	const uint32_t len = ep - sp + 1;

	if (sp != str) {
		memmove(str, sp, len);
	}

	clean_up(str, len);
}

void string_totitle(String str)
{
	const uint32_t len   = string_len(str);
	bool           space = true;

	for (uint32_t i = 0; i < len; ++i) {
		char c      = str[i];
		bool letter = isalpha(c);

		if (space && letter) {
			str[i] = (char)toupper(c);
		} else if (letter) {
			str[i] = (char)tolower(c);
		}

		space = (isspace(c) || ispunct(c)) ? true : false;
	}
}

ALLOC FORMAT_EXT String arena_string_new(Arena *arena, const char *fmt, ...)
{
	assert(arena);

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
	assert(arena);

	return string_construct_from_stream(stream, arena_alloc, arena);
}

ALLOC String arena_string_cat(Arena *arena, String dest, ConstString src)
{
	assert(arena);

	return string_concatenate(dest, src, arena_realloc, arena);
}

ALLOC String arena_string_dup(Arena *arena, ConstString src)
{
	assert(arena);

	return string_duplicate(src, arena_alloc, arena);
}

ALLOC String arena_string_sub(Arena      *arena,
                              String      str,
                              const char *old,
                              const char *new)
{
	assert(arena);

	return string_replace(str, old, new, arena_realloc, arena);
}
