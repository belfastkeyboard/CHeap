#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct List List;


__attribute__((warn_unused_result))
List *create_list(size_t size);

void destroy_list(List **list);


void push_back_list(List *list,
                    void *value);

void push_front_list(List *list,
                     void *value);

size_t insert_list(List *list,
                   void *value,
                   size_t index);


void *front_list(List *list);

void *back_list(List *list);


void pop_front_list(List *list);

void pop_back_list(List *list);

size_t erase_list(List *list,
                  size_t index);

void clear_list(List *list);


bool empty_list(List *list);

size_t size_list(List *list);
