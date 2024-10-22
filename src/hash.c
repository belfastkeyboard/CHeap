#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include "../hash.h"

struct Bucket
{
    hash_t hash;
    size_t index;
    bool tombstone;
};
static struct Bucket create_bucket(const hash_t hash, const size_t nmemb, void *keys, const void *key, const size_t k_size, void *values, const void *value, const size_t v_size)
{
    memcpy(keys + (k_size * nmemb), key, k_size);

    if (values)
        memcpy(values + (v_size * nmemb), value, v_size);

    return (struct Bucket){ .hash = hash, .index = nmemb, .tombstone = false };
}

void initialise_buckets(struct Bucket **buckets, size_t *capacity)
{
    if (*buckets)
        free(*buckets);

    *capacity = TABLE_MIN;
    size_t size = *capacity * sizeof(struct Bucket);

    *buckets = malloc(size);
    memset(*buckets, UNSET, size);
}

hash_t djb2(const void *item, const size_t size)
{
    hash_t hash = 5381;

    unsigned char data[size];
    memcpy(data, item, size);

    for (int i = 0; i < size; i++)
        hash = ((hash << 5) + hash) + data[i];

    return hash;
}
size_t get_index(const hash_t hash, const size_t capacity)
{
    return hash % capacity;
}
static void *key_from_index(void *keys, size_t size, size_t index)
{
    return keys + (size * index);
}
static bool is_found(KeyComp k_comp, void *keys, const size_t k_size, struct Bucket bucket, const void *key, bool skip_tombstones)
{
    bool found_bucket = !bucket.tombstone &&
                        bucket.index != UNSET &&
                        k_comp(key, key_from_index(keys, k_size, bucket.index));

    bool invalid_or_tombstone = !skip_tombstones &&
                                (bucket.hash == INVALID || bucket.tombstone); // TODO: think about .hash == INVALID, it's a bit weird

    return found_bucket || invalid_or_tombstone;
}
static size_t probe(KeyComp k_comp, struct Bucket *buckets, void *keys, const size_t k_size, const size_t capacity, const void *key, size_t index, bool skip_tombstones)
{
    assert(buckets);

    size_t found = NOT_FOUND;
    size_t tombstone = INVALID;
    size_t count = 1;

    while (found == NOT_FOUND)
    {
        struct Bucket bucket = buckets[index];

        if (skip_tombstones)
        {
            if (tombstone == INVALID && bucket.hash != INVALID && bucket.tombstone)
                tombstone = index;
            else if (bucket.hash == INVALID)
                break;
        }

        if (is_found(k_comp, keys, k_size, bucket, key, skip_tombstones))
            found = index;
        else
            index = (index + 1) % capacity;

        if (++count > capacity)
            break;
    }

    if (tombstone != INVALID && found != NOT_FOUND)
    {
        buckets[tombstone].tombstone = false;
        buckets[tombstone].index = buckets[found].index;
        buckets[tombstone].hash = buckets[found].hash;

        buckets[found].tombstone = true;
        buckets[found].index = UNSET;

        found = tombstone;
    }

    return found;
}
static bool key_exists(struct Bucket *buckets, size_t index)
{
    return !buckets[index].tombstone && buckets[index].index != UNSET;
}
static size_t find_bucket(KeyComp k_comp, struct Bucket *buckets, void *keys, const size_t capacity, const size_t k_size, void *key)
{
    size_t index = NOT_FOUND;

    if (capacity)
    {
        hash_t hash = djb2(key, k_size);
        index = get_index(hash, capacity);
        index = probe(k_comp, buckets, keys, k_size, capacity, key, index, true);
    }

    return index;
}

