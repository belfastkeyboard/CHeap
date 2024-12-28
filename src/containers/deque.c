#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include <stdbool.h>
#include "../../internals/base.h"
#include "../../internals/nalloc.h"
#include "../../deque.h"


#define MIN_BLOCK_SIZE 4096
#define MIN_ELEM_COUNT 16


struct Block
{
    void *array;
    size_t nmemb;
    size_t capacity;
};


typedef struct DoubleEndedQueue
{
    struct Block *blocks;
    size_t block_count;
    size_t nmemb;
    size_t capacity;
    size_t arr_cap;
    size_t size;
    size_t front;
    size_t back;
} DoubleEndedQueue, Deque;


static size_t max(const size_t size)
{
    return (MIN_BLOCK_SIZE > size * MIN_ELEM_COUNT) ? MIN_BLOCK_SIZE :
                                                      size * MIN_ELEM_COUNT;
}


static bool has_space_at_front(const size_t front)
{
    return front == 0;
}

static bool has_space_at_back(const size_t back,
                              const size_t capacity)
{
    return back == capacity - 1;
}

static bool is_full(const struct Block *block)
{
    return block->nmemb >= block->capacity;
}

static bool is_front(const size_t index,
                     const size_t front)
{
    return index == front;
}


Deque *create_deque(const size_t size)
{
    Deque *deque = memory_allocate_container(sizeof(Deque));

    const size_t array_size = max(size);
    const size_t capacity = array_size / size;
    const size_t init_index = capacity / 2;

    deque->blocks = calloc(1,
                           sizeof(struct Block));

    deque->arr_cap = capacity;
    deque->size = size;
    deque->capacity = 1;
    deque->block_count = 1;

    deque->blocks[0].array = malloc(array_size * capacity);
    deque->blocks[0].capacity = capacity;

    deque->front = init_index;
    deque->back = init_index - 1;

    return deque;
}

void destroy_deque(Deque **deque)
{
    const size_t front = (*deque)->front;
    const size_t nmemb = (*deque)->block_count;
    const size_t capacity = (*deque)->capacity;

    for (size_t i = front, j = 0; j < nmemb; i = i + 1 % capacity, j++)
    {
        free((*deque)->blocks[i].array);
    }

    free((*deque)->blocks);

    memory_free_buffer((void**)deque);
}


static void resize_from_single(struct Block **dblocks,
                               const void *value,
                               size_t *front,
                               size_t *back,
                               size_t *dcap,
                               size_t *block_count,
                               const size_t index,
                               const size_t capacity,
                               const size_t size)
{
    assert(index == 0 || index == 1);

    struct Block *blocks = malloc(sizeof(struct Block) * 2);

    memcpy(blocks + 1 - index,
           *dblocks,
           sizeof(struct Block));

    free(*dblocks);

    struct Block block = { 0 };

    block.array = malloc(capacity * size); // not leaked
    block.capacity = capacity;
    block.nmemb = index;

    blocks[index] = block;

    *dblocks = blocks;
    *front = 0;
    *back = 1;
    *dcap = 2;
    (*block_count)++;

    const size_t nmemb = (*dblocks)[0].nmemb + 1;
    void *array = (*dblocks)[index].array;

    void *dest = (index) ? array :
                           array + (capacity - nmemb) * size;

    memcpy(dest,
           value,
           size);
}

static void resize_from_multiple(struct Block **dblocks,
                                 struct Block **block,
                                 size_t *front,
                                 size_t *back,
                                 size_t *nmemb,
                                 size_t *dcap,
                                 const size_t arr_size,
                                 const size_t size,
                                 const bool forward)
{
    if (*nmemb >= *dcap)
    {
        *dcap *= 2;

        const size_t f_index = (*dcap) / 2 - (*nmemb) / 2;

        const size_t f = f_index;
        const size_t b = f + (*nmemb) - 1;

        struct Block *blocks = malloc(sizeof(struct Block) * (*dcap));

        if ((*front) > (*back))
        {
            const size_t span = (*nmemb) - (*front);
            const size_t diff = (forward) ? f + span :
                                            span - f;

            memcpy(blocks + f,
                   (*dblocks) + (*front),
                   sizeof(struct Block) * span);

            memcpy(blocks + f + span,
                   *dblocks,
                   sizeof(struct Block) * diff);
        }
        else
        {
            memcpy(blocks + f,
                   *dblocks,
                   sizeof(struct Block) * (*nmemb));
        }

        free(*dblocks);

        *dblocks = blocks;

        *front = f;
        *back = b;
    }

    if (forward)
    {
        *front = ((*front) - 1) % (*dcap);

        *block = (*dblocks) + *front;
    }
    else
    {
        *back = ((*back) + 1) % (*dcap);

        *block = (*dblocks) + *back;
    }

    (*block)->capacity = arr_size;
    (*block)->nmemb = 0;
    (*block)->array = malloc(arr_size * size);

    (*nmemb)++;
}


static void insert(void *dest,
                   const void *src,
                   const size_t size,
                   size_t *nmemb)
{
    memcpy(dest,
           src,
           size);

    (*nmemb)++;
}


static void push_front_single(struct Block **blocks,
                              const void *value,
                              size_t *front,
                              size_t *back,
                              size_t *capacity,
                              size_t *block_count,
                              const size_t arr_cap,
                              const size_t size)
{
    if (has_space_at_front(*front))
    {
        resize_from_single(blocks,
                           value,
                           front,
                           back,
                           capacity,
                           block_count,
                           0,
                           arr_cap,
                           size);
    }
    else
    {
        (*front)--;

        insert((*blocks)[0].array + ((*front) * size),
               value,
               size,
               &(*blocks)[0].nmemb);
    }
}

