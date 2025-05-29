#include "../../iter.h"

extern Iter *next_vector(Iter *iter);
extern Iter *prev_vector(Iter *iter);
extern void *get_vector(const Iter *iter);

extern Iter *next_hash(Iter *iter);
extern Iter *prev_hash(Iter *iter);
extern void *get_hash_table(const Iter *iter);
extern void *get_hash_set(const Iter *iter);

Iter *next_iter(Iter *iter)
{
	switch (iter->type) {
		case ITERATOR_VECTOR:
			return next_vector(iter);
		case ITERATOR_HASH_TABLE:
		case ITERATOR_HASH_SET:
			return next_hash(iter);
	}
}

Iter *prev_iter(Iter *iter)
{
	switch (iter->type) {
		case ITERATOR_VECTOR:
			return prev_vector(iter);
		case ITERATOR_HASH_TABLE:
		case ITERATOR_HASH_SET:
			return prev_hash(iter);
	}
}

void *get_iter(const Iter *iter)
{
	switch (iter->type) {
		case ITERATOR_VECTOR:
			return get_vector(iter);
		case ITERATOR_HASH_TABLE:
			return get_hash_table(iter);
		case ITERATOR_HASH_SET:
			return get_hash_set(iter);
	}
}

bool done_iter(const Iter *begin, const Iter *end)
{
	return begin->ptr <= end->ptr;
}

bool done_iter_r(const Iter *begin, const Iter *end)
{
	return begin->ptr >= end->ptr;
}
