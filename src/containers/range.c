#include "../../range.h"

Range create_range(const void *array, const size_t nmemb, const size_t size)
{
	Range range = { .array = array, .nmemb = nmemb, .size = size };

	return range;
}
