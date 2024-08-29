#include <memory.h>
#include "../flist.h"
#include "../base.h"
#include "../arena.h"

typedef struct Node
{
    void *value;
    struct Node *next;
} Node;
typedef struct ForwardList
{
    Node *head;

    size_t nmemb;
    size_t size;

    Arena *arena;
} ForwardList;

static Node *create_node(Arena *arena, size_t size, void *value)
{
    struct Node *node = alloc_arena(arena, sizeof(Node));

    node->value = alloc_arena(arena, size);
    memcpy(node->value, value, size);
    node->next = NULL;

    return node;
}
static void destroy_node(Arena *arena, Node *node, size_t size)
{
    free_arena(arena, node->value, size);
    free_arena(arena, node, size);
}

ForwardList *create_forward_list(size_t size)
{
    ForwardList *flist = memory_allocate_container(sizeof(ForwardList));
    flist->arena = create_arena((sizeof(Node) + size) * 8, ARENA_DYNAMIC);

    flist->head = NULL;
    flist->nmemb = 0;
    flist->size = size;

    return flist;
}
void destroy_forward_list(ForwardList *flist)
{
    destroy_arena(&flist->arena);
    memory_free_container((void**)&flist);
}

void push_front(ForwardList *flist, void *value)
{
    Node *curr = create_node(flist->arena, flist->size, value);

    if (flist->head)
        curr->next = flist->head;

    flist->head = curr;
    flist->nmemb++;
}
Iter insert(ForwardList *flist, Iter iter, void *value)
{
    if (iter)
    {
        Node *node = create_node(flist->arena, flist->size, value);

        node->next = ((Node*)iter)->next;
        iter = node;

        flist->nmemb++;
    }

    return iter;
}

void *front(ForwardList *flist)
{
    return flist->head->value;
}

void pop_front(ForwardList *flist)
{
    if (flist->head)
    {
        Node *front = flist->head;
        flist->head = flist->head->next;

        destroy_node(flist->arena, front, flist->size);

        flist->nmemb--;
    }
}
Iter erase_after(ForwardList *flist, Iter iter)
{
    Node *node = (Node*)iter;

    if (node && node->next)
    {
        Node *next = node->next;

        node->next = next->next;
        iter = (void*)node->next;

        destroy_node(flist->arena, next, flist->size);

        flist->nmemb--;
    }

    return iter;
}
void clear(ForwardList *flist)
{
    clear_arena(flist->arena);
}

bool empty(ForwardList *flist)
{
    return (flist->nmemb == 0);
}
size_t size(ForwardList *flist)
{
    return flist->nmemb;
}
