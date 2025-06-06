#pragma once

#include "../range.h"
#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

ALLOC void *generic_mempool_alloc(size_t nmemb, size_t size);

void generic_mempool_push_back(void      **array,
                               const void *value,
                               size_t     *capacity,
                               size_t     *nmemb,
                               size_t      size);

void generic_mempool_push_front(void      **array,
                                const void *value,
                                size_t     *capacity,
                                size_t     *nmemb,
                                size_t      size);

void generic_mempool_insert(void      **array,
                            const void *value,
                            size_t      index,
                            size_t     *capacity,
                            size_t     *nmemb,
                            size_t      size);

void generic_mempool_set(void       *array,
                         const void *value,
                         size_t      index,
                         size_t      nmemb,
                         size_t      size);

void generic_mempool_range_insert(void  **array,
                                  size_t  index,
                                  size_t *capacity,
                                  size_t *nmemb,
                                  size_t  size,
                                  Range   range);

void generic_mempool_pop_back(size_t *nmemb);

void generic_mempool_pop_front(void **array, size_t *nmemb, size_t size);

void generic_mempool_erase(void  **array,
                           size_t  index,
                           size_t *nmemb,
                           size_t  size);

void generic_mempool_clear(size_t *nmemb);

void *generic_mempool_access_front(void *array, size_t nmemb);

void *generic_mempool_access_back(void *array, size_t nmemb, size_t size);

void *generic_mempool_random_access(void  *array,
                                    size_t index,
                                    size_t nmemb,
                                    size_t size);

void generic_mempool_reserve(void  **array,
                             size_t *capacity,
                             size_t  size,
                             size_t  new_cap);

void generic_mempool_shrink_to_fit(void  **array,
                                   size_t *capacity,
                                   size_t  nmemb,
                                   size_t  size);
