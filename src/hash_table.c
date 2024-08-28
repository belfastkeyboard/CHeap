#include "../hash_table.h"

#include <assert.h>
#include <malloc.h>
#include <memory.h>

#define GROW_FACTOR 2.f
#define SHRINK_FACTOR 0.5f
#define LF_UPPER_THRESHOLD 0.75f
#define LF_LOWER_THRESHOLD 0.1f
#define TABLE_MIN 8

#define UNSET (-1)
#define INVALID UNSET
#define NOT_FOUND INVALID

typedef unsigned long hash_t;

typedef struct Bucket
{
    hash_t hash;
    void *key;
    void *value;
    bool tombstone;
} Bucket;
typedef struct Table
{
    Bucket* array;

    size_t k_size;
    size_t v_size;
    KeyComp k_comp;
    ValueComp v_comp;

    size_t nmemb;
    size_t capacity;
} Table;

static Bucket create_bucket(hash_t hash, void *key, size_t k_size, void *value, size_t v_size)
{
    Bucket bucket = { 0 };

    bucket.hash = hash;
    bucket.tombstone = false;

    bucket.key = malloc(k_size);
    memcpy(bucket.key, key, k_size);

    bucket.value = malloc(v_size);
    memcpy(bucket.value, value, v_size);

    return bucket;
}
static void destroy_bucket(Bucket *bucket)
{
    if (bucket->key != (void*)-1 || bucket->value != (void*)-1)
    {
        free(bucket->key);
        free(bucket->value);
    }
}

Table *create_table(size_t key_size, size_t value_size, KeyComp kc, ValueComp vc)
{
    Table *table = malloc(sizeof(Table));

    table->array = NULL;

    table->capacity = 0;
    table->nmemb = 0;

    table->k_size = key_size;
    table->v_size = value_size;
    table->k_comp = kc;
    table->v_comp = vc;

    return table;
}
void destroy_table(Table **table)
{
    clear(*table);

    if ((*table)->array)
        free((*table)->array);

    free(*table);
    *table = NULL;
}

// slightly modified version of djb2 algorithm to allow handling of null terminators (e.g. hash an int 0)
// there are other ways to accomplish this (e.g. snprintf) but this method is simple and general purpose
static hash_t djb2(void *item, size_t size)
{
    hash_t hash = 5381;

    unsigned char data[size];
    memcpy(data, item, size);

    for (int i = 0; i < size; i++)
        hash = ((hash << 5) + hash) + data[i];

    return hash;
}
static size_t get_index(hash_t hash, size_t capacity)
{
    return hash % capacity;
}
static size_t probe(Table *table, void *value, unsigned int index, bool skip_tombstones)
{
    assert(table && table->array);

    size_t found = NOT_FOUND;
    size_t tombstone = INVALID;
    size_t count = 1;

    while (found == NOT_FOUND)
    {
        Bucket bucket = table->array[index];

        if (skip_tombstones)
        {
            if (tombstone == INVALID && bucket.hash != INVALID && bucket.tombstone)
                tombstone = index;
            else if (bucket.hash == INVALID)
                break;
        }

        if (
            (!bucket.tombstone && bucket.value != (void*)-1 && table->v_comp(bucket.value, value)) ||
            (!skip_tombstones && (bucket.hash == INVALID || bucket.tombstone))
        )
            found = index;
        else
            index = (index + 1) % table->capacity;

        if (++count > table->capacity)
            break;
    }

    if (tombstone != INVALID && found != NOT_FOUND)
    {
        void *key, *val;

        key = table->array[tombstone].key;
        val = table->array[tombstone].value;

        table->array[tombstone].key = table->array[found].key;
        table->array[tombstone].value = table->array[found].value;
        table->array[tombstone].tombstone = false;

        table->array[found].key = key;
        table->array[found].value = val;
        table->array[found].tombstone = true;

        found = tombstone;
    }

    return found;
}
static void resize(Table *table, float factor)
{
    assert(table->array);

    size_t old_capacity = table->capacity;

    Bucket* tmp = malloc(table->capacity * sizeof(Bucket));
    memset(tmp, UNSET, table->capacity * sizeof(Bucket));
    memcpy(tmp, table->array, table->capacity * sizeof(Bucket));

    table->capacity = (size_t)((float)table->capacity * factor);
    free(table->array);
    table->array = malloc(table->capacity * sizeof(Bucket));

    assert(table->array);
    memset(table->array, UNSET, (size_t)((float)(table->capacity * sizeof(Bucket))));

    Bucket bucket;
    for (int i = 0; i < old_capacity; i++)
    {
        bucket = tmp[i];

        if (bucket.hash == INVALID || bucket.tombstone)
            continue;

        size_t re_index = get_index(bucket.hash, table->capacity);
        re_index = probe(table, bucket.value, re_index, false);
        table->array[re_index] = bucket;
    }

    free(tmp);
}
static Bucket *find_bucket(Table *table, void *key)
{
    Bucket *bucket = NULL;

    hash_t hash = djb2(key, table->k_size);
    size_t index = get_index(hash, table->capacity);

    index = probe(table, key, index, true);

    if (index != NOT_FOUND)
        bucket = &table->array[index];

    return bucket;
}

