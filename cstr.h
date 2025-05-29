#pragma once

#include <stdbool.h>
#include <stdint.h>


typedef char       *String;
typedef const char *ConstString;
typedef const char *StringView;


__attribute__( (format(printf, 1, 2), warn_unused_result()) )
String string_new (const char *fmt, ...);

__attribute__((warn_unused_result))
String string_from_stream (FILE *stream);

void string_free(String string);


// returns the size of the string buffer
uint32_t string_size(ConstString string);

// returns the number of characters in the string
uint32_t string_len(ConstString string);


__attribute__((warn_unused_result))
String string_cpy(String restrict dest,
                  ConstString restrict src);

__attribute__((warn_unused_result))
String string_cat(String restrict dest,
                  ConstString restrict src);

__attribute__((warn_unused_result))
String string_dup(ConstString restrict src);


int string_cmp(ConstString str1,
               ConstString str2);


StringView string_chr(ConstString string,
                      int c);

StringView string_rchr(ConstString string,
                       int c);

StringView string_pbrk(ConstString string,
                       ConstString accept);

StringView string_str(ConstString haystack,
                      ConstString needle);

uint32_t string_cspn(ConstString string,
                     ConstString reject);

uint32_t string_spn(ConstString string,
                   ConstString accept);

#ifdef CHEAP_ARENA_AVAILABLE

#endif
