#include "../../stack.h"
#include "../../internals/base.h"
#include "../../internals/mpool.h"

typedef struct Stack
{
	void  *array;
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
	memory_free_container_generic((void **)stack, (*stack)->array);
}

void push_stack(Stack *stack, const void *value)
{
	generic_mempool_push_back(&stack->array,
	                          value,
	                          &stack->capacity,
	                          &stack->nmemb,
	                          stack->size);
}

void push_range_stack(Stack *stack, Range range)
{
	generic_mempool_range_insert(&stack->array,
	                             stack->nmemb,
	                             &stack->capacity,
	                             &stack->nmemb,
	                             stack->size,
	                             range);
}

void pop_stack(Stack *stack)
{
	generic_mempool_pop_back(&stack->nmemb);
}

void *top_stack(const Stack *stack)
{
	return generic_mempool_access_back(stack->array, stack->nmemb, stack->size);
}

bool empty_stack(const Stack *stack)
{
	return generic_empty(stack->nmemb);
}

size_t size_stack(const Stack *stack)
{
	return generic_size(stack->nmemb);
}
