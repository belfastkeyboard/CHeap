#pragma once

#include "../iter.h"
#include "nalloc.h"
#include <stdbool.h>
#include <stddef.h>

#define TABLE_MIN 8

typedef unsigned long Hash;
typedef Hash (*HashFnc)(const void *item, size_t size);
typedef int (*KComp)(const void *a, const void *b);

#ifndef CHEAP_KEY_VALUE_PAIR_DEFINED
typedef struct PairKV
{
	const void *key;
	void       *value;
} PairKV;
#define CHEAP_KEY_VALUE_PAIR_DEFINED
#endif

struct Bucket;

void hash_insert(struct Bucket   **buckets,
                 struct NodeAlloc *alloc,
                 HashFnc           fnc,
                 size_t            k_size,
                 size_t            v_size,
                 KComp             k_comp,
                 size_t           *nmemb,
                 size_t           *capacity,
                 const void       *key,
                 const void       *value);

void hash_erase(struct Bucket **buckets,
                HashFnc         fnc,
                size_t          k_size,
                KComp           k_comp,
                size_t         *nmemb,
                size_t         *capacity,
                const void     *key);
void hash_clear(struct Bucket   **buckets,
                struct NodeAlloc *alloc,
                size_t           *nmemb,
                size_t           *capacity);

size_t hash_count(struct Bucket *buckets,
                  HashFnc        fnc,
                  size_t         k_size,
                  KComp          k_comp,
                  size_t         capacity,
                  const void    *key);
void  *hash_find(struct Bucket *buckets,
                 HashFnc        fnc,
                 size_t         k_size,
                 KComp          k_comp,
                 size_t         capacity,
                 size_t         nmemb,
                 const void    *key);
bool   hash_contains(struct Bucket *buckets,
                     HashFnc        fnc,
                     size_t         k_size,
                     KComp          k_comp,
                     size_t         capacity,
                     const void    *key);

Iter begin_hash(IteratorType type, struct Bucket *buckets, size_t capacity);
Iter end_hash(IteratorType type, struct Bucket *buckets, size_t capacity);
