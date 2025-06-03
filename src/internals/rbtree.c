#include "../../internals/rbtree.h"
#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stddef.h>

typedef int (*KComp)(const void *, const void *);

static struct Node *create_node(struct NodeAlloc *alloc,
                                const void       *key,
                                const void       *value,
                                const size_t      k_size,
                                const size_t      v_size)
{
	void        *memory = alloc_node(alloc);
	struct Node *node   = memory;

	node->key    = memory + sizeof(struct Node);
	node->value  = memory + sizeof(struct Node) + k_size;
	node->colour = RED;
	node->left   = NULL;
	node->right  = NULL;
	node->parent = NULL;

	memcpy(node->key, key, k_size);
	memcpy(node->value, value, v_size);

	return node;
}

static struct Node *grandparent(struct Node *node)
{
	assert(node);
	assert(node->parent);
	assert(node->parent->parent);

	return node->parent->parent;
}

static struct Node *sibling(struct Node *node)
{
	assert(node);
	assert(node->parent);

	return (node == node->parent->left) ? node->parent->right
	                                    : node->parent->left;
}

static struct Node *uncle(struct Node *n)
{
	assert(n);
	assert(n->parent);
	assert(n->parent->parent);

	return sibling(n->parent);
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

static struct Node *lookup_node(struct Node *head,
                                const void  *key,
                                KComp        compare)
{
	assert(!head || !head->parent);

	struct Node *node = head;

	while (node) {
		int result = compare(key, node->key);

		if (result == 0) {
			return node;
		} else if (result < 0) {
			node = node->left;
		} else {
			assert(result > 0);
			node = node->right;
		}
	}

	return node;
}

static void replace_node(struct Node **head, struct Node *old, struct Node *new)
{
	if (!old->parent) {
		*head = new;
	} else {
		if (old == old->parent->left) {
			old->parent->left = new;
		} else {
			old->parent->right = new;
		}
	}

	if (new) {
		new->parent = old->parent;
	}
}

static void rotate_left(struct Node **head, struct Node *node)
{
	struct Node *right = node->right;

	replace_node(head, node, right);

	node->right = right->left;

	if (right->left) {
		right->left->parent = node;
	}

	right->left  = node;
	node->parent = right;
}

static void rotate_right(struct Node **head, struct Node *node)
{
	struct Node *left = node->left;

	replace_node(head, node, left);

	node->left = left->right;

	if (left->right) {
		left->right->parent = node;
	}

	left->right  = node;
	node->parent = left;
}

static void insert_fixup(struct Node **head, struct Node *node)
{
	if (!node->parent) {
		node->colour = BLACK;
	} else if (!is_black(node->parent)) {
		if (is_red(uncle(node))) {
			node->parent->colour      = BLACK;
			uncle(node)->colour       = BLACK;
			grandparent(node)->colour = RED;
			insert_fixup(head, grandparent(node));
		} else {
			if (node == node->parent->right &&
			    node->parent == grandparent(node)->left)
			{
				rotate_left(head, node->parent);
				node = node->left;
			} else if (node == node->parent->left &&
			           node->parent == grandparent(node)->right)
			{
				rotate_right(head, node->parent);
				node = node->right;
			}

			node->parent->colour      = BLACK;
			grandparent(node)->colour = RED;

			if (node == node->parent->left &&
			    node->parent == grandparent(node)->left)
			{
				rotate_right(head, grandparent(node));
			} else {
				assert(node == node->parent->right &&
				       node->parent == grandparent(node)->right);

				rotate_left(head, grandparent(node));
			}
		}
	}
}

static struct Node *insert_node(struct NodeAlloc *alloc,
                                struct Node     **sentinel,
                                const void       *key,
                                const void       *value,
                                const size_t      k_size,
                                const size_t      v_size)
{
	*sentinel = create_node(alloc, key, value, k_size, v_size);

	return *sentinel;
}

static void rbt_insert(struct NodeAlloc *alloc,
                       struct Node     **head,
                       const void       *key,
                       const void       *value,
                       KComp             compare,
                       const size_t      k_size,
                       const size_t      v_size)
{
	struct Node *inserted_node = NULL;

	if (!(*head)) {
		*head = insert_node(alloc, &inserted_node, key, value, k_size, v_size);
	} else {
		struct Node *node = *head;

		while (true) {
			int result = compare(key, node->key);

			if (result == 0) {
				memcpy(node->value, value, v_size);
				return;
			} else if (result < 0) {
				if (!node->left) {
					node->left = insert_node(alloc,
					                         &inserted_node,
					                         key,
					                         value,
					                         k_size,
					                         v_size);
					break;
				} else {
					node = node->left;
				}
			} else {
				assert(result > 0);
				if (!node->right) {
					node->right = insert_node(alloc,
					                          &inserted_node,
					                          key,
					                          value,
					                          k_size,
					                          v_size);
					break;
				} else {
					node = node->right;
				}
			}
		}

		// this should probably go into the constructor parameters
		inserted_node->parent = node;
	}

	assert(inserted_node);

	insert_fixup(head, inserted_node);
}

static struct Node *maximum_node(struct Node *node)
{
	assert(node);

