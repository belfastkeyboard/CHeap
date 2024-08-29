#include <assert.h>
#include <memory.h>
#include <malloc.h>
#include "../base.h"
#include "../array.h"

typedef struct Array
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
} Array;

Array *create_array(const size_t size)
{
    Array *array = malloc(sizeof(Array));

    array->array = NULL;
    array->capacity = 0;
    array->nmemb = 0;
    array->size = size;

    return array;
}
void destroy_array(Array **array)
{
    if ((*array)->array)
        free((*array)->array);

    free(*array);
    *array = NULL;
}

void push_back(Array *array, void *const item)
{
    if (array->nmemb >= array->capacity)
        array->array = sequential_resize(array->array, &array->capacity, array->size);

    sequential_insert(array->array, array->nmemb, item, array->nmemb, array->size);

    array->nmemb++;
}
void insert(Array *array, void *const item, const size_t index)
{
    if (array->nmemb >= array->capacity)
        array->array = sequential_resize(array->array, &array->capacity, array->size);

    sequential_insert(array->array, index, item, array->nmemb, array->size);
    array->nmemb++;
}

void pop_back(Array *array)
{
    assert(array->nmemb);

    array->nmemb--;
}
size_t erase(Array *array, size_t index)
{
    index = sequential_remove(array->array, index, array->nmemb, array->size);
    array->nmemb--;

    return index;
}
void clear(Array *array)
{
    sequential_clear(array->array, array->capacity, array->size);
    array->nmemb = 0;
}

void *at(Array *array, size_t index)
{
    return sequential_index_access(array->array, index, array->size);
}
void *front(Array *array)
{
    return sequential_index_access(array->array, 0, array->size);
}
void *back(Array *array)
{
    return sequential_index_access(array->array, array->nmemb - 1, array->size);
}

bool empty(Array *array)
{
    return container_empty(array->nmemb);
}
size_t size(Array *array)
{
    return container_size(array->nmemb);
}
size_t capacity(Array *array)
{
    return container_capacity(array->capacity);
}

void reserve(Array *array, size_t amount)
{
    if (amount > array->capacity)
    {
        array->capacity = amount;
        sequential_realloc(&array->array, array->capacity, array->size);
    }
}
void shrink(Array *array)
{
    if (array->nmemb < array->capacity)
    {
        array->capacity = array->nmemb;
        sequential_realloc(&array->array, array->capacity, array->size);
    }
}
