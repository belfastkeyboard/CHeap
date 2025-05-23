#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "range.h"

/**
 * @brief A container provides LIFO (last-in, first-out) functionality
 *
 * The stack uses the same internal contiguous array implementation as
 * the vector.
 *
 * @warning The Stack object must be constructed and destroyed by the provided functions
 * @note The Stack object is a pointer to an incomplete type and should not be dereferenced
 */
typedef struct Stack Stack;

/**
 * @brief Create a Stack object
 *
 * @param size The size of the type the stack is specialised to contain
 * @return Stack object specialised to hold values of size @p size
 *
 * @warning Must capture the returned object or memory will be leaked
 * @warning Must pass stack to destroy_stack() or memory will be leaked
 * @note Use sizeof() to capture the correct @p size
 */
__attribute__((warn_unused_result))
Stack *create_stack(size_t size);

/**
 * @brief Destroy a Stack object
 *
 * @param stack A pointer to a Stack object
 * @return Nothing
 *
 * @note The Stack object is set to NULL upon successful execution
 */
void destroy_stack(Stack **stack);


/**
 * @brief Push a copy of @p value to the top of the stack
 *
 * @param stack The Stack object
 * @param value The value to push
 * @return Nothing
 *
 * @warning Ensure @p value is of the correct specialised type
 */
void push_stack(Stack *stack,
                const void* value);

/**
 * @brief Push a copy of each element of @p range to the top of the stack
 *
 * @param stack The Stack object
 * @param range The range to push
 * @return Nothing
 */
void push_range_stack(Stack *stack,
                      const Range *range);


/**
 * @brief Returns a pointer to the top element in the stack
 *
 * @param stack The Stack object
 * @return Pointer to the element
 *
 * @warning Calling on an empty stack is undefined behaviour
 * @note The return value is of type @c void* and must be cast to the correct type
 */
void *top_stack(const Stack *stack);


/**
 * @brief Erases the top element from the stack
 *
 * @param stack The Stack object
 * @return Nothing
 *
 * @warning Popping back an empty stack in release mode is undefined behaviour
 */
void pop_stack(Stack *stack);


/**
 * @brief Checks if the stack has no elements
 *
 * @param stack The Stack object
 * @return @c true if the stack is empty, @c false otherwise
 */
bool empty_stack(const Stack *stack);

/**
 * @brief Returns the number of elements in the stack
 *
 * @param stack The Stack object
 * @return Number of elements in the stack
 */
size_t size_stack(const Stack *stack);
