#pragma once

#include "nalloc.h"


struct Node
{
    void *value;
    struct Node *next;
    struct Node *prev;
};


void generic_push_front_singly_linked(struct NodeAlloc *alloc,
                                      size_t *nmemb,
                                      size_t size,
                                      struct Node **head,
                                      const void *value);

size_t generic_insert_singly_linked(struct NodeAlloc *alloc,
                                    size_t *nmemb,
                                    size_t size,
                                    struct Node **head,
                                    const void *value,
                                    size_t index,
                                    int prior);


void generic_push_front_doubly_linked(struct NodeAlloc *alloc,
                                      size_t *nmemb,
                                      size_t size,
                                      struct Node **head,
                                      struct Node **tail,
                                      const void *value);

void generic_push_back_doubly_linked(struct NodeAlloc *alloc,
                                     size_t *nmemb,
                                     size_t size,
                                     struct Node **head,
                                     struct Node **tail,
                                     const void *value);

size_t generic_insert_doubly_linked(struct NodeAlloc *alloc,
                                    size_t *nmemb,
                                    size_t size,
                                    struct Node **head,
                                    struct Node **tail,
                                    const void *value,
                                    size_t index);


void generic_pop_front_singly_linked(struct NodeAlloc *alloc,
                                     size_t *nmemb,
                                     struct Node **head);

size_t generic_erase_singly_linked(struct NodeAlloc *alloc,
                                   size_t *nmemb,
                                   size_t index,
                                   struct Node **head,
                                   int prior);


void generic_pop_front_doubly_linked(struct NodeAlloc *alloc,
                                     size_t *nmemb,
                                     struct Node **head,
                                     struct Node **tail);

void generic_pop_back_doubly_linked(struct NodeAlloc *alloc,
                                    size_t *nmemb,
                                    struct Node **head,
                                    struct Node **tail);

size_t generic_erase_doubly_linked(struct NodeAlloc *alloc,
                                   size_t *nmemb,
                                   size_t index,
                                   struct Node **head,
                                   struct Node **tail);

void generic_clear_linked(struct NodeAlloc *alloc,
                          struct Node **head,
                          struct Node **tail,
                          size_t *nmemb);


void *generic_access_linked(struct Node *node);
