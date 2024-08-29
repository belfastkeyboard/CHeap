#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include "../base.h"
#include "../stack.h"

typedef struct Stack
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
} Stack;

Stack *create_stack(size_t size)
{
    Stack *stack = malloc(sizeof(Stack));

    stack->capacity = 0;
    stack->nmemb = 0;
    stack->size = size;

    stack->array = malloc(stack->capacity * size);
    memset(stack->array, 0, stack->capacity * size);

    return stack;
}
void destroy_stack(Stack **stack)
{
    if ((*stack)->array)
        free((*stack)->array);

    free(*stack);
    *stack = NULL;
}

void push(Stack *stack, void* item)
{
    if (stack->nmemb >= stack->capacity)
        stack->array = sequential_resize(stack->array, &stack->capacity, stack->size);

    memcpy(stack->array + (stack->nmemb * stack->size), item, stack->size);
    stack->nmemb++;
}
void *top(Stack *stack)
{
    return sequential_index_access(stack->array, stack->nmemb - 1, stack->size);
}

void pop(Stack *stack)
{
    assert(stack->nmemb);

    stack->nmemb--;
}

bool empty(Stack *stack)
{
    return container_empty(stack->nmemb);
}
size_t size(Stack *stack)
{
    return container_size(stack->nmemb);
}

