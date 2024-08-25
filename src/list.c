#include <malloc.h>
#include <memory.h>
#include "../list.h"

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

    Comparator cmp;
} List;

List *create_list(size_t size, Comparator comparator)
{
    List *list = malloc(sizeof(List));

    list->head = NULL;
    list->tail = NULL;
    list->nmemb = 0;
    list->size = size;
    list->cmp = comparator;

    return list;
}
void destroy_list(List **list)
{
    clear(*list);

    free(*list);
    *list = NULL;
}

void push_back(List *list, void *value)
{
    Node *curr = malloc(sizeof(Node));

    curr->value = malloc(sizeof(list->size));
    memcpy(curr->value, value, list->size);
    curr->prev = NULL;
    curr->next = NULL;

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
    Node *curr = malloc(sizeof(Node));

    curr->value = malloc(sizeof(list->size));
    memcpy(curr->value, value, list->size);
    curr->prev = NULL;
    curr->next = NULL;

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
    if (iter)
    {
        Node *node = malloc(sizeof(Node));

        node->value = malloc(list->size);
        memcpy(node->value, value, list->size);
        node->prev = iter->prev;
        node->next = iter;

        iter->prev->next = node;
        iter->prev = node;

        list->nmemb++;
    }
}

void *front(List *list)
{
    return list->head->value;
}
void *back(List *list)
{
    return list->tail->value;
}
Iter *find(List *list, void *value)
{
    Node *node = NULL;

    if (list->head)
    {
        Node *curr = list->head;

        do {
            if (list->cmp(curr->value, value))
                node = curr;
            else
                curr = curr->next;
        } while (!node && curr);
    }

    return node;
}

void pop_front(List *list)
{
    if (list->head)
    {
        Node *front = list->head;
        list->head = list->head->next;
        if (list->head)
            list->head->prev = NULL;
        free(front);

        list->nmemb--;

        if (!list->nmemb)
            list->tail = NULL;
    }
}
void pop_back(List *list)
{
    if (list->tail)
    {
        Node *back = list->tail;
        list->tail = list->tail->prev;
        if (list->tail)
            list->tail->next = NULL;
        free(back);

        list->nmemb--;

        if (!list->nmemb)
            list->head = NULL;
    }
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

        free(iter);

        list->nmemb--;
    }

    return node;
}
void clear(List *list)
{
    if (list->tail)
    {
        Node *curr = list->tail;
        do {
            free(curr->value);
            curr = curr->prev;
            if (curr)
                free(curr->next);
        } while (curr);
        free(list->head);

        list->head = NULL;
        list->tail = NULL;
        list->nmemb = 0;
    }
}

bool empty(List *list)
{
    return (list->nmemb == 0);
}
size_t size(List *list)
{
    return list->nmemb;
}
