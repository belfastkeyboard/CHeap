#include "../../algorithms.h"
#include "../../vector.h"
#include <memory.h>
#include <stdlib.h>

static void fisher_yates_shuffle(void        *data,
                                 const size_t size,
                                 const size_t nmemb)
{
	char buff[size];

	for (size_t i = nmemb - 1; i > 0; i--)
	{
		size_t j = random() % (i + 1);

		void *a = data + i * size;
		void *b = data + j * size;

		memcpy(buff, a, size);
		memcpy(a, b, size);
		memcpy(b, buff, size);
	}
}

void transform_span(Span span, Transform transform)
{
	for (int i = 0; i < span.nmemb; ++i)
	{
		void *item = span.data + i * span.size;
		transform(item);
	}
}

void sort_span(Span span, Compare compare)
{
	qsort(span.data, span.nmemb, span.size, compare);
}

void shuffle_span(Span span)
{
	fisher_yates_shuffle(span.data, span.size, span.nmemb);
}

void *find_span(Span span, Predicate predicate)
{
	void *result = NULL;

	for (int i = 0; i < span.nmemb; ++i)
	{
		void *item = span.data + i * span.size;

		if (predicate(item))
		{
			result = item;
			break;
		}
	}

	return result;
}

bool any_span(Span span, Predicate predicate)
{
	bool result = false;

	for (int i = 0; i < span.nmemb; ++i)
	{
		void *item = span.data + i * span.size;

		if (predicate(item))
		{
			result = true;
			break;
		}
	}

	return result;
}

bool all_span(Span span, Predicate predicate)
{
	bool result = true;

	for (int i = 0; i < span.nmemb; ++i)
	{
		void *item = span.data + i * span.size;

		if (!predicate(item))
		{
			result = false;
			break;
		}
	}

	return result;
}

Vector *filter_span(Span span, Predicate predicate)
{
	Vector *matches = create_vector(span.size);

	for (int i = 0; i < span.nmemb; ++i)
	{
		const void *item = span.data + i * span.size;

		if (predicate(item))
		{
			push_back_vector(matches, item);
		}
	}

	return matches;
}
