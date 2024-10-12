#include "../flist.h"
#include "../linked.h"
#include "../base.h"

typedef struct ForwardList
{
    Arena *arena;

    size_t nmemb;
    size_t size;

    struct Node *head;
} ForwardList, FList;

FList *create_forward_list(const size_t size)
{
    FList *flist = memory_allocate_container(sizeof(FList));

    flist->arena = create_arena(ARENA_CONTAINER_INIT_COUNT, (sizeof(struct Node) + size), ARENA_DYNAMIC);
    flist->size = size;

    return flist;
}
void destroy_forward_list(FList **flist)
{
    memory_free_container_arena((void**)flist, (*flist)->arena);
}

void push_front_forward_list(FList *flist, void *value)
{
    generic_push_front_singly_linked(flist->arena, &flist->nmemb, flist->size, &flist->head, value);
}
size_t insert_after_forward_list(FList *flist, void *value, size_t index)
{
    return generic_insert_singly_linked(flist->arena, &flist->nmemb, flist->size, &flist->head, value, index, 0);
}

void *front_forward_list(FList *flist)
{
    return generic_access_linked(flist->head);
}

void pop_front_forward_list(FList *flist)
{
    generic_pop_front_singly_linked(flist->arena, &flist->nmemb, flist->size, &flist->head);
}
size_t erase_after_forward_list(FList *flist, size_t index)
{
    return generic_erase_singly_linked(flist->arena, &flist->nmemb, flist->size, index, &flist->head, 0);
}
void clear_forward_list(FList *flist)
{
    generic_clear_linked(flist->arena, &flist->head, NULL, &flist->nmemb);
}

bool empty_forward_list(FList *flist)
{
    return generic_empty(flist->nmemb);
}
size_t size_forward_list(FList *flist)
{
    return generic_size(flist->nmemb);
}
