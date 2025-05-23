#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "internals/comp.h"


/**
 * @brief A sorted associative container that stores unique keys
 *
 * Keys are sorted using the @p compare function pointer provided during initialisation.
 * Find, erase, and insert operations have logarithmic complexity.
 * The set is implemented as a red-black tree.
 *
 * @warning The Set object must be constructed and destroyed by the provided functions
 * @note The Set object is a pointer to an incomplete type and should not be dereferenced
 */
typedef struct Set Set;

Set *create_set(size_t size,
                KComp compare);

void destroy_set(Set **set);


void insert_set(Set *set,
                const void *key);


size_t count_set(const Set *set,
                 const void *key);

void *find_set(const Set *set,
               const void *key);

bool contains_set(const Set *set,
                  const void *key);


void erase_set(Set *set,
               const void *key);

void clear_set(Set *set);


bool empty_set(const Set *set);

size_t size_set(const Set *set);
