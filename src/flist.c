#include <malloc.h>
#include <memory.h>
#include "../flist.h"

typedef struct Node
{
    void *value;
    struct Node *next;
} Node, Iter;
typedef struct ForwardList
{
    Node *head;

    size_t nmemb;
    size_t size;

    Comparator cmp;
} ForwardList;

ForwardList *create_forward_list(size_t size, Comparator comparator)
{
    ForwardList *flist = malloc(sizeof(ForwardList));

    flist->head = NULL;
    flist->nmemb = 0;
    flist->size = size;
    flist->cmp = comparator;

    return flist;
}
void destroy_forward_list(ForwardList **flist)
{
    clear(*flist);

    free(*flist);
    *flist = NULL;
}

void push_front(ForwardList *flist, void *value)
{
    Node *curr = malloc(sizeof(Node));

    curr->value = malloc(sizeof(flist->size));
    memcpy(curr->value, value, flist->size);
    curr->next = NULL;

    if (flist->head)
        curr->next = flist->head;

    flist->head = curr;
    flist->nmemb++;
}
void insert(ForwardList *flist, Iter *iter, void *value)
{
    if (iter)
    {
        Node *node = malloc(sizeof(Node));

        node->value = malloc(flist->size);
        memcpy(node->value, value, flist->size);

        node->next = iter->next;
        iter->next = node;

        flist->nmemb++;
    }
}

void *front(ForwardList *flist)
{
    return flist->head->value;
}
Iter *find(ForwardList *flist, void *value)
{
    Node *node = NULL;

    if (flist->head)
    {
        Node *curr = flist->head;

        while (!node && curr)
        {
            if (flist->cmp(curr->value, value))
                node = curr;
            else
                curr = curr->next;
        }
    }

    return node;
}

void pop_front(ForwardList *flist)
{
    if (flist->head)
    {
        Node *front = flist->head;
        flist->head = flist->head->next;

        free(front);

        flist->nmemb--;
    }
}
Node *erase_after(ForwardList *flist, Iter *iter)
{
    Node *node = iter;

    if (iter && iter->next)
    {
        Node *next = iter->next;

        iter->next = next->next;
        node = iter->next;

        free(next->value);
        free(next);

        flist->nmemb--;
    }

    return node;
}
void clear(ForwardList *flist)
{
    if (flist->head)
    {
        Node *curr = flist->head;
        Node *prev = NULL;

        do {
            free(curr->value);

            prev = curr;
            curr = curr->next;

            free(prev);
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
