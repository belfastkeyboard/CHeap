#pragma once

#include "../range.h"
#include "nalloc.h"

typedef int (*Comp)(const void *a, const void *b);

enum Colour
{
	RED,
	BLACK
};

struct Node
{
	void        *key;
	void        *value;
	enum Colour  colour;
	struct Node *parent;
	struct Node *left;
	struct Node *right;
};

void insert_rbtree(struct NodeAlloc *alloc,
                   struct Node     **head,
                   const void       *key,
                   const void       *value,
                   Comp              compare,
                   size_t            k_size,
                   size_t            v_size,
                   size_t           *nmemb);

/*
void insert_range_rbtree(struct NodeAlloc *alloc,
                         struct Node     **head,
                         const Range      *range,
                         Comp              compare,
                         size_t            size,
                         size_t           *nmemb);
*/

void delete_rbtree(struct NodeAlloc *alloc,
                   struct Node     **head,
                   const void       *key,
                   Comp              compare,
                   size_t           *nmemb);

void clear_rbtree(struct NodeAlloc *alloc, struct Node **head, size_t *nmemb);

void *rbt_search_k(struct Node *head, const void *key, Comp compare);

void *rbt_search_v(struct Node *head, const void *key, Comp compare);

void *rbt_min(struct Node *head);

void *rbt_max(struct Node *head);
