#include "../../algorithms.h"
#include "../../internals/cassert.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

void sort(const Iter begin, const Iter end, const Comp comp)
{
	CHEAP_ASSERT(begin.size == end.size, "Cannot sort different types.");

	const size_t    size  = begin.size;
	const ptrdiff_t diff  = (ptrdiff_t)(end.ptr - begin.ptr + size);
	const size_t    nmemb = diff / size;

	qsort(begin.ptr, nmemb, size, comp);
}

void shuffle(const Iter begin, const Iter end)
{
	CHEAP_ASSERT(begin.size == end.size, "Cannot sort different types.");

	const size_t    size  = begin.size;
	const ptrdiff_t diff  = (ptrdiff_t)(end.ptr - begin.ptr + size);
	const size_t    nmemb = diff / size;
	Iter            iter  = begin;
	void           *temp  = malloc(size);

	for (range(iter, end))
	{
		size_t i = random() % nmemb;

		memcpy(temp, iter.ptr, size);
		memcpy(iter.ptr, begin.ptr + i * size, size);
		memcpy(begin.ptr + i * size, temp, size);
	}

	free(temp);
}