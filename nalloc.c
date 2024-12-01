#include <assert.h>
#include <malloc.h>
#include "nalloc.h"

#define INIT_COUNT 8

struct Block
{
    struct Block *next;
};

struct Page
{
    void *pool;
    size_t cursor;
    size_t nmemb;
    size_t size;

    struct Page *prev;
};

struct NodeAllocator
{
    struct Block *blocks;
    struct Page *pages;
    size_t size;
};


static struct Page *create_page(struct Page *prev,
                                const size_t nmemb,
                                const size_t size)
{
    void *mem = malloc(sizeof(struct Page) + nmemb * size);

    assert(mem);

    struct Page *page = mem;
    void *pool = mem + sizeof(struct Page);

    *page = (struct Page){
        .pool = pool,
        .cursor = 0,
        .nmemb = nmemb,
        .size = size,
        .prev = prev
    };

    return page;
}

static struct Page *destroy_page(struct Page *page)
{
    assert(page);

    struct Page *prev = page->prev;

    free(page);

    return prev;
}


static void *search_freeblocks(struct Block **blocks)
{
    assert(*blocks);

    struct Block *head = *blocks;

    void *ptr = head;

    *blocks = head->next;

    return ptr;
}

static void *page_allocate(struct Page **page,
                           const size_t size)
{
    assert(*page);

    struct Page *curr = *page;

    if (curr->cursor >= curr->size)
    {
        *page = create_page(*page,
                            (*page)->nmemb,
                            (*page)->size);

        curr = *page;
    }

    void *ptr = curr->pool + curr->cursor;

    curr->cursor += size;

    return ptr;
}

static void *allocate_memory(struct Block **blocks,
                             struct Page **page,
                             const size_t size)
{
    assert(page);

    void *ptr = NULL;

    if (blocks)
    {
        ptr = search_freeblocks(blocks);
    }
    else
    {
        ptr = page_allocate(page,
                            size);
    }

    return ptr;
}


NodeAllocator *create_node_allocator(const size_t size)
{
    NodeAllocator *allocator = malloc(sizeof(NodeAllocator));

    // initialising with a compound literal tricks static analysis into thinking that create_page() leaks memory
    // so we just do it this way instead

    allocator->blocks = NULL;

    allocator->pages = create_page(NULL,
                                   INIT_COUNT,
                                   size);

    allocator->size = (size > sizeof(struct Block)) ? size :
                                                      sizeof(struct Block);

    return allocator;
}

void destroy_node_allocator(NodeAllocator **allocator)
{
    while ((*allocator)->pages)
    {
        (*allocator)->pages = destroy_page((*allocator)->pages);
    }

    free(*allocator);

    *allocator = NULL;
}


void *node_alloc(NodeAllocator *allocator)
{
    return allocate_memory(&allocator->blocks,
                           &allocator->pages,
                           allocator->size);
}

void node_free(NodeAllocator *allocator,
               void *ptr)
{
    if (allocator->pages->pool + allocator->pages->cursor == ptr)
    {
        allocator->pages->cursor -= allocator->size;
    }
    else
    {
        struct Block *block = ptr;

        block->next = allocator->blocks;

        allocator->blocks = block;
    }
}