	while (node->right) {
		node = node->right;
	}

	return node;
}

void remove_fixup(struct Node **head, struct Node *node)
{
	if (!node->parent) {
		return;
	}

	if (is_red(sibling(node))) {
		node->parent->colour  = RED;
		sibling(node)->colour = BLACK;

		if (node == node->parent->left) {
			rotate_left(head, node->parent);
		} else {
			rotate_right(head, node->parent);
		}
	}

	if (is_black(node->parent) && is_black(sibling(node)) &&
	    is_black(sibling(node)->left) && is_black(sibling(node)->right))
	{
		sibling(node)->colour = RED;
		remove_fixup(head, node->parent);
	} else {
		if (is_red(node->parent) && is_black(sibling(node)) &&
		    is_black(sibling(node)->left) && is_black(sibling(node)->right))
		{
			sibling(node)->colour = RED;
			node->parent->colour  = BLACK;
		} else {
			if (node == node->parent->left && is_black(sibling(node)) &&
			    is_red(sibling(node)->left) && is_black(sibling(node)->right))
			{
				sibling(node)->colour       = RED;
				sibling(node)->left->colour = BLACK;

				rotate_right(head, sibling(node));
			} else if (node == node->parent->right && is_black(sibling(node)) &&
			           is_red(sibling(node)->right) &&
			           is_black(sibling(node)->left))
			{
				sibling(node)->colour        = RED;
				sibling(node)->right->colour = BLACK;

				rotate_left(head, sibling(node));
			}

			sibling(node)->colour = node_colour(node->parent);
			node->parent->colour  = BLACK;

			if (node == node->parent->left) {
				assert(is_red(sibling(node)->right));
				sibling(node)->right->colour = BLACK;
				rotate_left(head, node->parent);
			} else {
				assert(is_red(sibling(node)->left));
				sibling(node)->left->colour = BLACK;
				rotate_right(head, node->parent);
			}
		}
	}
}

void rbt_delete(struct NodeAlloc *alloc,
                struct Node     **head,
                const void       *key,
                KComp             compare,
                size_t           *nmemb)
{
	struct Node *child;
	struct Node *node = lookup_node(*head, key, compare);

	if (!node) {
		return;
	}

	if (node->left && node->right) {
		struct Node *pred = maximum_node(node->left);
		node->key         = pred->key;
		node->value       = pred->value;
		node              = pred;
	}

	assert(!node->left || !node->right);

	child = (!node->right) ? node->left : node->right;

	if (is_black(node)) {
		node->colour = node_colour(child);
		remove_fixup(head, node);
	}

	replace_node(head, node, child);

	if (!node->parent && child) {
		child->colour = BLACK;
	}

	free_node(alloc, node);
	(*nmemb)--;
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

	rbt_insert(alloc, head, key, value, compare, k_size, v_size);

	assert(is_black(*head));

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

	rbt_delete(alloc, head, key, compare, nmemb);

	assert(!(*head) || (!(*head)->parent));
}

void clear_rbtree(struct NodeAlloc *alloc, struct Node **head, size_t *nmemb)
{
	clear_nodes(alloc);

	*head  = NULL;
	*nmemb = 0;
}

void *rbt_search_k(struct Node *head, const void *key, KComp compare)
{
	void *result = NULL;

	struct Node *node = lookup_node(head, key, compare);

	if (node) {
		result = node->key;
	}

	return result;
}

void *rbt_search_v(struct Node *head, const void *key, KComp compare)
{
	void *result = NULL;

	struct Node *node = lookup_node(head, key, compare);

	if (node) {
		result = node->value;
	}

	return result;
}
