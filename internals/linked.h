#pragma once

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

size_t generic_insert_singly_linked(struct NodeAlloc   *alloc,
                                    size_t             *nmemb,
                                    size_t              size,
                                    struct LinkedNode **head,
                                    const void         *value,
                                    size_t              index,
                                    int                 prior);

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

size_t generic_insert_doubly_linked(struct NodeAlloc   *alloc,
                                    size_t             *nmemb,
                                    size_t              size,
                                    struct LinkedNode **head,
                                    struct LinkedNode **tail,
                                    const void         *value,
                                    size_t              index);

void generic_pop_front_singly_linked(struct NodeAlloc   *alloc,
                                     size_t             *nmemb,
                                     struct LinkedNode **head);

size_t generic_erase_singly_linked(struct NodeAlloc   *alloc,
                                   size_t             *nmemb,
                                   size_t              index,
                                   struct LinkedNode **head,
                                   int                 prior);

void generic_pop_front_doubly_linked(struct NodeAlloc   *alloc,
                                     size_t             *nmemb,
                                     struct LinkedNode **head,
                                     struct LinkedNode **tail);

void generic_pop_back_doubly_linked(struct NodeAlloc   *alloc,
                                    size_t             *nmemb,
                                    struct LinkedNode **head,
                                    struct LinkedNode **tail);

size_t generic_erase_doubly_linked(struct NodeAlloc   *alloc,
                                   size_t             *nmemb,
                                   size_t              index,
                                   struct LinkedNode **head,
                                   struct LinkedNode **tail);

void generic_clear_linked(struct NodeAlloc   *alloc,
                          struct LinkedNode **head,
                          struct LinkedNode **tail,
                          size_t             *nmemb);

void *generic_access_linked(struct LinkedNode *node);
