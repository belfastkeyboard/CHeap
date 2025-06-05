#pragma once

#include "../range.h"
#include "nalloc.h"
#include "pair.h"

typedef int (*Comp)(const void *a, const void *b);

enum Colour
{
	RED,
	BLACK
};

struct TreeNode
{
	PairKV           pair;
	enum Colour      colour;
	struct TreeNode *parent;
	struct TreeNode *left;
	struct TreeNode *right;
};

void insert_rbtree(struct NodeAlloc *alloc,
                   struct TreeNode **head,
                   const void       *key,
                   const void       *value,
                   Comp              compare,
                   size_t            k_size,
                   size_t            v_size,
                   size_t           *nmemb);

void insert_range_rbtree_set(struct NodeAlloc *alloc,
                             struct TreeNode **head,
                             Range             range,
                             Comp              compare,
                             size_t            size,
                             size_t           *nmemb);

void insert_range_rbtree_table(struct NodeAlloc *alloc,
                               struct TreeNode **head,
                               Range             range,
                               Comp              compare,
                               size_t            k_size,
                               size_t            v_size,
                               size_t           *nmemb);

void delete_rbtree(struct NodeAlloc *alloc,
                   struct TreeNode **head,
                   const void       *key,
                   Comp              compare,
                   size_t           *nmemb);

void clear_rbtree(struct NodeAlloc *alloc,
                  struct TreeNode **head,
                  size_t           *nmemb);

void *rbt_search_k(struct TreeNode *head, const void *key, Comp compare);
void *rbt_search_v(struct TreeNode *head, const void *key, Comp compare);

struct TreeNode *rbt_min(struct TreeNode *head);
struct TreeNode *rbt_max(struct TreeNode *head);
