#pragma once

#include "nalloc.h"
#include "../range.h"


typedef int (*Comp)(const void *a,
                    const void *b);


struct Node
{
    void *key;
    void *value;
    int colour;
    struct Node *l;
    struct Node *r;
};


void insert_rbtree(struct NodeAlloc *alloc,
                   struct Node **head,
                   const void *key,
                   const void *value,
                   Comp compare,
                   size_t k_size,
                   size_t v_size,
                   size_t *nmemb);


void insert_range_rbtree(struct NodeAlloc *alloc,
                         struct Node **head,
                         const Range *range,
                         Comp compare,
                         size_t size,
                         size_t *nmemb);


void delete_rbtree(struct NodeAlloc *alloc,
                   struct Node **head,
                   const void *key,
                   Comp compare,
                   size_t *nmemb);


void clear_rbtree(struct NodeAlloc *alloc,
                  size_t *nmemb);


void *rbt_search_k(struct Node *head,
                   const void *key,
                   Comp compare);

void *rbt_search_v(struct Node *head,
                   const void *key,
                   Comp compare);
