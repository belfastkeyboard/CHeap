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
 * @brief Insert a @p key @p value pair into the table
 *
 * @param table The Table object
 * @param key The key to insert
 * @param value The value to insert
 * @return Nothing
 *
 * @warning Ensure @p key and @p value are of the correct specialised types
 * @note If key already exists, the function is does not overwrite the existing value
 */
void insert_table(Table *table,
                  const void *key,
                  const void *value);


/**
 * @brief Count the number of @p key @c value pairs in the table with an equivalent @p key
 *
 * @param table The Table object
 * @param key Key from which to count the resulting stored elements
 * @return The count of @p key @c value pairs in the table
 *
 * @note Because keys are unique the result is always either 1 or 0
 */
size_t count_table(const Table *table,
                   const void *key);

/**
 * @brief Finds a @c value with key equivalent to @p key
 *
 * @param table The Table object
 * @param key Key from which to derive the @c value
 * @return Pointer to the @c value if found, @c NULL otherwise
 *
 * @note The result is of @c void* type and must be cast to the correct type
 */
void *find_table(const Table *table,
                 const void *key);

/**
 * @brief Check if there is a @c value with @p key equivalent in the table
 *
 * @param table The Table object
 * @param key Key from which to find the @c value
 * @return @c true if found, @c false otherwise
 */
bool contains_table(const Table *table,
                    const void *key);

/**
 * @brief Erases the @p key @c value pair from the table
 *
 * @param table The Table object
 * @param key The key to erase the equivalent @p key @c value pair
 * @return Nothing
 *
 * @note If no @p key @c value pair is found the function does nothing
 */
void erase_table(Table *table,
                 const void *key);

/**
 * @brief Erases all pairs from the table
 *
 * @param table The Table object
 * @return Nothing
 */
void clear_table(Table *table);

/**
 * @brief Checks if the table has no pairs
 *
 * @param table The Table object
 * @return @c true if the table is empty, @c false otherwise
 */
bool empty_table(const Table *table);

/**
 * @brief Returns the number of pairs in the table
 *
 * @param table The Table object
 * @return Number of elements in the table
 */
size_t size_table(const Table *table);
