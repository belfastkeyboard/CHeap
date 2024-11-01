#pragma once

#include <stdbool.h>
#include <stddef.h>

struct Bucket;
typedef unsigned long hash_t;

typedef int (*KComp)(const void *a, const void *b);

hash_t djb2(const void *item, size_t size);
size_t get_index(hash_t hash, size_t capacity);

void hash_insert(struct Bucket **buckets, void **keys, void **values, size_t k_size, size_t v_size, KComp k_comp, size_t *nmemb, size_t *capacity, void *key, void *value);
void hash_erase(struct Bucket **buckets, void **keys, void **values, size_t k_size, size_t v_size, KComp k_comp, size_t *nmemb, size_t *capacity, void *key);
void hash_clear(struct Bucket **buckets, void **keys, void **values, size_t *nmemb, size_t *capacity);

size_t hash_count(struct Bucket *buckets, void *keys, size_t k_size, KComp k_comp, size_t capacity, void *key);
void *hash_find(struct Bucket *buckets, void *keys, void *values, size_t k_size, size_t v_size, KComp k_comp, size_t capacity, size_t nmemb, void *key);
bool hash_contains(struct Bucket *buckets, void *keys, size_t k_size, KComp k_comp, size_t capacity, void *key);
