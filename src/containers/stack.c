#include "../../internals/base.h"
#include "../../internals/mpool.h"
#include "../../stack.h"

typedef struct Stack
{
    void *array;
    size_t capacity;
    size_t nmemb;
    size_t size;
} Stack;


Stack *create_stack(const size_t size)
{
    Stack *stack = memory_allocate_container(sizeof(Stack));

    stack->size = size;

    return stack;
}

void destroy_stack(Stack **stack)
{
    memory_free_container_mempool((void **) stack,
                                  (*stack)->array);
}


void push_stack(Stack *stack,
                void* value)
{
    generic_mempool_push_back(&stack->array,
                              value,
                              &stack->capacity,
                              &stack->nmemb,
                              stack->size);
}


void pop_stack(Stack *stack)
{
    generic_mempool_pop_back(&stack->nmemb);
}


void *top_stack(Stack *stack)
{
    return generic_mempool_access_back(stack->array,
                                       stack->nmemb,
                                       stack->size);
}


bool empty_stack(Stack *stack)
{
    return generic_empty(stack->nmemb);
}

size_t size_stack(Stack *stack)
{
    return generic_size(stack->nmemb);
}
