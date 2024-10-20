#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct HashSet HashSet;

typedef int (*KeyComp)(const void *a, const void *b);

__attribute__((warn_unused_result))
HashSet *create_hash_set(size_t key_size, KeyComp kc);
void destroy_hash_set(HashSet **set);

void insert_hash_set(HashSet *set, void *key);

size_t count_hash_set(HashSet *set, void *key);
void *find_hash_set(HashSet *set, void *key);
bool contains_hash_set(HashSet *set, void *key);

void erase_hash_set(HashSet *set, void *key);
void clear_hash_set(HashSet *set);

bool empty_hash_set(HashSet *set);
size_t size_hash_set(HashSet *set);
