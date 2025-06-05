#pragma once

#include <stdbool.h>
#include <stddef.h>

#ifndef CHEAP_ITERATOR_AVAILABLE
#define CHEAP_ITERATOR_AVAILABLE
#endif

typedef enum IteratorType
{
	// forward iterators
	ITERATOR_FORWARD,
	// contiguous
	ITERATOR_ARRAY,
	ITERATOR_VECTOR,
	// linked
	ITERATOR_LIST,
	ITERATOR_FORWARD_LIST,
	// balanced
	ITERATOR_SET,
	ITERATOR_TABLE,
	// hashed
	ITERATOR_HASH_SET,
	ITERATOR_HASH_TABLE,
	// deque
	ITERATOR_DEQUE,
	// reverse iterators
	ITERATOR_REVERSE,
	// contiguous
	ITERATOR_ARRAY_REVERSE,
	ITERATOR_VECTOR_REVERSE,
	// linked
	ITERATOR_LIST_REVERSE,
	// balanced
	ITERATOR_SET_REVERSE,
	ITERATOR_TABLE_REVERSE,
	// hashed
	ITERATOR_HASH_SET_REVERSE,
	ITERATOR_HASH_TABLE_REVERSE,
	// deque
	ITERATOR_DEQUE_REVERSE
} IteratorType;

struct LinkedNode;
struct TreeNode;
struct Bucket;
struct ControlArray;
typedef struct DoubleEndedQueue DoubleEndedQueue, Deque;

// array, vector
struct IteratorContiguousArray
{
	void        *array;
	const size_t size;
};
// list, forward list
struct IteratorLinkedList
{
	struct LinkedNode *node;
};
// set, table
struct IteratorBalancedTree
{
	struct TreeNode *node;
};
// hash set, hash table
struct IteratorHashBuckets
{
	struct Bucket *bucket;
	struct Bucket *end;
};
// deque
struct IteratorDeque
{
	const Deque *deque;
	size_t       index;
};

union IteratorData
{
	struct IteratorContiguousArray contiguous;
	struct IteratorLinkedList      linked;
	struct IteratorBalancedTree    balanced;
	struct IteratorHashBuckets     hashed;
	struct IteratorDeque           deque;
};

// TODO: implement unions

typedef struct Iter
{
	const IteratorType type;
	union IteratorData data;
} Iter, Iterator;

void next_iter(Iter *iter);
void prev_iter(Iter *iter);

void *get_iter(Iter iter);

bool done_iter(Iter begin, Iter end);
bool done_iter_r(Iter begin, Iter end);
