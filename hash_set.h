#pragma once

#include <stdbool.h>
#include <stddef.h>


typedef struct HashSet HashSet;
typedef unsigned long Hash;

typedef Hash (*HashFnc)(const void *item,
                        size_t size);

typedef int (*KComp)(const void *a,
                     const void *b);


Hash djb2(const void *item,
          size_t size);

Hash djb2s(const void *item,
           size_t size);


__attribute__((warn_unused_result))
HashSet *create_hash_set(size_t key_size,
                         KComp kc);

__attribute__((warn_unused_result))
HashSet *create_hash_set_ext(size_t key_size,
                             KComp kc,
                             HashFnc hash);

void destroy_hash_set(HashSet **set);


void insert_hash_set(HashSet *set,
                     const void *key);


size_t count_hash_set(HashSet *set,
                      const void *key);

void *find_hash_set(HashSet *set,
                    const void *key);

bool contains_hash_set(HashSet *set,
                       const void *key);


void erase_hash_set(HashSet *set,
                    const void *key);

void clear_hash_set(HashSet *set);


bool empty_hash_set(const HashSet *set);

size_t size_hash_set(const HashSet *set);
