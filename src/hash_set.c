#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include "../hash_set.h"

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
    bool tombstone;
} Bucket;
typedef struct Set
{
    Bucket* array;

    size_t k_size;
    KeyComp k_comp;

    size_t nmemb;
    size_t capacity;
} Set;

static Bucket create_bucket(hash_t hash, void *key, size_t k_size)
{
    Bucket bucket = { 0 };

    bucket.hash = hash;
    bucket.tombstone = false;

    bucket.key = malloc(k_size);
    memcpy(bucket.key, key, k_size);

    return bucket;
}
static void destroy_bucket(Bucket *bucket)
{
    if (bucket->key != (void*)-1)
        free(bucket->key);
}

Set *create_set(size_t key_size, KeyComp kc)
{
    Set *set = malloc(sizeof(Set));

    set->array = NULL;

    set->capacity = 0;
    set->nmemb = 0;

    set->k_size = key_size;
    set->k_comp = kc;

    return set;
}
void destroy_set(Set **set)
{
    clear(*set);

    if ((*set)->array)
        free((*set)->array);

    free(*set);
    *set = NULL;
}

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
static size_t probe(Set *set, void *key, size_t index, bool skip_tombstones)
{
    assert(set && set->array);

    size_t found = NOT_FOUND;
    size_t tombstone = INVALID;
    size_t count = 1;

    while (found == NOT_FOUND)
    {
        Bucket bucket = set->array[index];

        if (skip_tombstones)
        {
            if (tombstone == INVALID && bucket.hash != INVALID && bucket.tombstone)
                tombstone = index;
            else if (bucket.hash == INVALID)
                break;
        }

        if (
                (!bucket.tombstone && bucket.key != (void*)-1 && set->k_comp(bucket.key, key)) ||
                (!skip_tombstones && (bucket.hash == INVALID || bucket.tombstone))
                )
            found = index;
        else
            index = (index + 1) % set->capacity;

        if (++count > set->capacity)
            break;
    }

    if (tombstone != INVALID && found != NOT_FOUND)
    {
        void *tmp;

        tmp = set->array[tombstone].key;

        set->array[tombstone].key = set->array[found].key;
        set->array[tombstone].tombstone = false;

        set->array[found].key = tmp;
        set->array[found].tombstone = true;

        found = tombstone;
    }

    return found;
}
static void resize(Set *set, float factor)
{
    assert(set->array);

    size_t old_capacity = set->capacity;

    Bucket* tmp = malloc(set->capacity * sizeof(Bucket));
    memset(tmp, UNSET, set->capacity * sizeof(Bucket));
    memcpy(tmp, set->array, set->capacity * sizeof(Bucket));

    set->capacity = (size_t)((float)set->capacity * factor);
    free(set->array);
    set->array = malloc(set->capacity * sizeof(Bucket));

    assert(set->array);
    memset(set->array, UNSET, (size_t)((float)(set->capacity * sizeof(Bucket))));

    Bucket bucket;
    for (int i = 0; i < old_capacity; i++)
    {
        bucket = tmp[i];

        if (bucket.hash == INVALID || bucket.tombstone)
            continue;

        size_t re_index = get_index(bucket.hash, set->capacity);
        re_index = probe(set, bucket.key, re_index, false);
        set->array[re_index] = bucket;
    }

    free(tmp);
}
static Bucket *find_bucket(Set *set, void *key)
{
    Bucket *bucket = NULL;

    hash_t hash = djb2(key, set->k_size);
    size_t index = get_index(hash, set->capacity);

    index = probe(set, key, index, true);

    if (index != NOT_FOUND)
        bucket = &set->array[index];

    return bucket;
}

void insert(Set *set, void *key)
{
    if (set->nmemb == 0)
    {
        if (set->array)
            clear(set);

        set->capacity = TABLE_MIN;

        set->array = malloc(set->capacity * sizeof(Bucket));
        memset(set->array, UNSET, set->capacity * sizeof(Bucket));
    }
    else
    {
        float load_factor = ((float)set->nmemb / (float)set->capacity);
        if (load_factor >= LF_UPPER_THRESHOLD)
            resize(set, GROW_FACTOR);
    }

    assert(set->array);

    hash_t hash = djb2(key, set->k_size);
    size_t index = get_index(hash, set->capacity);
    index = probe(set, key, index, false);

    if (
            !set->array[index].tombstone &&
            (set->array[index].key == (void*)-1 || !set->k_comp(set->array[index].key, key))
            )
    {
        Bucket bucket = create_bucket(hash, key, set->k_size);
        set->array[index] = bucket;

        set->nmemb++;
    }
}

void erase(Set* set, void *key)
{
    if(set->nmemb > 0)
    {
        hash_t hash = djb2(key, set->k_size);
        size_t index = get_index(hash, set->capacity);

        index = probe(set, key, index, true);
        if (index != NOT_FOUND)
        {
            set->array[index].tombstone = true;
            set->nmemb--;
        }

        float load_factor = ((float)set->nmemb / (float)set->capacity);
        if (load_factor <= LF_LOWER_THRESHOLD && set->capacity > TABLE_MIN)
            resize(set, SHRINK_FACTOR);
    }
}

void clear(Set *set)
{
    if (set->array)
    {
        for (size_t i = 0; i < set->capacity; i++)
        {
            Bucket *bucket = &set->array[i];
            destroy_bucket(bucket);
        }

        free(set->array);
    }

    set->capacity = 0;
    set->nmemb = 0;
    set->array = NULL;
}

size_t count(Set *set, void *key)
{
    return (find_bucket(set, key)) ? 1 : 0;
}
void *find(Set *set, void *key)
{
    void *k = NULL;

    Bucket *bucket;
    if ((bucket = find_bucket(set, key)))
        k = bucket->key;

    return k;
}
bool contains(Set* set, void *key)
{
    return (find_bucket(set, key)) ? true : false;
}

bool empty(Set* set)
{
    return (set->nmemb == 0);
}
size_t size(Set* set)
{
    return set->nmemb;
}
