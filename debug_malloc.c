#include <assert.h>
#include <malloc.h>
#include <stdio.h>

#include "debug_malloc.h"

static void log_allocation_data(void *ptr, const size_t size, const char *text)
{
    static size_t call_count = 0;

    char *mode = (call_count) ? "a" : "w";

    FILE *file = fopen("../log.txt", mode);

    if (!call_count) fprintf(file, "Pointer\t\tSize\tAfter\t\tDescription\n\n");

    fprintf(file, "%p\t%5zu\t%p\t%s\n", ptr, size, ptr + size, text);
    fclose(file);

    call_count++;
}

void *debug_malloc(size_t size, const char *text)
{
    void *ptr = malloc(size);

    assert(ptr);

    log_allocation_data(ptr, size, text);

    return ptr;
}
void *debug_realloc(void *data, size_t size, const char *text)
{
    void *ptr = realloc(data, size);

    assert(ptr);

    log_allocation_data(ptr, size, text);

    return ptr;
}
void debug_free(void *ptr, const size_t size, const char *text)
{
    free(ptr);

    log_allocation_data(ptr, size, text);
}
