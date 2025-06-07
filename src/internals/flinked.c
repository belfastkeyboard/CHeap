#include "../../internals/flinked.h"
#include "../../internals/cassert.h"
#include <memory.h>

static struct SingleLinkedNode *create_node(struct NodeAlloc *alloc,
                                      const size_t      size,
                                      const void       *value)
{
	void              *memory = alloc_node(alloc);
	struct SingleLinkedNode *node   = memory;
	node->value               = memory + sizeof(struct SingleLinkedNode);

	memcpy(node->value, value, size);

	return node;
}

static void push_front_singly_linked_node(struct NodeAlloc   *alloc,
                                          const size_t        size,
                                          const void         *value,
                                          struct SingleLinkedNode **head)
{
	struct SingleLinkedNode *node = create_node(alloc, size, value);

	node->next = *head;
	*head      = node;
}

static struct SingleLinkedNode *insert_singly_linked_node(struct NodeAlloc
                                                                    *alloc,
                                                    const size_t       size,
                                                    const void        *value,
                                                    struct SingleLinkedNode
                                                                            *position,
                                                    struct SingleLinkedNode
                                                            **head)
{
	struct SingleLinkedNode *node = create_node(alloc, size, value);

	if (!position)
	{
		*head      = node;
		node->next = position;
	}
	else
	{
		node->next     = position->next;
		position->next = node;
	}

	return node;
}

static void erase_node_singly_linked(struct NodeAlloc  *alloc,
                                     struct SingleLinkedNode *position)
{
	struct SingleLinkedNode *to_erase = position->next;

	position->next = to_erase->next;

	free_node(alloc, to_erase);
}

void generic_push_front_singly_linked(struct NodeAlloc   *alloc,
                                      size_t             *nmemb,
                                      const size_t        size,
                                      struct SingleLinkedNode **head,
                                      const void         *value)
{
	push_front_singly_linked_node(alloc, size, value, head);
	(*nmemb)++;
}

Iter generic_insert_singly_linked(struct NodeAlloc   *alloc,
                                  size_t             *nmemb,
                                  const size_t        size,
                                  struct SingleLinkedNode **head,
                                  const void         *value,
                                  Iter                pos)
{
	struct SingleLinkedNode *node = pos.data.flinked.node;

	pos.data.flinked.node = insert_singly_linked_node(alloc,
	                                                 size,
	                                                 value,
	                                                 node,
	                                                 head);

	(*nmemb)++;

	return pos;
}

void generic_pop_front_singly_linked(struct NodeAlloc   *alloc,
                                     size_t             *nmemb,
                                     struct SingleLinkedNode **head)
{
	CHEAP_ASSERT(*nmemb, "Cannot pop an empty list");

	struct SingleLinkedNode *front = *head;
	*head                    = front->next;
	free_node(alloc, front);
	(*nmemb)--;
}

Iter generic_erase_singly_linked(struct NodeAlloc *alloc,
                                 size_t           *nmemb,
                                 Iter              pos)
{
	struct SingleLinkedNode *node = pos.data.flinked.node;

	erase_node_singly_linked(alloc, node);

	(*nmemb)--;

	return pos;
}

void generic_clear_linked(struct NodeAlloc   *alloc,
                          struct SingleLinkedNode **head,
                          struct SingleLinkedNode **tail,
                          size_t             *nmemb)
{
	clear_nodes(alloc);

	*head = NULL;

	if (tail)
	{
		*tail = NULL;
	}

	*nmemb = 0;
}

void *generic_access_linked(struct SingleLinkedNode *node)
{
	return node->value;
}

void next_linked(Iter *iter)
{
	iter->data.flinked.node = iter->data.flinked.node->next;
}

void prev_linked(Iter *iter)
{
	iter->data.flinked.node = iter->data.flinked.node->prev;
}

void *get_linked(const Iter iter)
{
	return iter.data.flinked.node->value;
}