#pragma once

#include "../iter.h"
#include "nalloc.h"

struct SingleLinkedNode
{
	void              *value;
	struct SingleLinkedNode *next;
	struct SingleLinkedNode *prev;
};

void generic_push_front_singly_linked(struct NodeAlloc   *alloc,
                                      size_t             *nmemb,
                                      size_t              size,
                                      struct SingleLinkedNode **head,
                                      const void         *value);

Iter generic_insert_singly_linked(struct NodeAlloc   *alloc,
                                  size_t             *nmemb,
                                  size_t              size,
                                  struct SingleLinkedNode **head,
                                  const void         *value,
                                  Iter                pos);

void generic_pop_front_singly_linked(struct NodeAlloc   *alloc,
                                     size_t             *nmemb,
                                     struct SingleLinkedNode **head);

Iter generic_erase_singly_linked(struct NodeAlloc *alloc,
                                 size_t           *nmemb,
                                 Iter              pos);

void generic_clear_linked(struct NodeAlloc   *alloc,
                          struct SingleLinkedNode **head,
                          struct SingleLinkedNode **tail,
                          size_t             *nmemb);

void *generic_access_linked(struct SingleLinkedNode *node);
