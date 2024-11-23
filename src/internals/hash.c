#include "../../internals/cassert.h"
#include <malloc.h>
#include <memory.h>
#include "../../internals/hash.h"

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
    size_t index;
    bool tombstone;
};


static hash_t djb2(const void *item,
                   const size_t size)
{
    hash_t hash = 5381;

    unsigned char data[size];

    memcpy(data,
           item,
           size);

    for (int i = 0; i < size; i++)
    {
        hash = ((hash << 5) + hash) + data[i];
    }

    return hash;
}

static size_t get_index(const hash_t hash,
                        const size_t capacity)
{
    return hash % capacity;
}

static void *data_from_index(void *data,
                             const size_t size,
                             const size_t index)
{
    return data + size * index;
}


static bool key_exists(const struct Bucket *buckets,
                       const size_t index)
{
    return !buckets[index].tombstone && buckets[index].index != UNSET;
}

static bool is_found(void *keys,
                     const size_t k_size,
                     const KComp k_comp,
                     const struct Bucket bucket,
                     const void *key,
                     const bool skip_tombstones)
{
    bool found_bucket = !bucket.tombstone &&
                        bucket.index != UNSET &&
                        k_comp(key,
                               data_from_index(keys,
                                               k_size,
                                               bucket.index));

    bool invalid_or_tombstone = !skip_tombstones &&
                                (bucket.hash == INVALID || bucket.tombstone); // TODO: think about .hash == INVALID

    return found_bucket || invalid_or_tombstone;
}


