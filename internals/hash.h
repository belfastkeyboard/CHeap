#pragma once

#include <stdbool.h>
#include <stddef.h>

struct Bucket;

typedef int (*KComp)(const void *a, const void *b);

void hash_insert(struct Bucket **buckets, void **keys, void **values, size_t k_size, size_t v_size, KComp k_comp, size_t *nmemb, size_t *capacity, void *key, void *value);
void hash_erase(struct Bucket **buckets, void **keys, void **values, size_t k_size, size_t v_size, KComp k_comp, size_t *nmemb, size_t *capacity, void *key);
void hash_clear(struct Bucket **buckets, void **keys, void **values, size_t *nmemb, size_t *capacity);

size_t hash_count(struct Bucket *buckets, void *keys, size_t k_size, KComp k_comp, size_t capacity, void *key);
void *hash_find(struct Bucket *buckets, void *keys, void *values, size_t k_size, size_t v_size, KComp k_comp, size_t capacity, size_t nmemb, void *key);
bool hash_contains(struct Bucket *buckets, void *keys, size_t k_size, KComp k_comp, size_t capacity, void *key);
