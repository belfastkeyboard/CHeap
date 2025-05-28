#pragma once

#include <stdbool.h>
#include <stddef.h>


typedef char *String;
typedef const char *ConstString;


__attribute__((format(printf, 1, 2)))
__attribute__((warn_unused_result))
String string_new(const char *fmt,
                  ...);

__attribute__((warn_unused_result))
String string_from_stream(FILE *stream);

void string_free(String string);


// returns the size of the string buffer
size_t string_size(ConstString string);

// returns the number of characters in the string
size_t string_len(ConstString string);


String string_cpy(String restrict dest,
                  ConstString restrict src);

String string_dup(ConstString restrict src);


int string_cmp(ConstString str1,
               ConstString str2);


/*  The below are a mix of proposed functions inspired by Python and some random utility functions
    None of them are yet implemented
__attribute__((warn_unused_result))
String *copy_string(const String *string);

String *append_string(String *dest,
                      const String *src);

String *insert_string(String *dest,
                      const String *src,
                      size_t index);

String *replace_string(String *string,
                       const String *find,
                       const String *replace);

String *lpad_string(String *string,
                    size_t pad);

String *rpad_string(String *string,
                    size_t pad);


String *reverse_string(String *string);

String *capitalise_string(String *string);

String *lower_string(String *string);

String *upper_string(String *string);

String *title_string(String *string);

String *swapcase_string(String *string);

String *lstrip_string(String *string);

String *rstrip_string(String *string);

String *strip_string(String *string);

String *truncate_string(String *string, size_t size);

String *join_string(Vector *strings,
                    const String *delim);

Vector *split_string(const String *string,
                     const String *delim);


bool is_lower_string(const String *string);

bool is_upper_string(const String *string);

bool is_title_string(const String *string);

bool starts_with_string(const String *string,
                        const String *prefix);

bool ends_with_string(const String *string,
                      const String *suffix);

bool is_alnum_string(const String *string);

bool is_alpha_string(const String *string);

bool is_ascii_string(const String *string);

bool is_decimal_string(const String *string);

bool is_printable_string(const String *string);

bool is_space_string(const String *string);


size_t count_string(const String *string,
                    const String *value);

size_t find_string(const String *string,
                   const String *value);

size_t rfind_string(const String *string,
                    const String *value);

size_t word_count_string(const String *string);

*/