#include "../../internals/hash.h"
#include "../../internals/base.h"
#include "../../hash_table.h"


typedef struct HashTable
{
    struct Bucket *buckets;
    struct NodeAlloc alloc;
    HashFnc hash;
    KComp k_comp;
    size_t k_size;
    size_t v_size;
    size_t nmemb;
    size_t capacity;
} HashTable;


HashTable *create_hash_table(const size_t key_size,
                             const size_t value_size,
                             const KComp kc)
{
    return create_hash_table_ext(key_size,
                                 value_size,
                                 kc,
                                 djb2);
}

__attribute__((warn_unused_result))
HashTable *create_hash_table_ext(size_t key_size,
                                 size_t value_size,
                                 KComp kc,
                                 HashFnc hash)
{
    HashTable *table = memory_allocate_container(sizeof(HashTable));

    table->alloc = create_node_allocator(0,
                                         TABLE_MIN,
                                         key_size,
                                         value_size);

    table->hash = hash;
    table->k_size = key_size;
    table->v_size = value_size;
    table->k_comp = kc;

    return table;
}

void destroy_hash_table(HashTable **table)
{
    destroy_node_allocator(&(*table)->alloc);

    memory_free_container_generic((void**)table,
                                  (*table)->buckets);
}


void insert_hash_table(HashTable *table,
                       const void *key,
                       const void *value)
{
    hash_insert(&table->buckets,
                &table->alloc,
                table->hash,
                table->k_size,
                table->v_size,
                table->k_comp,
                &table->nmemb,
                &table->capacity,
                key,
                value);
}


size_t count_hash_table(HashTable *table,
                        const void *key)
{
    return hash_count(table->buckets,
                      table->hash,
                      table->k_size,
                      table->k_comp,
                      table->capacity,
                      key);
}

void *find_hash_table(HashTable *table,
                      const void *key)
{
    return hash_find(table->buckets,
                     table->hash,
                     table->k_size,
                     table->k_comp,
                     table->capacity,
                     table->nmemb,
                     key);
}

bool contains_hash_table(HashTable* table,
                         const void *key)
{
    return hash_contains(table->buckets,
                         table->hash,
                         table->k_size,
                         table->k_comp,
                         table->capacity,
                         key);
}


void erase_hash_table(HashTable* table,
                      const void *key)
{
    hash_erase(&table->buckets,
               table->hash,
               table->k_size,
               table->k_comp,
               &table->nmemb,
               &table->capacity,
               key);
}


void clear_hash_table(HashTable *table)
{
    hash_clear(&table->buckets,
               &table->alloc,
               &table->nmemb,
               &table->capacity);
}


bool empty_hash_table(const HashTable* table)
{
    return generic_empty(table->nmemb);
}

size_t size_hash_table(const HashTable* table)
{
    return generic_size(table->nmemb);
}
