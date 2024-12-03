#pragma once

#include <stddef.h>

struct NodeAlloc;


struct NodeAlloc *create_node_allocator(size_t size);

void destroy_node_allocator(struct NodeAlloc **alloc);


void *alloc_node(struct NodeAlloc *alloc);

void free_node(struct NodeAlloc *alloc,
               void *ptr);

void clear_nodes(struct NodeAlloc *alloc);
