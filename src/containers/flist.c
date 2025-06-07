#include "../../flist.h"
#include "../../internals/base.h"
#include "../../internals/flinked.h"

typedef struct ForwardList
{
	struct NodeAlloc         alloc;
	size_t                   nmemb;
	size_t                   size;
	struct SingleLinkedNode *before;
	struct SingleLinkedNode *head;
} ForwardList, FList;

FList *create_forward_list(const size_t size)
{
	return create_forward_list_capacity(size, NODE_COUNT_DEFAULT);
}

FList *create_forward_list_capacity(size_t size, size_t init)
{
	FList *flist = memory_allocate_container(sizeof(FList));

	flist->alloc = create_node_allocator(sizeof(struct SingleLinkedNode),
	                                     init,
	                                     size,
	                                     0);

	flist->size   = size;
	flist->nmemb  = 0;
	flist->head   = NULL;
	flist->before = create_sentinel_node(&flist->alloc);

	return flist;
}

void destroy_forward_list(FList **flist)
{
	destroy_node_allocator(&(*flist)->alloc);

	memory_free_buffer((void **)flist);
}

void push_front_forward_list(FList *flist, const void *value)
{
	push_front_singly_linked(&flist->alloc,
	                         &flist->nmemb,
	                         flist->size,
	                         &flist->head,
	                         flist->before,
	                         value);
}

void *front_forward_list(const FList *flist)
{
	return access_singly_linked(flist->head);
}

void pop_front_forward_list(FList *flist)
{
	pop_front_singly_linked(&flist->alloc,
	                        &flist->nmemb,
	                        &flist->head,
	                        flist->before);
}

void clear_forward_list(FList *flist)
{
	clear_singly_linked(&flist->alloc,
	                    &flist->head,
	                    &flist->before,
	                    &flist->nmemb);
}

Iter insert_after_forward_list(FList *flist, const void *value, Iter pos)
{
	return insert_singly_linked(&flist->alloc,
	                            &flist->nmemb,
	                            flist->size,
	                            &flist->head,
	                            flist->before,
	                            value,
	                            pos);
}

Iter erase_after_forward_list(FList *flist, Iter pos)
{
	return erase_singly_linked(&flist->alloc,
	                           &flist->nmemb,
	                           &flist->head,
	                           flist->before,
	                           pos);
}

Iter before_begin_forward_list(const FList *flist)
{
	struct SingleLinkedNode *node = flist->before;

	Iter iter = { .type         = ITERATOR_FORWARD_LIST,
		          .data.flinked = { .node = node } };

	return iter;
}

Iter begin_forward_list(const FList *flist)
{
	struct SingleLinkedNode *node = flist->head;

	Iter iter = { .type         = ITERATOR_FORWARD_LIST,
		          .data.flinked = { .node = node } };

	return iter;
}

Iter end_forward_list(const FList *)
{
	struct SingleLinkedNode *node = NULL;

	Iter iter = { .type         = ITERATOR_FORWARD_LIST,
		          .data.flinked = { .node = node } };

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
