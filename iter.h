#pragma once

#include <stdbool.h>
#include <stddef.h>

#ifndef CHEAP_ITERATOR_AVAILABLE
#define CHEAP_ITERATOR_AVAILABLE
#endif

typedef enum IteratorType
{
	ITERATOR_ARRAY,
	ITERATOR_VECTOR,
	ITERATOR_LIST,
	ITERATOR_FORWARD_LIST,
	ITERATOR_SET,
	ITERATOR_TABLE,
	ITERATOR_HASH_SET,
	ITERATOR_HASH_TABLE,
	ITERATOR_DEQUE
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

Iter *next_iter(Iter *iter);
Iter *prev_iter(Iter *iter);

void *get_iter(const Iter *iter);

bool done_iter(const Iter *begin, const Iter *end);
bool done_iter_r(const Iter *begin, const Iter *end);

#define range(begin, end)                                                      \
	;                                                                          \
	!done_iter(&begin, &end);                                                  \
	next_iter(&begin)

#define range_r(begin, end)                                                    \
	;                                                                          \
	!done_iter_r(&begin, &end);                                                \
	prev_iter(&begin)
