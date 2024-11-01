#include <string.h>
#include "../../stringview.h"

StringView sv_from_str(const char *string)
{
    StringView view = { 0 };

    view.text = string;
    view.size = strlen(string);

    return view;
}
StringView sv_from_strn(const char *string, size_t n)
{
    StringView view = { 0 };

    view.text = string;
    view.size = n;

    return view;
}
StringView sv_from_str_reverse(const char *string, size_t n)
{
    StringView view = { 0 };

    view.text = &string[strlen(string) - n];
    view.size = n;

    return view;
}
StringView sv_trim_left(const char *string)
{
    StringView view = { 0 };

    while (*string == ' ')
        string++;

    view.text = string;
    view.size = strlen(view.text);

    return view;
}
StringView sv_trim_right(const char *string)
{
    StringView view = { 0 };

    view.text = string;
    view.size = strlen(string);

    while (string[view.size-1] == ' ')
        view.size--;

    return view;
}
StringView sv_trim(const char *string)
{
    StringView view = { 0 };

    while (*string == ' ')
        string++;

    view.text = string;
    view.size = strlen(view.text);

    while (string[view.size-1] == ' ')
        view.size--;

    return view;
}
StringView sv_split_delim(const char *string, char delim)
{
    StringView view = { 0 };

    while (*string != '\0' && *string != delim)
        string++;

    view.text = ++string;
    view.size = strlen(string);

    return view;
}

bool sv_strcmp(StringView a, StringView b)
{
    bool s = a.size >= b.size;
    int c = strncmp(a.text, b.text, b.size);
    return (a.size >= b.size && strncmp(a.text, b.text, b.size) == 0);
}
bool sv_streqcase(StringView a, StringView b)
{
    return (a.size >= b.size && strncasecmp(a.text, b.text, b.size) == 0);
}
bool sv_starts_with(StringView sv, const char *prefix)
{
    if (sv.size < strlen(prefix))
        return false;
    StringView sv_string = sv_from_strn(sv.text, strlen(prefix));
    StringView sv_prefix = sv_from_str(prefix);
    return sv_strcmp(sv_string, sv_prefix);
}
bool sv_ends_with(StringView sv, const char *suffix)
{
    if (sv.size < strlen(suffix))
        return false;
    StringView sv_string = sv_from_str_reverse(sv.text, strlen(suffix));
    StringView sv_suffix = sv_from_str(suffix);
    return sv_strcmp(sv_string, sv_suffix);
}

long sv_to_long(StringView sv)
{
    return strtol(sv.text, NULL, 10);
}
float sv_to_float(StringView sv)
{
    return strtof(sv.text, NULL);
}