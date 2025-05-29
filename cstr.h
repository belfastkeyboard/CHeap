#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef CHEAP_ARENA_AVAILABLE
#include "arena.h"
#endif

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

ALLOC String string_cpy(String dest, ConstString src);
ALLOC String string_cat(String dest, ConstString src);
ALLOC String string_dup(ConstString src);

void string_slice(String str, uint32_t start, uint32_t end);

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

#ifdef CHEAP_ARENA_AVAILABLE
ALLOC FORMAT_EXT String arena_string_new(Arena *arena, const char *fmt, ...);
ALLOC String            arena_string_from_stream(Arena *arena, FILE *stream);

ALLOC String arena_string_cpy(Arena *arena, String dest, ConstString src);
ALLOC String arena_string_cat(Arena *arena, String dest, ConstString src);
ALLOC String arena_string_dup(Arena *arena, ConstString src);
#endif
