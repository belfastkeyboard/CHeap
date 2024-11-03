#include <stdio.h>
#include <string.h>

#include "../../string_t.h"

String create_string(const char *text)
{
    String string = { .text = text, .len = strlen(text) };

    return string;
}


void print_string(String string)
{
    printf("%s", string.text);
}
