#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define ALLOC      __attribute__((warn_unused_result))
#define FORMAT     __attribute__((format(printf, 1, 2)))
#define FORMAT_EXT __attribute__((format(printf, 2, 3)))

typedef char       *String;
typedef const char *ConstString;
typedef const char *StringView;

ALLOC FORMAT String string_new(const char *fmt, ...);
ALLOC String        string_from_stream(FILE *stream);
void                string_free(String str);

uint32_t string_buffer(ConstString str);
uint32_t string_len(ConstString str);

ALLOC String string_cat(String dest, ConstString src);
ALLOC String string_dup(ConstString src);
ALLOC String string_sub(String str, const char *old, const char *new);

ALLOC String string_ndup(ConstString src, uint32_t n);

int string_cmp(ConstString str1, ConstString str2);

StringView string_chr(ConstString str, int c);
StringView string_rchr(ConstString str, int c);
StringView string_pbrk(ConstString str, ConstString accept);
StringView string_str(ConstString haystack, ConstString needle);

uint32_t string_cspn(ConstString str, ConstString reject);
uint32_t string_spn(ConstString str, ConstString accept);

void string_tolower(String str);
void string_toupper(String str);
void string_totitle(String str);

void string_clear(String str);
void string_slice(String str, uint32_t start, uint32_t end);
void string_strip(String str, const char *reject);

#ifdef CHEAP_ARENA_AVAILABLE
ALLOC FORMAT_EXT String arena_string_new(Arena *arena, const char *fmt, ...);
ALLOC String            arena_string_from_stream(Arena *arena, FILE *stream);

ALLOC String arena_string_cat(Arena *arena, String dest, ConstString src);
ALLOC String arena_string_dup(Arena *arena, ConstString src);
ALLOC String arena_string_sub(Arena      *arena,
                              String      str,
                              const char *old,
                              const char *new);

ALLOC String arena_string_ndup(Arena *arena, ConstString src, uint32_t n);
#endif

#ifdef CHEAP_VECTOR_AVAILABLE
Vector *string_split(ConstString str, ConstString delim);
String  string_join(Vector *strings, ConstString delim);
#endif

#if defined(CHEAP_ARENA_AVAILABLE) && defined(CHEAP_VECTOR_AVAILABLE)
Vector *arena_string_split(Arena *arena, ConstString str, ConstString delim);
String  arena_string_join(Arena *arena, Vector *strings, ConstString delim);
#endif
