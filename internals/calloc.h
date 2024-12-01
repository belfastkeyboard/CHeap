#pragma once

#include <stddef.h>

#ifdef CHEAP_ALLOC
    #define CHEAP_MALLOC(sz)        cheap_malloc_impl(sz)
    #define CHEAP_CALLOC(nmemb, sz) cheap_calloc_impl(nmemb, sz)
    #define CHEAP_REALLOC(ptr, sz)  cheap_realloc_impl(ptr, sz)
    #define CHEAP_FREE(ptr)         cheap_free_impl(ptr)
#else
    #define CHEAP_MALLOC(sz)        malloc(sz)
    #define CHEAP_CALLOC(nmemb, sz) calloc(nmemb, sz)
    #define CHEAP_REALLOC(ptr, sz)  realloc(ptr, sz)
    #define CHEAP_FREE(ptr)         free(ptr)
#endif


void *cheap_malloc_impl(size_t size);

void *cheap_calloc_impl(size_t nmemb, size_t size);

void *cheap_realloc_impl(void *ptr, size_t size);

void cheap_free_impl(void *ptr);

void cheap_clear_impl(void);
