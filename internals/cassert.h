#pragma once

#include <stdio.h>
#include <stdlib.h>

#ifdef NDEBUG
#define CHEAP_ASSERT(expr, msg) ((void)0)
#else
#define CHEAP_ASSERT(expr, msg)                                                \
	do                                                                         \
	{                                                                          \
		if (!(expr))                                                           \
		{                                                                      \
			fprintf(stderr, "%s\n", msg);                                      \
			fprintf(stderr,                                                    \
			        "CHeap: %s:%d: %s: Assertion '%s' failed.\n",              \
			        __FILE__,                                                  \
			        __LINE__,                                                  \
			        __extension__ __PRETTY_FUNCTION__,                         \
			        #expr);                                                    \
			abort();                                                           \
		}                                                                      \
	}                                                                          \
	while (0)
#endif
