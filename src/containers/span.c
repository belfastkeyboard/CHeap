#include <malloc.h>
#include "../../span.h"

typedef struct Span
{
    const void *array;
    size_t nmemb;
    size_t size;
} Span;

Span *create_span(void *view, size_t nmemb, size_t size)
{
    Span *span = malloc(sizeof(Span));

    span->array = view;
    span->nmemb = nmemb;
    span->size = size;

    return span;
}
void destroy_span(Span **span)
{
    free(*span);
    *span = NULL;
}

const void *front(Span *span)
{
    return span->array;
}
const void *back(Span *span)
{
    return span->array + (span->nmemb - 1) * span->size;
}
const void *at(Span *span, size_t index)
{
    const void *ptr = NULL;

    if (index < span->nmemb)
        ptr = span->array + index * span->size;

    return ptr;
}

bool empty(Span *span)
{
    return (span->nmemb == 0);
}
size_t size(Span *span)
{
    return span->nmemb;
}