static void push_front_multiple(struct Block **blocks,
                                const void *value,
                                size_t *front,
                                size_t *back,
                                size_t *block_count,
                                size_t *capacity,
                                const size_t arr_cap,
                                const size_t size)
{
    struct Block *block = &(*blocks)[*front];

    if (block->nmemb >= block->capacity)
    {
        resize_from_multiple(blocks,
                             &block,
                             front,
                             back,
                             block_count,
                             capacity,
                             arr_cap,
                             size,
                             true);
    }

    block->nmemb++;

    memcpy(block->array + (block->capacity - block->nmemb) * size,
           value,
           size);
}


static void push_back_single(struct Block **blocks,
                             const void *value,
                             size_t *front,
                             size_t *back,
                             size_t *capacity,
                             size_t *block_count,
                             const size_t arr_cap,
                             const size_t size)
{
    if (has_space_at_back(*back,
                          arr_cap))
    {
        resize_from_single(blocks,
                           value,
                           front,
                           back,
                           capacity,
                           block_count,
                           1,
                           arr_cap,
                           size);
    }
    else
    {
        (*back)++;

        insert((*blocks)[0].array + ((*back) * size),
               value,
               size,
               &(*blocks)[0].nmemb);
    }
}

static void push_back_multiple(struct Block **blocks,
                               const void *value,
                               size_t *front,
                               size_t *back,
                               size_t *block_count,
                               size_t *capacity,
                               const size_t arr_cap,
                               const size_t size)
{
    struct Block *block = &(*blocks)[*back];

    if (block->nmemb >= block->capacity)
    {
        resize_from_multiple(blocks,
                             &block,
                             front,
                             back,
                             block_count,
                             capacity,
                             arr_cap,
                             size,
                             false);
    }

    memcpy(block->array + block->nmemb * size,
           value,
           size);

    block->nmemb++;
}


void push_front_deque(Deque *deque,
                      const void *value)
{
    if (deque->block_count == 1)
    {
        push_front_single(&deque->blocks,
                          value,
                          &deque->front,
                          &deque->back,
                          &deque->capacity,
                          &deque->block_count,
                          deque->arr_cap,
                          deque->size);
    }
    else
    {
        push_front_multiple(&deque->blocks,
                            value,
                            &deque->front,
                            &deque->back,
                            &deque->block_count,
                            &deque->capacity,
                            deque->arr_cap,
                            deque->size);
    }

    deque->nmemb++;
}

void push_back_deque(Deque *deque,
                     const void *value)
{
    if (deque->block_count == 1)
    {
        push_back_single(&deque->blocks,
                         value,
                         &deque->front,
                         &deque->back,
                         &deque->capacity,
                         &deque->block_count,
                         deque->arr_cap,
                         deque->size);
    }
    else
    {
        push_back_multiple(&deque->blocks,
                           value,
                           &deque->front,
                           &deque->back,
                           &deque->block_count,
                           &deque->capacity,
                           deque->arr_cap,
                           deque->size);
    }

    deque->nmemb++;
}


static size_t locate_block(Deque *deque,
                           size_t *index)
{
    size_t result = deque->front;
    size_t accumulate = deque->blocks[result].nmemb;

    while (accumulate < *index) // TODO: try the first block
    {
        *index -= accumulate;
        result = (result + 1) % deque->capacity;
        accumulate = deque->blocks[result].nmemb;
    }

    return result;
}

static void try_insert(struct Block **blocks,
                       const void *value,
                       size_t *front,
                       size_t *back,
                       size_t *block_count,
                       size_t *capacity,
                       const size_t arr_cap,
                       const size_t index,
                       const size_t b_index,
                       const size_t size)
{
    struct Block *block = (*blocks) + b_index;

    if(is_full(block))
    {
        void *hold = malloc(size);

        memcpy(hold,
               block->array + ((block->nmemb - 1) * size),
               size);

        try_insert(blocks,
                   hold,
                   front,
                   back,
                   block_count,
                   capacity,
                   arr_cap,
                   0,
                   b_index + 1,
                   size);

        memmove(block->array + (index + 1) * size,
                block->array + index * size,
                (block->nmemb - index - 1) * size);

        memcpy(block->array + index * size,
               value,
               size);

        free(hold);
    }
    else
    {
        if (is_front(b_index,
                     *front))
        {
            memmove(block->array + (block->capacity - block->nmemb - 1) * size,
                    block->array + (block->capacity - block->nmemb) * size,
                    index * size);

            memcpy(block->array + index * size,
                   value,
                   size);
        }
        else
        {
            memmove(block->array + 1 * size,
                    block->array,
                    block->nmemb * size);

            memcpy(block->array,
                   value,
                   size);
        }

        block->nmemb++;

        // do the insert part here
        // consider:
        //  1. inserting at the front block
        //  2. inserting where the whole deque is full
        //  3. normal insertion
    }
}


void insert_deque(Deque *deque,
                  const void *value,
                  size_t index)
{
    assert(index <= deque->nmemb);

    if (!index)
    {
        push_front_deque(deque,
                         value);
    }
    else if (index == deque->nmemb)
    {
        push_back_deque(deque,
                        value);
    }
    else
    {
        const size_t b_index = locate_block(deque,
                                            &index);

        try_insert(&deque->blocks,
                   value,
                   &deque->front,
                   &deque->back,
                   &deque->block_count,
                   &deque->capacity,
                   deque->arr_cap,
                   index,
                   b_index,
                   deque->size);
    }


    deque->nmemb++;
}


void *front_deque(const Deque *deque)
{
    void *result = NULL;

    if (deque->block_count == 1)
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

    if (deque->block_count == 1)
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
