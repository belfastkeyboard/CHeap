#include <assert.h>
#include <malloc.h>
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
    Stack *stack = memory_allocate_container(sizeof(Stack));

    SEQ_CONTAINER_INIT(stack, size);

    return stack;
}
void destroy_stack(Stack *stack)
{
    assert(stack);

    memory_free_buffer(stack->array);
    memory_free_container((void**)&stack);
}

void push(Stack *stack, void* item)
{
    if (stack->nmemb >= stack->capacity)
        stack->array = sequential_resize(stack->array, &stack->capacity, stack->size);

    sequential_insert(stack->array, stack->nmemb, item, stack->nmemb, stack->size);
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
