#include "../../set.h"
#include "../../internals/base.h"
#include "../../internals/rbtree.h"

typedef struct Set
{
	struct NodeAlloc alloc;
	size_t           size;
	KComp            k_comp;
	size_t           nmemb;
	struct TreeNode *head;
} Set;

Set *create_set(const size_t size, const KComp compare)
{
	Set *set = memory_allocate_container(sizeof(Set));

	set->alloc = create_node_allocator(sizeof(struct TreeNode),
	                                   NODE_COUNT_DEFAULT,
	                                   size,
	                                   0);

	set->size   = size;
	set->k_comp = compare;
	set->nmemb  = 0;
	set->head   = NULL;

	return set;
}

void destroy_set(Set **set)
{
	destroy_node_allocator(&(*set)->alloc);
	memory_free_buffer((void **)set);
}

void insert_set(Set *set, const void *key)
{
	insert_rbtree(&set->alloc,
	              &set->head,
	              key,
	              NULL,
	              set->k_comp,
	              set->size,
	              0,
	              &set->nmemb);
}

void insert_range_set(Set *set, Range range)
{
	insert_range_rbtree_set(&set->alloc,
	                        &set->head,
	                        range,
	                        set->k_comp,
	                        set->size,
	                        &set->nmemb);
}

size_t count_set(const Set *set, const void *key)
{
	return (rbt_search_k(set->head, key, set->k_comp)) ? 1 : 0;
}

void *find_set(const Set *set, const void *key)
{
	return rbt_search_k(set->head, key, set->k_comp);
}

bool contains_set(const Set *set, const void *key)
{
	return rbt_search_k(set->head, key, set->k_comp) ? true : false;
}

void erase_set(Set *set, const void *key)
{
	delete_rbtree(&set->alloc, &set->head, key, set->k_comp, &set->nmemb);
}

void clear_set(Set *set)
{
	clear_rbtree(&set->alloc, &set->head, &set->nmemb);
}

Iter begin_set(const Set *set)
{
	struct TreeNode *node = rbt_min(set->head);
	Iter iter = { .type = ITERATOR_SET, .data.balanced = { .node = node } };
	return iter;
}

Iter end_set(const Set *)
{
	struct TreeNode *node = NULL;
	Iter iter = { .type = ITERATOR_SET, .data.balanced = { .node = node } };
	return iter;
}

Iter rbegin_set(const Set *set)
{
	struct TreeNode *node = rbt_max(set->head);
	Iter iter = { .type = ITERATOR_SET, .data.balanced = { .node = node } };
	return iter;
}

Iter rend_set(const Set *)
{
	struct TreeNode *node = NULL;
	Iter iter = { .type = ITERATOR_SET, .data.balanced = { .node = node } };
	return iter;
}

bool empty_set(const Set *set)
{
	return generic_empty(set->nmemb);
}

size_t size_set(const Set *set)
{
	return generic_size(set->nmemb);
}
