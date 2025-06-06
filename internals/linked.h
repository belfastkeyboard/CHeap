#pragma once

#include "../iter.h"
#include "nalloc.h"

struct LinkedNode
{
	void              *value;
	struct LinkedNode *next;
	struct LinkedNode *prev;
};

void generic_push_front_singly_linked(struct NodeAlloc   *alloc,
                                      size_t             *nmemb,
                                      size_t              size,
                                      struct LinkedNode **head,
                                      const void         *value);

Iter generic_insert_singly_linked(struct NodeAlloc   *alloc,
                                  size_t             *nmemb,
                                  size_t              size,
                                  struct LinkedNode **head,
                                  const void         *value,
                                  Iter                pos);

void generic_push_front_doubly_linked(struct NodeAlloc   *alloc,
                                      size_t             *nmemb,
                                      size_t              size,
                                      struct LinkedNode **head,
                                      struct LinkedNode **tail,
                                      const void         *value);

void generic_push_back_doubly_linked(struct NodeAlloc   *alloc,
                                     size_t             *nmemb,
                                     size_t              size,
                                     struct LinkedNode **head,
                                     struct LinkedNode **tail,
                                     const void         *value);

Iter generic_insert_doubly_linked(struct NodeAlloc   *alloc,
                                  size_t             *nmemb,
                                  size_t              size,
                                  struct LinkedNode **head,
                                  struct LinkedNode **tail,
                                  const void         *value,
                                  Iter                pos);

void generic_pop_front_singly_linked(struct NodeAlloc   *alloc,
                                     size_t             *nmemb,
                                     struct LinkedNode **head);

Iter generic_erase_singly_linked(struct NodeAlloc *alloc,
                                 size_t           *nmemb,
                                 Iter              pos);

void generic_pop_front_doubly_linked(struct NodeAlloc   *alloc,
                                     struct LinkedNode **head,
                                     struct LinkedNode **tail);

void generic_pop_back_doubly_linked(struct NodeAlloc   *alloc,
                                    struct LinkedNode **head,
                                    struct LinkedNode **tail);

Iter generic_erase_doubly_linked(struct NodeAlloc   *alloc,
                                 size_t             *nmemb,
                                 Iter                pos,
                                 struct LinkedNode **head,
                                 struct LinkedNode **tail);

void generic_clear_linked(struct NodeAlloc   *alloc,
                          struct LinkedNode **head,
                          struct LinkedNode **tail,
                          size_t             *nmemb);

void *generic_access_linked(struct LinkedNode *node);
