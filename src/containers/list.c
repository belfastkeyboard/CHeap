#include "../../list.h"
#include "../../internals/base.h"
#include "../../internals/linked.h"

typedef struct List
{
	struct NodeAlloc         alloc;
	size_t                   nmemb;
	size_t                   size;
	struct DoubleLinkedNode *head;
	struct DoubleLinkedNode *tail;
} List;

List *create_list(const size_t size)
{
	return create_list_capacity(size, NODE_COUNT_DEFAULT);
}

List *create_list_capacity(size_t size, size_t init)
{
	List *list = memory_allocate_container(sizeof(List));

	list->alloc = create_node_allocator(sizeof(struct DoubleLinkedNode),
	                                    init,
	                                    size,
	                                    0);
	list->size  = size;
	list->head  = NULL;
	list->tail  = NULL;

	return list;
}

void destroy_list(List **list)
{
	destroy_node_allocator(&(*list)->alloc);
	memory_free_buffer((void **)list);
}

void push_back_list(List *list, const void *value)
{
	push_back_doubly_linked(&list->alloc,
	                        &list->nmemb,
	                        list->size,
	                        &list->head,
	                        &list->tail,
	                        value);
}

void push_front_list(List *list, const void *value)
{
	push_front_doubly_linked(&list->alloc,
	                         &list->nmemb,
	                         list->size,
	                         &list->head,
	                         &list->tail,
	                         value);
}

void *front_list(const List *list)
{
	return access_doubly_linked(list->head);
}

void *back_list(const List *list)
{
	return access_doubly_linked(list->tail);
}

void pop_front_list(List *list)
{
	pop_front_doubly_linked(&list->alloc, &list->head, &list->tail);
}

void pop_back_list(List *list)
{
	pop_back_doubly_linked(&list->alloc, &list->head, &list->tail);
}

void clear_list(List *list)
{
	clear_doubly_linked(&list->alloc, &list->head, &list->tail, &list->nmemb);
}

Iter insert_list(List *list, const void *value, Iter pos)
{
	return insert_doubly_linked(&list->alloc,
	                            &list->nmemb,
	                            list->size,
	                            &list->head,
	                            &list->tail,
	                            value,
	                            pos);
}

Iter erase_list(List *list, Iter index)
{
	return erase_doubly_linked(&list->alloc,
	                           &list->nmemb,
	                           index,
	                           &list->head,
	                           &list->tail);
}

Iter begin_list(const List *list)
{
	struct DoubleLinkedNode *node = list->head;
	Iter iter = { .type = ITERATOR_LIST, .data.linked = { .node = node } };
	return iter;
}

Iter end_list(const List *)
{
	struct DoubleLinkedNode *node = NULL;
	Iter iter = { .type = ITERATOR_LIST, .data.linked = { .node = node } };
	return iter;
}

Iter rbegin_list(const List *list)
{
	struct DoubleLinkedNode *node = list->tail;
	Iter                     iter = { .type        = ITERATOR_LIST_REVERSE,
		                              .data.linked = { .node = node } };
	return iter;
}

Iter rend_list(const List *)
{
	struct DoubleLinkedNode *node = NULL;
	Iter                     iter = { .type        = ITERATOR_LIST_REVERSE,
		                              .data.linked = { .node = node } };
	return iter;
}

bool empty_list(const List *list)
{
	return generic_empty(list->nmemb);
}

size_t size_list(const List *list)
{
	return generic_size(list->nmemb);
}
