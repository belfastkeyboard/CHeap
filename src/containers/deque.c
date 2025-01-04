#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include <stdbool.h>
#include "../../internals/base.h"
#include "../../internals/nalloc.h"
#include "../../internals/deq.h"
#include "../../deque.h"


#define MIN_BLOCK_SIZE 4096
#define MIN_ELEM_COUNT 16


typedef struct DoubleEndedQueue
{
    struct ControlArray control;
    size_t nmemb;
    size_t arr_cap;
    size_t size;
    size_t front;
    size_t back;
} DoubleEndedQueue, Deque;


Deque *create_deque(const size_t size)
{
    Deque *deque = memory_allocate_container(sizeof(Deque));

    const size_t array_size = (MIN_BLOCK_SIZE > size * MIN_ELEM_COUNT) ? MIN_BLOCK_SIZE :
                                                                         size * MIN_ELEM_COUNT;

    const size_t capacity = array_size / size;
    const size_t init_index = capacity / 2;

    deque->control = create_control_array(capacity,
                                          size);

    deque->arr_cap = capacity;
    deque->size = size;

    deque->front = init_index;
    deque->back = init_index - 1;

    return deque;
}

void destroy_deque(Deque **deque)
{
    destroy_control_array(&(*deque)->control,
                          (*deque)->front);

    memory_free_buffer((void**)deque);
}


void push_front_deque(Deque *deque,
                      const void *value)
{
    deque_push_front(&deque->control,
                     &deque->front,
                     &deque->back,
                     deque->arr_cap,
                     value,
                     deque->size,
                     &deque->nmemb);
}

void push_back_deque(Deque *deque,
                     const void *value)
{
    deque_push_back(&deque->control,
                    &deque->front,
                    &deque->back,
                    deque->arr_cap,
                    value,
                    deque->size,
                    &deque->nmemb);
}

void insert_deque(Deque *deque,
                  const void *value,
                  size_t index)
{
    deque_insert(&deque->control,
                 &deque->front,
                 &deque->back,
                 value,
                 deque->arr_cap,
                 deque->size,
                 &deque->nmemb,
                 index);
}


void *at_deque(const Deque *deque,
               size_t index)
{
    return deque_at(&deque->control,
                    deque->front,
                    deque->arr_cap,
                    deque->nmemb,
                    deque->size,
                    index);
}

void *front_deque(const Deque *deque)
{
    return deque_front(&deque->control,
                       deque->front,
                       deque->arr_cap,
                       deque->size,
                       deque->nmemb);
}

void *back_deque(const Deque *deque)
{
    return deque_back(&deque->control,
                      deque->back,
                      deque->size,
                      deque->nmemb);
}


void pop_front_deque(Deque *deque)
{
    deque_pop_front(&deque->control,
                    &deque->front,
                    &deque->back,
                    &deque->nmemb);
}

void pop_back_deque(Deque *deque)
{
    deque_pop_back(&deque->control,
                   &deque->front,
                   &deque->back,
                   &deque->nmemb,
                   deque->arr_cap);
}

size_t erase_deque(Deque *deque,
                   size_t index)
{
    return deque_erase(&deque->control,
                       &deque->front,
                       &deque->back,
                       &deque->nmemb,
                       deque->arr_cap,
                       index,
                       deque->size);
}

void clear_deque(Deque *deque)
{
    deque_clear(&deque->control,
                &deque->front,
                &deque->back,
                &deque->nmemb,
                deque->arr_cap);
}


bool empty_deque(const Deque *deque)
{
    return generic_empty(deque->nmemb);
}

size_t size_deque(const Deque *deque)
{
    return generic_size(deque->nmemb);
}
