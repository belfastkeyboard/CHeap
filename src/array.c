#include <memory.h>
#include <malloc.h>
#include "../array.h"

typedef struct Array
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;

} Array;

static void resize(Array *array)
{
    array->capacity = (array->capacity > 0) ? array->capacity * 2 : 1;

    void *tmp = realloc(array->array, array->size * array->capacity);
    if (tmp)
        array->array = tmp;
}

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
        resize(array);

    memcpy(array->array + array->nmemb * array->size, item, array->size);
    array->nmemb++;
}
void insert(Array *array, void *const item, const size_t index)
{
    if (index <= array->nmemb)
    {
        if (array->nmemb >= array->capacity)
            resize(array);

        if (index == array->nmemb)
            push_back(array, item);
        else
        {
            memmove(
                array->array + (index + 1) * array->size,
                array->array + index * array->size,
                (array->nmemb - index) * array->size
            );
            memcpy(array->array + index * array->size, item, array->size);
            array->nmemb++;
        }
    }
}

void pop_back(Array *array)
{
    if (array->nmemb)
        array->nmemb--;
}
size_t erase(Array *array, size_t index)
{
    if (array->nmemb)
    {
        if (index == array->nmemb - 1)
        {
            pop_back(array);
            index--;
        }
        else
        {
            memmove(array->array + index, array->array + index + 1, (array->nmemb - index) * array->size);
            array->nmemb--;
            index--;
        }
    }

    return index;
}
void clear(Array *array)
{
    array->nmemb = 0;
    memset(array->array, 0, array->capacity * array->size);
}

void *at(Array *array, size_t index)
{
    return array->array + (index * array->size);
}
void *front(Array *array)
{
    return array->array;
}
void *back(Array *array)
{
    return array->array + array->nmemb - 1;
}

bool empty(Array *array)
{
    return (array->nmemb == 0);
}
size_t size(Array *array)
{
    return array->nmemb;
}

void reserve(Array *array, size_t amount)
{
    if (amount > array->capacity)
    {
        array->capacity = amount;
        void *tmp = realloc(array->array, array->size * amount);

        if(tmp)
            array->array = tmp;
    }
}
void shrink(Array *array)
{
    if (array->nmemb < array->capacity)
    {
        array->capacity = array->nmemb;
        void *tmp = realloc(array->array, array->size * array->capacity);

        if (tmp)
            array->array = tmp;
    }
}
