#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include "../../internals/base.h"
#include "../../internals/mpool.h"
#include "../../internals/nalloc.h"
#include "../../deque.h"


#define MIN_BLOCK_SIZE 4096
#define MIN_ELEM_COUNT 16


static size_t max(const size_t size)
{
    return (MIN_BLOCK_SIZE > size * MIN_ELEM_COUNT) ? MIN_BLOCK_SIZE :
                                                      size * MIN_ELEM_COUNT;
}

struct Block
{
    void *array;
    size_t nmemb;
    size_t capacity;
};


typedef struct DoubleEndedQueue
{
    struct Block *blocks;
    size_t nmemb;
    size_t capacity;
    size_t arr_size;
    size_t size;
    size_t front;
    size_t back;
} DoubleEndedQueue, Deque;


Deque *create_deque(const size_t size)
{
    Deque *deque = memory_allocate_container(sizeof(Deque));

    const size_t array_size = max(size);
    const size_t capacity = array_size / size;
    const size_t init_index = capacity / 2;

    deque->blocks = calloc(1,
                           sizeof(struct Block));

    deque->arr_size = array_size;
    deque->size = size;
    deque->capacity = 1;
    deque->nmemb = 1;

    deque->blocks[0].array = malloc(array_size);
    deque->blocks[0].capacity = capacity;

    deque->front = init_index;
    deque->back = init_index - 1;

    return deque;
}

void destroy_deque(Deque **deque)
{
    const size_t front = (*deque)->front;
    const size_t nmemb = (*deque)->nmemb;
    const size_t capacity = (*deque)->capacity;

    for (size_t i = front, j = 0; j < nmemb; i = i + 1 % capacity, j++)
    {
        free((*deque)->blocks[i].array);
    }

    free((*deque)->blocks);

    memory_free_buffer((void**)deque);
}


static void resize(Deque *deque,
                   const void *value,
                   const size_t index)
{
    assert(index == 0 || index == 1);

    struct Block *blocks = malloc(sizeof(struct Block) * 2);

    memcpy(blocks + 1 - index,
           deque->blocks,
           sizeof(struct Block));

    free(deque->blocks);

    struct Block block = {
            .array = malloc(deque->arr_size),
            .capacity = deque->arr_size / deque->size,
            .nmemb = index
    };

    blocks[index] = block;

    deque->blocks = blocks;
    deque->front = 0;
    deque->back = 1;
    deque->capacity = 2;
    deque->nmemb++;

    const size_t capacity = block.capacity;
    const size_t nmemb = deque->blocks[0].nmemb + 1;
    const size_t size = deque->size;
    void *array = deque->blocks[index].array;

    void *dest = (index) ? array :
                           array + (capacity - nmemb) * size;

    memcpy(dest,
           value,
           size);
}


void push_front_deque(Deque *deque,
                      const void *value)
{
    if (deque->nmemb == 1)
    {
        if (!deque->front)
        {
            resize(deque,
                   value,
                   0);
        }
        else
        {
            deque->front--;

            memcpy(deque->blocks[0].array + (deque->front * deque->size),
                   value,
                   deque->size);
        }

        deque->blocks[0].nmemb++;
    }
    else
    {
        struct Block *block = &deque->blocks[deque->front];

        if (block->nmemb >= block->capacity)
        {
            if (deque->nmemb >= deque->capacity)
            {
                deque->capacity *= 2;

                const size_t capacity = deque->capacity;
                const size_t nmemb = deque->nmemb;
                const size_t hcap = capacity / 2;
                const size_t hn = nmemb / 2;
                const size_t f_index = hcap - hn;

                const size_t f = f_index;
                const size_t b = f + deque->nmemb - 1;

                struct Block *blocks = malloc(sizeof(struct Block) * deque->capacity);

                if (deque->front > deque->back)
                {
                    const size_t front_i = deque->front;
                    const size_t span = nmemb - front_i;

                    memcpy(blocks + f,
                           deque->blocks + front_i,
                           sizeof(struct Block) * span);

                    memcpy(blocks + f + span,
                           deque->blocks,
                           sizeof(struct Block) * (f + span));
                }
                else
                {
                    memcpy(blocks + f,
                           deque->blocks,
                           sizeof(struct Block) * deque->nmemb);
                }

                free(deque->blocks);

                deque->blocks = blocks;

                deque->front = f;
                deque->back = b;
            }

            deque->front = (deque->front - 1) % deque->capacity;

            block = &deque->blocks[deque->front];

            block->capacity = deque->arr_size / deque->size;
            block->nmemb = 0;
            block->array = malloc(deque->arr_size);

            deque->nmemb++;
        }

        block->nmemb++;

        memcpy(block->array + (block->capacity - block->nmemb) * deque->size,
               value,
               deque->size);
    }
}

void push_back_deque(Deque *deque,
                     const void *value)
{
    if (deque->nmemb == 1)
    {
        if (deque->back == deque->blocks[0].capacity - 1)
        {
            resize(deque,
                   value,
                   1);
        }
        else
        {
            deque->back++;
            deque->blocks[0].nmemb++;

            const size_t back = deque->back;
            const size_t size = deque->size;
            const size_t b_index = back * size;
            void *array = deque->blocks[0].array;
            void *dest = array + b_index;
            const void *src = value;

            memcpy(dest,
                   src,
                   size);
        }
    }
    else
    {
        struct Block *block = &deque->blocks[deque->back];

        if (block->nmemb >= block->capacity)
        {
            if (deque->nmemb >= deque->capacity)
            {
                deque->capacity *= 2;

                const size_t capacity = deque->capacity;
                const size_t nmemb = deque->nmemb;
                const size_t hcap = capacity / 2;
                const size_t hn = nmemb / 2;
                const size_t f_index = hcap - hn;

                const size_t f = f_index;
                const size_t b = f + nmemb - 1;

                struct Block *blocks = malloc(sizeof(struct Block) * deque->capacity);

                if (deque->front > deque->back)
                {
                    const size_t front_i = deque->front;
                    const size_t span = nmemb - front_i;

                    memcpy(blocks + f,
                           deque->blocks + front_i,
                           sizeof(struct Block) * span);

                    memcpy(blocks + f + span,
                           deque->blocks,
                           sizeof(struct Block) * (span - f));
                }
                else
                {
                    memcpy(blocks + f,
                           deque->blocks,
                           sizeof(struct Block) * nmemb);
                }

                free(deque->blocks);

                deque->blocks = blocks;

                deque->front = f;
                deque->back = b;
            }

            deque->back = (deque->back + 1) % deque->capacity;

            block = &deque->blocks[deque->back];

            block->capacity = deque->arr_size / deque->size;
            block->nmemb = 0;
            block->array = malloc(deque->arr_size);

            deque->nmemb++;
        }

        memcpy(block->array + block->nmemb * deque->size,
               value,
               deque->size);

        block->nmemb++;
    }
}

void *front_deque(const Deque *deque)
{
    void *result = NULL;

    if (deque->nmemb == 1)
    {
        result = deque->blocks[0].array + (deque->front * deque->size);
    }
    else
    {
        struct Block block = deque->blocks[deque->front];

        result = block.array + (block.capacity - block.nmemb) * deque->size;
    }

    return result;
}

void *back_deque(const Deque *deque)
{
    void *result = NULL;

    if (deque->nmemb == 1)
    {
        result = deque->blocks[0].array + (deque->back * deque->size);
    }
    else
    {
        struct Block block = deque->blocks[deque->back];

        result = block.array + (block.nmemb - 1) * deque->size;
    }

    return result;
}
