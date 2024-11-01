#include "../../flist.h"
#include "../../internals/linked.h"
#include "../../internals/base.h"

typedef struct ForwardList
{
    struct Page *curr;

    size_t nmemb;
    size_t size;

    struct Node *head;
} ForwardList, FList;

FList *create_forward_list(const size_t size)
{
    FList *flist = memory_allocate_container(sizeof(FList));

    flist->curr = create_pages(size);
    flist->size = size;

    return flist;
}
void destroy_forward_list(FList **flist)
{
    destroy_pages(&(*flist)->curr);
    memory_free_buffer((void**)flist);
}

void push_front_forward_list(FList *flist, void *value)
{
    generic_push_front_singly_linked(&flist->curr, &flist->nmemb, flist->size, &flist->head, value);
}
size_t insert_after_forward_list(FList *flist, void *value, size_t index)
{
    return generic_insert_singly_linked(&flist->curr, &flist->nmemb, flist->size, &flist->head, value, index, 0);
}

void *front_forward_list(FList *flist)
{
    return generic_access_linked(flist->head);
}

void pop_front_forward_list(FList *flist)
{
    generic_pop_front_singly_linked(&flist->nmemb, &flist->head);
}
size_t erase_after_forward_list(FList *flist, size_t index)
{
    return generic_erase_singly_linked(&flist->nmemb, index, &flist->head, 0);
}
void clear_forward_list(FList *flist)
{
    generic_clear_linked(&flist->curr, &flist->head, NULL, &flist->nmemb);
}

bool empty_forward_list(FList *flist)
{
    return generic_empty(flist->nmemb);
}
size_t size_forward_list(FList *flist)
{
    return generic_size(flist->nmemb);
}
