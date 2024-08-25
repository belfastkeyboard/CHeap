#include <assert.h>
#include <malloc.h>
#include "arena.h"

struct Page
{
    struct Page *prev;

    void *base;
    size_t size;
    size_t offset;
};
typedef struct Arena
{
    ArenaType type;
    struct Page *curr;
    size_t size;
} Arena;

static struct Page *construct_page(struct Page *prev, const size_t size)
{
    struct Page *page = malloc(sizeof(struct Page));

    page->prev = prev;
    page->base = malloc(size);
    page->size = size;
    page->offset = 0;

    return page;
}
static struct Page *destruct_page(Arena *arena, struct Page *page)
{
    arena->size -= page->size;

    struct Page *prev = page->prev;

    free(page->base);
    free(page);

    return prev;
}

Arena *g_arena = NULL;

Arena *create_arena(const size_t size, const ArenaType type)
{
    Arena *arena = malloc(sizeof(Arena));

    arena->size = size;
    arena->curr = construct_page(NULL, size);

    if (type == ARENA_GLOBAL)
    {
        arena->type = ARENA_STATIC;
        g_arena = arena;
        arena = NULL;
    }
    else
        arena->type = type;

    return arena;
}
void destroy_arena(Arena **arena)
{
    if (*arena == NULL)
    {
        assert(g_arena && "Global arena not defined. Did you forget to create one, or did you destroy the arena?");
        *arena = g_arena;
        g_arena = NULL;
    }

    do {
        (*arena)->curr = destruct_page(*arena, (*arena)->curr);
    } while ((*arena)->curr);

    free(*arena);
    *arena = NULL;
}
void clear_arena(Arena *arena)
{
    if (arena == NULL)
    {
        assert(g_arena && "Global arena not defined. Did you forget to create one, or did you destroy the arena?");
        arena = g_arena;
    }

    while (arena->curr->prev)
        arena->curr = destruct_page(arena, arena->curr);

    arena->curr->offset = 0;
}
void *alloc_arena(Arena *arena, const size_t size)
{
    if (arena == NULL)
    {
        assert(g_arena && "Global arena not defined. Did you forget to create one, or did you destroy the arena?");
        arena = g_arena;
    }

    void *ptr = NULL;

    if (arena->curr->size - arena->curr->offset >= size)
    {
        ptr = arena->curr->base + arena->curr->offset;
        arena->curr->offset += size;
    }
    else if (arena->type == ARENA_DYNAMIC)
    {
        arena->curr = construct_page(arena->curr, arena->size);
        arena->curr->offset += size;
        ptr = arena->curr->base + arena->curr->offset;
        arena->size += arena->curr->size;
    }

    return ptr;
}
void free_arena(Arena *arena, size_t size)
{
    if (arena == NULL)
    {
        assert(g_arena && "Global arena not defined. Did you forget to create one, or did you destroy the arena?");
        arena = g_arena;
    }

    if (arena->type == ARENA_STATIC)
    {
        if (size > arena->curr->offset)
            size = arena->curr->offset;

        arena->curr->offset -= size;
    }
    else if (arena->type == ARENA_DYNAMIC)
    {
        while (size > 0)
        {
            if (!arena->curr->prev)
            {
                if (size > arena->curr->offset)
                    size = arena->curr->offset;

                arena->curr->offset -= size;
                size = 0;
            }
            else
            {
                if (size > arena->curr->offset)
                {
                    size -= arena->curr->offset;
                    arena->curr = destruct_page(arena, arena->curr);
                }
                else
                {
                    arena->curr->offset -= size;
                    size = 0;
                }
            }
        }
    }
}

size_t memory_remaining_arena(const Arena *arena)
{
    if (arena == NULL)
    {
        assert(g_arena && "Global arena not defined. Did you forget to create one, or did you destroy the arena?");
        arena = g_arena;
    }

    return arena->curr->size - arena->curr->offset;
}
ArenaType get_type_arena(const Arena *arena)
{
    if (arena == NULL)
    {
        assert(g_arena && "Global arena not defined. Did you forget to create one, or did you destroy the arena?");
        arena = g_arena;
    }

    return arena->type;
}
