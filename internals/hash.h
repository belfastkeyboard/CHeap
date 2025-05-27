#pragma once

#include <stdbool.h>
#include <stddef.h>


typedef unsigned long Hash;

typedef Hash (*HashFnc)(const void *item,
                        size_t size);

typedef int (*KComp)(const void *a,
                     const void *b);


struct Bucket;


void hash_insert(struct Bucket **buckets,
                 void **keys,
                 void **values,
                 HashFnc fnc,
                 size_t k_size,
                 size_t v_size,
                 KComp k_comp,
                 size_t *nmemb,
                 size_t *capacity,
                 const void *key,
                 const void *value);

void hash_erase(struct Bucket **buckets,
                void **keys,
                void **values,
                HashFnc fnc,
                size_t k_size,
                size_t v_size,
                KComp k_comp,
                size_t *nmemb,
                size_t *capacity,
                const void *key);

void hash_clear(struct Bucket **buckets,
                void **keys,
                void **values,
                size_t *nmemb,
                size_t *capacity);


size_t hash_count(struct Bucket *buckets,
                  void *keys,
                  HashFnc fnc,
                  size_t k_size,
                  KComp k_comp,
                  size_t capacity,
                  const void *key);

void *hash_find(struct Bucket *buckets,
                void *keys,
                void *values,
                HashFnc fnc,
                size_t k_size,
                size_t v_size,
                KComp k_comp,
                size_t capacity,
                size_t nmemb,
                const void *key);

bool hash_contains(struct Bucket *buckets,
                   void *keys,
                   HashFnc fnc,
                   size_t k_size,
                   KComp k_comp,
                   size_t capacity,
                   const void *key);
