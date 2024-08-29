#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct Stack Stack;

__attribute__((warn_unused_result))
Stack *create_stack(size_t size);
void destroy_stack(Stack *stack);

void push(Stack *stack, void* item);

void *top(Stack *stack);

void pop(Stack *stack);

bool empty(Stack *stack);
size_t size(Stack *stack);

