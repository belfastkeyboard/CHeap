#include "../../range.h"

Range create_range(Iter begin, Iter end)
{
	Range range = { .begin = begin, .end = end };
	return range;
}

void *get_range(Range range)
{
	return get_iter(range.begin);
}

bool done_range(Range range)
{
	return done_iter(range.begin, range.end);
}

bool done_range_r(Range range)
{
	return done_iter_r(range.begin, range.end);
}
