#include "../../internals/rbtree.h"
#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stddef.h>

#define VERIFY_RBTREE

typedef int (*KComp)(const void *, const void *);

static void insert_case1(struct Node **head, struct Node *node);
static void delete_case1(struct Node **head, struct Node *node);

static struct Node *create_node(struct NodeAlloc *alloc,
                                const void       *key,
                                const void       *value,
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
	node->left   = left;
	node->right  = right;

	if (left) {
		left->parent = node;
	}

	if (right) {
		right->parent = node;
	}

	node->parent = NULL;

	node->key   = memcpy(node->key, key, k_size);
	node->value = memcpy(node->value, value, v_size);

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

static void verify_property_1(struct Node *node)
{
	assert(is_red(node) || is_black(node));

	if (node == NULL) {
		return;
	}

	verify_property_1(node->left);
	verify_property_1(node->right);
}

static void verify_property_2(struct Node *head)
{
	assert(is_black(head));
}

static void verify_property_4(struct Node *node)
{
	if (is_red(node)) {
		assert(is_black(node->left));
		assert(is_black(node->right));
		assert(is_black(node->parent));
	}

	if (!node) {
		return;
	}

	verify_property_4(node->left);
	verify_property_4(node->right);
}

static void verify_property_5_helper(struct Node *node,
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

	verify_property_5_helper(node->left, black_count, path_black_count);
	verify_property_5_helper(node->right, black_count, path_black_count);
}

static void verify_property_5(struct Node *head)
{
	int black_count_path = -1;
	verify_property_5_helper(head, 0, &black_count_path);
}

static void verify_properties(struct Node *head)
{
#ifdef VERIFY_RBTREE
	verify_property_1(head);
	verify_property_2(head);
	// property 3 is implicit
	verify_property_4(head);
	verify_property_5(head);
#endif
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

void insert_case5(struct Node **head, struct Node *node)
{
	node->parent->colour      = BLACK;
	grandparent(node)->colour = RED;

	if (node == node->parent->left && node->parent == grandparent(node)->left) {
		rotate_right(head, grandparent(node));
	} else {
		assert(node == node->parent->right &&
		       node->parent == grandparent(node)->right);

		rotate_left(head, grandparent(node));
	}
}

void insert_case4(struct Node **head, struct Node *node)
{
	if (node == node->parent->right && node->parent == grandparent(node)->left)
	{
		rotate_left(head, node->parent);
		node = node->left;
	} else if (node == node->parent->left &&
	           node->parent == grandparent(node)->right)
	{
		rotate_right(head, node->parent);
		node = node->right;
	}

	insert_case5(head, node);
}

void insert_case3(struct Node **head, struct Node *node)
{
	if (is_red(uncle(node))) {
		node->parent->colour      = BLACK;
		uncle(node)->colour       = BLACK;
		grandparent(node)->colour = RED;
		insert_case1(head, grandparent(node));
	} else {
		insert_case4(head, node);
	}
}

void insert_case2(struct Node **head, struct Node *node)
{
	if (is_black(node->parent)) {
		return; /* Tree is still valid */
	} else {
		insert_case3(head, node);
	}
}

void insert_case1(struct Node **head, struct Node *node)
{
	if (!node->parent) {
		node->colour = BLACK;
	} else {
		insert_case2(head, node);
	}
}

static void rbt_insert(struct NodeAlloc *alloc,
                       struct Node     **head,
                       const void       *key,
                       const void       *value,
                       KComp             compare,
                       const size_t      k_size,
                       const size_t      v_size)
{
	// this leaks memory when the key already exists
	struct Node *inserted_node = create_node(alloc,
	                                         key,
	                                         value,
	                                         RED,
	                                         NULL,
	                                         NULL,
	                                         k_size,
	                                         v_size);

	if (!(*head)) {
		*head = inserted_node;
	} else {
		struct Node *node = *head;

		while (true) {
			int result = compare(key, node->key);

			// key already exists
			if (result == 0) {
				/* node->value = value; */ // needs memcpy for ownership of data
				// perhaps one node could be freed and replaced with another
				// node here
				memcpy(node->value, value, v_size);
				return;
			} else if (result < 0) {
				if (!node->left) {
					node->left = inserted_node;
					break;
				} else {
					node = node->left;
				}
			} else {
				assert(result > 0);
				if (node->right == NULL) {
					node->right = inserted_node;
					break;
				} else {
					node = node->right;
				}
			}
		}

		// this is an issue (see comments above)
		inserted_node->parent = node;
	}

	insert_case1(head, inserted_node);
	verify_properties(*head);
}

static struct Node *maximum_node(struct Node *node)
{
	assert(node);

	while (node->right) {
		node = node->right;
	}

	return node;
}

void delete_case6(struct Node **head, struct Node *node)
{
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

void delete_case5(struct Node **head, struct Node *node)
{
	if (node == node->parent->left && is_black(sibling(node)) &&
	    is_red(sibling(node)->left) && is_black(sibling(node)->right))
	{
		sibling(node)->colour       = RED;
		sibling(node)->left->colour = BLACK;

		rotate_right(head, sibling(node));
	} else if (node == node->parent->right && is_black(sibling(node)) &&
	           is_red(sibling(node)->right) && is_black(sibling(node)->left))
	{
		sibling(node)->colour        = RED;
		sibling(node)->right->colour = BLACK;

		rotate_left(head, sibling(node));
	}

	delete_case6(head, node);
}

void delete_case4(struct Node **head, struct Node *node)
{
	if (is_red(node->parent) && is_black(sibling(node)) &&
	    is_black(sibling(node)->left) && is_black(sibling(node)->right))
	{
		sibling(node)->colour = RED;
		node->parent->colour  = BLACK;
	} else {
		delete_case5(head, node);
	}
}

void delete_case3(struct Node **head, struct Node *node)
{
	if (is_black(node->parent) && is_black(sibling(node)) &&
	    is_black(sibling(node)->left) && is_black(sibling(node)->right))
	{
		sibling(node)->colour = RED;
		delete_case1(head, node->parent);
	} else {
		delete_case4(head, node);
	}
}

void delete_case2(struct Node **head, struct Node *node)
{
	if (is_red(sibling(node))) {
		node->parent->colour  = RED;
		sibling(node)->colour = BLACK;

		if (node == node->parent->left) {
			rotate_left(head, node->parent);
		} else {
			rotate_right(head, node->parent);
		}
	}

	delete_case3(head, node);
}

void delete_case1(struct Node **head, struct Node *node)
{
	if (!node->parent) {
		return;
	} else {
		delete_case2(head, node);
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
		// Copy key/value from predecessor and then delete it instead
		struct Node *pred = maximum_node(node->left);
		node->key         = pred->key;
		node->value       = pred->value;
		node              = pred;
	}

	assert(!node->left || !node->right);

	child = (!node->right) ? node->left : node->right;

	if (is_black(node)) {
		node->colour = node_colour(child);
		delete_case1(head, node);
	}

	replace_node(head, node, child);

	if (!node->parent && child) {
		child->colour = BLACK;
	}

	free_node(alloc, node);
	verify_properties(*head);

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

	assert((*head)->colour == BLACK);
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

#include <stdio.h>
// PRINT TREE //
#define INDENT_STEP 4

void print_tree_helper(struct Node *node, int indent)
{
	if (!node) {
		fputs("<empty tree>", stdout);
		return;
	}

	if (node->right) {
		print_tree_helper(node->right, indent + INDENT_STEP);
	}

	for (int i = 0; i < indent; i++) {
		fputs(" ", stdout);
	}

	if (node->colour == BLACK) {
		printf("[%d]\n", *(int *)node->key);
	} else {
		printf("<%d>\n", *(int *)node->key);
	}

	if (node->left) {
		print_tree_helper(node->left, indent + INDENT_STEP);
	}
}

void print(struct Node *head)
{
	print_tree_helper(head, 0);
}
