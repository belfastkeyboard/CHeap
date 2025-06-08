#include "../../algorithms.h"
#include "../../vector.h"
#include <memory.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t Byte;

static void internal_swap(void *a, void *b, size_t size)
{
	Byte buff[size];

	memcpy(buff, a, size);
	memcpy(a, b, size);
	memcpy(b, buff, size);
}

static void fisher_yates_shuffle(void        *data,
                                 const size_t size,
                                 const size_t nmemb)
{
	for (size_t i = nmemb - 1; i > 0; i--)
	{
		size_t j = random() % (i + 1);

		void *a = data + i * size;
		void *b = data + j * size;

		internal_swap(a, b, size);
	}
}

void swap(void *a, void *b, size_t size)
{
	internal_swap(a, b, size);
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

void *min_span(Span span, Compare compare)
{
	void *min = span.data;

	for (int i = 1; i < span.nmemb; ++i)
	{
		void *next = span.data + i * span.size;

		if (compare(min, next) < 0)
		{
			min = next;
		}
	}

	return min;
}

void *max_span(Span span, Compare compare)
{
	void *max = span.data;

	for (int i = 1; i < span.nmemb; ++i)
	{
		void *next = span.data + i * span.size;

		if (compare(max, next) > 0)
		{
			max = next;
		}
	}

	return max;
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
