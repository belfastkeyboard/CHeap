#pragma once

#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

#define NODE_COUNT_DEFAULT 8

struct NodeBlock;
struct NodePage;

struct NodeAlloc
{
	struct NodeBlock *blocks;
	struct NodePage  *pages;
};

ALLOC struct NodeAlloc create_node_allocator(size_t node_size,
                                             size_t nmemb,
                                             size_t t1_size,
                                             size_t t2_size);

void destroy_node_allocator(struct NodeAlloc *alloc);

ALLOC void *alloc_node(struct NodeAlloc *alloc);

void free_node(struct NodeAlloc *alloc, void *ptr);

void clear_nodes(struct NodeAlloc *alloc);
