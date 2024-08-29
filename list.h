#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct List List;
typedef struct Node Iter;
typedef bool (*Comparator)(const void*, const void*);

__attribute__((warn_unused_result))
List *create_list(size_t size, Comparator comparator);
void destroy_list(List **list);

void push_back(List *list, void *value);
void push_front(List *list, void *value);
void insert(List *list, Iter *iter, void *value);

void *front(List *list);
void *back(List *list);
Iter *find(List *list, void *value);

void pop_front(List *list);
void pop_back(List *list);
Iter *erase(List *list, Iter *iter);
void clear(List *list);

bool empty(List *list);
size_t size(List *list);
