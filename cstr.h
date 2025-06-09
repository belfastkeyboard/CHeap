#pragma once

#include <stdint.h>
#include <stdio.h>

#define ALLOC      __attribute__((warn_unused_result))
#define FORMAT     __attribute__((format(printf, 1, 2)))
#define FORMAT_EXT __attribute__((format(printf, 2, 3)))

typedef char       *String;
typedef const char *ConstString;
typedef const char *StringView;

ALLOC FORMAT String cstr_new(const char *fmt, ...);
ALLOC String        cstr_from_stream(FILE *stream);
void                cstr_free(String *str);

uint32_t cstr_buffer(ConstString str);
uint32_t cstr_len(ConstString str);

ALLOC String cstr_cat(String dest, const char *src);
ALLOC String cstr_cat_cstr(String dest, ConstString src);
ALLOC String cstr_dup(ConstString src);
ALLOC String cstr_sub(String str, const char *old, const char *new);
ALLOC String cstr_ndup(ConstString src, uint32_t n);

int cstr_cmp(ConstString str1, ConstString str2);

StringView cstr_chr(ConstString str, int c);
StringView cstr_rchr(ConstString str, int c);
StringView cstr_pbrk(ConstString str, ConstString accept);
StringView cstr_str(ConstString haystack, ConstString needle);

uint32_t cstr_cspn(ConstString str, ConstString reject);
uint32_t cstr_spn(ConstString str, ConstString accept);

void cstr_tolower(String str);
void cstr_toupper(String str);
void cstr_totitle(String str);

void cstr_clear(String str);
void cstr_slice(String str, uint32_t start, uint32_t end);
void cstr_strip(String str, const char *reject);

#ifdef CHEAP_ARENA_AVAILABLE
ALLOC FORMAT_EXT String arena_cstr_new(Arena *arena, const char *fmt, ...);
ALLOC String            arena_cstr_from_stream(Arena *arena, FILE *stream);

ALLOC String arena_cstr_cat(Arena *arena, String dest, const char *src);
ALLOC String arena_cstr_cat_cstr(Arena *arena, String dest, ConstString src);
ALLOC String arena_cstr_dup(Arena *arena, ConstString src);
ALLOC String arena_cstr_sub(Arena *arena, String str, const char *old, const char *new);

ALLOC String arena_cstr_ndup(Arena *arena, ConstString src, uint32_t n);
#endif

#ifdef CHEAP_VECTOR_AVAILABLE
Vector *cstr_split(ConstString str, ConstString delim);
String  cstr_join(Vector *strings, ConstString delim);
#endif

#if defined(CHEAP_ARENA_AVAILABLE) && defined(CHEAP_VECTOR_AVAILABLE)
Vector *arena_cstr_split(Arena *arena, ConstString str, ConstString delim);
String  arena_cstr_join(Arena *arena, Vector *strings, ConstString delim);
#endif
