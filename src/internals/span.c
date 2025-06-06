#include "../../span.h"

Span make_span(void *data, const size_t size, const size_t nmemb)
{
	Span span = {
		.data = data,
		.size = size,
		.nmemb = nmemb
	};

	return span;
}