#pragma once

#include <stdbool.h>
#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

typedef struct List List;

ALLOC List *create_list(size_t size);
ALLOC List *create_list_capacity(size_t size, size_t init);
void        destroy_list(List **list);

void push_back_list(List *list, const void *value);
void push_front_list(List *list, const void *value);

void *front_list(const List *list);
void *back_list(const List *list);

void pop_front_list(List *list);
void pop_back_list(List *list);
void clear_list(List *list);

#ifdef CHEAP_ITERATOR_AVAILABLE
Iter insert_list(List *list, const void *value, Iter pos);
Iter erase_list(List *list, Iter index);

Iter begin_list(const List *list);
Iter end_list(const List *list);

Iter rbegin_list(const List *list);
Iter rend_list(const List *list);
#endif

bool   empty_list(const List *list);
size_t size_list(const List *list);
