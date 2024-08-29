#include <memory.h>
#include "../list.h"
#include "../base.h"
#include "../arena.h"

typedef struct Node
{
    void *value;
    struct Node *next;
    struct Node *prev;
} Node, Iter;
typedef struct List
{
    Node *head;
    Node *tail;

    size_t nmemb;
    size_t size;

    Arena *arena;
} List;

static Node *create_node(Arena *arena, size_t size, void *value)
{
    struct Node *node = alloc_arena(arena, sizeof(Node));

    node->value = alloc_arena(arena, size);
    memcpy(node->value, value, size);

    node->next = NULL;
    node->prev = NULL;

    return node;
}
static void destroy_node(Arena *arena, Node *node, size_t size)
{
    free_arena(arena, node->value, size);
    free_arena(arena, node, sizeof(Node));
}

List *create_list(size_t size)
{
    List *list = memory_allocate_container(sizeof(List));

    list->head = NULL;
    list->tail = NULL;
    list->nmemb = 0;
    list->size = size;

    list->arena = create_arena((sizeof(Node) + size) * 8, ARENA_DYNAMIC);

    return list;
}
void destroy_list(List *list)
{
    destroy_arena(&list->arena);
    memory_free_container((void**)&list);
}

void push_back(List *list, void *value)
{
    Node *curr = create_node(list->arena, list->size, value);

    if (list->tail)
    {
        curr->prev = list->tail;
        curr->prev->next = curr;
    }
    else
        list->head = curr;

    list->tail = curr;

    list->nmemb++;
}
void push_front(List *list, void *value)
{
    Node *curr = create_node(list->arena, list->size, value);

    if (list->head)
    {
        curr->next = list->head;
        curr->next->prev = curr;
    }
    else
        list->tail = curr;

    list->head = curr;
    list->nmemb++;
}
void insert(List *list, Iter *iter, void *value)
{
    Node *node = create_node(list->arena, list->size, value);

    node->prev = iter->prev;
    node->next = iter;

    iter->prev->next = node;
    iter->prev = node;

    list->nmemb++;
}

void *front(List *list)
{
    return list->head->value;
}
void *back(List *list)
{
    return list->tail->value;
}

void pop_front(List *list)
{
    Node *front = list->head;
    list->head = list->head->next;

    if (list->head)
        list->head->prev = NULL;

    destroy_node(list->arena, front, list->size);

    list->nmemb--;

    if (!list->nmemb)
        list->tail = NULL;
}
void pop_back(List *list)
{
    Node *back = list->tail;
    list->tail = list->tail->prev;
    if (list->tail)
        list->tail->next = NULL;

    destroy_node(list->arena, back, list->size);

    list->nmemb--;

    if (!list->nmemb)
        list->head = NULL;
}
Node *erase(List *list, Iter *iter)
{
    Node *node = iter;

    if (iter)
    {
        if (iter->prev)
            iter->prev->next = iter->next;
        else
            list->head = iter->next;

        if (iter->next)
            iter->next->prev = iter->prev;
        else
            list->tail = iter->prev;

        node = iter->next;

        destroy_node(list->arena, iter, list->size);

        list->nmemb--;
    }

    return node;
}
void clear(List *list)
{
    clear_arena(list->arena);

    list->head = NULL;
    list->tail = NULL;
    list->nmemb = 0;
}

bool empty(List *list)
{
    return (list->nmemb == 0);
}
size_t size(List *list)
{
    return list->nmemb;
}
