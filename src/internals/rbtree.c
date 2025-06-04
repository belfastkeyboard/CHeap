// This implementation is a modified version of the Red-Black Tree found here:
// https://web.archive.org/web/20140328232325/http://en.literateprograms.org/Red-black_tree_(C)

#include "../../internals/rbtree.h"
#include "../../iter.h"
#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stddef.h>

typedef int (*KComp)(const void *, const void *);

static enum Colour node_colour(const struct TreeNode *node)
{
	return (!node) ? BLACK : node->colour;
}

static bool is_red(const struct TreeNode *node)
{
	return node_colour(node) == RED;
}

static bool is_black(const struct TreeNode *node)
{
	return node_colour(node) == BLACK;
}

static struct TreeNode *create_node(struct NodeAlloc *alloc,
                                    struct TreeNode  *parent,
                                    const void       *key,
                                    const void       *value,
                                    const size_t      k_size,
                                    const size_t      v_size)
{
	void            *memory = alloc_node(alloc);
	struct TreeNode *node   = memory;

	node->pair.key   = memory + sizeof(struct TreeNode);
	node->pair.value = memory + sizeof(struct TreeNode) + k_size;
	node->colour     = RED;
	node->parent     = parent;
	node->left       = NULL;
	node->right      = NULL;

	memcpy((void *)node->pair.key, key, k_size);
	memcpy(node->pair.value, value, v_size);

	return node;
}

static struct TreeNode *grandparent(struct TreeNode *node)
{
	assert(node);
	assert(node->parent);
	assert(node->parent->parent);

	return node->parent->parent;
}

static struct TreeNode *sibling(struct TreeNode *node)
{
	assert(node);
	assert(node->parent);

	return (node == node->parent->left) ? node->parent->right
	                                    : node->parent->left;
}

static struct TreeNode *uncle(struct TreeNode *n)
{
	assert(n);
	assert(n->parent);
	assert(n->parent->parent);

	return sibling(n->parent);
}

static struct TreeNode *lookup_node(struct TreeNode *head,
                                    const void      *key,
                                    KComp            compare)
{
	assert(!head || !head->parent);

	struct TreeNode *node = head;

	while (node)
	{
		int result = compare(key, node->pair.key);

		if (result == 0)
		{
			break;
		}
		else if (result < 0)
		{
			node = node->left;
		}
		else
		{
			node = node->right;
		}
	}

	return node;
}

static void replace_node(struct TreeNode **head,
                         struct TreeNode  *old,
                         struct TreeNode *new)
{
	if (!old->parent)
	{
		*head = new;
	}
	else
	{
		if (old == old->parent->left)
		{
			old->parent->left = new;
		}
		else
		{
			old->parent->right = new;
		}
	}

	if (new)
	{
		new->parent = old->parent;
	}
}

static void rotate_left(struct TreeNode **head, struct TreeNode *node)
{
	struct TreeNode *right = node->right;

	replace_node(head, node, right);

	node->right = right->left;

	if (right->left)
	{
		right->left->parent = node;
	}

	right->left  = node;
	node->parent = right;
}

static void rotate_right(struct TreeNode **head, struct TreeNode *node)
{
	struct TreeNode *left = node->left;

	replace_node(head, node, left);

	node->left = left->right;

	if (left->right)
	{
		left->right->parent = node;
	}

	left->right  = node;
	node->parent = left;
}

static void insert_fixup(struct TreeNode **head, struct TreeNode *node)
{
	if (!node->parent)
	{
		node->colour = BLACK;
		return;
	}

	if (is_red(node->parent))
	{
		if (is_red(uncle(node)))
		{
			node->parent->colour      = BLACK;
			uncle(node)->colour       = BLACK;
			grandparent(node)->colour = RED;
			insert_fixup(head, grandparent(node));
			return;
		}

		if (node == node->parent->right &&
		    node->parent == grandparent(node)->left)
		{
			rotate_left(head, node->parent);
			node = node->left;
		}
		else if (node == node->parent->left &&
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
		}
		else
		{
			assert(node == node->parent->right &&
			       node->parent == grandparent(node)->right);

			rotate_left(head, grandparent(node));
		}
	}
}

static struct TreeNode *insert_node(struct NodeAlloc *alloc,
                                    struct TreeNode **sentinel,
                                    struct TreeNode  *parent,
                                    const void       *key,
                                    const void       *value,
                                    const size_t      k_size,
                                    const size_t      v_size)
{
	*sentinel = create_node(alloc, parent, key, value, k_size, v_size);

	return *sentinel;
}

static void rbt_insert(struct NodeAlloc *alloc,
                       struct TreeNode **head,
                       const void       *key,
                       const void       *value,
                       KComp             compare,
                       const size_t      k_size,
                       const size_t      v_size)
{
	struct TreeNode *inserted_node = NULL;

	if (!(*head))
	{
		*head = insert_node(alloc,
		                    &inserted_node,
		                    NULL,
		                    key,
		                    value,
		                    k_size,
		                    v_size);
	}
	else
	{
		struct TreeNode *node = *head;

		while (true)
		{
			int result = compare(key, node->pair.key);

			if (result == 0)
			{
				memcpy(node->pair.value, value, v_size);
				break;
			}
			else if (result < 0)
			{
				if (!node->left)
				{
					node->left = insert_node(alloc,
					                         &inserted_node,
					                         node,
					                         key,
					                         value,
					                         k_size,
					                         v_size);
					break;
				}
				else
				{
					node = node->left;
				}
			}
			else
			{
				if (!node->right)
				{
					node->right = insert_node(alloc,
					                          &inserted_node,
					                          node,
					                          key,
					                          value,
					                          k_size,
					                          v_size);
					break;
				}
				else
				{
					node = node->right;
				}
			}
		}
	}

	if (inserted_node)
	{
		insert_fixup(head, inserted_node);
	}
}

