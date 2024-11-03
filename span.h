#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct Span Span;


__attribute__((warn_unused_result))
Span *create_span(void *view,
                  size_t nmemb,
                  size_t size);

void destroy_span(Span **span);


const void *front(Span *span);

const void *back(Span *span);

const void *at(Span *span,
               size_t index);


bool empty(Span *span);

size_t size(Span *span);
