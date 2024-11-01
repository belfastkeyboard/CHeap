#include <memory.h>
#include "../linked.h"

static struct Node *create_node(Arena *arena, const size_t size, const void *value)
{
    struct Node *node = calloc_arena(arena, sizeof(struct Node));

    node->value = alloc_arena(arena, size);
    memcpy(node->value, value, size);

    return node;
}
static void destroy_node(Arena *arena, struct Node *node, const size_t size)
{
    free_arena(arena, node->value, size);
    free_arena(arena, node, sizeof(struct Node));
}

static void create_node_singly_linked(Arena *arena, const size_t nmemb, const size_t size, const void *value, struct Node *position, struct Node **head, int prior)
{
    struct Node *node = create_node(arena, size, value);

    if (prior || !nmemb)
    {
        *head = node;
        node->next = position;
    }
    else
    {
        node->next = position->next;
        position->next = node;
    }
}
static void erase_node_singly_linked(Arena *arena, struct Node *position, const size_t size, struct Node **head, int prior)
{
    if (prior)
    {
        *head = position->next;
        destroy_node(arena, position, size);
    }
    else
    {
        struct Node *erase = position->next;
        position->next = position->next->next;
        destroy_node(arena, erase, size);
    }
}

static void create_node_doubly_linked(Arena *arena, const size_t nmemb, const size_t size, const void *value, struct Node *position, struct Node **head, struct Node **tail)
{
    struct Node *node = create_node(arena, size, value);

    if (!nmemb)
    {
        *head = node;
        *tail = node;
    }
    else if (!position)
    {
        (*tail)->next = node;
        node->prev = *tail;
        *tail = node;
    }
    else
    {
        if (position->prev)
        {
            position->prev->next = node;
            node->prev = position->prev;
        }
        else
        {
            *head = node;
        }

        position->prev = node;
        node->next = position;
    }
}
static void erase_node_doubly_linked(Arena *arena, struct Node *position, const size_t size, struct Node **head, struct Node **tail)
{
    if (position->prev)
        position->prev->next = position->next;
    else
        *head = position->next;

    if(position->next)
        position->next->prev = position->prev;
    else
        *tail = position->prev;

    destroy_node(arena, position, size);
}

static struct Node *seek_node(const size_t nmemb, const size_t index, struct Node *head, struct Node *tail)
{
    struct Node *seek = NULL;

    if (index < nmemb)
    {
        if (tail && index > nmemb / 2)
        {
            seek = tail;

            for (size_t i = nmemb - 1; i > index; i--)
                seek = seek->prev;
        }
        else
        {
            seek = head;

            for (size_t i = 0; i < index; i++)
                seek = seek->next;
        }
    }

    return seek;
}

void generic_push_front_singly_linked(Arena *arena, size_t *nmemb, size_t size, struct Node **head, const void *value)
{
    generic_insert_singly_linked(arena, nmemb, size, head, value, 0, 1);
}
size_t generic_insert_singly_linked(Arena *arena, size_t *nmemb, const size_t size, struct Node **head, const void *value, size_t index, int prior)
{
    struct Node *seek = seek_node(*nmemb, index, *head, NULL);
    create_node_singly_linked(arena, *nmemb, size, value, seek, head, prior);
    (*nmemb)++;

    return index;
}

void generic_push_front_doubly_linked(Arena *arena, size_t *nmemb, size_t size, struct Node **head, struct Node **tail, const void *value)
{
    generic_insert_doubly_linked(arena, nmemb, size, head, tail, value, 0);
}
void generic_push_back_doubly_linked(Arena *arena, size_t *nmemb, size_t size, struct Node **head, struct Node **tail, const void *value)
{
    generic_insert_doubly_linked(arena, nmemb, size, head, tail, value, *nmemb);
}
size_t generic_insert_doubly_linked(Arena *arena, size_t *nmemb, const size_t size, struct Node **head, struct Node **tail, const void *value, size_t index)
{
    struct Node *seek = seek_node(*nmemb, index, *head, *tail);
    create_node_doubly_linked(arena, *nmemb, size, value, seek, head, tail);
    (*nmemb)++;

    return index;
}

void generic_pop_front_singly_linked(Arena *arena, size_t *nmemb, size_t size, struct Node **head)
{
    generic_erase_singly_linked(arena, nmemb, size, 0, head, 1);
}
size_t generic_erase_singly_linked(Arena *arena, size_t *nmemb, size_t size, size_t index, struct Node **head, int prior)
{
    struct Node *seek = seek_node(*nmemb, index, *head, NULL);
    erase_node_singly_linked(arena, seek, size, head, prior);
    (*nmemb)--;

    return index;
}

void generic_pop_front_doubly_linked(Arena *arena, size_t *nmemb, size_t size, struct Node **head, struct Node **tail)
{
    generic_erase_doubly_linked(arena, nmemb, size, 0, head, tail);
}
void generic_pop_back_doubly_linked(Arena *arena, size_t *nmemb, size_t size, struct Node **head, struct Node **tail)
{
    generic_erase_doubly_linked(arena, nmemb, size, *nmemb - 1, head, tail);
}
size_t generic_erase_doubly_linked(Arena *arena, size_t *nmemb, size_t size, size_t index, struct Node **head, struct Node **tail)
{
    struct Node *seek = seek_node(*nmemb, index, *head, *tail);
    erase_node_doubly_linked(arena, seek, size, head, tail);
    (*nmemb)--;

    return index;
}
void generic_clear_linked(Arena *arena, struct Node **head, struct Node **tail, size_t *nmemb)
{
    clear_arena(arena);

    *head = NULL;

    if (tail)
        *tail = NULL;

    *nmemb = 0;
}

void *generic_access_linked(struct Node *node)
{
    return node->value;
}