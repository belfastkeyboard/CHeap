#pragma once

#include <stdbool.h>
#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

typedef struct ForwardList ForwardList, FList;

ALLOC FList *create_forward_list(size_t size);
ALLOC FList *create_forward_list_capacity(size_t size, size_t init);
void         destroy_forward_list(FList **flist);

void   push_front_forward_list(FList *flist, const void *value);
size_t insert_after_forward_list(FList *flist, const void *value, size_t index);

void *front_forward_list(const FList *flist);

void   pop_front_forward_list(FList *flist);
size_t erase_after_forward_list(FList *flist, size_t index);
void   clear_forward_list(FList *flist);

#ifdef CHEAP_ITERATOR_AVAILABLE
Iter begin_forward_list(const FList *flist);
Iter end_forward_list(const FList *flist);
#endif

bool   empty_forward_list(const FList *flist);
size_t size_forward_list(const FList *flist);