void insert(Table *table, void *key, void *value)
{
    if (table->nmemb == 0)
    {
        if (table->array)
            clear(table);

        table->capacity = TABLE_MIN;

        table->array = malloc(table->capacity * sizeof(Bucket));
        memset(table->array, UNSET, table->capacity * sizeof(Bucket));
    }
    else
    {
        float load_factor = ((float)table->nmemb / (float)table->capacity);
        if (load_factor >= LF_UPPER_THRESHOLD)
            resize(table, GROW_FACTOR);
    }

    assert(table->array);

    hash_t hash = djb2(key, table->k_size);
    size_t index = get_index(hash, table->capacity);
    index = probe(table, value, index, false);

    Bucket bucket;
    if (
        !table->array[index].tombstone &&
        (table->array[index].value == (void*)-1 || !table->v_comp(table->array[index].value, value))
    )
    {
        bucket = create_bucket(hash, key, table->k_size, value, table->v_size);
        table->nmemb++;
    }
    else
    {
        bucket = table->array[index];
        memcpy(bucket.value, value, table->v_size);
    }

    table->array[index] = bucket;
}

void erase(Table* table, void *key)
{
    if(table->nmemb > 0)
    {
        hash_t hash = djb2(key, table->k_size);
        size_t index = get_index(hash, table->capacity);

        index = probe(table, key, index, true);
        if (index != NOT_FOUND)
        {
            table->array[index].tombstone = true;
            table->nmemb--;
        }

        float load_factor = ((float)table->nmemb / (float)table->capacity);
        if (load_factor <= LF_LOWER_THRESHOLD && table->capacity > TABLE_MIN)
            resize(table, SHRINK_FACTOR);
    }
}

void clear(Table *table)
{
    if (table->array)
    {
        for (size_t i = 0; i < table->capacity; i++)
        {
            Bucket *bucket = &table->array[i];
            destroy_bucket(bucket);
        }

        free(table->array);
    }

    table->capacity = 0;
    table->nmemb = 0;
    table->array = NULL;
}

size_t count(Table *table, void *key)
{
    return (find_bucket(table, key)) ? 1 : 0;
}
void *find(Table *table, void *key)
{
    void *value = NULL;

    Bucket *bucket;
    if ((bucket = find_bucket(table, key)))
        value = bucket->value;

    return value;
}
bool contains(Table* table, void *key)
{
    return (find_bucket(table, key)) ? true : false;
}

bool empty(Table* table)
{
    return (table->nmemb == 0);
}
size_t size(Table* table)
{
    return table->nmemb;
}
