#include <memory.h>
#include "../../internals/cassert.h"
#include "../../internals/nalloc.h"
#include "../../internals/hash.h"


#define GROW_FACTOR        2.0f
#define SHRINK_FACTOR      0.5f
#define LF_UPPER_THRESHOLD 0.75f
#define LF_LOWER_THRESHOLD 0.1f

#define UNSET     (-1)
#define INVALID   UNSET
#define NOT_FOUND INVALID

struct Bucket
{
    Hash hash;
    void *key;
    void *value;
    bool tombstone;
};


Hash djb2s(const void *item,
           const size_t size)
{
    Hash hash = 5381;
    unsigned char *data = *(unsigned char**)item;

    while (*data)
    {
        hash = ((hash << 5) + hash) + (*data);
        data++;
    }

    return hash;
}

Hash djb2(const void *item,
          const size_t size)
{
    Hash hash = 5381;

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


static size_t get_index(const Hash hash,
                        const size_t capacity)
{
    return hash % capacity;
}

static bool key_exists(const struct Bucket *buckets,
                       const size_t index)
{
    return !buckets[index].tombstone && buckets->key != (void*)UNSET;
}

static bool is_found(const KComp k_comp,
                     const struct Bucket bucket,
                     const void *key,
                     const bool skip_tombstones)
{
    bool found_bucket = !bucket.tombstone &&
                        bucket.key != (void*)UNSET &&
                        k_comp(key,
                               bucket.key);

    bool invalid_or_tombstone = !skip_tombstones &&
                                (bucket.hash == INVALID || // TODO: think about .hash == INVALID
                                bucket.tombstone);         //   from the future: why?

    return found_bucket ||
           invalid_or_tombstone;
}


static size_t probe(struct Bucket *buckets,
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
            if (tombstone == INVALID &&
                bucket.hash != INVALID &&
                bucket.tombstone)
            {
                tombstone = index;
            }
            else if (bucket.hash == INVALID)
            {
                break;
            }
        }

        if (is_found(k_comp,
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
        buckets[tombstone].key = buckets[found].key;
        buckets[tombstone].value = buckets[found].value;
        buckets[tombstone].hash = buckets[found].hash;

        buckets[found].tombstone = true;
        buckets[found].key = (void*)UNSET;

        found = tombstone;
    }

    return found;
}

static size_t find_bucket(struct Bucket *buckets,
                          const HashFnc fnc,
                          const size_t k_size,
                          const KComp k_comp,
                          const size_t capacity,
                          const void *key)
{
    size_t index = NOT_FOUND;

    if (capacity)
    {
        Hash hash = fnc(key,
                        k_size);

        index = get_index(hash,
                          capacity);

        index = probe(buckets,
                      k_comp,
                      capacity,
                      key,
                      index,
                      true);
    }

    return index;
}


static struct Bucket create_bucket(const Hash hash,
                                   struct NodeAlloc *alloc,
                                   const void *key,
                                   const size_t k_size,
                                   const void *value,
                                   const size_t v_size)
{
    void *memory = alloc_node(alloc);

    void *k = memory;
    void *v = (value) ? memory + k_size :
                        NULL;

    memcpy(k,
           key,
           k_size);

    if (value)
    {
        memcpy(v,
               value,
               v_size);
    }

    struct Bucket bucket = {
        .hash = hash,
        .key = k,
        .value = v,
        .tombstone = false
    };

    return bucket;
}

static void initialise_buckets(struct Bucket **buckets,
                               size_t *capacity)
{
    if (*buckets)
    {
        free(*buckets);
    }

    *capacity = TABLE_MIN;
    const size_t size = *capacity * sizeof(struct Bucket);

    *buckets = malloc(size);

    memset(*buckets,
           UNSET,
           size);
}

static void reindex_buckets(struct Bucket *buckets,
                            const struct Bucket *tmp,
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
                      k_comp,
                      new_capacity,
                      bucket.key,
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
                           KComp k_comp,
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
                    tmp,
                    k_comp,
                    old_capacity,
                    new_capacity);

    free(tmp);
}


