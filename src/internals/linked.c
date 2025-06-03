#include "../../internals/linked.h"
#include "../../iter.h"
#include <memory.h>

static struct Node *create_node(struct NodeAlloc *alloc,
                                const size_t      size,
                                const void       *value)
{
	void        *memory = alloc_node(alloc);
	struct Node *node   = memory;
	node->value         = memory + sizeof(struct Node);

	memcpy(node->value, value, size);

	return node;
}

static void create_node_singly_linked(struct NodeAlloc *alloc,
                                      const size_t      nmemb,
                                      const size_t      size,
                                      const void       *value,
                                      struct Node      *position,
                                      struct Node     **head,
                                      const int         prior)
{
	struct Node *node = create_node(alloc, size, value);

	if (prior || !nmemb)
	{
		*head      = node;
		node->next = position;
	}
	else
	{
		node->next     = position->next;
		position->next = node;
	}
}

static void erase_node_singly_linked(struct NodeAlloc *alloc,
                                     struct Node      *position,
                                     struct Node     **head,
                                     const int         prior)
{
	if (prior)
	{
		*head = position->next;
	}
	else
	{
		position->next = position->next->next;
	}

	free_node(alloc, position);
}

static void create_node_doubly_linked(struct NodeAlloc *alloc,
                                      const size_t      nmemb,
                                      const size_t      size,
                                      const void       *value,
                                      struct Node      *position,
                                      struct Node     **head,
                                      struct Node     **tail)
{
	struct Node *node = create_node(alloc, size, value);

	if (!nmemb)
	{
		*head = node;
		*tail = node;
	}
	else if (!position)
	{
		(*tail)->next = node;
		node->prev    = *tail;
		*tail         = node;
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
}

static void erase_node_doubly_linked(struct NodeAlloc *alloc,
                                     struct Node      *position,
                                     struct Node     **head,
                                     struct Node     **tail)
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

	free_node(alloc, position);
}

static struct Node *seek_node(const size_t nmemb,
                              const size_t index,
                              struct Node *head,
                              struct Node *tail)
{
	struct Node *seek = NULL;

	if (index < nmemb)
	{
		if (tail && index > nmemb / 2)
		{
			seek = tail;

			for (size_t i = nmemb - 1; i > index; i--)
			{
				seek = seek->prev;
			}
		}
		else
		{
			seek = head;

			for (size_t i = 0; i < index; i++)
			{
				seek = seek->next;
			}
		}
	}

	return seek;
}

void generic_push_front_singly_linked(struct NodeAlloc *alloc,
                                      size_t           *nmemb,
                                      const size_t      size,
                                      struct Node     **head,
                                      const void       *value)
{
	generic_insert_singly_linked(alloc, nmemb, size, head, value, 0, 1);
}

size_t generic_insert_singly_linked(struct NodeAlloc *alloc,
                                    size_t           *nmemb,
                                    const size_t      size,
                                    struct Node     **head,
                                    const void       *value,
                                    const size_t      index,
                                    const int         prior)
{
	struct Node *seek = seek_node(*nmemb, index, *head, NULL);

	create_node_singly_linked(alloc, *nmemb, size, value, seek, head, prior);

	(*nmemb)++;

	return index;
}

void generic_push_front_doubly_linked(struct NodeAlloc *alloc,
                                      size_t           *nmemb,
                                      const size_t      size,
                                      struct Node     **head,
                                      struct Node     **tail,
                                      const void       *value)
{
	generic_insert_doubly_linked(alloc, nmemb, size, head, tail, value, 0);
}

void generic_push_back_doubly_linked(struct NodeAlloc *alloc,
                                     size_t           *nmemb,
                                     const size_t      size,
                                     struct Node     **head,
                                     struct Node     **tail,
                                     const void       *value)
{
	generic_insert_doubly_linked(alloc, nmemb, size, head, tail, value, *nmemb);
}

size_t generic_insert_doubly_linked(struct NodeAlloc *alloc,
                                    size_t           *nmemb,
                                    const size_t      size,
                                    struct Node     **head,
                                    struct Node     **tail,
                                    const void       *value,
                                    const size_t      index)
{
	struct Node *seek = seek_node(*nmemb, index, *head, *tail);

	create_node_doubly_linked(alloc, *nmemb, size, value, seek, head, tail);

	(*nmemb)++;

	return index;
}

void generic_pop_front_singly_linked(struct NodeAlloc *alloc,
                                     size_t           *nmemb,
                                     struct Node     **head)
{
	generic_erase_singly_linked(alloc, nmemb, 0, head, 1);
}

size_t generic_erase_singly_linked(struct NodeAlloc *alloc,
                                   size_t           *nmemb,
                                   const size_t      index,
                                   struct Node     **head,
                                   const int         prior)
{
	struct Node *seek = seek_node(*nmemb, index, *head, NULL);

	erase_node_singly_linked(alloc, seek, head, prior);

	(*nmemb)--;

	return index;
}

void generic_pop_front_doubly_linked(struct NodeAlloc *alloc,
                                     size_t           *nmemb,
                                     struct Node     **head,
                                     struct Node     **tail)
{
	generic_erase_doubly_linked(alloc, nmemb, 0, head, tail);
}

void generic_pop_back_doubly_linked(struct NodeAlloc *alloc,
                                    size_t           *nmemb,
                                    struct Node     **head,
                                    struct Node     **tail)
{
	generic_erase_doubly_linked(alloc, nmemb, *nmemb - 1, head, tail);
}

size_t generic_erase_doubly_linked(struct NodeAlloc *alloc,
                                   size_t           *nmemb,
                                   const size_t      index,
                                   struct Node     **head,
                                   struct Node     **tail)
{
	struct Node *seek = seek_node(*nmemb, index, *head, *tail);

	erase_node_doubly_linked(alloc, seek, head, tail);

	(*nmemb)--;

	return index;
}

void generic_clear_linked(struct NodeAlloc *alloc,
                          struct Node     **head,
                          struct Node     **tail,
                          size_t           *nmemb)
{
	clear_nodes(alloc);

	*head = NULL;

	if (tail)
	{
		*tail = NULL;
	}

	*nmemb = 0;
}

void *generic_access_linked(struct Node *node)
{
	return node->value;
}

Iter *next_linked(Iter *iter)
{
	struct Node *node = iter->ptr;

	iter->ptr = node->next;

	return iter;
}

Iter *prev_linked(Iter *iter)
{
	struct Node *node = iter->ptr;

	iter->ptr = node->prev;

	return iter;
}

void *get_linked(const Iter *iter)
{
	struct Node *node = iter->ptr;
	return node->value;
}