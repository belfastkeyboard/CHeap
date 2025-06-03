#include <assert.h>
#include <malloc.h>
#include "../../internals/nalloc.h"


struct NodeBlock
{
    struct NodeBlock *next;
};

struct NodePage
{
    void *pool;
    size_t cursor;
    size_t max;
    size_t size;

    struct NodePage *prev;
};


static size_t growth_policy(const size_t x)
{
    return x * 2;
}


static struct NodePage *create_page(struct NodePage *prev,
                                const size_t nmemb,
                                const size_t size)
{
    void *mem = calloc(1,
                       sizeof(struct NodePage) + nmemb * size);

    assert(mem);

    struct NodePage *page = mem;
    void *pool = mem + sizeof(struct NodePage);

    *page = (struct NodePage){
        .pool = pool,
        .cursor = 0,
        .max = nmemb,
        .size = size,
        .prev = prev
    };

    return page;
}

static struct NodePage *destroy_page(struct NodePage *page)
{
    assert(page);

    struct NodePage *prev = page->prev;

    free(page);

    return prev;
}


static void *search_freeblocks(struct NodeBlock **blocks)
{
    assert(*blocks);

    void *ptr = *blocks;

    *blocks = (*blocks)->next;

    return ptr;
}

static void *page_allocate(struct NodePage **page)
{
    assert(*page);

    struct NodePage *curr = *page;

    if (curr->cursor >= curr->max)
    {
        *page = create_page(curr,
                            growth_policy(curr->max),
                            curr->size);

        curr = *page;
    }

    void *ptr = curr->pool + curr->cursor * curr->size;

    curr->cursor++;

    return ptr;
}

static void *allocate_memory(struct NodeBlock **blocks,
                             struct NodePage **page)
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

static void free_memory(struct NodePage **page,
                        struct NodeBlock **blocks,
                        void *ptr)
{
    struct NodePage *curr = *page;

    static int i = 0;

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
        struct NodeBlock *block = ptr;

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


struct NodeAlloc create_node_allocator(const size_t node_size,
                                       const size_t nmemb,
                                       const size_t t1_size,
                                       const size_t t2_size)
{
    struct NodeAlloc allocator = {
        .blocks = NULL,
        .pages = create_page(NULL,
                             nmemb,
                             node_size + t1_size + t2_size)
    };

    return allocator;
}

void destroy_node_allocator(struct NodeAlloc *allocator)
{
    destroy_pages(allocator);
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

	allocator->blocks = NULL;
	allocator->pages->cursor = 0;
}
