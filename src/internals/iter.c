#include "../../iter.h"
#include "../../internals/error.h"
#include <stdio.h>
#include <stdlib.h>

extern void  next_mempool(Iter *iter);
extern void  prev_mempool(Iter *iter);
extern void *get_mempool(Iter iter);

extern void  next_hash(Iter *iter);
extern void  prev_hash(Iter *iter);
extern void *get_hash_table(Iter iter);
extern void *get_hash_set(Iter iter);

extern void  next_linked(Iter *iter);
extern void  prev_linked(Iter *iter);
extern void *get_linked(Iter iter);

extern void  next_rbtree(Iter *iter);
extern void  prev_rbtree(Iter *iter);
extern void *get_rbtree_set(Iter iter);
extern void *get_rbtree_table(Iter iter);

extern void  next_deque(Iter *iter);
extern void  prev_deque(Iter *iter);
extern void *get_deque(Iter iter);

void next_iter(Iter *iter)
{
	switch (iter->type)
	{
		case ITERATOR_VECTOR:
		case ITERATOR_VECTOR_REVERSE:
		case ITERATOR_ARRAY:
		case ITERATOR_ARRAY_REVERSE:
			return next_mempool(iter);
		case ITERATOR_HASH_TABLE:
		case ITERATOR_HASH_TABLE_REVERSE:
		case ITERATOR_HASH_SET:
		case ITERATOR_HASH_SET_REVERSE:
			return next_hash(iter);
		case ITERATOR_LIST:
		case ITERATOR_LIST_REVERSE:
		case ITERATOR_FORWARD_LIST:
			return next_linked(iter);
		case ITERATOR_SET:
		case ITERATOR_SET_REVERSE:
		case ITERATOR_TABLE:
		case ITERATOR_TABLE_REVERSE:
			return next_rbtree(iter);
		case ITERATOR_DEQUE:
		case ITERATOR_DEQUE_REVERSE:
			return next_deque(iter);
		default:
			throw(__FILE__,
			      __FUNCTION__,
			      __LINE__,
			      "Invalid iterator type: %d",
			      iter->type);
	}
}

void prev_iter(Iter *iter)
{
	switch (iter->type)
	{
		case ITERATOR_VECTOR:
		case ITERATOR_VECTOR_REVERSE:
		case ITERATOR_ARRAY:
		case ITERATOR_ARRAY_REVERSE:
			return prev_mempool(iter);
		case ITERATOR_HASH_TABLE:
		case ITERATOR_HASH_TABLE_REVERSE:
		case ITERATOR_HASH_SET:
		case ITERATOR_HASH_SET_REVERSE:
			return prev_hash(iter);
		case ITERATOR_LIST:
		case ITERATOR_LIST_REVERSE:
			return prev_linked(iter);
		case ITERATOR_SET:
		case ITERATOR_SET_REVERSE:
		case ITERATOR_TABLE:
		case ITERATOR_TABLE_REVERSE:
			return prev_rbtree(iter);
		case ITERATOR_DEQUE:
		case ITERATOR_DEQUE_REVERSE:
			return prev_deque(iter);
		default:
			throw(__FILE__,
			      __FUNCTION__,
			      __LINE__,
			      "Invalid iterator type: %d",
			      iter->type);
	}
}

void *get_iter(const Iter iter)
{
	switch (iter.type)
	{
		case ITERATOR_VECTOR:
		case ITERATOR_VECTOR_REVERSE:
		case ITERATOR_ARRAY:
		case ITERATOR_ARRAY_REVERSE:
			return get_mempool(iter);
		case ITERATOR_HASH_TABLE:
		case ITERATOR_HASH_TABLE_REVERSE:
			return get_hash_table(iter);
		case ITERATOR_HASH_SET:
		case ITERATOR_HASH_SET_REVERSE:
			return get_hash_set(iter);
		case ITERATOR_LIST:
		case ITERATOR_LIST_REVERSE:
		case ITERATOR_FORWARD_LIST:
			return get_linked(iter);
		case ITERATOR_SET:
		case ITERATOR_SET_REVERSE:
			return get_rbtree_set(iter);
		case ITERATOR_TABLE:
		case ITERATOR_TABLE_REVERSE:
			return get_rbtree_table(iter);
		case ITERATOR_DEQUE:
		case ITERATOR_DEQUE_REVERSE:
			return get_deque(iter);
		default:
			throw(__FILE__,
			      __FUNCTION__,
			      __LINE__,
			      "Invalid iterator type: %d",
			      iter.type);
	}
}

bool done_iter(const Iter begin, const Iter end)
{
	switch (begin.type)
	{
		case ITERATOR_ARRAY:
		case ITERATOR_VECTOR:
			return begin.data.contiguous.array == end.data.contiguous.array;
		case ITERATOR_HASH_SET:
		case ITERATOR_HASH_TABLE:
			return begin.data.hashed.bucket == end.data.hashed.end;
		case ITERATOR_LIST:
		case ITERATOR_FORWARD_LIST:
			return begin.data.linked.node == end.data.linked.node;
		case ITERATOR_SET:
		case ITERATOR_TABLE:
			return begin.data.balanced.node == end.data.balanced.node;
		case ITERATOR_DEQUE:
			return begin.data.deque.index == end.data.deque.index;
		default:
			throw(__FILE__,
			      __FUNCTION__,
			      __LINE__,
			      "Invalid iterator type: %d",
			      begin.type);
	}
}

bool done_iter_r(const Iter begin, const Iter end)
{
	switch (begin.type)
	{
		case ITERATOR_ARRAY_REVERSE:
		case ITERATOR_VECTOR_REVERSE:
			return begin.data.contiguous.array == end.data.contiguous.array;
		case ITERATOR_HASH_SET_REVERSE:
		case ITERATOR_HASH_TABLE_REVERSE:
			return begin.data.hashed.bucket == end.data.hashed.end;
		case ITERATOR_LIST_REVERSE:
			return begin.data.linked.node == end.data.linked.node;
		case ITERATOR_SET_REVERSE:
		case ITERATOR_TABLE_REVERSE:
			return begin.data.balanced.node == end.data.balanced.node;
		case ITERATOR_DEQUE_REVERSE:
			return begin.data.deque.index == end.data.deque.index;
		default:
			throw(__FILE__,
			      __FUNCTION__,
			      __LINE__,
			      "Invalid iterator type: %d",
			      begin.type);
	}
}
