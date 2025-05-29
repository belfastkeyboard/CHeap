/**
 * @file set.h
 * @brief Set implementation for C
 *
 * This header defines a sorted associative @c Set
 * which stores unique keys. The sort comparison
 * function is provided by the user and the set supports
 * standard operations such as @c insert @c erase @c find and @c clear .
 *
 * @author Riain Ó Tuathail
 * @date 2025-05-23
 * @version 0.8
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

/**
 * @brief A sorted associative container that stores unique keys
 *
 * Keys are sorted using the @p compare function pointer provided during
 * initialisation. Find, erase, and insert operations have logarithmic
 * complexity. The set is implemented as a red-black tree.
 *
 * @warning The Set object must be constructed and destroyed by the provided
 * functions
 * @note The Set object is a pointer to an incomplete type and should not be
 * dereferenced
 */
typedef struct Set Set;

/**
 * @brief A comparison function for sorting keys
 *
 * @param a The first key
 * @param b The second key
 *
 * @return An integer less than, equal to, or greater than zero depending on the
 * comparison.
 */
typedef int (*KComp)(const void *, const void *);

/**
 * @brief Create a Set object
 *
 * @param size The size of the key type
 * @param compare A function pointer for comparing keys
 * @return Set object specialised for the given key type
 *
 * @warning Must capture the returned object or memory will be leaked
 * @warning Must pass set to destroy_set() or memory will be leaked
 * @note Use sizeof() to capture the correct @p size
 */
ALLOC Set *create_set(size_t size, KComp compare);

/**
 * @brief Destroy a Set object
 *
 * @param set A pointer to a Set object
 * @return Nothing
 *
 * @note The Set object is set to NULL upon successful execution
 */
void destroy_set(Set **set);

/**
 * @brief Insert a @p key into the set
 *
 * @param set The Set object
 * @param key The key to insert
 * @return Nothing
 *
 * @warning Ensure @p key is of the correct specialised type
 */
void insert_set(Set *set, const void *key);

/**
 * @brief Count the number of @c keys in the set
 *
 * @param set The Set object
 * @param key Key to count
 * @return The count of @c keys in the set
 *
 * @note Because keys are unique the result is always either 1 or 0
 */
size_t count_set(const Set *set, const void *key);

/**
 * @brief Finds a @p key in the set
 *
 * @param set The Set object
 * @param key Key to find
 * @return Pointer to @p key if found, @c NULL otherwise
 *
 * @note The result is of @c void* type and must be cast to the correct type
 */
void *find_set(const Set *set, const void *key);

/**
 * @brief Check if there is a @p key in the set
 *
 * @param set The Set object
 * @param key Key to check
 * @return @c true if found, @c false otherwise
 */
bool contains_set(const Set *set, const void *key);

/**
 * @brief Erases the @p key from the set
 *
 * @param set The Set object
 * @param key The key to erase
 * @return Nothing
 *
 * @note If no @p key is found the function does nothing
 */
void erase_set(Set *set, const void *key);

/**
 * @brief Erases all keys from the set
 *
 * @param set The Set object
 * @return Nothing
 */
void clear_set(Set *set);

/**
 * @brief Checks if the set has no keys
 *
 * @param set The Set object
 * @return @c true if the set is empty, @c false otherwise
 */
bool empty_set(const Set *set);

/**
 * @brief Returns the number of keys in the set
 *
 * @param set The Set object
 * @return Number of keys in the set
 */
size_t size_set(const Set *set);
