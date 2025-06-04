#include "../../iter.h"
#include <stdio.h>
#include <stdlib.h>

extern Iter *next_mempool(Iter *iter);
extern Iter *prev_mempool(Iter *iter);
extern void *get_mempool(const Iter *iter);

extern Iter *next_hash(Iter *iter);
extern Iter *prev_hash(Iter *iter);
extern void *get_hash_table(const Iter *iter);
extern void *get_hash_set(const Iter *iter);

extern Iter *next_linked(Iter *iter);
extern Iter *prev_linked(Iter *iter);
extern void *get_linked(const Iter *iter);

extern Iter *next_rbtree(Iter *iter);
extern Iter *prev_rbtree(Iter *iter);
extern void *get_rbtree_set(const Iter *iter);
extern void *get_rbtree_table(const Iter *iter);

extern Iter *next_deque(Iter *iter);
extern Iter *prev_deque(Iter *iter);
extern void *get_deque(const Iter *iter);

Iter *next_iter(Iter *iter)
{
	switch (iter->type)
	{
		case ITERATOR_VECTOR:
		case ITERATOR_ARRAY:
			return next_mempool(iter);
		case ITERATOR_HASH_TABLE:
		case ITERATOR_HASH_SET:
			return next_hash(iter);
		case ITERATOR_LIST:
		case ITERATOR_FORWARD_LIST:
			return next_linked(iter);
		case ITERATOR_SET:
		case ITERATOR_TABLE:
			return next_rbtree(iter);
		case ITERATOR_DEQUE:
			return next_deque(iter);
		default:
			fprintf(stderr, "Unknown iterator type: %d.\n", iter->type);
			exit(EXIT_FAILURE);
	}
}

Iter *prev_iter(Iter *iter)
{
	switch (iter->type)
	{
		case ITERATOR_VECTOR:
		case ITERATOR_ARRAY:
			return prev_mempool(iter);
		case ITERATOR_HASH_TABLE:
		case ITERATOR_HASH_SET:
			return prev_hash(iter);
		case ITERATOR_LIST:
			return prev_linked(iter);
		case ITERATOR_SET:
		case ITERATOR_TABLE:
			return prev_rbtree(iter);
		case ITERATOR_DEQUE:
			return prev_deque(iter);
		default:
			fprintf(stderr, "Unknown iterator type: %d.\n", iter->type);
			exit(EXIT_FAILURE);
	}
}

void *get_iter(const Iter *iter)
{
	switch (iter->type)
	{
		case ITERATOR_VECTOR:
		case ITERATOR_ARRAY:
			return get_mempool(iter);
		case ITERATOR_HASH_TABLE:
			return get_hash_table(iter);
		case ITERATOR_HASH_SET:
			return get_hash_set(iter);
		case ITERATOR_LIST:
		case ITERATOR_FORWARD_LIST:
			return get_linked(iter);
		case ITERATOR_SET:
			return get_rbtree_set(iter);
		case ITERATOR_TABLE:
			return get_rbtree_table(iter);
		case ITERATOR_DEQUE:
			return get_deque(iter);
		default:
			fprintf(stderr, "Unknown iterator type: %d.\n", iter->type);
			exit(EXIT_FAILURE);
	}
}

bool done_iter(const Iter *begin, const Iter *end)
{
	switch (begin->type)
	{
		case ITERATOR_ARRAY:
		case ITERATOR_VECTOR:
			return begin->data.contiguous.array >= end->data.contiguous.array;
		case ITERATOR_HASH_SET:
		case ITERATOR_HASH_TABLE:
			return begin->data.hashed.bucket >= end->data.hashed.end;
		case ITERATOR_LIST:
		case ITERATOR_FORWARD_LIST:
			return !begin->data.linked.node;
		case ITERATOR_SET:
		case ITERATOR_TABLE:
			return begin->data.balanced.node == end->data.balanced.node;
		case ITERATOR_DEQUE:
			return begin->data.deque.index >= end->data.deque.index;
		default:
			fprintf(stderr, "Unknown iterator type: %d.\n", begin->type);
			exit(EXIT_FAILURE);
	}
}

bool done_iter_r(const Iter *begin, const Iter *end)
{
	switch (begin->type)
	{
		case ITERATOR_ARRAY:
		case ITERATOR_VECTOR:
			return begin->data.contiguous.array <= end->data.contiguous.array;
		case ITERATOR_HASH_SET:
		case ITERATOR_HASH_TABLE:
			return begin->data.hashed.bucket <= end->data.hashed.end;
		case ITERATOR_LIST:
			return !begin->data.linked.node;
		case ITERATOR_SET:
		case ITERATOR_TABLE:
			return !begin->data.balanced.node;
		case ITERATOR_DEQUE:
			return begin->data.deque.index < end->data.deque.index;
		default:
			fprintf(stderr, "Unknown iterator type: %d.\n", begin->type);
			exit(EXIT_FAILURE);
	}
}
