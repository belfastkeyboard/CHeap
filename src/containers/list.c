#include "../../list.h"
#include "../../internals/linked.h"
#include "../../internals/base.h"

struct Page;

typedef struct List
{
    struct Page *curr;
    size_t nmemb;
    size_t size;
    struct Node *head;
    struct Node *tail;
} List;


List *create_list(const size_t size)
{
    List *list = memory_allocate_container(sizeof(List));

    list->curr = create_pages(size);
    list->size = size;

    return list;
}

void destroy_list(List **list)
{
    destroy_pages(&(*list)->curr);
    memory_free_buffer((void**)list);
}


void push_back_list(List *list,
                    const void *value)
{
    generic_push_back_doubly_linked(&list->curr,
                                    &list->nmemb,
                                    list->size,
                                    &list->head,
                                    &list->tail,
                                    value);
}

void push_front_list(List *list,
                     const void *value)
{
    generic_push_front_doubly_linked(&list->curr,
                                     &list->nmemb,
                                     list->size,
                                     &list->head,
                                     &list->tail,
                                     value);
}

size_t insert_list(List *list,
                   const void *value,
                   const size_t index)
{
    return generic_insert_doubly_linked(&list->curr,
                                        &list->nmemb,
                                        list->size,
                                        &list->head,
                                        &list->tail,
                                        value,
                                        index);
}


void *front_list(const List *list)
{
    return generic_access_linked(list->head);
}

void *back_list(const List *list)
{
    return generic_access_linked(list->tail);
}


void pop_front_list(List *list)
{
    generic_pop_front_doubly_linked(&list->nmemb,
                                    &list->head,
                                    &list->tail);
}

void pop_back_list(List *list)
{
    generic_pop_back_doubly_linked(&list->nmemb,
                                   &list->head,
                                   &list->tail);
}

size_t erase_list(List *list,
                  const size_t index)
{
    return generic_erase_doubly_linked(&list->nmemb,
                                       index,
                                       &list->head,
                                       &list->tail);
}

void clear_list(List *list)
{
    generic_clear_linked(&list->curr,
                         &list->head,
                         &list->tail,
                         &list->nmemb);
}


bool empty_list(const List *list)
{
    return generic_empty(list->nmemb);
}

size_t size_list(const List *list)
{
    return generic_size(list->nmemb);
}