static void resize_underlying_data(void **data, const size_t capacity, const size_t size)
{
    void *tmp = realloc(*data, capacity * size);
    assert(tmp);
    *data = tmp;
}
static void reindex_buckets(KeyComp k_comp, struct Bucket *buckets, const struct Bucket *tmp, void *keys, const size_t k_size, const size_t old_capacity, const size_t new_capacity)
{
    for (int i = 0; i < old_capacity; i++)
    {
        struct Bucket bucket = tmp[i];

        if (bucket.hash == INVALID || bucket.tombstone)
            continue;

        size_t index = get_index(bucket.hash, new_capacity);
        index = probe(k_comp, buckets, keys, k_size, new_capacity, key_from_index(keys, k_size, bucket.index), index, false);
        buckets[index] = bucket;
    }
}
static void resize_buckets(KeyComp k_comp, struct Bucket **buckets, void *keys, size_t *capacity, const size_t k_size, float factor)
{
    assert(*buckets);

    size_t old_capacity = *capacity;
    size_t new_capacity = (size_t)((float)old_capacity * factor);
    size_t t_size = old_capacity * sizeof(struct Bucket);
    size_t m_size = new_capacity * sizeof(struct Bucket);

    struct Bucket *tmp = malloc(t_size);
    assert(tmp);
    memcpy(tmp, *buckets, t_size);

    *capacity = new_capacity;

    free(*buckets);
    *buckets = malloc(m_size);
    assert(*buckets);
    memset(*buckets, UNSET, m_size);

    reindex_buckets(k_comp, *buckets, tmp, keys, k_size, old_capacity, new_capacity);

    free(tmp);
}

void hash_insert(KeyComp k_comp, struct Bucket **buckets, void **keys, size_t *capacity, const size_t k_size, size_t *nmemb, void *key)
{
    if (*nmemb == 0)
    {
        initialise_buckets(buckets, capacity);
        resize_underlying_data(keys, *capacity, k_size);
    }
    else
    {
        float load_factor = ((float)*nmemb / (float)*capacity);

        if (load_factor >= LF_UPPER_THRESHOLD)
        {
            resize_buckets(k_comp, buckets, *keys, capacity, k_size, GROW_FACTOR);
            resize_underlying_data(keys, *capacity, k_size);
        }
    }

    assert(buckets && keys);

    hash_t hash = djb2(key, k_size);
    size_t index = get_index(hash, *capacity);
    index = probe(k_comp, *buckets, keys, k_size, *capacity, key, index, false);

    if (!key_exists(*buckets, index))
    {
        (*buckets)[index] = create_bucket(hash, *nmemb, *keys, key, k_size, NULL, NULL, 0);
        (*nmemb)++;
    }
}
void hash_erase(KeyComp k_comp, struct Bucket *buckets, void *keys, size_t *capacity, const size_t k_size, void *key, size_t *nmemb)
{
    size_t index = find_bucket(k_comp, buckets, keys, *capacity, k_size, key);

    if (index != NOT_FOUND)
    {
        void *last_key = keys + (k_size * (*nmemb - 1));
        size_t last_index = find_bucket(k_comp, buckets, keys, *capacity, k_size, last_key);

        struct Bucket *bucket_erase = &buckets[index];
        struct Bucket *bucket_last = &buckets[last_index];

        void *dest = key_from_index(keys, k_size, bucket_erase->index);
        void *src = last_key;

        memcpy(dest, src, k_size);

        bucket_last->index = bucket_erase->index;

        bucket_erase->tombstone = true;
        bucket_erase->index = UNSET;
//        bucket_erase->hash = UNSET; TODO: cannot do this because of some weird .hash checks I think
        (*nmemb)--;

        float load_factor = ((float)*nmemb / (float)*capacity);
        if (load_factor <= LF_LOWER_THRESHOLD && *capacity > TABLE_MIN)
        {
            resize_buckets(k_comp, &buckets, keys, capacity, k_size, SHRINK_FACTOR);
            resize_underlying_data(&keys, *capacity, k_size);
        }
    }
}
void hash_clear(struct Bucket **buckets, void **keys, size_t *capacity, size_t *nmemb)
{
    if (*buckets)
        free(*buckets);

    if (*keys)
        free(*keys);

    *buckets = NULL;
    *keys = NULL;

    *capacity = 0;
    *nmemb = 0;
}

size_t hash_count(KeyComp k_comp, struct Bucket *buckets, void *keys, const size_t capacity, const size_t k_size, void *key)
{
    return (find_bucket(k_comp, buckets, keys, capacity, k_size, key) != NOT_FOUND) ? 1 : 0;
}
void *hash_find(KeyComp k_comp, struct Bucket *buckets, void *keys, const size_t capacity, const size_t k_size, void *key)
{
    void *value = NULL;

    size_t index = find_bucket(k_comp, buckets, keys, capacity, k_size, key);
    if (index != NOT_FOUND)
        value = key_from_index(keys, k_size, buckets[index].index);

    return value;
}
bool hash_contains(KeyComp k_comp, struct Bucket *buckets, void *keys, const size_t capacity, const size_t k_size, void *key)
{
    return (find_bucket(k_comp, buckets, keys, capacity, k_size, key) != NOT_FOUND);
}