static void should_resize(struct Bucket **buckets,
                          const KComp k_comp,
                          const size_t nmemb,
                          size_t *capacity)
{
    float resize_factor = get_resize_factor(nmemb,
                                            *capacity,
                                            *capacity > TABLE_MIN);

    if (!*buckets)
    {
        initialise_buckets(buckets,
                           capacity);
    }
    else if (resize_factor)
    {
        resize_buckets(buckets,
                       k_comp,
                       capacity,
                       resize_factor);
    }
}


void hash_insert(struct Bucket **buckets,
                 struct NodeAlloc *alloc,
                 const HashFnc fnc,
                 const size_t k_size,
                 const size_t v_size,
                 const KComp k_comp,
                 size_t *nmemb,
                 size_t *capacity,
                 const void *key,
                 const void *value)
{
    should_resize(buckets,
                  k_comp,
                  *nmemb,
                  capacity);

    CHEAP_ASSERT(buckets,
                 "Buckets cannot be NULL.");

    Hash hash = fnc(key,
                    k_size);

    size_t index = get_index(hash,
                             *capacity);

    index = probe(*buckets,
                  k_comp,
                  *capacity,
                  key,
                  index,
                  false);

    bool exists = key_exists(*buckets,
                             index);

    if (!exists)
    {
        (*buckets)[index] = create_bucket(hash,
                                          alloc,
                                          key,
                                          k_size,
                                          value,
                                          v_size);

        (*nmemb)++;
    }
    else
    {
        memcpy((*buckets)[index].value,
               value,
               v_size);
    }
}


void hash_erase(struct Bucket **buckets,
                const HashFnc fnc,
                const size_t k_size,
                const KComp k_comp,
                size_t *nmemb,
                size_t *capacity,
                const void *key)
{
    size_t index = find_bucket(*buckets,
                               fnc,
                               k_size,
                               k_comp,
                               *capacity,
                               key);

    if (index != NOT_FOUND)
    {
        struct Bucket *bucket_erase = &(*buckets)[index];
        bucket_erase->tombstone = true;

        (*nmemb)--;

        should_resize(buckets,
                      k_comp,
                      *nmemb,
                      capacity);
    }
}

void hash_clear(struct Bucket **buckets,
                struct NodeAlloc *alloc,
                size_t *nmemb,
                size_t *capacity)
{
    if (*buckets)
    {
        free(*buckets);
        *buckets = NULL;
    }

    clear_nodes(alloc);

    *capacity = 0;
    *nmemb = 0;
}


size_t hash_count(struct Bucket *buckets,
                  const HashFnc fnc,
                  const size_t k_size,
                  const KComp k_comp,
                  const size_t capacity,
                  const void *key)
{
    return (find_bucket(buckets,
                        fnc,
                        k_size,
                        k_comp,
                        capacity,
                        key) != NOT_FOUND) ? 1 : 0;
}

void *hash_find(struct Bucket *buckets,
                const HashFnc fnc,
                const size_t k_size,
                const KComp k_comp,
                const size_t capacity,
                const size_t nmemb,
                const void *key)
{
    void *value = NULL;

    if (nmemb)
    {
        size_t index = find_bucket(buckets,
                                   fnc,
                                   k_size,
                                   k_comp,
                                   capacity,
                                   key);

        if (index != NOT_FOUND)
        {
            struct Bucket bucket = buckets[index];

            value = (bucket.value) ? bucket.value :
                                     buckets->key;
        }
    }

    return value;
}

bool hash_contains(struct Bucket *buckets,
                   HashFnc fnc,
                   const size_t k_size,
                   const KComp k_comp,
                   const size_t capacity,
                   const void *key)
{
    return (find_bucket(buckets,
                        fnc,
                        k_size,
                        k_comp,
                        capacity,
                        key) != NOT_FOUND);
}
