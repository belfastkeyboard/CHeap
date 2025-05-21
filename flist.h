#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct ForwardList ForwardList, FList;


__attribute__((warn_unused_result))
FList *create_forward_list(size_t size);

__attribute__((warn_unused_result))
FList *create_forward_list_capacity(size_t size,
                                    size_t init);

void destroy_forward_list(FList **flist);


void push_front_forward_list(FList *flist,
                             const void *value);

size_t insert_after_forward_list(FList *flist,
                                 const void *value,
                                 size_t index);


void *front_forward_list(const FList *flist);


void pop_front_forward_list(FList *flist);

size_t erase_after_forward_list(FList *flist,
                                size_t index);

void clear_forward_list(FList *flist);


bool empty_forward_list(const FList *flist);

size_t size_forward_list(const FList *flist);
