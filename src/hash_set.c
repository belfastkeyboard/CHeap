#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include "../hash_set.h"

#define GROW_FACTOR        2.0f
#define SHRINK_FACTOR      0.5f
#define LF_UPPER_THRESHOLD 0.75f
#define LF_LOWER_THRESHOLD 0.1f
#define TABLE_MIN          8

#define UNSET     (-1)
#define INVALID   UNSET
#define NOT_FOUND INVALID

typedef unsigned long hash_t;

struct Bucket
{
    hash_t hash;
    size_t key_index;
    bool tombstone;
};
static struct Bucket create_bucket(hash_t hash, void *key, size_t k_size, void *values, const size_t nmemb)
{
    memcpy(values + (k_size * nmemb), key, k_size);
    struct Bucket bucket = { .hash = hash, .tombstone = false, .key_index = nmemb };

    return bucket;
}

typedef struct HashSet
{
    struct Bucket *buckets;
    void *keys;

    size_t k_size;
    KeyComp k_comp;

    size_t nmemb;
    size_t capacity;
} HashSet;
HashSet *create_hash_set(size_t key_size, KeyComp kc)
{
    HashSet *set = malloc(sizeof(HashSet));

    set->buckets = NULL;
    set->keys = NULL;

    set->capacity = 0;
    set->nmemb = 0;

    set->k_size = key_size;
    set->k_comp = kc;

    return set;
}
void destroy_hash_set(HashSet **set)
{
    clear_hash_set(*set);

    if ((*set)->buckets)
        free((*set)->buckets);

    free(*set);
    *set = NULL;
}

// helper functions
static hash_t djb2(const void *item, const size_t size)
{
    hash_t hash = 5381;

    unsigned char data[size];
    memcpy(data, item, size);

    for (int i = 0; i < size; i++)
        hash = ((hash << 5) + hash) + data[i];

    return hash;
}
static size_t get_index(const hash_t hash, const size_t capacity)
{
    return hash % capacity;
}
static void *key_from_index(void *keys, size_t size, size_t index)
{
    return keys + (size * index);
}
static bool key_exists(HashSet *set, size_t index)
{
    return !set->buckets[index].tombstone && set->buckets[index].key_index != UNSET;
}
static bool is_found(HashSet *set, struct Bucket bucket, void *key, bool skip_tombstones)
{
    bool valid_bucket = !bucket.tombstone &&
                        bucket.key_index != UNSET &&
                        set->k_comp(key, key_from_index(set->keys, set->k_size, bucket.key_index));

    bool invalid_or_tombstone = !skip_tombstones &&
                                (bucket.hash == INVALID || bucket.tombstone); // TODO: think about .hash == INVALID, it's a bit weird

    return valid_bucket || invalid_or_tombstone;
}

