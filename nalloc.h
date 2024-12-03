#pragma once

#include <stddef.h>

typedef struct NodeAlloc NodeAlloc;


NodeAlloc *create_node_allocator(size_t size);

void destroy_node_allocator(NodeAlloc **allocator);


void *alloc_node(NodeAlloc *allocator);

void free_node(NodeAlloc *allocator,
               void *ptr);

void clear_nodes(NodeAlloc *allocator);
