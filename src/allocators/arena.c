#include <memory.h>
#include "../../internals/base.h"
#include "../../internals/cassert.h"
#include "../../arena.h"

struct Page
{
    struct Page *prev;
    void *base;
    size_t size;
    size_t offset;
};

typedef struct Arena
{
    struct Page *curr;
} Arena;


static struct Page *construct_page(struct Page *prev,
                                   const size_t growth_policy,
                                   const size_t size)
{
    const size_t n_size = growth_policy * size;

    void *memory = malloc(sizeof(struct Page) + n_size);

    CHEAP_ASSERT(memory,
                 "Arena unable to acquire memory.");

    struct Page *page = memory;

    page->prev = prev;
    page->base = memory + sizeof(struct Page);
    page->size = n_size;
    page->offset = 0;

    return page;
}

static struct Page *arena_destroy_page(struct Page *page)
{
    struct Page *prev = page->prev;

    free(page);

    return prev;
}

static void arena_destroy_pages(struct Page **page)
{
    do {
        *page = arena_destroy_page(*page);
    } while (*page);
}


static void *arena_alloc(struct Page **curr,
                         const size_t size)
{
    if ((*curr)->size - (*curr)->offset < size)
    {
        *curr = construct_page(*curr,
                               2,
                               (*curr)->size);
    }

    void *ptr = ((*curr)->base + (*curr)->offset);

    (*curr)->offset += size;

    return ptr;
}

static void *arena_c_alloc(struct Page **curr,
                           const size_t size)
{
    void *ptr = arena_alloc(curr,
                      size);

    return memset(ptr,
                  0,
                  size);
}

static void arena_clear(struct Page **curr)
{
    while ((*curr)->prev)
    {
        (*curr) = arena_destroy_page(*curr);
    }

    (*curr)->offset = 0;
}


Arena *create_arena(const size_t size)
{
    Arena *arena = memory_allocate_container(sizeof(Arena));

    arena->curr = construct_page(NULL,
                                 1,
                                 size);

    return arena;
}

void destroy_arena(Arena **arena)
{
    arena_destroy_pages(&(*arena)->curr);

    memory_free_buffer((void**)arena);
}


void *alloc_arena(Arena *arena,
                  const size_t size)
{
    return arena_alloc(&arena->curr,
                       size);
}

void *calloc_arena(Arena *arena,
                   const size_t size)
{
    return arena_c_alloc(&arena->curr,
                         size);
}


void clear_arena(Arena *arena)
{
    arena_clear(&arena->curr);
}
