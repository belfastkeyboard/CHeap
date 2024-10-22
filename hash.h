#pragma once

#include <stdbool.h>

struct Bucket;
typedef unsigned long hash_t;

#define GROW_FACTOR        2.0f
#define SHRINK_FACTOR      0.5f
#define LF_UPPER_THRESHOLD 0.75f
#define LF_LOWER_THRESHOLD 0.1f
#define TABLE_MIN          8

#define UNSET     (-1)
#define INVALID   UNSET
#define NOT_FOUND INVALID

typedef int (*KeyComp)(const void *a, const void *b);

hash_t djb2(const void *item, size_t size);
size_t get_index(hash_t hash, size_t capacity);

void hash_insert(KeyComp k_comp, struct Bucket **buckets, void **keys, size_t *capacity, size_t k_size, size_t *nmemb, void *key);
void hash_erase(KeyComp k_comp, struct Bucket *buckets, void *keys, size_t *capacity, size_t k_size, void *key, size_t *nmemb);
void hash_clear(struct Bucket **buckets, void **keys, size_t *capacity, size_t *nmemb);

size_t hash_count(KeyComp k_comp, struct Bucket *buckets, void *keys, size_t capacity, size_t k_size, void *key);
void *hash_find(KeyComp k_comp, struct Bucket *buckets, void *keys, size_t capacity, size_t k_size, void *key);
bool hash_contains(KeyComp k_comp, struct Bucket *buckets, void *keys, size_t capacity, size_t k_size, void *key);
