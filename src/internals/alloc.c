#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include "../../internals/alloc.h"

struct Block
{
    size_t size;
    struct Block *next;
};

struct Cheap_Allocator
{
    void *memory;
    size_t size;
    size_t curr;
    struct Block *blocks;
};


static struct Cheap_Allocator allocator;


static struct Block *next_block(struct Block *block)
{
    return (void*)block + block->size + sizeof(struct Block);
}

static void rebase(struct Block *prev, struct Block *curr)
{
    if (prev)
    {
        prev->next = curr->next;
    }
    else
    {
        allocator.blocks = curr->next;
    }
}


static void *malloc_free_list_search(const size_t size)
{
    assert(allocator.blocks);

    struct Block *curr = allocator.blocks;
    struct Block *prev = NULL;

    void *memory = NULL;

    while (curr &&
           !memory)
    {
        if (curr->size >= size)
        {
            if (curr->size > size + sizeof(struct Block))
            {
                struct Block *block = (void*)curr + size;

                block->size = curr->size - size - sizeof(struct Block);
                block->next = curr->next;

                curr->size = size;
                curr->next = block;
            }

            rebase(prev, curr);

            memory = (void*)curr + sizeof(struct Block);
        }
        else
        {
            if (curr->next &&
                next_block(curr) == curr->next)
            {
                curr->size += sizeof(struct Block) + curr->next->size;
                curr->next = curr->next->next;
            }
            else if (curr->next &&
                     next_block(curr) == allocator.memory + allocator.curr)
            {
                allocator.curr -= curr->size + sizeof(struct Block);

                rebase(prev, curr);

                curr = curr->next;
            }
            else
            {
                prev = curr;
                curr = curr->next;
            }
        }
    }

    return memory;
}

static void *malloc_bump_alloc(const size_t size)
{
    assert(allocator.curr + size + sizeof(struct Block) <= allocator.size);

    struct Block* block = allocator.memory + allocator.curr;
    void *memory = allocator.memory + allocator.curr + sizeof(struct Block);

    block->size = size;
    allocator.curr += size + sizeof(struct Block);

    return memory;
}


void open_cheap_context(const size_t size)
{
    assert(!allocator.memory &&
           size > 16);

    allocator = (struct Cheap_Allocator){
        .memory = NULL,
        .size   = size,
        .curr   = 0,
        .blocks = NULL
    };

    allocator.memory = malloc(size);

    assert(allocator.memory);
}

void close_cheap_context(void)
{
    assert(allocator.blocks);

    free(allocator.memory);

    allocator = (struct Cheap_Allocator){ 0 };
}


void *cheap_malloc_impl(size_t size)
{
    assert(allocator.memory);

    void *memory = NULL;

    if (allocator.curr + size + sizeof(struct Block) <= allocator.size)
    {
        memory = malloc_bump_alloc(size);
    }
    else
    {
        memory = malloc_free_list_search(size);
    }

    assert(memory);

    return memory;
}

void *cheap_calloc_impl(size_t size)
{
    void *memory = cheap_malloc_impl(size);

    return memset(memory, 0, size);
}

void *cheap_realloc_impl(void *ptr, const size_t size)
{
    assert(allocator.memory &&
           ptr + size <= allocator.memory + allocator.size);

    struct Block *block = ptr - sizeof(struct Block);

    if (ptr + block->size == allocator.memory + allocator.curr)
    {
        allocator.curr += size - block->size;
        block->size = size;
    }
    else
    {
        void *tmp = cheap_malloc_impl(size);

        memcpy(ptr, tmp, block->size);

        cheap_free_impl(ptr);

        ptr = tmp;
    }

    return ptr;
}

void cheap_free_impl(void *ptr)
{
    assert(allocator.memory && ptr);

    struct Block *block = ptr - sizeof(struct Block);

    if (ptr + block->size == allocator.memory + allocator.curr)
    {
        allocator.curr -= block->size + sizeof(struct Block);
    }
    else
    {
        block->next = allocator.blocks;
        allocator.blocks = block;
    }
}

void cheap_clear_impl(void)
{
    assert(allocator.memory);

    allocator.curr = 0;
    allocator.blocks = NULL;
}
