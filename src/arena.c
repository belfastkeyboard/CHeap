#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include "../arena.h"

struct Page
{
    struct Page *prev;

    void *base;
    size_t size;
    size_t offset;
};
__attribute__((warn_unused_result))
static struct Page *construct_page(struct Page *prev, const size_t nmemb, const size_t size)
{
    struct Page *page = malloc(sizeof(struct Page));

    page->prev = prev;
    page->base = calloc(nmemb, size);
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

static int arena_error = ARENA_NO_ERROR;

typedef struct Arena
{
    ArenaType type;
    struct Page *curr;
} Arena;

// helper functions
static void *arena_do_alloc(Arena *arena, const size_t size)
{
    void *ptr = (arena->curr->base + arena->curr->offset);

    arena->curr->offset += size;

    return ptr;
}
void *static_arena_alloc(Arena *arena, const size_t size)
{
    void *ptr = NULL;

    if (size + arena->curr->offset > arena->curr->size)
        arena_error = ARENA_INSUFFICIENT_MEMORY;
    else
        ptr = arena_do_alloc(arena, size);

    return ptr;
}
void *dynamic_arena_alloc(Arena *arena, const size_t size)
{
    void *ptr = NULL;

    if (arena->curr->size - arena->curr->offset < size && arena->type == ARENA_DYNAMIC)
        arena->curr = construct_page(arena->curr, 2, arena->curr->size);

    ptr = arena_do_alloc(arena, size);

    return ptr;
}

Arena *create_arena(const size_t nmemb, const size_t size, const ArenaType type)
{
    Arena *arena = malloc(sizeof(Arena));

    arena->curr = construct_page(NULL, nmemb, size);
    arena->type = type;

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

void *alloc_arena(Arena *arena, const size_t size)
{
    void *ptr = NULL;

    if (size + arena->curr->offset < size)
        arena_error = ARENA_REQUEST_OVERFLOW;
    else if (arena->type == ARENA_STATIC)
        ptr = static_arena_alloc(arena, size);
    else if (arena->type == ARENA_DYNAMIC)
        ptr = dynamic_arena_alloc(arena, size);

    return ptr;
}
void *calloc_arena(Arena *arena, size_t size)
{
    return memset(alloc_arena(arena, size), 0, size);
}

void free_arena(Arena *arena, void *ptr, const size_t size)
{
    if (size <= arena->curr->offset && arena->curr->base + arena->curr->offset - size == ptr)
        arena->curr->offset -= size;

    if (arena->type == ARENA_DYNAMIC)
    {
        if (arena->curr->offset == 0 && arena->curr->prev)
            arena->curr = destroy_page(arena->curr);
    }
}
void clear_arena(Arena *arena)
{
    while (arena->curr->prev)
        arena->curr = destroy_page(arena->curr);

    arena->curr->offset = 0;
}

size_t memory_remaining_arena(const Arena *arena)
{
    return arena->curr->size - arena->curr->offset;
}
ArenaType get_type_arena(const Arena *arena)
{
    return arena->type;
}
int get_arena_error_code(void)
{
    return arena_error;
}
