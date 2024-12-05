#pragma once

#include "comp.h"
#include "nalloc.h"
#include "../range.h"


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
                   KComp compare,
                   size_t k_size,
                   size_t v_size,
                   size_t *nmemb);


void insert_range_rbtree(struct NodeAlloc *alloc,
                         struct Node **head,
                         const Range *range,
                         KComp compare,
                         size_t size,
                         size_t *nmemb);


void delete_rbtree(struct NodeAlloc *alloc,
                   struct Node **head,
                   const void *key,
                   KComp compare,
                   size_t *nmemb);


void clear(struct NodeAlloc *alloc);


void print(const struct Node *head);


void *rbt_search(struct Node *head,
                 const void *key,
                 KComp compare);
