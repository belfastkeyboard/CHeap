#pragma once

#include <stdbool.h>
#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

typedef struct List List;

ALLOC List *create_list(size_t size);
ALLOC List *create_list_capacity(size_t size, size_t init);
void        destroy_list(List **list);

void   push_back_list(List *list, const void *value);
void   push_front_list(List *list, const void *value);
size_t insert_list(List *list, const void *value, size_t index);

void *front_list(const List *list);
void *back_list(const List *list);

void   pop_front_list(List *list);
void   pop_back_list(List *list);
size_t erase_list(List *list, size_t index);
void   clear_list(List *list);

bool   empty_list(const List *list);
size_t size_list(const List *list);
