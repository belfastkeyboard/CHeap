#include "../../flist.h"
#include "../../internals/linked.h"
#include "../../internals/base.h"

typedef struct ForwardList
{
    struct NodeAlloc *alloc;
    size_t nmemb;
    size_t size;
    struct Node *head;
} ForwardList, FList;


FList *create_forward_list(const size_t size)
{
    FList *flist = memory_allocate_container(sizeof(FList));

    flist->alloc = create_node_allocator(sizeof(struct Node),
                                         size,
                                         0);

    flist->size = size;

    return flist;
}

void destroy_forward_list(FList **flist)
{
    destroy_node_allocator(&(*flist)->alloc);

    memory_free_buffer((void**)flist);
}


void push_front_forward_list(FList *flist,
                             const void *value)
{
    generic_push_front_singly_linked(flist->alloc,
                                     &flist->nmemb,
                                     flist->size,
                                     &flist->head,
                                     value);
}

size_t insert_after_forward_list(FList *flist,
                                 const void *value,
                                 size_t index)
{
    return generic_insert_singly_linked(flist->alloc,
                                        &flist->nmemb,
                                        flist->size,
                                        &flist->head,
                                        value,
                                        index,
                                        0);
}


void *front_forward_list(const FList *flist)
{
    return generic_access_linked(flist->head);
}


void pop_front_forward_list(FList *flist)
{
    generic_pop_front_singly_linked(flist->alloc,
                                    &flist->nmemb,
                                    &flist->head);
}

size_t erase_after_forward_list(FList *flist,
                                const size_t index)
{
    return generic_erase_singly_linked(flist->alloc,
                                       &flist->nmemb,
                                       index,
                                       &flist->head,
                                       0);
}

void clear_forward_list(FList *flist)
{
    generic_clear_linked(flist->alloc,
                         &flist->head,
                         NULL,
                         &flist->nmemb);
}


bool empty_forward_list(const FList *flist)
{
    return generic_empty(flist->nmemb);
}

size_t size_forward_list(const FList *flist)
{
    return generic_size(flist->nmemb);
}
