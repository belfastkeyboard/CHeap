#include "../../internals/flinked.h"
#include "../../internals/cassert.h"
#include <memory.h>

static struct SingleLinkedNode *create_node(struct NodeAlloc *alloc,
                                            const size_t      size,
                                            const void       *value)
{
	void                    *memory = alloc_node(alloc);
	struct SingleLinkedNode *node   = memory;
	node->value                     = memory + sizeof(struct SingleLinkedNode);

	memcpy(node->value, value, size);

	return node;
}

static void push_front(struct NodeAlloc         *alloc,
                       const size_t              size,
                       const void               *value,
                       struct SingleLinkedNode **head)
{
	struct SingleLinkedNode *node = create_node(alloc, size, value);

	node->next = *head;
	*head      = node;
}

static struct SingleLinkedNode *insert(struct NodeAlloc         *alloc,
                                       const size_t              size,
                                       const void               *value,
                                       struct SingleLinkedNode  *position,
                                       struct SingleLinkedNode **head)
{
	struct SingleLinkedNode *node = create_node(alloc, size, value);

	if (position->next == *head)
	{
		*head = node;
	}

	node->next     = position->next;
	position->next = node;

	return node;
}

static void erase(struct NodeAlloc *alloc, struct SingleLinkedNode *position)
{
	struct SingleLinkedNode *to_erase = position->next;
	position->next                    = to_erase->next;

	free_node(alloc, to_erase);
}

struct SingleLinkedNode *create_sentinel_node(struct NodeAlloc *alloc)
{
	struct SingleLinkedNode *sentinel = create_node(alloc, 0, NULL);

	sentinel->value = NULL;
	sentinel->next  = NULL;

	return sentinel;
}

void push_front_singly_linked(struct NodeAlloc         *alloc,
                              size_t                   *nmemb,
                              const size_t              size,
                              struct SingleLinkedNode **head,
                              struct SingleLinkedNode  *sentinel,
                              const void               *value)
{
	push_front(alloc, size, value, head);
	(*nmemb)++;

	if (sentinel->next != *head)
	{
		sentinel->next = *head;
	}
}

Iter insert_singly_linked(struct NodeAlloc         *alloc,
                          size_t                   *nmemb,
                          const size_t              size,
                          struct SingleLinkedNode **head,
                          struct SingleLinkedNode  *sentinel,
                          const void               *value,
                          Iter                      pos)
{
	struct SingleLinkedNode *node = pos.data.flinked.node;
	pos.data.flinked.node         = insert(alloc, size, value, node, head);

	(*nmemb)++;

	if (sentinel->next != *head)
	{
		sentinel->next = *head;
	}

	return pos;
}

void pop_front_singly_linked(struct NodeAlloc         *alloc,
                             size_t                   *nmemb,
                             struct SingleLinkedNode **head,
                             struct SingleLinkedNode  *sentinel)
{
	CHEAP_ASSERT(*nmemb, "Cannot pop an empty list");

	struct SingleLinkedNode *front = *head;
	*head                          = front->next;

	free_node(alloc, front);

	if (sentinel->next != *head)
	{
		sentinel->next = *head;
	}

	(*nmemb)--;
}

Iter erase_singly_linked(struct NodeAlloc         *alloc,
                         size_t                   *nmemb,
                         struct SingleLinkedNode **head,
                         struct SingleLinkedNode  *sentinel,
                         Iter                      pos)
{
	struct SingleLinkedNode *node = pos.data.flinked.node;

	erase(alloc, node);

	(*nmemb)--;

	if (sentinel->next != *head)
	{
		sentinel->next = *head;
	}

	return pos;
}

void clear_singly_linked(struct NodeAlloc         *alloc,
                         struct SingleLinkedNode **head,
                         struct SingleLinkedNode **sentinel,
                         size_t                   *nmemb)
{
	clear_nodes(alloc);

	*nmemb    = 0;
	*head     = NULL;
	*sentinel = create_sentinel_node(alloc);
}

void *access_singly_linked(struct SingleLinkedNode *node)
{
	return node->value;
}

void next_flinked(Iter *iter)
{
	iter->data.flinked.node = iter->data.flinked.node->next;
}

void *get_flinked(const Iter iter)
{
	return iter.data.flinked.node->value;
}