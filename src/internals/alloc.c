#include "../../internals/cassert.h"
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
    CHEAP_ASSERT(allocator.blocks,
                 "Free blocks list cannot be NULL.");

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
    CHEAP_ASSERT(allocator.curr + size + sizeof(struct Block) <= allocator.size,
                 "Bump allocator has insufficient available memory.");

    struct Block* block = allocator.memory + allocator.curr;
    void *memory = allocator.memory + allocator.curr + sizeof(struct Block);

    block->size = size;
    allocator.curr += size + sizeof(struct Block);

    return memory;
}


void open_cheap_context(const size_t size)
{
    CHEAP_ASSERT(!allocator.memory,
                 "CHeap allocator context is already open.");

    CHEAP_ASSERT(size > 16,
                 "Must allocate more than 16 bytes.");

    allocator = (struct Cheap_Allocator){
        .memory = NULL,
        .size   = size,
        .curr   = 0,
        .blocks = NULL
    };

    allocator.memory = malloc(size);

    CHEAP_ASSERT(allocator.memory,
                 "Failed to allocate memory.");
}

void close_cheap_context(void)
{
    CHEAP_ASSERT(allocator.memory,
                 "Cheap context is already closed.");

    free(allocator.memory);

    allocator = (struct Cheap_Allocator){ 0 };
}


void *cheap_malloc_impl(size_t size)
{
    CHEAP_ASSERT(allocator.memory,
                 "Trying to allocate memory without opening CHeap context.");

    void *memory = NULL;

    if (allocator.curr + size + sizeof(struct Block) <= allocator.size)
    {
        memory = malloc_bump_alloc(size);
    }
    else
    {
        memory = malloc_free_list_search(size);
    }

    CHEAP_ASSERT(memory,
                 "CHeap allocator failed to provision memory.");

    return memory;
}

void *cheap_calloc_impl(size_t size)
{
    void *memory = cheap_malloc_impl(size);

    return memset(memory, 0, size);
}

void *cheap_realloc_impl(void *ptr, const size_t size)
{
    CHEAP_ASSERT(allocator.memory,
                 "Trying to reallocate memory without opening CHeap context.");

    CHEAP_ASSERT(ptr + size <= allocator.memory + allocator.size,
                 "Insufficient memory to reallocate.");

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
    CHEAP_ASSERT(allocator.memory,
                 "Trying to free memory without opening CHeap context.");

    if (ptr)
    {
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
}

void cheap_clear_impl(void)
{
    CHEAP_ASSERT(allocator.memory,
                 "Trying to clear memory without opening CHeap context.");

    allocator.curr = 0;
    allocator.blocks = NULL;
}
