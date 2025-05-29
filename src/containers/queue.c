#include "../../queue.h"
#include "../../internals/base.h"
#include "../../internals/deq.h"
#include "../../range.h"

typedef struct Queue
{
	struct ControlArray control;
	size_t              nmemb;
	size_t              arr_cap;
	size_t              size;
	size_t              front;
	size_t              back;
} Queue;

Queue *create_queue(const size_t size)
{
	Queue *queue = memory_allocate_container(sizeof(Queue));

	const size_t array_size = minimum_array_size(size);
	const size_t capacity = array_size / size;
	const size_t init_index = capacity / 2;

	queue->control = create_control_array(capacity, size);
	queue->arr_cap = capacity;
	queue->size = size;
	queue->front = init_index;
	queue->back = init_index - 1;

	return queue;
}

void destroy_queue(Queue **queue)
{
	destroy_control_array(&(*queue)->control, (*queue)->front);
	memory_free_buffer((void **)queue);
}

void push_queue(Queue *queue, const void *value)
{
	deque_push_back(&queue->control,
	                &queue->front,
	                &queue->back,
	                queue->arr_cap,
	                value,
	                queue->size,
	                &queue->nmemb);
}

void push_range_queue(Queue *queue, const Range *range)
{
	for (int i = 0; i < range->nmemb; i++) {
		deque_push_back(&queue->control,
		                &queue->front,
		                &queue->back,
		                queue->arr_cap,
		                range->array + i * range->size,
		                queue->size,
		                &queue->nmemb);
	}
}

void *front_queue(const Queue *queue)
{
	return deque_front(&queue->control,
	                   queue->front,
	                   queue->arr_cap,
	                   queue->size,
	                   queue->nmemb);
}

void *back_queue(const Queue *queue)
{
	return deque_back(&queue->control, queue->back, queue->size, queue->nmemb);
}

void pop_queue(Queue *queue)
{
	deque_pop_front(&queue->control,
	                &queue->front,
	                &queue->back,
	                &queue->nmemb);
}

bool empty_queue(const Queue *queue)
{
	return generic_empty(queue->nmemb);
}

size_t size_queue(const Queue *queue)
{
	return generic_size(queue->nmemb);
}
