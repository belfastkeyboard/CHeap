#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define SV_Fmt "%.*s"
#define SV_Arg(sv) (int)(sv).size, (sv).text

typedef struct
{
    const char *text;
    size_t size;
} StringView;

StringView sv_from_str(const char *string);
StringView sv_from_strn(const char *string, size_t n);
StringView sv_from_str_reverse(const char *string, size_t n);
StringView sv_trim_left(const char *string);
StringView sv_trim_right(const char *string);
StringView sv_trim(const char *string);
StringView sv_split_delim(const char *string, char delim);

bool sv_streq(StringView a, StringView b);
bool sv_streqcase(StringView a, StringView b);
bool sv_starts_with(StringView sv, const char *prefix);
bool sv_ends_with(StringView sv, const char *suffix);

long sv_to_long(StringView sv);
float sv_to_float(StringView sv);
