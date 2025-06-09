#include "vector.h"
// #include "../../internals/mpool.h"
#include <assert.h>
#include <malloc.h>
#include <memory.h>

typedef struct Vector
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
} Vector;

static void vector_resize(Vector *vector)
{
    const size_t capacity = vector->capacity;
    const size_t size = vector->nmemb;
    const size_t new_capacity = (capacity > 0) ? capacity * 2 : 1;

    void *tmp = realloc(vector->array, new_capacity * size);

    assert(tmp);

    vector->array = tmp;
    vector->capacity = new_capacity;
}


Vector *create_vector(const size_t size)
{
    Vector *vector = calloc(1, sizeof(Vector));

    assert(vector);

    vector->size = size;

    return vector;
}

void destroy_vector(Vector **vector)
{
    assert(*vector);

    if ((*vector)->array)
    {
        free((*vector)->array);
    }

    free(*vector);
    *vector = NULL;
}

void push_back_vector(Vector *vector, const void *value)
{
    assert(value);

    if (vector->nmemb >= vector->capacity)
    {
        vector_resize(vector);
    }

    memcpy(vector->array + vector->nmemb * vector->size, value, vector->size);

    vector->nmemb++;
}

void insert_vector(Vector *vector, const void *value, const size_t index)
{
    if (vector->nmemb >= vector->capacity)
    {
        vector_resize(vector);
    }

    const size_t nmemb = vector->nmemb;
    const size_t size = vector->size;

    assert(index < nmemb + 1);

    if (index < nmemb)
    {
        memmove(vector->array + (index + 1) * size,
                vector->array + index * size,
                (nmemb - index) * size);
    }

    memcpy(vector->array + index * size, value, size);
}

void pop_back_vector(Vector *vector)
{
    assert(vector->nmemb);
    vector->nmemb--;
}

void erase_vector(Vector *vector, const size_t index)
{
    assert(vector->array);
    assert(index < nmemb);

    const size_t nmemb = vector->nmemb;
    const size_t size = vector->size;

    memmove(vector->array + index * size,
            vector->array + (index + 1) * size,
            (nmemb - index - 1) * size);

    vector->nmemb--;
}

void clear_vector(Vector *vector)
{
    vector->nmemb = 0;
}

void *at_vector(const Vector *vector, const size_t index)
{
    const size_t nmemb = vector->nmemb;
    const size_t size = vector->size;

    assert(index < nmemb);
    assert(vector->array);

    return vector->array + index * size;
}

void *front_vector(const Vector *vector)
{
    assert(vector->array);
    return vector->array;
}

void *back_vector(const Vector *vector)
{
    assert(vector->array);
    return vector->array + (vector->nmemb - 1) * vector->size;
}

bool empty_vector(const Vector *vector)
{
    return vector->nmemb;
}

size_t size_vector(const Vector *vector)
{
    return vector->nmemb;
}

size_t capacity_vector(const Vector *vector)
{
    return vector->capacity;
}

void reserve_vector(Vector *vector, const size_t amount)
{
    if (amount > vector->capacity)
    {
        void *tmp = realloc(vector->array, amount * vector->size);

        assert(tmp);

        vector->array = tmp;
        vector->capacity = amount;
    }
}

void shrink_to_fit_vector(Vector *vector)
{
    const size_t nmemb = vector->nmemb;
    const size_t capacity = vector->capacity;

    if (nmemb < capacity)
    {
        void *tmp = realloc(vector->array, nmemb * vector->size);

        assert(tmp);

        vector->array = tmp;
        vector->capacity = nmemb;
    }
}
