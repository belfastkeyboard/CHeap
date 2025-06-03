#include "../../hash_set.h"
#include "../../internals/base.h"
#include "../../internals/hash.h"

typedef struct HashSet
{
	struct Bucket   *buckets;
	struct NodeAlloc alloc;
	HashFnc          hash;
	size_t           k_size;
	KComp            k_comp;
	size_t           nmemb;
	size_t           capacity;
} HashSet;

HashSet *create_hash_set(const size_t key_size, const KComp kc)
{
	return create_hash_set_ext(key_size, kc, djb2);
}

HashSet *create_hash_set_ext(size_t key_size, KComp kc, HashFnc hash)
{
	HashSet *set = memory_allocate_container(sizeof(HashSet));

	set->alloc  = create_node_allocator(0, TABLE_MIN, key_size, 0);
	set->hash   = hash;
	set->k_size = key_size;
	set->k_comp = kc;

	return set;
}

void destroy_hash_set(HashSet **set)
{
	destroy_node_allocator(&(*set)->alloc);
	memory_free_container_generic((void **)set, (*set)->buckets);
}

void insert_hash_set(HashSet *set, const void *key)
{
	hash_insert(&set->buckets,
	            &set->alloc,
	            set->hash,
	            set->k_size,
	            0,
	            set->k_comp,
	            &set->nmemb,
	            &set->capacity,
	            key,
	            NULL);
}

size_t count_hash_set(HashSet *set, const void *key)
{
	return hash_count(set->buckets,
	                  set->hash,
	                  set->k_size,
	                  set->k_comp,
	                  set->capacity,
	                  key);
}

const void *find_hash_set(HashSet *set, const void *key)
{
	return hash_find(set->buckets,
	                 set->hash,
	                 set->k_size,
	                 set->k_comp,
	                 set->capacity,
	                 set->nmemb,
	                 key);
}

bool contains_hash_set(HashSet *set, const void *key)
{
	return hash_contains(set->buckets,
	                     set->hash,
	                     set->k_size,
	                     set->k_comp,
	                     set->capacity,
	                     key);
}

void erase_hash_set(HashSet *set, const void *key)
{
	hash_erase(&set->buckets,
	           set->hash,
	           set->k_size,
	           set->k_comp,
	           &set->nmemb,
	           &set->capacity,
	           key);
}

void clear_hash_set(HashSet *set)
{
	hash_clear(&set->buckets, &set->alloc, &set->nmemb, &set->capacity);
}

Iter begin_hash_set(const HashSet *set)
{
	return begin_hash(ITERATOR_HASH_SET, set->buckets, set->capacity);
}

Iter end_hash_set(const HashSet *set)
{
	return end_hash(ITERATOR_HASH_SET, set->buckets, set->capacity);
}

bool empty_hash_set(const HashSet *set)
{
	return generic_empty(set->nmemb);
}

size_t size_hash_set(const HashSet *set)
{
	return generic_size(set->nmemb);
}
