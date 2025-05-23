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


__attribute__((warn_unused_result))
Stack *create_stack(size_t size);

void destroy_stack(Stack **stack);


void push_stack(Stack *stack,
                const void* value);

void push_range_stack(Stack *stack,
                      const Range *range);


void *top_stack(const Stack *stack);


void pop_stack(Stack *stack);


bool empty_stack(const Stack *stack);

size_t size_stack(const Stack *stack);
