#pragma once

#include "iter.h"
#include <stdbool.h>

#ifndef CHEAP_RANGE_AVAILABLE
#define CHEAP_RANGE_AVAILABLE
#endif

typedef struct Range
{
	Iter begin;
	Iter end;
} Range;

Range create_range(Iter begin, Iter end);
void *get_range(Range range);

bool done_range(Range range);
bool done_range_r(Range range);

#define for_each(range)   for (; !done_range(range); next_iter(&range.begin))
#define for_each_r(range) for (; !done_range_r(range); prev_iter(&range.begin))
