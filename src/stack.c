#include <stdlib.h>
#include <memory.h>
#include "../stack.h"

typedef struct Stack
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
} Stack;

static void resize(Stack *stack)
{
    stack->capacity = (stack->capacity > 0) ? stack->capacity * 2 : 1;

    void *tmp = realloc(stack->array, stack->size * stack->capacity);
    if (tmp)
        stack->array = tmp;
}

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
        resize(stack);

    memcpy(stack->array + (stack->nmemb * stack->size), item, stack->size);
    stack->nmemb++;
}
void *top(Stack *stack)
{
    return (stack->nmemb) ? stack->array + (stack->nmemb - 1) * stack->size : NULL;
}

void pop(Stack *stack)
{
    if (stack->nmemb)
        stack->nmemb--;
}

bool empty(Stack *stack)
{
    return stack->nmemb == 0;
}
size_t size(Stack *stack)
{
    return stack->nmemb;
}