static size_t probe(struct Bucket *buckets,
                    void *keys,
                    const size_t k_size,
                    const KComp k_comp,
                    const size_t capacity,
                    const void *key,
                    size_t index,
                    const bool skip_tombstones)
{
    CHEAP_ASSERT(buckets,
                 "Buckets cannot be NULL.");

    size_t found = NOT_FOUND;
    size_t tombstone = INVALID;
    size_t count = 1;

    while (found == NOT_FOUND)
    {
        struct Bucket bucket = buckets[index];

        if (skip_tombstones)
        {
            if (tombstone == INVALID && bucket.hash != INVALID && bucket.tombstone)
            {
                tombstone = index;
            }
            else if (bucket.hash == INVALID)
            {
                break;
            }
        }

        if (is_found(keys,
                     k_size,
                     k_comp,
                     bucket,
                     key,
                     skip_tombstones))
        {
            found = index;
        }
        else
        {
            index = (index + 1) % capacity;
        }

        if (++count > capacity)
        {
            break;
        }
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

static size_t find_bucket(struct Bucket *buckets,
                          void *keys,
                          const size_t k_size,
                          const KComp k_comp,
                          const size_t capacity,
                          const void *key)
{
    size_t index = NOT_FOUND;

    if (capacity)
    {
        hash_t hash = djb2(key,
                           k_size);

        index = get_index(hash,
                          capacity);

        index = probe(buckets,
                      keys,
                      k_size,
                      k_comp,
                      capacity,
                      key,
                      index,
                      true);
    }

    return index;
}


static struct Bucket create_bucket(const hash_t hash,
                                   const size_t nmemb,
                                   void *keys,
                                   const void *key,
                                   const size_t k_size,
                                   void *values,
                                   const void *value,
                                   const size_t v_size)
{
    memcpy(keys + (k_size * nmemb),
           key,
           k_size);

    if (values)
    {
        memcpy(values + (v_size * nmemb),
               value,
               v_size);
    }

    return (struct Bucket){ .hash = hash, .index = nmemb, .tombstone = false };
}

static void initialise_buckets(struct Bucket **buckets,
                               size_t *capacity)
{
    if (*buckets)
        free(*buckets);

    *capacity = TABLE_MIN;
    const size_t size = *capacity * sizeof(struct Bucket);

    *buckets = malloc(size);

    memset(*buckets,
           UNSET,
           size);
}

static void reindex_buckets(struct Bucket *buckets,
                            void *keys,
                            const struct Bucket *tmp,
                            const size_t k_size,
                            const KComp k_comp,
                            const size_t old_capacity,
                            const size_t new_capacity)
{
    for (int i = 0; i < old_capacity; i++)
    {
        struct Bucket bucket = tmp[i];

        if (bucket.hash == INVALID ||
            bucket.tombstone)
        {
            continue;
        }

        size_t index = get_index(bucket.hash,
                                 new_capacity);

        index = probe(buckets,
                      keys,
                      k_size,
                      k_comp,
                      new_capacity,
                      data_from_index(keys,
                                      k_size,
                                      bucket.index),
                      index,
                      false);

        buckets[index] = bucket;
    }
}


static float get_resize_factor(const size_t nmemb,
                               const size_t capacity,
                               const bool capacity_to_shrink)
{
    float factor = 0;

    if (capacity)
    {
        float load_factor = (float)nmemb / (float)capacity;

        if (load_factor >= LF_UPPER_THRESHOLD)
        {
            factor = GROW_FACTOR;
        }
        else if (load_factor <= LF_LOWER_THRESHOLD && capacity_to_shrink)
        {
            factor = SHRINK_FACTOR;
        }
    }

    return factor;
}

static void resize_buckets(struct Bucket **buckets,
                           void *keys,
                           const size_t k_size,
                           const KComp k_comp,
                           size_t *capacity,
                           const float factor)
{
    CHEAP_ASSERT(*buckets,
                 "Buckets cannot be NULL.");

    size_t old_capacity = *capacity;
    size_t new_capacity = (size_t)((float)old_capacity * factor);
    size_t t_size = old_capacity * sizeof(struct Bucket);
    size_t m_size = new_capacity * sizeof(struct Bucket);

    *capacity = new_capacity;

    struct Bucket *tmp = malloc(t_size);

    CHEAP_ASSERT(tmp,
                 "Failed to allocate memory.");

    memcpy(tmp,
           *buckets,
           t_size);

    free(*buckets);

    *buckets = malloc(m_size);

    CHEAP_ASSERT(*buckets,
                 "Failed to allocate memory,");

    memset(*buckets,
           UNSET,
           m_size);

    reindex_buckets(*buckets,
                    keys,
                    tmp,
                    k_size,
                    k_comp,
                    old_capacity,
                    new_capacity);

    free(tmp);
}

static void resize_underlying_data(void **data,
                                   const size_t size,
                                   const size_t capacity)
{
    const size_t new_size = capacity * size;

    void *tmp = realloc(*data,
                        new_size);

    CHEAP_ASSERT(tmp,
                 "Failed to reallocate memory.");

    *data = tmp;
}

static void should_resize(struct Bucket **buckets,
                          void **keys,
                          void **values,
                          const size_t k_size,
                          const size_t v_size,
                          const KComp k_comp,
                          const size_t nmemb,
                          size_t *capacity)
{
    float resize_factor = get_resize_factor(nmemb,
                                            *capacity,
                                            *capacity > TABLE_MIN);
    bool resize = true;

    if (!*keys)
    {
        initialise_buckets(buckets,
                           capacity);
    }
    else if (resize_factor)
    {
        resize_buckets(buckets,
                       *keys,
                       k_size,
                       k_comp,
                       capacity,
                       resize_factor);
    }
    else
    {
        resize = false;
    }

    if (resize)
    {
        if (k_size)
        {
            resize_underlying_data(keys,
                                   k_size,
                                   *capacity);
        }

        if (v_size)
        {
            resize_underlying_data(values,
                                   v_size,
                                   *capacity);
        }
    }
}


void hash_insert(struct Bucket **buckets,
                 void **keys,
                 void **values,
                 const size_t k_size,
                 const size_t v_size,
                 const KComp k_comp,
                 size_t *nmemb,
                 size_t *capacity,
                 const void *key,
                 const void *value)
{
    should_resize(buckets,
                  keys,
                  values,
                  k_size,
                  v_size,
                  k_comp,
                  *nmemb,
                  capacity);

    CHEAP_ASSERT(buckets && *keys,
                 "Buckets and keys cannot be NULL.");

    hash_t hash = djb2(key,
                       k_size);

    size_t index = get_index(hash,
                             *capacity);

    index = probe(*buckets,
                  *keys,
                  k_size,
                  k_comp,
                  *capacity,
                  key,
                  index,
                  false);

    bool exists = key_exists(*buckets,
                             index);

    if (!exists)
    {
        void *val_tmp = (values) ? *values : NULL;

        (*buckets)[index] = create_bucket(hash,
                                          *nmemb,
                                          *keys,
                                          key,
                                          k_size,
                                          val_tmp,
                                          value,
                                          v_size);

        (*nmemb)++;
    }
    else if (values)
    {
        void *dest = *values + (*buckets)[index].index * v_size;

        memcpy(dest,
               value,
               v_size);
    }
}


void hash_erase(struct Bucket **buckets,
                void **keys,
                void **values,
                const size_t k_size,
                const size_t v_size,
                const KComp k_comp,
                size_t *nmemb,
                size_t *capacity,
                const void *key)
{
    size_t index = find_bucket(*buckets,
                               *keys,
                               k_size,
                               k_comp,
                               *capacity,
                               key);

    if (index != NOT_FOUND)
    {
        void *last_key = *keys + k_size * (*nmemb - 1);

        size_t last_index = find_bucket(*buckets,
                                        *keys,
                                        k_size,
                                        k_comp,
                                        *capacity,
                                        last_key);

        struct Bucket *bucket_erase = &(*buckets)[index];
        struct Bucket *bucket_last = &(*buckets)[last_index];

        void *dest = data_from_index(*keys,
                                     k_size,
                                     bucket_erase->index);

        void *src = last_key;

        memcpy(dest,
               src,
               k_size);

        if (values)
        {
            dest = data_from_index(*values,
                                   v_size,
                                   bucket_erase->index);

            src = *values + v_size * (*nmemb - 1);

            memcpy(dest,
                   src,
                   v_size);
        }

        bucket_last->index = bucket_erase->index;

        bucket_erase->tombstone = true;
        bucket_erase->index = UNSET;
        // bucket_erase->hash = UNSET; TODO: cannot do this because of some weird .hash checks I think

        (*nmemb)--;

        should_resize(buckets,
                      keys,
                      values,
                      k_size,
                      v_size,
                      k_comp,
                      *nmemb,
                      capacity);
    }
}

void hash_clear(struct Bucket **buckets,
                void **keys,
                void **values,
                size_t *nmemb,
                size_t *capacity)
{
    if (*buckets)
    {
        free(*buckets);
        *buckets = NULL;
    }

    if (*keys)
    {
        free(*keys);
        *keys = NULL;
    }

    if (values && *values)
    {
        free(*values);
        *values = NULL;
    }

    *capacity = 0;
    *nmemb = 0;
}


size_t hash_count(struct Bucket *buckets,
                  void *keys,
                  const size_t k_size,
                  const KComp k_comp,
                  const size_t capacity,
                  const void *key)
{
    return (find_bucket(buckets,
                        keys,
                        k_size,
                        k_comp,
                        capacity,
                        key) != NOT_FOUND) ? 1 : 0;
}

void *hash_find(struct Bucket *buckets,
                void *keys,
                void *values,
                const size_t k_size,
                const size_t v_size,
                const KComp k_comp,
                const size_t capacity,
                const size_t nmemb,
                const void *key)
{
    void *value = NULL;

    if (nmemb)
    {
        size_t index = find_bucket(buckets,
                                   keys,
                                   k_size,
                                   k_comp,
                                   capacity,
                                   key);

        void *ptr = (values) ? values : keys;
        const size_t size = (values) ? v_size : k_size;

        if (index != NOT_FOUND)
        {
            struct Bucket bucket = buckets[index];

            value = data_from_index(ptr,
                                    size,
                                    bucket.index);
        }
    }

    return value;
}

bool hash_contains(struct Bucket *buckets,
                   void *keys,
                   const size_t k_size,
                   const KComp k_comp,
                   const size_t capacity,
                   const void *key)
{
    return (find_bucket(buckets,
                        keys,
                        k_size,
                        k_comp,
                        capacity,
                        key) != NOT_FOUND);
}
