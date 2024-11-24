#include <memory.h>
#include "../../internals/alloc.h"
#include "../../internals/base.h"
#include "../../internals/cassert.h"
#include "../../arena.h"

#ifndef CHEAP_ALLOC
    #include <malloc.h>
#endif

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


__attribute__((warn_unused_result))
static struct Page *construct_page(struct Page *prev,
                                   const size_t nmemb,
                                   const size_t size)
{
    struct Page *page = CHEAP_MALLOC(sizeof(struct Page));

    page->prev = prev;

    page->base = CHEAP_CALLOC(nmemb,
                              size);

    page->size = nmemb * size;
    page->offset = 0;

    return page;
}

__attribute__((warn_unused_result))
static struct Page *destroy_page(struct Page *page)
{
    struct Page *prev = page->prev;

    CHEAP_FREE(page->base);
    CHEAP_FREE(page);

    return prev;
}


// helper functions
static void *arena_do_alloc(const Arena *arena,
                            const size_t size)
{
    void *ptr = (arena->curr->base + arena->curr->offset);

    arena->curr->offset += size;

    return ptr;
}

void *dynamic_arena_alloc(Arena *arena,
                          const size_t size)
{
    if (arena->curr->size - arena->curr->offset < size)
    {
        arena->curr = construct_page(arena->curr,
                                     2,
                                     arena->curr->size);
    }

    return arena_do_alloc(arena,
                          size);
}


Arena *create_arena(const size_t nmemb,
                    const size_t size)
{
    Arena *arena = memory_allocate_container(sizeof(Arena));

    arena->curr = construct_page(NULL,
                                 nmemb,
                                 size);

    return arena;
}

void destroy_arena(Arena **arena)
{
    do {
        (*arena)->curr = destroy_page((*arena)->curr);
    } while ((*arena)->curr);

    CHEAP_FREE(*arena);
    *arena = NULL;
}


void *alloc_arena(Arena *arena,
                  const size_t size)
{
    void *ptr = NULL;

    ptr = dynamic_arena_alloc(arena,
                              size);

    CHEAP_ASSERT(ptr, "Arena unable to acquire memory.");

    return ptr;
}

void *calloc_arena(Arena *arena,
                   const size_t size)
{
    void *ptr = alloc_arena(arena,
                            size);

    return memset(ptr,
                  0,
                  size);
}


void free_arena(Arena *arena,
                const void *ptr,
                const size_t size)
{
    if (size <= arena->curr->offset && arena->curr->base + arena->curr->offset - size == ptr)
    {
        arena->curr->offset -= size;
    }

    if (arena->curr->offset == 0 && arena->curr->prev)
    {
        arena->curr = destroy_page(arena->curr);
    }
}

void clear_arena(Arena *arena)
{
    while (arena->curr->prev)
    {
        arena->curr = destroy_page(arena->curr);
    }

    arena->curr->offset = 0;
}
