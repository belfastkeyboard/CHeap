#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "internals/comp.h"

typedef struct HashTable HashTable;


__attribute__((warn_unused_result))
HashTable *create_hash_table(size_t key_size,
                             size_t value_size,
                             KComp kc);

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


bool empty_hash_table(const HashTable *table);

size_t size_hash_table(const HashTable *table);
