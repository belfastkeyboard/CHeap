#pragma once

#include <stdbool.h>
#include "span.h"

#define ALLOC __attribute__((warn_unused_result))

typedef bool (*Predicate)(const void*);
typedef void (*Transform)(void*);
typedef int (*Compare)(const void*, const void*);

void swap(void *a, void *b, size_t size);

void transform_span(Span span, Transform transform);
void sort_span(Span span, Compare compare);
void fill_span(Span span, void* value);
void shuffle_span(Span span);
void reverse_span(Span span);

void *find_span(Span span, Predicate predicate);

void *min_span(Span span, Compare compare);
void *max_span(Span span, Compare compare);

bool any_span(Span span, Predicate predicate);
bool all_span(Span span, Predicate predicate);

#if defined(CHEAP_VECTOR_AVAILABLE) && defined(CHEAP_SPAN_AVAILABLE)
ALLOC Vector *filter_span(Span span, Predicate predicate);
#endif
