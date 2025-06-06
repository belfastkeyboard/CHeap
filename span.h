#pragma once

#define CHEAP_SPAN_AVAILABLE

#include <stddef.h>

typedef struct Span
{
	void  *data;
	size_t size;
	size_t nmemb;
} Span;

Span make_span(void *data, size_t size, size_t nmemb);
