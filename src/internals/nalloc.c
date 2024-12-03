#include <assert.h>
#include <malloc.h>
#include "../../internals/nalloc.h"

#define INIT_COUNT 8

#define MIN_SIZE sizeof(void*)

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

struct NodeAlloc
{
    struct Block *blocks;
    struct Page *pages;
};


static size_t growth_policy(const size_t x)
{
    return x * 2;
}

/*  This safety check exists purely to facilitate debugging.
 *  Allocated types must be convertible into a block, which is the size of a pointer (usually 8 bytes).
 *  All node-types will meet the minimum size as, by definition, nodes must point to another node,
 *  either in a list structure or a tree structure.
 */
static size_t min_size(const size_t size)
{
    return (size < MIN_SIZE) ? MIN_SIZE : size;
}


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

    void *ptr = *blocks;

    *blocks = (*blocks)->next;

    return ptr;
}

static void *page_allocate(struct Page **page)
{
    assert(*page);

    struct Page *curr = *page;

    if (curr->cursor >= curr->nmemb)
    {
        *page = create_page(curr,
                            growth_policy(curr->nmemb),
                            curr->size);

        curr = *page;
    }

    void *ptr = curr->pool + curr->cursor * curr->size;

    curr->cursor++;

    return ptr;
}

static void *allocate_memory(struct Block **blocks,
                             struct Page **page)
{
    assert(page);

    void *ptr = NULL;

    if (*blocks)
    {
        ptr = search_freeblocks(blocks);
    }
    else
    {
        ptr = page_allocate(page);
    }

    return ptr;
}

static void free_memory(struct Page **page,
                        struct Block **blocks,
                        void *ptr)
{
    struct Page *curr = *page;

    if (curr->pool + (curr->cursor - 1) * curr->size == ptr)
    {
        curr->cursor--;

        if (!curr->cursor &&
            curr->prev)
        {
            *page = destroy_page(curr);
        }
    }
    else
    {
        struct Block *block = ptr;

        block->next = *blocks;

        *blocks = block;
    }
}

static void destroy_pages(struct NodeAlloc *allocator)
{
    while (allocator->pages)
    {
        allocator->pages = destroy_page(allocator->pages);
    }
}


struct NodeAlloc *create_node_allocator(const size_t size)
{
    struct NodeAlloc *allocator = malloc(sizeof(struct NodeAlloc));

    // initialising with a compound literal tricks static analysis into thinking that create_page() leaks memory
    // so we just do it this way instead

    allocator->blocks = NULL;

    allocator->pages = create_page(NULL,
                                   INIT_COUNT,
                                   min_size(size));

    return allocator;
}

void destroy_node_allocator(struct NodeAlloc **allocator)
{
    destroy_pages(*allocator);

    free(*allocator);

    *allocator = NULL;
}


void *alloc_node(struct NodeAlloc *allocator)
{
    return allocate_memory(&allocator->blocks,
                           &allocator->pages);
}

void free_node(struct NodeAlloc *allocator,
               void *ptr)
{
    free_memory(&allocator->pages,
                &allocator->blocks,
                ptr);
}

void clear_nodes(struct NodeAlloc *allocator)
{
    while (allocator->pages->prev)
    {
        allocator->pages = destroy_page(allocator->pages);
    }
}
