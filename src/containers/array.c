#include "../../internals/base.h"
#include "../../internals/mpool.h"
#include "../../array.h"

typedef struct Array
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
} Array;


Array *create_array(const size_t size)
{
    Array *array = memory_allocate_container(sizeof(Array));

    array->size = size;

    return array;
}

void destroy_array(Array **array)
{
    memory_free_container_mempool((void **)array,
                                  (*array)->array);
}


void push_back_array(Array *array,
                     void *const value)
{
    generic_mempool_push_back(&array->array,
                              value,
                              &array->capacity,
                              &array->nmemb,
                              array->size);
}

void insert_array(Array *array,
                  void *const value,
                  const size_t index)
{
    generic_mempool_insert(&array->array,
                           value,
                           index,
                           &array->capacity,
                           &array->nmemb,
                           array->size);
}


void pop_back_array(Array *array)
{
    generic_mempool_pop_back(&array->nmemb);
}

size_t erase_array(Array *array,
                   const size_t index)
{
    return generic_mempool_erase(&array->array,
                                 index,
                                 &array->nmemb,
                                 array->size);
}

void clear_array(Array *array)
{
    generic_mempool_clear(&array->array,
                          array->capacity,
                          &array->nmemb,
                          array->size);
}


void *at_array(Array *array,
               size_t index)
{
    return mempool_random_access(array->array,
                                 index,
                                 array->size);
}

void *front_array(Array *array)
{
    return mempool_random_access(array->array,
                                 0,
                                 array->size);
}

void *back_array(Array *array)
{
    return mempool_random_access(array->array,
                                 array->nmemb - 1,
                                 array->size);
}


bool empty_array(Array *array)
{
    return generic_empty(array->nmemb);
}

size_t size_array(Array *array)
{
    return generic_size(array->nmemb);
}

size_t capacity_array(Array *array)
{
    return generic_capacity(array->capacity);
}


void reserve_array(Array *array,
                   const size_t new_cap)
{
    generic_mempool_reserve(&array->array,
                            &array->capacity,
                            array->size,
                            new_cap);
}

void shrink_to_fit_array(Array *array)
{
    generic_mempool_shrink_to_fit(&array->array,
                                  &array->capacity,
                                  &array->nmemb,
                                  array->size);
}
