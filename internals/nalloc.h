#pragma once

#include <stddef.h>

struct NodeAlloc
{
    struct Block *blocks;
    struct Page *pages;
};


struct NodeAlloc create_node_allocator(size_t node_size,
                                       size_t t1_size,
                                       size_t t2_size);

void destroy_node_allocator(struct NodeAlloc *alloc);


void *alloc_node(struct NodeAlloc *alloc);

void free_node(struct NodeAlloc *alloc,
               void *ptr);

void clear_nodes(struct NodeAlloc *alloc);
