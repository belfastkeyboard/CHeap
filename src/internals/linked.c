#include "../../internals/linked.h"
#include "../../internals/cassert.h"
#include <memory.h>

static struct DoubleLinkedNode *create_node(struct NodeAlloc *alloc,
                                      const size_t      size,
                                      const void       *value)
{
	void              *memory = alloc_node(alloc);
	struct DoubleLinkedNode *node   = memory;
	node->value               = memory + sizeof(struct DoubleLinkedNode);

	memcpy(node->value, value, size);

	return node;
}

static void push_front_doubly_linked_node(struct NodeAlloc   *alloc,
                                          const size_t        size,
                                          const void         *value,
                                          struct DoubleLinkedNode **head,
                                          struct DoubleLinkedNode **tail)
{
	struct DoubleLinkedNode *node = create_node(alloc, size, value);

	if (!(*head) && !(*tail))
	{
		*head = node;
		*tail = node;
	}
	else
	{
		(*head)->prev = node;
		node->next    = *head;
		*head         = node;
	}
}

static void push_back_doubly_linked_node(struct NodeAlloc   *alloc,
                                         const size_t        size,
                                         const void         *value,
                                         struct DoubleLinkedNode **head,
                                         struct DoubleLinkedNode **tail)
{
	struct DoubleLinkedNode *node = create_node(alloc, size, value);

	if (!(*head) && !(*tail))
	{
		*head = node;
		*tail = node;
	}
	else
	{
		(*tail)->next = node;
		node->prev    = *tail;
		*tail         = node;
	}
}

static struct DoubleLinkedNode *insert_doubly_linked_node(struct NodeAlloc  *alloc,
                                                    const size_t       size,
                                                    const void        *value,
                                                    struct DoubleLinkedNode  *position,
                                                    struct DoubleLinkedNode **head,
                                                    struct DoubleLinkedNode **tail)
{
	struct DoubleLinkedNode *node = create_node(alloc, size, value);

	if (!position)
	{
		*head = node;
		*tail = node;
	}
	else
	{
		if (position->prev)
		{
			position->prev->next = node;
			node->prev           = position->prev;
		}
		else
		{
			*head = node;
		}

		position->prev = node;
		node->next     = position;
	}

	return node;
}

static struct DoubleLinkedNode *erase_node_doubly_linked(struct NodeAlloc   *alloc,
                                                   struct DoubleLinkedNode  *position,
                                                   struct DoubleLinkedNode **head,
                                                   struct DoubleLinkedNode **tail)
{
	if (position->prev)
	{
		position->prev->next = position->next;
	}
	else
	{
		*head = position->next;
	}

	if (position->next)
	{
		position->next->prev = position->prev;
	}
	else
	{
		*tail = position->prev;
	}

	struct DoubleLinkedNode *pos = position->next;

	free_node(alloc, position);

	return pos;
}

void generic_push_front_doubly_linked(struct NodeAlloc   *alloc,
                                      size_t             *nmemb,
                                      const size_t        size,
                                      struct DoubleLinkedNode **head,
                                      struct DoubleLinkedNode **tail,
                                      const void         *value)
{
	push_front_doubly_linked_node(alloc, size, value, head, tail);
	(*nmemb)++;
}

void generic_push_back_doubly_linked(struct NodeAlloc   *alloc,
                                     size_t             *nmemb,
                                     const size_t        size,
                                     struct DoubleLinkedNode **head,
                                     struct DoubleLinkedNode **tail,
                                     const void         *value)
{
	push_back_doubly_linked_node(alloc, size, value, head, tail);
	(*nmemb)++;
}

Iter generic_insert_doubly_linked(struct NodeAlloc   *alloc,
                                  size_t             *nmemb,
                                  const size_t        size,
                                  struct DoubleLinkedNode **head,
                                  struct DoubleLinkedNode **tail,
                                  const void         *value,
                                  Iter                pos)
{
	CHEAP_ASSERT(pos.type == ITERATOR_LIST || pos.type == ITERATOR_LIST_REVERSE,
	             "Iterator must be of list type");

	struct DoubleLinkedNode *node = pos.data.linked.node;

	pos.data.linked
	    .node = insert_doubly_linked_node(alloc, size, value, node, head, tail);

	(*nmemb)++;

	return pos;
}

void generic_pop_front_doubly_linked(struct NodeAlloc   *alloc,
                                     struct DoubleLinkedNode **head,
                                     struct DoubleLinkedNode **tail)
{
	erase_node_doubly_linked(alloc, *head, head, tail);
}

void generic_pop_back_doubly_linked(struct NodeAlloc   *alloc,
                                    struct DoubleLinkedNode **head,
                                    struct DoubleLinkedNode **tail)
{
	erase_node_doubly_linked(alloc, *tail, head, tail);
}

Iter generic_erase_doubly_linked(struct NodeAlloc   *alloc,
                                 size_t             *nmemb,
                                 Iter                pos,
                                 struct DoubleLinkedNode **head,
                                 struct DoubleLinkedNode **tail)
{
	CHEAP_ASSERT(pos.type == ITERATOR_LIST || pos.type == ITERATOR_LIST_REVERSE,
	             "Iterator must be of list type");

	struct DoubleLinkedNode *node = pos.data.linked.node;
	pos.data.linked.node    = erase_node_doubly_linked(alloc, node, head, tail);

	(*nmemb)++;

	return pos;
}

void generic_clear_linked(struct NodeAlloc   *alloc,
                          struct DoubleLinkedNode **head,
                          struct DoubleLinkedNode **tail,
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

void *generic_access_linked(struct DoubleLinkedNode *node)
{
	return node->value;
}

void next_linked(Iter *iter)
{
	iter->data.linked.node = iter->data.linked.node->next;
}

void prev_linked(Iter *iter)
{
	iter->data.linked.node = iter->data.linked.node->prev;
}

void *get_linked(const Iter iter)
{
	return iter.data.linked.node->value;
}