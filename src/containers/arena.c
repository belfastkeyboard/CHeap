#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include "../../internals/base.h"
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


__attribute__((warn_unused_result))
static struct Page *construct_page(struct Page *prev,
                                   const size_t nmemb,
                                   const size_t size)
{
    struct Page *page = malloc(sizeof(struct Page));

    page->prev = prev;

    page->base = calloc(nmemb,
                        size);

    page->size = nmemb * size;
    page->offset = 0;

    return page;
}

__attribute__((warn_unused_result))
static struct Page *destroy_page(struct Page *page)
{
    struct Page *prev = page->prev;

    free(page->base);
    free(page);

    return prev;
}


// helper functions
static void *arena_do_alloc(Arena *arena,
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

    free(*arena);
    *arena = NULL;
}


void *alloc_arena(Arena *arena,
                  const size_t size)
{
    void *ptr = NULL;

    ptr = dynamic_arena_alloc(arena,
                              size);

    assert(ptr);

    return ptr;
}

void *calloc_arena(Arena *arena,
                   size_t size)
{
    return memset(alloc_arena(arena,
                              size),
                              0,
                              size);
}


void free_arena(Arena *arena,
                void *ptr,
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
