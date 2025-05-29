#pragma once

#include <stdbool.h>
#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

typedef struct HashSet HashSet;
typedef unsigned long  Hash;

typedef Hash (*HashFnc)(const void *item, size_t size);
typedef int (*KComp)(const void *a, const void *b);

Hash djb2(const void *item, size_t size);
Hash djb2s(const void *item, size_t size);

ALLOC HashSet *create_hash_set(size_t key_size, KComp kc);
ALLOC HashSet *create_hash_set_ext(size_t key_size, KComp kc, HashFnc hash);
void           destroy_hash_set(HashSet **set);

void insert_hash_set(HashSet *set, const void *key);

size_t      count_hash_set(HashSet *set, const void *key);
const void *find_hash_set(HashSet *set, const void *key);
bool        contains_hash_set(HashSet *set, const void *key);

void erase_hash_set(HashSet *set, const void *key);
void clear_hash_set(HashSet *set);

#ifdef CHEAP_ITERATOR_AVAILABLE
Iter begin_hash_set(const HashSet *set);
Iter end_hash_set(const HashSet *set);
#endif

bool   empty_hash_set(const HashSet *set);
size_t size_hash_set(const HashSet *set);
