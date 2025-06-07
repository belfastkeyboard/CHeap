#pragma once

#include "../iter.h"
#include "nalloc.h"

struct SingleLinkedNode
{
	void                    *value;
	struct SingleLinkedNode *next;
};

struct SingleLinkedNode *create_sentinel_node(struct NodeAlloc *alloc);

void push_front_singly_linked(struct NodeAlloc         *alloc,
                              size_t                   *nmemb,
                              size_t                    size,
                              struct SingleLinkedNode **head,
                              struct SingleLinkedNode  *sentinel,
                              const void               *value);

Iter insert_singly_linked(struct NodeAlloc         *alloc,
                          size_t                   *nmemb,
                          size_t                    size,
                          struct SingleLinkedNode **head,
                          struct SingleLinkedNode  *sentinel,
                          const void               *value,
                          Iter                      pos);

void pop_front_singly_linked(struct NodeAlloc         *alloc,
                             size_t                   *nmemb,
                             struct SingleLinkedNode **head,
                             struct SingleLinkedNode  *sentinel);

Iter erase_singly_linked(struct NodeAlloc         *alloc,
                         size_t                   *nmemb,
                         struct SingleLinkedNode **head,
                         struct SingleLinkedNode  *sentinel,
                         Iter                      pos);

void clear_singly_linked(struct NodeAlloc         *alloc,
                         struct SingleLinkedNode **head,
                         struct SingleLinkedNode **sentinel,
                         size_t                   *nmemb);

void *access_singly_linked(struct SingleLinkedNode *node);
