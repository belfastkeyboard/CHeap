#pragma once

#include <stddef.h>

typedef struct NodeAllocator NodeAllocator;


NodeAllocator *create_node_allocator(size_t size);

void destroy_node_allocator(NodeAllocator **allocator);


void *node_alloc(NodeAllocator *allocator);

void node_free(NodeAllocator *allocator,
               void *ptr);
