#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "range.h"


/**
 * @brief A container that provides FIFO (first-in, first-out) functionality
 *
 * The queue uses the same internal double-ended queue implementation as
 * the deque.
 *
 * @warning The Queue object must be constructed and destroyed by the provided functions
 * @note The Queue object is a pointer to an incomplete type and should not be dereferenced
 */
typedef struct Queue Queue;


/**
 * @brief Create a Queue object
 *
 * @param size The size of the type the queue is specialised to contain
 * @return Queue object specialised to hold values of size @p size
 *
 * @warning Must capture the returned object or memory will be leaked
 * @warning Must pass queue to destroy_queue() or memory will be leaked
 * @note Use sizeof() to capture the correct @p size
 */
__attribute__((warn_unused_result))
Queue *create_queue(size_t size);

/**
 * @brief Destroy a Queue object
 *
 * @param queue A pointer to a Queue object
 * @return Nothing
 *
 * @note The Queue object is set to NULL upon successful execution
 */
void destroy_queue(Queue **queue);


/**
 * @brief Push a copy of @p value to the back of the queue
 *
 * @param queue The Queue object
 * @param value The value to push
 * @return Nothing
 *
 * @warning Ensure @p value is of the correct specialised type
 */
void push_queue(Queue *queue,
                const void *value);

/**
 * @brief Push a copy of each element of @p range to the back of the queue
 *
 * @param queue The Queue object
 * @param range The range to push
 * @return Nothing
 */
void push_range_queue(Queue *queue,
                      const Range *range);

/**
 * @brief Returns a pointer to the first element in the queue
 *
 * This element will be the first element to be removed on a call to @c pop_queue()
 *
 * @param queue The Queue object
 * @return Pointer to the element
 *
 * @warning Calling on an empty queue is undefined behaviour
 * @note The return value is of type @c void* and must be cast to the correct type
 */
void *front_queue(const Queue *queue);

/**
 * @brief Returns a pointer to the last element in the queue
 *
 * This is the most recently pushed element
 *
 * @param queue The Queue object
 * @return Pointer to the element
 *
 * @warning Calling on an empty queue is undefined behaviour
 * @note The return value is of type @c void* and must be cast to the correct type
 */
void *back_queue(const Queue *queue);


/**
 * @brief Erases the element at the front of the queue
 *
 * @param queue The Queue object
 * @return Nothing
 *
 * @warning Popping back an empty queue in release mode is undefined behaviour
 */
void pop_queue(Queue *queue);


/**
 * @brief Checks if the queue has no elements
 *
 * @param queue The Queue object
 * @return @c true if the queue is empty, @c false otherwise
 */
bool empty_queue(const Queue *queue);

/**
 * @brief Returns the number of elements in the queue
 *
 * @param queue The Queue object
 * @return Number of elements in the queue
 */
size_t size_queue(const Queue *queue);