static struct TreeNode *maximum_node(struct TreeNode *node)
{
	assert(node);

	while (node->right)
	{
		node = node->right;
	}

	return node;
}

static void remove_fixup(struct TreeNode **head, struct TreeNode *node)
{
	if (!node->parent)
	{
		return;
	}

	if (is_red(sibling(node)))
	{
		node->parent->colour  = RED;
		sibling(node)->colour = BLACK;

		if (node == node->parent->left)
		{
			rotate_left(head, node->parent);
		}
		else
		{
			rotate_right(head, node->parent);
		}
	}

	if (is_black(node->parent) && is_black(sibling(node)) &&
	    is_black(sibling(node)->left) && is_black(sibling(node)->right))
	{
		sibling(node)->colour = RED;
		remove_fixup(head, node->parent);
		return;
	}

	if (is_red(node->parent) && is_black(sibling(node)) &&
	    is_black(sibling(node)->left) && is_black(sibling(node)->right))
	{
		sibling(node)->colour = RED;
		node->parent->colour  = BLACK;
		return;
	}

	if (node == node->parent->left && is_black(sibling(node)) &&
	    is_red(sibling(node)->left) && is_black(sibling(node)->right))
	{
		sibling(node)->colour       = RED;
		sibling(node)->left->colour = BLACK;
		rotate_right(head, sibling(node));
	}
	else if (node == node->parent->right && is_black(sibling(node)) &&
	         is_red(sibling(node)->right) && is_black(sibling(node)->left))
	{
		sibling(node)->colour        = RED;
		sibling(node)->right->colour = BLACK;
		rotate_left(head, sibling(node));
	}

	sibling(node)->colour = node_colour(node->parent);
	node->parent->colour  = BLACK;

	if (node == node->parent->left)
	{
		assert(is_red(sibling(node)->right));
		sibling(node)->right->colour = BLACK;
		rotate_left(head, node->parent);
	}
	else
	{
		assert(is_red(sibling(node)->left));
		sibling(node)->left->colour = BLACK;
		rotate_right(head, node->parent);
	}
}

static void rbt_delete(struct NodeAlloc *alloc,
                       struct TreeNode **head,
                       const void       *key,
                       KComp             compare,
                       size_t           *nmemb)
{
	struct TreeNode *child;
	struct TreeNode *node = lookup_node(*head, key, compare);

	if (!node)
	{
		return;
	}

	if (node->left && node->right)
	{
		struct TreeNode *pred = maximum_node(node->left);
		node->pair            = pred->pair;
		node                  = pred;
	}

	assert(!node->left || !node->right);

	child = (!node->right) ? node->left : node->right;

	if (is_black(node))
	{
		node->colour = node_colour(child);
		remove_fixup(head, node);
	}

	replace_node(head, node, child);

	if (!node->parent && child)
	{
		child->colour = BLACK;
	}

	free_node(alloc, node);
	(*nmemb)--;
}

void insert_rbtree(struct NodeAlloc *alloc,
                   struct TreeNode **head,
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
                   struct TreeNode **head,
                   const void       *key,
                   const KComp       compare,
                   size_t           *nmemb)
{
	if (!(*head))
	{
		return;
	}

	rbt_delete(alloc, head, key, compare, nmemb);

	assert(!(*head) || (!(*head)->parent));
}

void clear_rbtree(struct NodeAlloc *alloc,
                  struct TreeNode **head,
                  size_t           *nmemb)
{
	clear_nodes(alloc);

	*head  = NULL;
	*nmemb = 0;
}

void *rbt_search_k(struct TreeNode *head, const void *key, KComp compare)
{
	void *result = NULL;

	struct TreeNode *node = lookup_node(head, key, compare);

	if (node)
	{
		result = (void *)node->pair.key;
	}

	return result;
}

void *rbt_search_v(struct TreeNode *head, const void *key, KComp compare)
{
	void *result = NULL;

	struct TreeNode *node = lookup_node(head, key, compare);

	if (node)
	{
		result = node->pair.value;
	}

	return result;
}

void *rbt_min(struct TreeNode *node)
{
	while (node->left)
	{
		node = node->left;
	}

	return node;
}

void *rbt_max(struct TreeNode *head)
{
	struct TreeNode *node = head;

	while (node->right)
	{
		node = node->right;
	}

	return node;
}

void *get_rbtree_set(const Iter *iter)
{
	return (void *)((struct TreeNode *)iter->ptr)->pair.key;
}

void *get_rbtree_table(const Iter *iter)
{
	return &((struct TreeNode *)iter->ptr)->pair;
}

Iter *next_rbtree(Iter *iter)
{
	struct TreeNode *node = iter->ptr;

	if (node->right)
	{
		iter->ptr = rbt_min(node->right);
	}
	else
	{
		struct TreeNode *parent = node->parent;
		while (parent && node == parent->right)
		{
			node   = parent;
			parent = parent->parent;
		}

		iter->ptr = parent;
	}

	return iter;
}

Iter *prev_rbtree(Iter *iter)
{
	struct TreeNode *node = iter->ptr;

	if (node->left)
	{
		node      = node->left;
		node      = rbt_max(node);
		iter->ptr = node;
	}
	else
	{
		struct TreeNode *parent = node->parent;

		while (parent && node == parent->left)
		{
			node   = parent;
			parent = parent->parent;
		}

		iter->ptr = parent;
	}

	return iter;
}
