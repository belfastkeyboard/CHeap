#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "internals/comp.h"


/**
 * @brief A sorted associative container that stores key-value pairs with unique keys
 *
 * Keys are sorted using the @p compare function pointer provided during initialisation.
 * Find, erase, and insert operations have logarithmic complexity.
 * The table is implemented as a red-black tree.
 *
 * @warning The Table object must be constructed and destroyed by the provided functions
 * @note The Table object is a pointer to an incomplete type and should not be dereferenced
 */
typedef struct Table Table;


/**
 * @brief Create a Table object
 *
 * @param k_size The size of the key type
 * @param v_size The size of the value type
 * @param compare A function pointer for comparing keys
 * @return Table object specialised for the given key-value pairs
 *
 * @warning Must capture the returned object or memory will be leaked
 * @warning Must pass table to destroy_table() or memory will be leaked
 * @note Use sizeof() to capture the correct @p k_size and @p v_size
 */
__attribute__((warn_unused_result))
Table *create_table(size_t k_size,
                    size_t v_size,
                    KComp compare);

/**
 * @brief Destroy a Table object
 *
 * @param table A pointer to a Table object
 * @return Nothing
 *
 * @note The Table object is set to NULL upon successful execution
 */
void destroy_table(Table **table);


/**
 * @brief Insert a copy of @p value at the specified index
 *
 * @param vector The Vector object
 * @param value The value to insert
 * @param index The index at which to insert @p value
 * @return Nothing
 *
 * @warning Ensure @p value is of the correct specialised type
 * @warning In release mode out-of-bounds @p index errors are not caught
 */
void insert_table(Table *table,
                  const void *key,
                  const void *value);


size_t count_table(const Table *table,
                   const void *key);

void *find_table(const Table *table,
                 const void *key);

bool contains_table(const Table *table,
                    const void *key);


void erase_table(Table *table,
                 const void *key);

void clear_table(Table *table);


bool empty_table(const Table *table);

size_t size_table(const Table *table);
