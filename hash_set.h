#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "internals/comp.h"


typedef struct HashSet HashSet;


__attribute__((warn_unused_result))
HashSet *create_hash_set(size_t key_size,
                         KComp kc);

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
