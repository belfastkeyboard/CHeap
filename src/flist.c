#include <malloc.h>
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
} ForwardList;

static Node *create_node(size_t size, void *value)
{
    struct Node *node = malloc(sizeof(Node));

    node->value = malloc(size);
    memcpy(node->value, value, size);
    node->next = NULL;

    return node;
}
static void destroy_node(Node **node)
{
    free((*node)->value);
    free(*node);
}

ForwardList *create_forward_list(size_t size)
{
    ForwardList *flist = memory_allocate_container(sizeof(ForwardList));

    flist->head = NULL;
    flist->nmemb = 0;
    flist->size = size;

    return flist;
}
void destroy_forward_list(ForwardList *flist)
{
    clear(flist);

    memory_free_container((void**)&flist);
}

void push_front(ForwardList *flist, void *value)
{
    Node *curr = create_node(flist->size, value);

    if (flist->head)
        curr->next = flist->head;

    flist->head = curr;
    flist->nmemb++;
}
Iter insert(ForwardList *flist, Iter iter, void *value)
{
    if (iter)
    {
        Node *node = malloc(sizeof(Node));

        node->value = malloc(flist->size);
        memcpy(node->value, value, flist->size);

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

        destroy_node(&front);

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

        destroy_node(&next);

        flist->nmemb--;
    }

    return iter;
}
void clear(ForwardList *flist)
{
    if (flist->head)
    {
        Node *curr = flist->head;
        Node *prev = NULL;

        do {
            prev = curr;
            curr = curr->next;

            destroy_node(&prev);
        } while (curr);

        flist->head = NULL;
        flist->nmemb = 0;
    }
}

bool empty(ForwardList *flist)
{
    return (flist->nmemb == 0);
}
size_t size(ForwardList *flist)
{
    return flist->nmemb;
}
