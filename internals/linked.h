#pragma once

#include "../iter.h"
#include "nalloc.h"

struct DoubleLinkedNode
{
	void                    *value;
	struct DoubleLinkedNode *next;
	struct DoubleLinkedNode *prev;
};

void generic_push_front_doubly_linked(struct NodeAlloc         *alloc,
                                      size_t                   *nmemb,
                                      size_t                    size,
                                      struct DoubleLinkedNode **head,
                                      struct DoubleLinkedNode **tail,
                                      const void               *value);

void generic_push_back_doubly_linked(struct NodeAlloc         *alloc,
                                     size_t                   *nmemb,
                                     size_t                    size,
                                     struct DoubleLinkedNode **head,
                                     struct DoubleLinkedNode **tail,
                                     const void               *value);

Iter generic_insert_doubly_linked(struct NodeAlloc         *alloc,
                                  size_t                   *nmemb,
                                  size_t                    size,
                                  struct DoubleLinkedNode **head,
                                  struct DoubleLinkedNode **tail,
                                  const void               *value,
                                  Iter                      pos);

void generic_pop_front_doubly_linked(struct NodeAlloc         *alloc,
                                     struct DoubleLinkedNode **head,
                                     struct DoubleLinkedNode **tail);

void generic_pop_back_doubly_linked(struct NodeAlloc         *alloc,
                                    struct DoubleLinkedNode **head,
                                    struct DoubleLinkedNode **tail);

Iter generic_erase_doubly_linked(struct NodeAlloc         *alloc,
                                 size_t                   *nmemb,
                                 Iter                      pos,
                                 struct DoubleLinkedNode **head,
                                 struct DoubleLinkedNode **tail);

void generic_clear_linked(struct NodeAlloc         *alloc,
                          struct DoubleLinkedNode **head,
                          struct DoubleLinkedNode **tail,
                          size_t                   *nmemb);

void *generic_access_linked(struct DoubleLinkedNode *node);