static size_t probe(HashSet *set, void *key, size_t index, bool skip_tombstones)
{
    assert(set && set->buckets);

    size_t found = NOT_FOUND;
    size_t tombstone = INVALID;
    size_t count = 1;

    while (found == NOT_FOUND)
    {
        struct Bucket bucket = set->buckets[index];

        if (skip_tombstones)
        {
            if (tombstone == INVALID && bucket.hash != INVALID && bucket.tombstone)
                tombstone = index;
            else if (bucket.hash == INVALID)
                break;
        }

        if (is_found(set, bucket, key, skip_tombstones))
            found = index;
        else
            index = (index + 1) % set->capacity;

        if (++count > set->capacity)
            break;
    }

    if (tombstone != INVALID && found != NOT_FOUND)
    {
        size_t key_index = set->buckets[tombstone].key_index;

        set->buckets[tombstone].key_index = set->buckets[found].key_index;
        set->buckets[tombstone].tombstone = false;
        set->buckets[tombstone].hash = set->buckets[found].hash;

        set->buckets[found].key_index = key_index;
        set->buckets[found].tombstone = true;

        found = tombstone;
    }

    return found;
}
static void initialise(HashSet *set)
{
    if (set->buckets)
        clear_hash_set(set);

    set->capacity = TABLE_MIN;

    set->buckets = malloc(set->capacity * sizeof(struct Bucket));
    memset(set->buckets, UNSET, set->capacity * sizeof(struct Bucket));
    set->keys = malloc(set->capacity * set->k_size);
}
static void resize(HashSet *set, float factor)
{
    assert(set->buckets);

    size_t old_capacity = set->capacity;
    size_t new_capacity = (size_t)((float)old_capacity * factor);
    size_t t_size = old_capacity * sizeof(struct Bucket);
    size_t m_size = new_capacity * sizeof(struct Bucket);

    struct Bucket *tmp = malloc(t_size);
    memcpy(tmp, set->buckets, t_size);

    set->capacity = new_capacity;

    free(set->buckets);
    set->buckets = malloc(m_size);

    assert(set->buckets);
    memset(set->buckets, UNSET, m_size);

    for (int i = 0; i < old_capacity; i++)
    {
        struct Bucket bucket = tmp[i];

        if (bucket.hash == INVALID || bucket.tombstone)
            continue;

        size_t index = get_index(bucket.hash, set->capacity);
        index = probe(set, key_from_index(set->keys, set->k_size, bucket.key_index), index, false);
        set->buckets[index] = bucket;
    }

    free(tmp);

    tmp = realloc(set->keys, set->capacity * set->k_size);
    assert(tmp);
    set->keys = tmp;
}
static size_t find_bucket(HashSet *set, void *key)
{
    size_t index = NOT_FOUND;

    if (set->capacity)
    {
        hash_t hash = djb2(key, set->k_size);
        index = get_index(hash, set->capacity);
        index = probe(set, key, index, true);
    }

    return index;
}

void insert_hash_set(HashSet *set, void *key)
{
    if (set->nmemb == 0)
    {
        initialise(set);
    }
    else
    {
        float load_factor = ((float)set->nmemb / (float)set->capacity);

        if (load_factor >= LF_UPPER_THRESHOLD)
            resize(set, GROW_FACTOR);
    }

    assert(set->buckets && set->keys);

    hash_t hash = djb2(key, set->k_size);
    size_t index = get_index(hash, set->capacity);
    index = probe(set, key, index, false);

    if (!key_exists(set, index))
    {
        set->buckets[index] = create_bucket(hash, key, set->k_size, set->keys, set->nmemb);
        set->nmemb++;
    }
}

void erase_hash_set(HashSet* set, void *key)
{
    size_t index = find_bucket(set, key);

    if (index != NOT_FOUND)
    {
        void *last_key = set->keys + (set->k_size * (set->nmemb - 1));
        size_t last_index = find_bucket(set, last_key);

        struct Bucket *bucket_erase = &set->buckets[index];
        struct Bucket *bucket_last = &set->buckets[last_index];

        void *dest = key_from_index(set->keys, set->k_size, bucket_erase->key_index);
        void *src = last_key;

        memcpy(dest, src, set->k_size);

        bucket_last->key_index = bucket_erase->key_index;
        bucket_erase->tombstone = true;
        bucket_erase->key_index = UNSET;
//        bucket_erase->hash = UNSET; TODO: cannot do this because of some weird .hash checks I think
        set->nmemb--;

        float load_factor = ((float)set->nmemb / (float)set->capacity);
        if (load_factor <= LF_LOWER_THRESHOLD && set->capacity > TABLE_MIN)
            resize(set, SHRINK_FACTOR);
    }
}

void clear_hash_set(HashSet *set)
{
    if (set->buckets)
        free(set->buckets);

    if (set->keys)
        free(set->keys);

    set->buckets = NULL;
    set->keys = NULL;

    set->capacity = 0;
    set->nmemb = 0;
}

size_t count_hash_set(HashSet *set, void *key)
{
    return (find_bucket(set, key) != NOT_FOUND) ? 1 : 0;
}
void *find_hash_set(HashSet *set, void *key)
{
    void *value = NULL;

    size_t index = find_bucket(set, key);
    if (index != NOT_FOUND)
        value = key_from_index(set->keys, set->k_size, set->buckets[index].key_index);

    return value;
}
bool contains_hash_set(HashSet* set, void *key)
{
    return (find_bucket(set, key) != NOT_FOUND) ? true : false;
}

bool empty_hash_set(HashSet* set)
{
    return (set->nmemb == 0);
}
size_t size_hash_set(HashSet* set)
{
    return set->nmemb;
}
