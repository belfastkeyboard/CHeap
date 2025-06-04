#include "../../list.h"
#include "../../internals/base.h"
#include "../../internals/linked.h"
#include "../../iter.h"

typedef struct List
{
	struct NodeAlloc   alloc;
	size_t             nmemb;
	size_t             size;
	struct LinkedNode *head;
	struct LinkedNode *tail;
} List;

List *create_list(const size_t size)
{
	return create_list_capacity(size, NODE_COUNT_DEFAULT);
}

List *create_list_capacity(size_t size, size_t init)
{
	List *list = memory_allocate_container(sizeof(List));

	list->alloc = create_node_allocator(sizeof(struct LinkedNode),
	                                    init,
	                                    size,
	                                    0);
	list->size  = size;

	return list;
}

void destroy_list(List **list)
{
	destroy_node_allocator(&(*list)->alloc);
	memory_free_buffer((void **)list);
}

void push_back_list(List *list, const void *value)
{
	generic_push_back_doubly_linked(&list->alloc,
	                                &list->nmemb,
	                                list->size,
	                                &list->head,
	                                &list->tail,
	                                value);
}

void push_front_list(List *list, const void *value)
{
	generic_push_front_doubly_linked(&list->alloc,
	                                 &list->nmemb,
	                                 list->size,
	                                 &list->head,
	                                 &list->tail,
	                                 value);
}

size_t insert_list(List *list, const void *value, const size_t index)
{
	return generic_insert_doubly_linked(&list->alloc,
	                                    &list->nmemb,
	                                    list->size,
	                                    &list->head,
	                                    &list->tail,
	                                    value,
	                                    index);
}

void *front_list(const List *list)
{
	return generic_access_linked(list->head);
}

void *back_list(const List *list)
{
	return generic_access_linked(list->tail);
}

void pop_front_list(List *list)
{
	generic_pop_front_doubly_linked(&list->alloc,
	                                &list->nmemb,
	                                &list->head,
	                                &list->tail);
}

void pop_back_list(List *list)
{
	generic_pop_back_doubly_linked(&list->alloc,
	                               &list->nmemb,
	                               &list->head,
	                               &list->tail);
}

size_t erase_list(List *list, const size_t index)
{
	return generic_erase_doubly_linked(&list->alloc,
	                                   &list->nmemb,
	                                   index,
	                                   &list->head,
	                                   &list->tail);
}

void clear_list(List *list)
{
	generic_clear_linked(&list->alloc, &list->head, &list->tail, &list->nmemb);
}

Iter begin_list(const List *list)
{
	struct LinkedNode *node = list->head;
	Iter iter = { .type = ITERATOR_LIST, .data.linked = { .node = node } };
	return iter;
}

Iter end_list(const List *)
{
	struct LinkedNode *node = NULL;
	Iter iter = { .type = ITERATOR_LIST, .data.linked = { .node = node } };
	return iter;
}

Iter rbegin_list(const List *list)
{
	struct LinkedNode *node = list->tail;
	Iter iter = { .type = ITERATOR_LIST, .data.linked = { .node = node } };
	return iter;
}

Iter rend_list(const List *)
{
	struct LinkedNode *node = NULL;
	Iter iter = { .type = ITERATOR_LIST, .data.linked = { .node = node } };
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
