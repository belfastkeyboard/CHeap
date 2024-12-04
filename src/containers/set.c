#include "../../internals/rbtree.h"
#include "../../internals/base.h"
#include "../../set.h"


struct Node;

typedef struct Set
{
    struct NodeAlloc *alloc;
    size_t size;
    KComp k_comp;
    size_t nmemb;
    struct Node *head;
} Set;


Set *create_set(const size_t size,
                const KComp kc)
{
    Set *set = memory_allocate_container(sizeof(Set));

    set->alloc = create_node_allocator(sizeof(struct Node), size); // allocates enough for the node + data
    set->size = size;
    set->k_comp = kc;
    set->nmemb = 0;
    set->head = NULL;

    return set;
}

void destroy_set(Set **set)
{
    destroy_node_allocator(&(*set)->alloc);

    memory_free_buffer((void**)set);
}


void insert_set(Set *set,
                const void *key)
{
    insert_rbtree(set->alloc,
                  &set->head,
                  key,
                  set->k_comp,
                  set->size,
                  &set->nmemb);
}


size_t count_set(const Set *set,
                      const void *key)
{
    return (rbt_search(set->head,
                       key,
                       set->k_comp)) ? 1 :
                                       0;
}

void *find_set(const Set *set,
                    const void *key)
{
    return rbt_search(set->head,
                      key,
                      set->k_comp);
}

bool contains_set(const Set *set,
                        const void *key)
{
    return rbt_search(set->head,
                      key,
                      set->k_comp) ? true :
                                     false;
}


void erase_set(Set *set,
                    const void *key)
{
    delete_rbtree(set->alloc,
                  &set->head,
                  key,
                  set->k_comp,
                  &set->nmemb);
}

void clear_set(Set *set)
{
    clear_nodes(set->alloc);
}


bool empty_set(const Set *set)
{
    return generic_empty(set->nmemb);
}

size_t size_set(const Set *set)
{
    return generic_size(set->nmemb);
}
