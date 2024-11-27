#pragma once

#include <stdbool.h>
#include <stddef.h>


typedef struct String String;
typedef struct Vector Vector;


String *create_string(const char *text);

void destroy_string(String **string);


void append_string(String *dest,
                   const String *src);

void insert(String *dest,
            const String *src,
            size_t index);

String *replace(String *string,
                const String *find,
                const String *replace);

String *lpad(String *string,
           size_t pad);

String *rpad(String *string,
             size_t pad);


String *reverse(String *string);

String *capitalise(String *string); // Converts the first character to upper case

String *lower(String *string); // Converts a string into lower case

String *upper(String *string); // Converts a string into upper case

String *title(String *string); // Converts the first character of each word to upper case

String *swapcase(String *string); // Swaps cases, lower case becomes upper case and vice versa

String *lstrip(String *string); // Returns a left trim version of the string

String *rstrip(String *string); // Returns a right trim version of the string

String *strip(String *string); // Returns a trimmed version of the string

String *truncate(String *string, size_t size);


bool is_lower(const String *string); // Returns True if all characters in the string are lower case

bool is_upper(const String *string); // Returns True if all characters in the string are upper case

bool is_title(const String *string); // Returns True if the string follows the rules of a title

bool starts_with(const String *string,
                 const String *prefix); // Returns True if the string starts with the specified value

bool ends_with(const String *string,
               const String *suffix); // Returns true if the string ends with the specified value

bool is_alnum(const String *string); // Returns True if all characters in the string are alphanumeric

bool is_alpha(const String *string); // Returns True if all characters in the string are in the alphabet

bool is_ascii(const String *string); // Returns True if all characters in the string are ascii characters

bool is_decimal(const String *string); // Returns True if all characters in the string are decimals

bool is_printable(const String *string); // Returns True if all characters in the string are printable

bool is_space(const String *string); // Returns True if all characters in the string are whitespaces


void print(const String *string);


// Returns the number of times a specified value occurs in a string
size_t count(const String *string,
             const String *value);

// Searches the string for a specified value and returns the position of where it was found
size_t find(String *string,
            const String *value);

// Searches the string for a specified value and returns the last position of where it was found
size_t rfind(const String *string,
             const String *value);

size_t word_count(const String *string);


String *join(Vector *strings,
             const String *delim);

Vector *split(const String *string,
              const String *delim);
