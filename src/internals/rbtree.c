#include "../../internals/rbtree.h"
#include "../../internals/nalloc.h"
#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stddef.h>

#define VERIFY_RBTREE

typedef int (*KComp)(const void *, const void *);

struct Node *create_node(struct NodeAlloc *alloc,
                         void             *key,
                         void             *value,
                         enum Colour       node_color,
                         struct Node      *left,
                         struct Node      *right,
                         const size_t      k_size,
                         const size_t      v_size)
{
	void *memory = alloc_node(alloc);

	struct Node *node = memory;

	node->key    = memory + sizeof(struct Node);
	node->value  = memory + sizeof(struct Node) + k_size;
	node->colour = node_color;
	node->l      = left;
	node->r      = right;

	if (left) {
		left->p = node;
	}

	if (right) {
		right->p = node;
	}

	node->p = NULL;

	node->key   = memcpy(node->key, key, k_size);
	node->value = memcpy(node->value, value, v_size);

	return node;
}

static struct Node *grandparent(struct Node *node)
{
	assert(node);
	assert(node->p);
	assert(node->p->p);

	return node->p->p;
}

static struct Node *sibling(struct Node *node)
{
	assert(node);
	assert(node->p);

	return (node == node->p->l) ? node->p->r : node->p->l;
}

static struct Node *uncle(struct Node *n)
{
	assert(n);
	assert(n->p);
	assert(n->p->p);

	return sibling(n->p);
}

static enum Colour node_colour(struct Node *node)
{
	return (!node) ? BLACK : node->colour;
}

static bool is_red(struct Node *node)
{
	return node_colour(node) == RED;
}

static bool is_black(struct Node *node)
{
	return node_colour(node) == BLACK;
}

void verify_property_1(struct Node *node)
{
	assert(is_red(node) || is_black(node));

	if (node == NULL) {
		return;
	}

	verify_property_1(node->l);
	verify_property_1(node->r);
}

void verify_property_2(struct Node *head)
{
	assert(is_black(head) == BLACK);
}

void verify_property_4(struct Node *node)
{
	if (is_red(node) == RED) {
		assert(is_black(node->l));
		assert(is_black(node->r));
		assert(is_black(node->p));
	}

	if (!node) {
		return;
	}

	verify_property_4(node->l);
	verify_property_4(node->r);
}

void verify_property_5_helper(struct Node *node,
                              int          black_count,
                              int         *path_black_count)
{
	if (is_black(node)) {
		black_count++;
	}

	if (!node) {
		if (*path_black_count == -1) {
			*path_black_count = black_count;
		} else {
			assert(black_count == *path_black_count);
		}
		return;
	}

	verify_property_5_helper(node->l, black_count, path_black_count);
	verify_property_5_helper(node->r, black_count, path_black_count);
}

void verify_property_5(struct Node *head)
{
	int black_count_path = -1;
	verify_property_5_helper(head, 0, &black_count_path);
}

void verify_properties(struct Node *head)
{
#ifdef VERIFY_RBTREE

	verify_property_1(head);
	verify_property_2(head);
	// property 3 is implicit
	verify_property_4(head);
	verify_property_5(head);
#endif
}

void insert_rbtree(struct NodeAlloc *alloc,
                   struct Node     **head,
                   const void       *key,
                   const void       *value,
                   const KComp       compare,
                   const size_t      k_size,
                   const size_t      v_size,
                   size_t           *nmemb)
{
	assert(key);

	*head = rbt_insert(alloc, *head, NULL, key, value, compare, k_size, v_size);

	(*head)->colour = BLACK;

	(*nmemb)++;
}

void delete_rbtree(struct NodeAlloc *alloc,
                   struct Node     **head,
                   const void       *key,
                   const KComp       compare,
                   size_t           *nmemb)
{
	if (!(*head)) {
		return;
	}

	if (!is_red((*head)->l) && !is_red((*head)->r)) {
		(*head)->colour = RED;
	}

	*head = rbt_delete(alloc, *head, key, compare, nmemb);

	if (*head) {
		assert(!(*head)->p);
		(*head)->colour = BLACK;
	}
}

void clear_rbtree(struct NodeAlloc *alloc, size_t *nmemb)
{
	clear_nodes(alloc);

	*nmemb = 0;
}
