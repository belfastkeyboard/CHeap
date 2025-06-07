#include "../../flist.h"
#include "../../internals/base.h"
#include "../../internals/linked.h"
#include "../../iter.h"

typedef struct ForwardList
{
	struct NodeAlloc   alloc;
	size_t             nmemb;
	size_t             size;
	struct DoubleLinkedNode *head;
} ForwardList, FList;

FList *create_forward_list(const size_t size)
{
	return create_forward_list_capacity(size, NODE_COUNT_DEFAULT);
}

FList *create_forward_list_capacity(size_t size, size_t init)
{
	FList *flist = memory_allocate_container(sizeof(FList));

	flist->alloc = create_node_allocator(sizeof(struct DoubleLinkedNode),
	                                     init,
	                                     size,
	                                     0);
	flist->size  = size;

	return flist;
}

void destroy_forward_list(FList **flist)
{
	destroy_node_allocator(&(*flist)->alloc);

	memory_free_buffer((void **)flist);
}

void push_front_forward_list(FList *flist, const void *value)
{
	generic_push_front_singly_linked(&flist->alloc,
	                                 &flist->nmemb,
	                                 flist->size,
	                                 &flist->head,
	                                 value);
}

void *front_forward_list(const FList *flist)
{
	return generic_access_linked(flist->head);
}

void pop_front_forward_list(FList *flist)
{
	generic_pop_front_singly_linked(&flist->alloc, &flist->nmemb, &flist->head);
}

void clear_forward_list(FList *flist)
{
	generic_clear_linked(&flist->alloc, &flist->head, NULL, &flist->nmemb);
}

Iter insert_after_forward_list(FList *flist, const void *value, Iter pos)
{
	return generic_insert_singly_linked(&flist->alloc,
	                                    &flist->nmemb,
	                                    flist->size,
	                                    &flist->head,
	                                    value,
	                                    pos);
}

Iter erase_after_forward_list(FList *flist, Iter pos)
{
	return generic_erase_singly_linked(&flist->alloc,
	                                   &flist->nmemb,
	                                   pos);
}

Iter begin_forward_list(const FList *flist)
{
	void *ptr = flist->head;

	Iter iter = { .type        = ITERATOR_FORWARD_LIST,
		          .data.linked = { .node = ptr } };

	return iter;
}

Iter end_forward_list(const FList *)
{
	void *ptr = NULL;

	Iter iter = { .type        = ITERATOR_FORWARD_LIST,
		          .data.linked = { .node = ptr } };

	return iter;
}

bool empty_forward_list(const FList *flist)
{
	return generic_empty(flist->nmemb);
}

size_t size_forward_list(const FList *flist)
{
	return generic_size(flist->nmemb);
}
