#pragma once

#include "comp.h"
#include "nalloc.h"
#include "../range.h"


struct Node
{
    void *key;
    int colour;
    struct Node *l;
    struct Node *r;
};


void insert_rbtree(struct NodeAlloc *alloc,
                   struct Node **head,
                   const void *key,
                   KComp kc,
                   size_t size,
                   size_t *nmemb);


void insert_range_rbtree(struct NodeAlloc *alloc,
                         struct Node **head,
                         const Range *range,
                         KComp kc,
                         size_t size,
                         size_t *nmemb);


void delete_rbtree(struct NodeAlloc *alloc,
                   struct Node **head,
                   const void *key,
                   KComp kc,
                   size_t *nmemb);


void clear(struct NodeAlloc *alloc);


void print(const struct Node *head);


void *rbt_search(struct Node *head,
                 const void *key,
                 KComp kc);
