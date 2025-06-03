#include "../../internals/bheap.h"
#include <memory.h>
#include <stddef.h>

static size_t parent(const size_t i)
{
	return (i - 1) / 2;
}

static size_t left_child(const size_t i)
{
	return 2 * i + 1;
}

static size_t right_child(const size_t i)
{
	return 2 * i + 2;
}

static void swap(void        *array,
                 const size_t i,
                 const size_t pi,
                 const void  *v,
                 const void  *pv,
                 const size_t size)
{
	char temp[size];

	memcpy(temp, v, size);
	memcpy(array + i * size, pv, size);
	memcpy(array + pi * size, temp, size);
}

void bubble_up(void *array, size_t i, const size_t size, CompareFunc fnc)
{
	while (i > 0)
	{
		size_t pi  = parent(i);
		void  *v   = array + i * size;
		void  *pv  = array + pi * size;
		int    res = fnc(v, pv);

		if (res < 0)
		{
			swap(array, i, pi, v, pv, size);
			i = pi;
		}
		else
		{
			break;
		}
	}
}

void bubble_down(void        *array,
                 const size_t nmemb,
                 const size_t size,
                 CompareFunc  fnc,
                 size_t       i)
{
	while (i < nmemb)
	{
		size_t li = left_child(i);
		size_t ri = right_child(i);
		size_t ci;
		void  *cv;
		int    res;

		if (li >= nmemb)
		{
			break;
		}
		else if (ri >= nmemb)
		{
			ci = li;
			cv = array + li * size;
		}
		else
		{
			void *l = array + li * size;
			void *r = array + ri * size;

			res = fnc(l, r);
			ci  = (res > 0) ? ri : li;
			cv  = (res > 0) ? r : l;
		}

		void *v = array + i * size;

		res = fnc(v, cv);

		if (res > 0)
		{
			swap(array, i, ci, v, cv, size);
			i = ci;
		}
		else
		{
			break;
		}
	}
}

void make_heap(void             *array,
               const size_t      nmemb,
               const size_t      size,
               const CompareFunc fnc)
{
	for (size_t i = parent(nmemb - 1); i < nmemb; i--)
	{
		bubble_down(array, nmemb, size, fnc, i);
	}
}

void pop_front_binary_heap(void *array, size_t *nmemb, const size_t size)
{
	const size_t top    = 0;
	const size_t bottom = (*nmemb) - 1;

	void *tv = array + top * size;
	void *bv = array + bottom * size;

	swap(array, top, bottom, tv, bv, size);

	(*nmemb)--;
}
