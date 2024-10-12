#include "../list.h"
#include "../linked.h"
#include "../base.h"

typedef struct List
{
    Arena *arena;

    size_t nmemb;
    size_t size;

    struct Node *head;
    struct Node *tail;
} List;

List *create_list(const size_t size)
{
    List *list = memory_allocate_container(sizeof(List));

    list->arena = create_arena(ARENA_CONTAINER_INIT_COUNT, (sizeof(struct Node) + size), ARENA_DYNAMIC);
    list->size = size;

    return list;
}
void destroy_list(List **list)
{
    memory_free_container_arena((void**)list, (*list)->arena);
}

void push_back_list(List *list, void *value)
{
    generic_push_back_doubly_linked(list->arena, &list->nmemb, list->size, &list->head, &list->tail, value);
}
void push_front_list(List *list, void *value)
{
    generic_push_front_doubly_linked(list->arena, &list->nmemb, list->size, &list->head, &list->tail, value);
}
size_t insert_list(List *list, void *value, const size_t index)
{
    return generic_insert_doubly_linked(list->arena, &list->nmemb, list->size, &list->head, &list->tail, value, index);
}

void *front_list(List *list)
{
    return generic_access_linked(list->head);
}
void *back_list(List *list)
{
    return generic_access_linked(list->tail);
}

void pop_front_list(List *list)
{
    generic_pop_front_doubly_linked(list->arena, &list->nmemb, list->size, &list->head, &list->tail);
}
void pop_back_list(List *list)
{
    generic_pop_back_doubly_linked(list->arena, &list->nmemb, list->size, &list->head, &list->tail);
}
size_t erase_list(List *list, size_t index)
{
    return generic_erase_doubly_linked(list->arena, &list->nmemb, list->size, index, &list->head, &list->tail);
}
void clear_list(List *list)
{
    generic_clear_linked(list->arena, &list->head, &list->tail, &list->nmemb);
}

bool empty_list(List *list)
{
    return generic_empty(list->nmemb);
}
size_t size_list(List *list)
{
    return generic_size(list->nmemb);
}
