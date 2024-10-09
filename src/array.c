#include <assert.h>
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
    Array *array = memory_allocate_container(sizeof(Array));

    SEQ_CONTAINER_INIT(array, size);

    return array;
}
void destroy_array(Array *array)
{
    assert(array);

    memory_free_buffer(array->array);
    memory_free_container((void**)&array);
}

void push_back_array(Array *array, void *const value)
{
    /*if (array->nmemb >= array->capacity)
        array->array = sequential_resize(array->array, &array->capacity, array->size);

    sequential_insert(array->array, array->nmemb, item, array->nmemb, array->size);
    array->nmemb++;*/
    no_iterator_push_back(&array->array, value, &array->nmemb, &array->capacity, array->size);
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
        array->array = sequential_realloc(array->array, array->capacity, array->size);
    }
}
void shrink(Array *array)
{
    if (array->nmemb < array->capacity)
    {
        array->capacity = array->nmemb;
        array->array = sequential_realloc(array->array, array->capacity, array->size);
    }
}
