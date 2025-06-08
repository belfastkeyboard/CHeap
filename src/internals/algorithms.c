#include "../../algorithms.h"
#include "../../internals/cassert.h"
#include "../../vector.h"
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>

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

void fill_span(Span span, void *value)
{
	for (int i = 0; i < span.nmemb; ++i)
	{
		memcpy(span.data + i * span.size, value, span.size);
	}
}

void shuffle_span(Span span)
{
	fisher_yates_shuffle(span.data, span.size, span.nmemb);
}

void reverse_span(Span span)
{
	if (!span.nmemb)
	{
		return;
	}

	for (size_t begin = 0, end = span.nmemb - 1; begin < end; begin++, end--)
	{
		void *a = span.data + begin * span.size;
		void *b = span.data + end * span.size;

		swap(a, b, span.size);
	}
}

void duplicate_span(Span dest, const Span src)
{
	CHEAP_ASSERT(dest.size == src.size && dest.nmemb >= src.nmemb,
	             "Destination cannot store source values due size mismatch.");

	memcpy(dest.data, src.data, src.size * src.nmemb);
}

void *find_span(Span span, Compare compare, const void *value)
{
	void *result = NULL;

	for (int i = 0; i < span.nmemb; ++i)
	{
		void *item = span.data + i * span.size;

		if (compare(item, value) == 0)
		{
			result = item;
			break;
		}
	}

	return result;
}

bool contains_span(Span span, Compare compare, const void *value)
{
	return find_span(span, compare, value);
}

bool alias_span(Span span1, Span span2)
{
	if (span1.size != span2.size || span1.nmemb != span2.nmemb ||
	    span1.data != span2.data)
	{
		return false;
	}

	return true;
}

bool equal_span(const Span span1, const Span span2, Compare compare)
{
	if (span1.size != span2.size || span1.nmemb != span2.nmemb)
	{
		return false;
	}

	if (span1.data != span2.data)
	{
		return true;
	}

	for (int i = 0; i < span1.nmemb; ++i)
	{
		const void *a = span1.data + i * span1.size;
		const void *b = span2.data + i * span2.size;

		if (compare(a, b) != 0)
		{
			return false;
		}
	}

	return true;
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
