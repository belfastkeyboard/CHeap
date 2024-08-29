#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct ForwardList ForwardList, FList;
typedef void *Iter;
typedef bool (*Comparator)(const void*, const void*);

__attribute__((warn_unused_result))
ForwardList *create_forward_list(size_t size, Comparator comparator);
void destroy_forward_list(ForwardList *flist);

void push_front(ForwardList *flist, void *value);
Iter insert(ForwardList *flist, Iter iter, void *value);

void *front(ForwardList *flist);

void pop_front(ForwardList *flist);
Iter erase_after(ForwardList *flist, Iter iter);
void clear(ForwardList *flist);

bool empty(ForwardList *flist);
size_t size(ForwardList *flist);
