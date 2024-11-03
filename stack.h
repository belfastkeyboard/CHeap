#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct Stack Stack;


__attribute__((warn_unused_result))
Stack *create_stack(size_t size);

void destroy_stack(Stack **stack);


void push_stack(Stack *stack,
                void* value);


void *top_stack(Stack *stack);


void pop_stack(Stack *stack);


bool empty_stack(Stack *stack);

size_t size_stack(Stack *stack);
