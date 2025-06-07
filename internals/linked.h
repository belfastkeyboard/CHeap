#pragma once

#include "../iter.h"
#include "nalloc.h"

struct DoubleLinkedNode
{
	void                    *value;
	struct DoubleLinkedNode *next;
	struct DoubleLinkedNode *prev;
};

void push_front_doubly_linked(struct NodeAlloc         *alloc,
                              size_t                   *nmemb,
                              size_t              size,
                              struct DoubleLinkedNode **head,
                              struct DoubleLinkedNode **tail,
                              const void               *value);

void push_back_doubly_linked(struct NodeAlloc         *alloc,
                             size_t                   *nmemb,
                             size_t              size,
                             struct DoubleLinkedNode **head,
                             struct DoubleLinkedNode **tail,
                             const void               *value);

Iter insert_doubly_linked(struct NodeAlloc         *alloc,
                          size_t                   *nmemb,
                          size_t              size,
                          struct DoubleLinkedNode **head,
                          struct DoubleLinkedNode **tail,
                          const void               *value,
                          Iter                      pos);

void pop_front_doubly_linked(struct NodeAlloc         *alloc,
                             struct DoubleLinkedNode **head,
                             struct DoubleLinkedNode **tail);

void pop_back_doubly_linked(struct NodeAlloc         *alloc,
                            struct DoubleLinkedNode **head,
                            struct DoubleLinkedNode **tail);

Iter erase_doubly_linked(struct NodeAlloc         *alloc,
                         size_t                   *nmemb,
                         Iter                      pos,
                         struct DoubleLinkedNode **head,
                         struct DoubleLinkedNode **tail);

void clear_doubly_linked(struct NodeAlloc         *alloc,
                         struct DoubleLinkedNode **head,
                         struct DoubleLinkedNode **tail,
                         size_t                   *nmemb);

void *access_doubly_linked(struct DoubleLinkedNode *node);
