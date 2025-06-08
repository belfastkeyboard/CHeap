#pragma once

#include <stdbool.h>
#include "span.h"

#define ALLOC __attribute__((warn_unused_result))

typedef bool (*Predicate)(const void*);
typedef void (*Transform)(void*);

void transform_span(Span span, Transform transform);
void *find_span(Span span, Predicate predicate);
bool any_span(Span span, Predicate predicate);
bool all_span(Span span, Predicate predicate);

#if defined(CHEAP_VECTOR_AVAILABLE) && defined(CHEAP_SPAN_AVAILABLE)
ALLOC Vector *filter_span(Span span, Predicate predicate);
#endif
