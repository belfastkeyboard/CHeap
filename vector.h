/**
 * @file vector.h
 * @brief Vector implementation for C
 *
 * This header defines a generic dynamic array type @c Vector which can store an
 * arbitrary number of elements. It resizes automatically when new elements are
 * added and supports standard operations such as @c push @c pop @c insert and
 * @c erase .
 *
 * @author Riain Ó Tuathail
 * @date 2025-05-21
 * @version 0.8
 */

#pragma once

#ifndef CHEAP_VECTOR_AVAILABLE
#define CHEAP_VECTOR_AVAILABLE
#endif

#include <stdbool.h>
#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

#ifdef CHEAP_ITERATOR_AVAILABLE
#include "iter.h"
#endif

#ifdef CHEAP_RANGE_AVAILABLE
#include "range.h"
#endif

/**
 * @brief A container that encapsulates a dynamic size array
 *
 * Elements in a vector are stored contiguously, and the
 * storage is handled automatically by the provided
 * functions.
 *
 * A vector will usually occupy more space than a static
 * array, because the vector allocates memory up front.
 * The resize policy is to double the current @c capacity
 * of the vector each time the available memory is exhausted.
 *
 * @warning The Vector object must be constructed and destroyed by the provided
 * functions
 * @note The Vector object is a pointer to an incomplete type and should not be
 * dereferenced
 */
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
 */
ALLOC Vector *create_vector(size_t size);

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
void push_back_vector(Vector *vector, const void *value);

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
void insert_vector(Vector *vector, const void *value, size_t index);

#ifdef CHEAP_RANGE_AVAILABLE
/**
 * @brief Appends a copy of @p range at the specified index
 *
 * @param vector The Vector object
 * @param range The range to append
 * @return Nothing
 */
void push_back_range_vector(Vector *vector, const Range *range);

/**
 * @brief Inserts a copy of @p range to the end of the vector
 *
 * @param vector The Vector object
 * @param index The index at which to insert @p range
 * @param range The range to insert
 * @return Nothing
 */
void insert_range_vector(Vector *vector, size_t index, const Range *range);

/**
 * @brief Get a slice of the Vector as a Range object
 *
 * @param vector The Vector object
 * @param begin The index at which to begin, inclusive
 * @param range The index at which to end, exclusive
 * @return A Range object
 *
 * @note Range objects are intended to be read-only
 */
Range get_range_vector(const Vector *vector, size_t begin, size_t end);
#endif

/**
 * @brief Removes the last element of the vector
 *
 * @param vector The Vector object
 * @return Nothing
 *
 * @warning Popping back an empty vector in release mode is undefined behaviour
 */
void pop_back_vector(Vector *vector);

/**
 * @brief Erases the element at the specified index from the vector
 *
 * @param vector The Vector object
 * @param index The index at which to erase
 * @return Nothing
 *
 * @warning In release mode out-of-bounds @p index errors are not caught
 */
void erase_vector(Vector *vector, size_t index);

/**
 * @brief Erases all elements from the vector
 *
 * @param vector The Vector object
 * @return Nothing
 */
void clear_vector(Vector *vector);

/**
 * @brief Returns a pointer to the element at specified index
 *
 * @param vector The Vector object
 * @param index Index from which to get the pointer
 * @return Pointer to the element
 *
 * @warning Out-of-bounds @p index errors are not caught
 * @note The return value is of type @c void* and must be cast to the correct
 * type
 */
void *at_vector(const Vector *vector, size_t index);

/**
 * @brief Returns a pointer to the first element in the vector
 *
 * @param vector The Vector object
 * @return Pointer to the element
 *
 * @warning Calling on an empty vector is undefined behaviour
 * @note The return value is of type @c void* and must be cast to the correct
 * type
 */
void *front_vector(const Vector *vector);

/**
 * @brief Returns a pointer to the last element in the vector
 *
 * @param vector The Vector object
 * @return Pointer to the element
 *
 * @warning Calling on an empty vector is undefined behaviour
 * @note The return value is of type @c void* and must be cast to the correct
 * type
 */
void *back_vector(const Vector *vector);

#ifdef CHEAP_ITERATOR_AVAILABLE
/**
 * @brief Returns an iterator to the first element of the vector
 *
 * @param vector The Vector object
 * @return Iterator to the first element
 *
 * @warning Calling on an empty vector is undefined behaviour
 */
Iter begin_vector(const Vector *vector);

/**
 * @brief Returns an iterator to the last element of the vector
 *
 * @param vector The Vector object
 * @return Iterator to the last element
 *
 * @warning Calling on an empty vector is undefined behaviour
 */
Iter end_vector(const Vector *vector);
#endif

/**
 * @brief Checks if the vector has no elements
 *
 * @param vector The Vector object
 * @return @c true if the vector is empty, @c false otherwise
 */
bool empty_vector(const Vector *vector);

/**
 * @brief Returns the number of elements in the vector
 *
 * @param vector The Vector object
 * @return Number of elements in the vector
 */
size_t size_vector(const Vector *vector);

/**
 * @brief Returns the number of elements that the vector has currently allocated
 * space for
 *
 * @param vector The Vector object
 * @return Capacity of the currently allocated storage
 */
size_t capacity_vector(const Vector *vector);

/**
 * @brief Increase the capacity of the vector to the specified size
 *
 * @param vector The Vector object
 * @param amount The number of elements to reserve
 * @return Nothing
 *
 * @note If @p amount is less than the currently allocated capacity, the
 * function does nothing
 */
void reserve_vector(Vector *vector, size_t amount);

/**
 * @brief Removes unused capacity
 *
 * @param vector The Vector object
 * @return Nothing
 */
void shrink_to_fit_vector(Vector *vector);
