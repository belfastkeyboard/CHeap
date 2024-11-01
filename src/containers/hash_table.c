#include "../../internals/hash.h"
#include "../../hash_set.h"
#include "../../internals/base.h"

typedef struct HashTable
{
    struct Bucket *buckets;
    void *keys;
    void *values;

    size_t k_size;
    size_t v_size;

    KComp k_comp;

    size_t nmemb;
    size_t capacity;
} HashTable;
HashTable *create_hash_table(size_t key_size, size_t value_size, KComp kc)
{
    HashTable *table = memory_allocate_container(sizeof(HashTable));

    table->k_size = key_size;
    table->v_size = value_size;

    table->k_comp = kc;

    return table;
}
void destroy_hash_table(HashTable **table)
{
    memory_free_container_hash((void **)table, (*table)->buckets, (*table)->keys, (*table)->values);
}

void insert_hash_table(HashTable *table, void *key, void *value)
{
    hash_insert(&table->buckets, &table->keys, &table->values, table->k_size, table->v_size, table->k_comp, &table->nmemb, &table->capacity, key, value);
}

size_t count_hash_table(HashTable *table, void *key)
{
    return hash_count(table->buckets, table->keys, table->k_size, table->k_comp, table->capacity, key);
}
void *find_hash_table(HashTable *table, void *key)
{
    return hash_find(table->buckets, table->keys, table->values, table->k_size, table->v_size, table->k_comp, table->capacity, table->nmemb, key);
}
bool contains_hash_table(HashTable* table, void *key)
{
    return hash_contains(table->buckets, table->keys, table->k_size, table->k_comp, table->capacity, key);
}

void erase_hash_table(HashTable* table, void *key)
{
    hash_erase(&table->buckets, &table->keys, &table->values, table->k_size, table->v_size, table->k_comp, &table->nmemb, &table->capacity, key);
}

void clear_hash_table(HashTable *table)
{
    hash_clear(&table->buckets, &table->keys, &table->values, &table->nmemb, &table->capacity);
}

bool empty_hash_table(HashTable* table)
{
    return generic_empty(table->nmemb);
}
size_t size_hash_table(HashTable* table)
{
    return generic_size(table->nmemb);
}
