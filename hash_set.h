#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct Set Set;
typedef int (*KeyComp)(const void *a, const void *b);

__attribute__((warn_unused_result))
Set *create_set(size_t key_size, KeyComp kc);
void destroy_set(Set **set);

void insert(Set *set, void *key);

size_t count(Set *set, void *key);
void *find(Set *set, void *key);
bool contains(Set *set, void *key);

void erase(Set *set, void *key);
void clear(Set *set);

bool empty(Set *set);
size_t size(Set *set);

