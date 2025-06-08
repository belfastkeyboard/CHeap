#include "../../algorithms.h"
#include "../../vector.h"

void transform_span(Span span, Transform transform)
{
	for (int i = 0; i < span.nmemb; ++i)
	{
		void *item = span.data + i * span.size;
		transform(item);
	}
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
