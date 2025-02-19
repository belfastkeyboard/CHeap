#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "range.h"

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
