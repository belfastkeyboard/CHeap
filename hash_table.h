#pragma once

#include <stdbool.h>
#include <stddef.h>

#ifdef CHEAP_ITERATOR_AVAILABLE
#include "iter.h"
#endif


typedef struct HashTable HashTable;
typedef unsigned long Hash;

typedef Hash (*HashFnc)(const void *item,
                        size_t size);

typedef int (*KComp)(const void *a,
                     const void *b);

#ifndef CHEAP_KEY_VALUE_PAIR_DEFINED
typedef struct PairKV
{
    const void *key;
    void *value;
} PairKV;
#define CHEAP_KEY_VALUE_PAIR_DEFINED
#endif


Hash djb2(const void *item,
          size_t size);

Hash djb2s(const void *item,
           size_t size);


__attribute__((warn_unused_result))
HashTable *create_hash_table(size_t key_size,
                             size_t value_size,
                             KComp kc);

__attribute__((warn_unused_result))
HashTable *create_hash_table_ext(size_t key_size,
                                 size_t value_size,
                                 KComp kc,
                                 HashFnc hash);

void destroy_hash_table(HashTable **table);


void insert_hash_table(HashTable *table,
                       const void *key,
                       const void *value);


size_t count_hash_table(HashTable *table,
                        const void *key);

void *find_hash_table(HashTable *table,
                      const void *key);

bool contains_hash_table(HashTable *table,
                         const void *key);


void erase_hash_table(HashTable *table,
                      const void *key);

void clear_hash_table(HashTable *table);


#ifdef CHEAP_ITERATOR_AVAILABLE
Iter begin_hash_table(const HashTable *table);

Iter end_hash_table(const HashTable *table);
#endif


bool empty_hash_table(const HashTable *table);

size_t size_hash_table(const HashTable *table);
