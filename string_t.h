#pragma once

#include <stddef.h>

typedef struct String String;

String *create_string(const char *text);
void destroy_string(String **string);


void append_string(String *dest,
                   const String *src);

void insert(String *dest,
            const String *src,
            size_t index);

char *replace(String *string,
              const String *find,
              const String *replace);

/*
char *join(char **strings, size_t size, const char *delim);
char *lpad(const char *string, size_t pad);
char *rpad(const char *string, size_t pad);
*/

size_t count(const String *string,
             const String *value); // Returns the number of times a specified value occurs in a string

void print(const String *string);


/*
 *
 * char **split(const char *string, const char *delim, size_t *size);
 * void reverse(char *string);
 * void capitalise(char *string); // Converts the first character to upper case
 * void lower(char *string); // Converts a string into lower case
 * void upper(char *string); // Converts a string into upper case
 * void title(char *string); // Converts the first character of each word to upper case
 * void swapcase(char *string); // Swaps cases, lower case becomes upper case and vice versa
 * void lstrip(char *string); // Returns a left trim version of the string
 * void rstrip(char *string); // Returns a right trim version of the string
 * void strip(char *string); // Returns a trimmed version of the string
 * void truncate(char *string, size_t size);
 *
 * bool is_lower(const char *string); // Returns True if all characters in the string are lower case
 * bool is_upper(const char *string); // Returns True if all characters in the string are upper case
 * bool is_title(const char *string); // Returns True if the string follows the rules of a title
 * bool starts_with(const char *string, const char *prefix); // Returns True if the string starts with the specified value
 * bool ends_with(const char *string, const char *suffix); // Returns true if the string ends with the specified value
 * bool is_alnum(const char *string); // Returns True if all characters in the string are alphanumeric
 * bool is_alpha(const char *string); // Returns True if all characters in the string are in the alphabet
 * bool is_ascii(const char *string); // Returns True if all characters in the string are ascii characters
 * bool is_decimal(const char *string); // Returns True if all characters in the string are decimals
 * bool is_printable(const char *string); // Returns True if all characters in the string are printable
 * bool is_space(const char *string); // Returns True if all characters in the string are whitespaces
 *
 * size_t find(const char *string, const char *value); // Searches the string for a specified value and returns the position of where it was found
 * size_t rfind(const char *string, const char *value); // Searches the string for a specified value and returns the last position of where it was found
 * size_t word_count(const char *string);
 *
 * */