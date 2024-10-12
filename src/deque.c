#include "../base.h"
#include "../mpool.h"
#include "../deque.h"

typedef struct DoubleEndedQueue
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
} DoubleEndedQueue, Deque;

Deque *create_deque(const size_t size)
{
    Deque *deque = memory_allocate_container(sizeof(Deque));

    deque->size = size;

    return deque;
}
void destroy_deque(Deque **deque)
{
    memory_free_container_mempool((void **) deque, (*deque)->array);
}

void push_front_deque(Deque *deque, void *value)
{
    generic_mempool_push_front(&deque->array, value, &deque->capacity, &deque->nmemb, deque->size);
}
void push_back_deque(Deque *deque, void *value)
{
    generic_mempool_push_back(&deque->array, value, &deque->capacity, &deque->nmemb, deque->size);
}
void insert_deque(Deque *deque, void *const value, const size_t index)
{
    generic_mempool_insert(&deque->array, value, index, &deque->capacity, &deque->nmemb, deque->size);
}

void *front_deque(Deque *deque)
{
    return contiguous_random_access(deque->array, 0, deque->size);
}
void *back_deque(Deque *deque)
{
    return contiguous_random_access(deque->array, deque->nmemb - 1, deque->size);
}

void pop_front_deque(Deque *deque)
{
    generic_mempool_pop_front(&deque->array, &deque->nmemb, deque->size);
}
void pop_back_deque(Deque *deque)
{
    generic_mempool_pop_back(&deque->nmemb);
}
size_t erase_deque(Deque *deque, const size_t index)
{
    return generic_mempool_erase(&deque->array, index, &deque->nmemb, deque->size);
}
void clear_deque(Deque *deque)
{
    generic_mempool_clear(&deque->array, deque->capacity, &deque->nmemb, deque->size);
}

bool empty_deque(Deque *deque)
{
    return generic_empty(deque->nmemb);
}
size_t size_deque(Deque *deque)
{
    return generic_size(deque->nmemb);
}
