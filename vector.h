#pragma once

#include <stdbool.h>
#include <stddef.h>

#ifdef CHEAP_ITERATOR_AVAILABLE
#include "iter.h"
#endif

#ifdef CHEAP_RANGE_AVAILABLE
#include "range.h"
#endif


typedef struct Vector Vector;

/**
 * @brief Create a Vector object
 *
 * @param size The size of the type the vector is specialised to contain
 * @return Vector object specialised to hold values of size @p size
 *
 * @warning Must capture the returned object or memory will be leaked
 * @warning Must pass vector to destroy_vector() or memory will be leaked
 * @note Use sizeof() to capture the correct @p size
 * @note Vector object is a pointer to an incomplete type and cannot be dereferenced
 */
__attribute__((warn_unused_result))
Vector *create_vector(size_t size);

/**
 * @brief Destroy a Vector object
 *
 * @param vector A pointer to a Vector object
 * @return Nothing
 *
 * @note The Vector object is set to NULL upon successful execution
 */
void destroy_vector(Vector **vector);


/**
 * @brief Appends a copy of @p value to the end of the vector
 *
 * @param vector The Vector object
 * @param value The value to append
 * @return Nothing
 *
 * @warning Ensure @p value is of the correct specialised type
 */
void push_back_vector(Vector *vector,
                      const void *value);

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
void insert_vector(Vector *vector,
                   const void *value,
                   size_t index);


#ifdef CHEAP_RANGE_AVAILABLE
/**
 * @brief Appends a copy of @p range at the specified index
 *
 * @param vector The Vector object
 * @param range The range to append
 * @return Nothing
 */
void push_back_range_vector(Vector *vector,
                            const Range *range);

/**
 * @brief Inserts a copy of @p range to the end of the vector
 *
 * @param vector The Vector object
 * @param index The index at which to insert @p range
 * @param range The range to insert
 * @return Nothing
 */
void insert_range_vector(Vector *vector,
                         size_t index,
                         const Range *range);

/**
 * @brief Get a slice of the Vector as a Range object
 *
 * @param vector The Vector object
 * @param begin The index at which to begin, inclusive
 * @param range The index at which to end, exclusive
 * @return Nothing
 */
Range get_range_vector(const Vector *vector,
                       size_t begin,
                       size_t end);
#endif

void pop_back_vector(Vector *vector);

size_t erase_vector(Vector *vector,
                    size_t index);

void clear_vector(Vector *vector);


void *at_vector(const Vector *vector,
                size_t index);

void *front_vector(const Vector *vector);

void *back_vector(const Vector *vector);


#ifdef CHEAP_ITERATOR_AVAILABLE
Iter begin_vector(const Vector *vector);

Iter end_vector(const Vector *vector);
#endif


bool empty_vector(const Vector *vector);

size_t size_vector(const Vector *vector);

size_t capacity_vector(const Vector *vector);


void reserve_vector(Vector *vector,
                    size_t amount);

void shrink_to_fit_vector(Vector *vector);
