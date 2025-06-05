#include "../../internals/deq.h"
#include "../../deque.h"
#include "../../iter.h"
#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include <stdbool.h>
#include <stddef.h>

#define MIN_BLOCK_SIZE (4 * 1024)
#define MIN_ELEM_COUNT 16

typedef void (*PushFunc)(struct ControlArray *,
                         const void *,
                         size_t *,
                         size_t *,
                         const size_t,
                         const size_t);

static bool has_space_at_front(const size_t front)
{
	return front != 0;
}

static bool has_space_at_back(const size_t back, const size_t capacity)
{
	return back < capacity - 1;
}

static bool is_full(const size_t nmemb, const size_t capacity)
{
	return nmemb >= capacity;
}

static bool is_front(const size_t index, const size_t front)
{
	return index == front;
}

static bool is_back(const size_t index, const size_t back)
{
	return index == back;
}

static size_t locate_block(size_t             *index,
                           const struct Block *blocks,
                           const size_t        front,
                           const size_t        capacity)
{
	size_t result     = front;
	size_t accumulate = blocks[result].nmemb;

	while (accumulate <= *index)
	{
		*index -= accumulate;
		result     = (result + 1) % capacity;
		accumulate = blocks[result].nmemb;
	}

	return result;
}

static void resize_from_single(struct ControlArray *control,
                               size_t              *front,
                               size_t              *back,
                               const size_t         index,
                               const size_t         arr_cap,
                               const size_t         size)
{
	assert(index == 0 || index == 1);

	struct Block *blocks;

	if (control->capacity == 1)
	{
		blocks = malloc(sizeof(struct Block) * 2);
		memcpy(blocks + 1 - index, control->blocks, sizeof(struct Block));
		free(control->blocks);
	}
	else
	{
		blocks            = control->blocks;
		blocks[1 - index] = control->blocks[0];
	}

	struct Block block = { .array = malloc(arr_cap * size), .nmemb = 0 };

	blocks[index] = block;

	control->blocks   = blocks;
	control->capacity = 2;
	control->block_count++;
	*front = 0;
	*back  = 1;
}

static void resize_from_multiple(struct ControlArray *control,
                                 struct Block       **block,
                                 size_t              *front,
                                 size_t              *back,
                                 const size_t         arr_cap,
                                 const size_t         size,
                                 const bool           forward)
{
	if (control->block_count >= control->capacity)
	{
		control->capacity *= 2;

		const size_t f_index = (control->capacity) / 2 -
		                       (control->block_count) / 2;

		const size_t f = f_index;
		const size_t b = f + (control->block_count) - 1;

		struct Block *blocks = calloc(control->capacity, sizeof(struct Block));

		if ((*front) > (*back))
		{
			const size_t span = control->block_count - *front;
			const size_t diff = (forward) ? *front
			                              : control->block_count - span;

			memcpy(blocks + f,
			       control->blocks + (*front),
			       sizeof(struct Block) * span);

			memcpy(blocks + f + span,
			       control->blocks,
			       sizeof(struct Block) * diff);
		}
		else
		{
			memcpy(blocks + f,
			       control->blocks,
			       sizeof(struct Block) * control->block_count);
		}

		free(control->blocks);

		control->blocks = blocks;

		*front = f;
		*back  = b;
	}

	if (forward)
	{
		*front = (*front - 1) % control->capacity;

		*block = control->blocks + *front;
	}
	else
	{
		*back = (*back + 1) % control->capacity;

		*block = control->blocks + *back;
	}

	(*block)->nmemb = 0;
	(*block)->array = malloc(arr_cap * size);

	control->block_count++;
}

static void push_front_single(struct ControlArray *control,
                              const void          *value,
                              size_t              *front,
                              size_t              *back,
                              const size_t         arr_cap,
                              const size_t         size)
{
	void *dest;

	if (!has_space_at_front(*front))
	{
		resize_from_single(control, front, back, 0, arr_cap, size);

		dest = control->blocks[0].array + (arr_cap - 1) * size;
	}
	else
	{
		(*front)--;

		dest = control->blocks[0].array + ((*front) * size);
	}

	memcpy(dest, value, size);

	control->blocks[0].nmemb++;
}

static void push_front_multiple(struct ControlArray *control,
                                const void          *value,
                                size_t              *front,
                                size_t              *back,
                                const size_t         arr_cap,
                                const size_t         size)
{
	struct Block *block = &control->blocks[*front];

	if (is_full(block->nmemb, arr_cap))
	{
		resize_from_multiple(control, &block, front, back, arr_cap, size, true);
	}

	block->nmemb++;

	memcpy(block->array + (arr_cap - block->nmemb) * size, value, size);
}

static void push_back_single(struct ControlArray *control,
                             const void          *value,
                             size_t              *front,
                             size_t              *back,
                             const size_t         arr_cap,
                             const size_t         size)
{
	void  *dest;
	size_t i;

	if (!has_space_at_back(*back, arr_cap))
	{
		resize_from_single(control, front, back, 1, arr_cap, size);

		dest = control->blocks[1].array;
		i    = 1;
	}
	else
	{
		(*back)++;

		dest = control->blocks[0].array + *back * size;
		i    = 0;
	}

	memcpy(dest, value, size);

	control->blocks[i].nmemb++;
}

static void push_back_multiple(struct ControlArray *control,
                               const void          *value,
                               size_t              *front,
                               size_t              *back,
                               const size_t         arr_cap,
                               const size_t         size)
{
	struct Block *block = &control->blocks[*back];

	if (is_full(block->nmemb, arr_cap))
	{
		resize_from_multiple(control,
		                     &block,
		                     front,
		                     back,
		                     arr_cap,
		                     size,
		                     false);
	}

	memcpy(block->array + block->nmemb * size, value, size);

	block->nmemb++;
}

static void generic_deque_push(struct ControlArray *control,
                               size_t              *front,
                               size_t              *back,
                               const size_t         arr_cap,
                               const void          *value,
                               const size_t         size,
                               size_t              *nmemb,
                               PushFunc             push_single,
                               PushFunc             push_multiple)
{
	if (control->block_count == 1)
	{
		push_single(control, value, front, back, arr_cap, size);
	}
	else
	{
		push_multiple(control, value, front, back, arr_cap, size);
	}

	(*nmemb)++;
}

static void insert_back(struct ControlArray *control,
                        const void          *value,
                        size_t              *front,
                        size_t              *back,
                        const size_t         arr_cap,
                        const size_t         index,
                        const size_t         b_index,
                        const size_t         size)
{
	struct Block *block = control->blocks + b_index;

	if (is_full(block->nmemb, arr_cap))
	{
		void *hold = block->array + ((block->nmemb - 1) * size);

		insert_back(control,
		            hold,
		            front,
		            back,
		            arr_cap,
		            0,
		            (b_index + 1) % control->capacity,
		            size);

		memmove(block->array + (index + 1) * size,
		        block->array + index * size,
		        (block->nmemb - index - 1) * size);

		memcpy(block->array + index * size, value, size);
	}
	else
	{
		void        *dest = block->array + (index + 1) * size;
		void        *src  = block->array + index * size;
		const size_t n    = (block->nmemb - index) * size;

		memmove(dest, src, n);

		memcpy(block->array + index * size, value, size);

		block->nmemb++;
	}
}

static void insert_front(struct ControlArray *control,
                         const void          *value,
                         size_t              *front,
                         size_t              *back,
                         const size_t         arr_cap,
                         size_t               index,
                         const size_t         b_index,
                         const size_t         size)
{
	struct Block *block = control->blocks + b_index;

	if (is_full(block->nmemb, arr_cap))
	{
		void *hold = block->array;

		const size_t prev = (b_index - 1) % control->capacity;

		insert_front(control,
		             hold,
		             front,
		             back,
		             arr_cap,
		             control->blocks[prev].nmemb - 1,
		             prev,
		             size);

		memmove(block->array + (arr_cap - block->nmemb) * size,
		        block->array + (arr_cap - block->nmemb + 1) * size,
		        index * size);

		memcpy(block->array + index * size, value, size);
	}
	else
	{
		memmove(block->array + (arr_cap - block->nmemb - 1) * size,
		        block->array + (arr_cap - block->nmemb) * size,
		        index * size);

		memcpy(block->array + (arr_cap - block->nmemb + index - 1) * size,
		       value,
		       size);

		block->nmemb++;
	}
}

static void insert_single(struct ControlArray *control,
                          size_t              *front,
                          size_t              *back,
                          const size_t         arr_cap,
                          const size_t         size,
                          const void          *value,
                          size_t               index,
                          size_t              *nmemb)
{
	struct Block block = control->blocks[0];

	const size_t f   = *front;
	const size_t b   = *back;
	const size_t mid = (f + b) / 2;

	index += f;

	if (index < mid)
	{
		if (!has_space_at_front(f))
		{
			resize_from_single(control, front, back, 0, arr_cap, size);

			memcpy(control->blocks[0].array + (arr_cap - 1) * size,
			       control->blocks[1].array,
			       size);

			control->blocks[0].nmemb++;

			memmove(control->blocks[1].array,
			        control->blocks[1].array + 1 * size,
			        (index - 1) * size);

			memcpy(control->blocks[1].array + index * size, value, size);
		}
		else
		{
			(*front)--;

			control->blocks[0].nmemb++;

			void        *dest = block.array + (f - 1) * size;
			const void  *src  = block.array + f * size;
			const size_t n    = (index - f) * size;

			memmove(dest, src, n);

			memcpy(block.array + (index - 1) * size, value, size);
		}

		(*nmemb)++;
	}
	else
	{
		if (!has_space_at_back(b, arr_cap))
		{
			resize_from_single(control, front, back, 1, arr_cap, size);

			memcpy(control->blocks[1].array,
			       block.array + (block.nmemb - 1) * size,
			       size);

			control->blocks[1].nmemb++;
		}
		else
		{
			(*back)++;

			control->blocks[0].nmemb++;
		}

		memmove(block.array + (index + 1) * size,
		        block.array + index * size,
		        (b - index + 1) * size);

		memcpy(block.array + index * size, value, size);

		(*nmemb)++;
	}
}

static void insert_multiple(struct ControlArray *control,
                            const void          *value,
                            size_t               index,
                            size_t              *front,
                            size_t              *back,
                            const size_t         arr_cap,
                            const size_t         size,
                            size_t              *nmemb)
{
	const size_t mid     = *nmemb / 2;
	size_t       n_index = index;

	if (index > mid)
	{
		struct Block *back_block = control->blocks + *back;

		if (is_full(back_block->nmemb, arr_cap))
		{
			void *last_elem = back_block->array +
			                  (back_block->nmemb - 1) * size;

			resize_from_multiple(control,
			                     &back_block,
			                     front,
			                     back,
			                     arr_cap,
			                     size,
			                     false);

			memcpy(back_block->array, last_elem, size);

			back_block->nmemb++;
		}

		const size_t b_index = locate_block(&n_index,
		                                    control->blocks,
		                                    *front,
		                                    control->capacity);

		insert_back(control,
		            value,
		            front,
		            back,
		            arr_cap,
		            n_index,
		            b_index,
		            size);
	}
	else
	{
		struct Block *front_block = control->blocks + *front;

		if (is_full(front_block->nmemb, arr_cap))
		{
			void *first_elem = front_block->array;

			resize_from_multiple(control,
			                     &front_block,
			                     front,
			                     back,
			                     arr_cap,
			                     size,
			                     true);

			memcpy(front_block->array +
			           (arr_cap - front_block->nmemb - 1) * size,
			       first_elem,
			       size);

			front_block->nmemb++;
		}

		const size_t b_index = locate_block(&n_index,
		                                    control->blocks,
		                                    *front,
		                                    control->capacity);

		insert_front(control,
		             value,
		             front,
		             back,
		             arr_cap,
		             n_index,
		             b_index,
		             size);
	}

	(*nmemb)++;
}

static void shift_up(struct ControlArray *control,
                     const size_t         arr_cap,
                     const size_t         size,
                     size_t              *front,
                     size_t              *back,
                     size_t               index,
                     size_t               b_index)
{
	index += arr_cap - control->blocks[b_index].nmemb;

	while (1)
	{
		struct Block *block = &control->blocks[b_index];

		void  *dest = block->array + (arr_cap - block->nmemb + 1) * size;
		void  *src  = block->array + (arr_cap - block->nmemb) * size;
		size_t n    = (index - (arr_cap - block->nmemb)) * size;

		memmove(dest, src, n);

		if (is_front(b_index, *front))
		{
			block->nmemb--;

			if (!block->nmemb)
			{
				free(block->array);
				block->array = NULL;

				control->block_count--;

				if (control->block_count == 1)
				{
					control->blocks[0]           = control->blocks[*back];
					control->blocks[*back].array = NULL;

					*back  = control->blocks[0].nmemb - 1;
					*front = 0;
				}
				else
				{
					*front = ((*front) + 1) % control->capacity;
				}
			}

			break;
		}

		b_index = (b_index - 1) % control->capacity;
		index   = arr_cap - 1;

		dest = block->array;
		src  = control->blocks[b_index].array + index * size;
		n    = size;

		memcpy(dest, src, n);
	}
}

static void shift_down(struct ControlArray *control,
                       const size_t         arr_cap,
                       const size_t         size,
                       size_t              *front,
                       size_t              *back,
                       size_t               index,
                       size_t               b_index)
{
	while (1)
	{
		struct Block *block = &control->blocks[b_index];

		if (index < arr_cap - 1)
		{
			memmove(block->array + index * size,
			        block->array + (index + 1) * size,
			        (block->nmemb - index - 1) * size);
		}

		if (is_back(b_index, *back))
		{
			block->nmemb--;

			if (!block->nmemb)
			{
				free(block->array);
				block->array = NULL;

				control->block_count--;

				if (control->block_count == 1)
				{
					control->blocks[0]            = control->blocks[*front];
					control->blocks[*front].array = NULL;

					*back  = arr_cap - 1;
					*front = (*back) - (control->blocks[0].nmemb - 1);
				}
				else
				{
					*back = ((*back) - 1) % control->capacity;
				}
			}

			break;
		}

		const size_t n_index = (b_index + 1) % control->capacity;

		void  *dest = block->array + (arr_cap - 1) * size;
		void  *src  = control->blocks[n_index].array;
		size_t n    = size;

		memcpy(dest, src, n);

		b_index = n_index;
		index   = 0;
	}
}

static void erase_single(struct ControlArray *control,
                         size_t              *front,
                         size_t              *back,
                         const size_t         size,
                         const size_t         index)
{
	struct Block *block = control->blocks;

	const size_t f   = *front;
	const size_t b   = *back;
	const size_t mid = (f + b) / 2;

	const size_t n_index = index + f;

	void  *dest;
	void  *src;
	size_t n;

	if (n_index < mid)
	{
		dest = block->array + (f + 1) * size;
		src  = block->array + f * size;
		n    = (n_index - f) * size;

		(*front)++;
	}
	else
	{
		dest = block->array + n_index * size;
		src  = block->array + (n_index + 1) * size;
		n    = (block->nmemb - index - 1) * size;

		(*back)--;
	}

	memmove(dest, src, n);

	block->nmemb--;
}

static void erase_multiple(struct ControlArray *control,
                           size_t              *front,
                           size_t              *back,
                           const size_t         arr_cap,
                           const size_t         nmemb,
                           const size_t         size,
                           const size_t         index)
{
	size_t n_index = index;

	const size_t b_index = locate_block(&n_index,
	                                    control->blocks,
	                                    *front,
	                                    control->capacity);

	const size_t mid = nmemb / 2;

	if (index < mid)
	{
		shift_up(control, arr_cap, size, front, back, n_index, b_index);
	}
	else
	{
		shift_down(control, arr_cap, size, front, back, n_index, b_index);
	}
}

static void *deque_random_access_one_block(const struct ControlArray *control,
                                           const size_t               front,
                                           const size_t               size,
                                           size_t                     index)
{
	const struct Block block = control->blocks[0];
	return block.array + (front + index) * size;
}

static void *deque_random_access_multi_block(const struct ControlArray *control,
                                             const size_t               front,
                                             const size_t               arr_cap,
                                             const size_t               size,
                                             size_t                     index)
{
	const size_t b_index = locate_block(&index,
	                                    control->blocks,
	                                    front,
	                                    control->capacity);

	if (is_front(b_index, front))
	{
		const struct Block block = control->blocks[b_index];
		return block.array + (arr_cap - block.nmemb + index) * size;
	}
	else
	{
		return control->blocks[b_index].array + index * size;
	}
}

size_t minimum_array_size(const size_t size)
{
	return (MIN_BLOCK_SIZE > size * MIN_ELEM_COUNT) ? MIN_BLOCK_SIZE
	                                                : size * MIN_ELEM_COUNT;
}

struct ControlArray create_control_array(const size_t arr_cap,
                                         const size_t size)
{
	struct ControlArray control = { .blocks      = malloc(sizeof(struct Block)),
		                            .block_count = 1,
		                            .capacity    = 1 };

	struct Block block = { .array = malloc(arr_cap * size), .nmemb = 0 };

	control.blocks[0] = block;

	return control;
}

void destroy_control_array(struct ControlArray *control, const size_t front)
{
	if (control->block_count == 1)
	{
		free(control->blocks[0].array);
	}
	else
	{
		for (size_t i = front, j = 0; j < control->block_count;
		     i = (i + 1) % control->capacity, j++)
		{
			free(control->blocks[i].array);
		}
	}

	free(control->blocks);
}

void deque_push_front(struct ControlArray *control,
                      size_t              *front,
                      size_t              *back,
                      const size_t         arr_cap,
                      const void          *value,
                      const size_t         size,
                      size_t              *nmemb)
{
	generic_deque_push(control,
	                   front,
	                   back,
	                   arr_cap,
	                   value,
	                   size,
	                   nmemb,
	                   push_front_single,
	                   push_front_multiple);
}

void deque_push_back(struct ControlArray *control,
                     size_t              *front,
                     size_t              *back,
                     const size_t         arr_cap,
                     const void          *value,
                     const size_t         size,
                     size_t              *nmemb)
{
	generic_deque_push(control,
	                   front,
	                   back,
	                   arr_cap,
	                   value,
	                   size,
	                   nmemb,
	                   push_back_single,
	                   push_back_multiple);
}

void deque_insert(struct ControlArray *control,
                  size_t              *front,
                  size_t              *back,
                  const void          *value,
                  const size_t         arr_cap,
                  const size_t         size,
                  size_t              *nmemb,
                  size_t               index)
{
	assert(index <= *nmemb);

	if (!index)
	{
		deque_push_front(control, front, back, arr_cap, value, size, nmemb);
	}
	else if (index == *nmemb)
	{
		deque_push_back(control, front, back, arr_cap, value, size, nmemb);
	}
	else
	{
		if (control->block_count == 1)
		{
			insert_single(control,
			              front,
			              back,
			              arr_cap,
			              size,
			              value,
			              index,
			              nmemb);
		}
		else
		{
			insert_multiple(control,
			                value,
			                index,
			                front,
			                back,
			                arr_cap,
			                size,
			                nmemb);
		}
	}
}

void deque_push_back_range(struct ControlArray *control,
                           size_t              *front,
                           size_t              *back,
                           size_t               arr_cap,
                           Range                range,
                           size_t               size,
                           size_t              *nmemb)
{
	bool forward            = range.begin.type < ITERATOR_REVERSE;
	bool (*done)(Range)     = (forward) ? done_range : done_range_r;
	void (*iterate)(Iter *) = (forward) ? next_iter : prev_iter;

	for (; !done(range); iterate(&range.begin))
	{
		void *value = get_range(range);

		generic_deque_push(control,
		                   front,
		                   back,
		                   arr_cap,
		                   value,
		                   size,
		                   nmemb,
		                   push_back_single,
		                   push_back_multiple);
	}
}

void deque_push_front_range(struct ControlArray *control,
                            size_t              *front,
                            size_t              *back,
                            size_t               arr_cap,
                            Range                range,
                            size_t               size,
                            size_t              *nmemb)
{
	bool forward            = range.begin.type < ITERATOR_REVERSE;
	bool (*done)(Range)     = (forward) ? done_range : done_range_r;
	void (*iterate)(Iter *) = (forward) ? next_iter : prev_iter;

	for (; !done(range); iterate(&range.begin))
	{
		void *value = get_range(range);

		generic_deque_push(control,
		                   front,
		                   back,
		                   arr_cap,
		                   value,
		                   size,
		                   nmemb,
		                   push_front_single,
		                   push_front_multiple);
	}
}

void deque_insert_range(struct ControlArray *control,
                        size_t              *front,
                        size_t              *back,
                        Range                range,
                        size_t               arr_cap,
                        size_t               size,
                        size_t              *nmemb,
                        size_t               index)
{
	bool forward            = range.begin.type < ITERATOR_REVERSE;
	bool (*done)(Range)     = (forward) ? done_range : done_range_r;
	void (*iterate)(Iter *) = (forward) ? next_iter : prev_iter;

	for (; !done(range); iterate(&range.begin))
	{
		void *value = get_range(range);
		deque_insert(control, front, back, value, arr_cap, size, nmemb, index);
		index++;
	}
}

void *deque_at(const struct ControlArray *control,
               const size_t               front,
               const size_t               arr_cap,
               const size_t               nmemb,
               const size_t               size,
               size_t                     index)
{
	assert(index < nmemb);

	void *result = NULL;

	if (control->block_count == 1)
	{
		result = deque_random_access_one_block(control, front, size, index);
	}
	else
	{
		result = deque_random_access_multi_block(control,
		                                         front,
		                                         arr_cap,
		                                         size,
		                                         index);
	}

	return result;
}

void *deque_front(const struct ControlArray *control,
                  const size_t               front,
                  const size_t               arr_cap,
                  const size_t               size,
                  const size_t               nmemb)
{
	assert(nmemb);

	void *result = NULL;

	if (control->block_count == 1)
	{
		result = control->blocks[0].array + (front * size);
	}
	else
	{
		struct Block block = control->blocks[front];

		result = block.array + (arr_cap - block.nmemb) * size;
	}

	return result;
}

void *deque_back(const struct ControlArray *control,
                 const size_t               back,
                 const size_t               size,
                 const size_t               nmemb)
{
	assert(nmemb);

	void *result = NULL;

	if (control->block_count == 1)
	{
		result = control->blocks[0].array + (back * size);
	}
	else
	{
		struct Block block = control->blocks[back];

		result = block.array + (block.nmemb - 1) * size;
	}

	return result;
}

void deque_pop_front(struct ControlArray *control,
                     size_t              *front,
                     size_t              *back,
                     size_t              *nmemb)
{
	assert(*nmemb);

	if (control->block_count == 1)
	{
		(*nmemb)--;
		control->blocks[0].nmemb--;
		(*front)++;
	}
	else
	{
		struct Block *front_block = &control->blocks[*front];

		front_block->nmemb--;
		(*nmemb)--;

		if (!(front_block->nmemb))
		{
			free(front_block->array);
			front_block->array = NULL;

			control->block_count--;

			if (control->block_count == 1)
			{
				control->blocks[0]           = control->blocks[*back];
				control->blocks[*back].array = NULL;

				*back  = control->blocks[0].nmemb - 1;
				*front = 0;
			}
			else
			{
				*front = ((*front) + 1) % control->capacity;
			}
		}
	}
}

void deque_pop_back(struct ControlArray *control,
                    size_t              *front,
                    size_t              *back,
                    size_t              *nmemb,
                    const size_t         arr_cap)
{
	assert(*nmemb);

	if (control->block_count == 1)
	{
		(*nmemb)--;
		control->blocks[0].nmemb--;
		(*back)--;
	}
	else
	{
		struct Block *back_block = &control->blocks[*back];

		back_block->nmemb--;
		(*nmemb)--;

		if (!(back_block->nmemb))
		{
			free(back_block->array);
			back_block->array = NULL;

			control->block_count--;

			if (control->block_count == 1)
			{
				control->blocks[0]            = control->blocks[*front];
				control->blocks[*front].array = NULL;

				*back  = arr_cap - 1;
				*front = *back - (control->blocks[0].nmemb - 1);
			}
			else
			{
				*back = (*back - 1) % control->capacity;
			}
		}
	}
}

size_t deque_erase(struct ControlArray *control,
                   size_t              *front,
                   size_t              *back,
                   size_t              *nmemb,
                   const size_t         arr_cap,
                   const size_t         index,
                   const size_t         size)
{
	assert(index < *nmemb);

	if (!index)
	{
		deque_pop_front(control, front, back, nmemb);
	}
	else if (index == *nmemb)
	{
		deque_pop_back(control, front, back, nmemb, arr_cap);
	}
	else
	{
		if (control->block_count == 1)
		{
			erase_single(control, front, back, size, index);
		}
		else
		{
			erase_multiple(control, front, back, arr_cap, *nmemb, size, index);
		}

		(*nmemb)--;
	}

	return index;
}

void deque_clear(struct ControlArray *control,
                 size_t              *front,
                 size_t              *back,
                 size_t              *nmemb,
                 const size_t         arr_cap)
{
	if (control->block_count > 1)
	{
		for (size_t i = *front + 1, j = 1; j < control->block_count;
		     i = (i + 1) % control->capacity, j++)
		{
			struct Block *block = &control->blocks[i];

			if (block->array)
			{
				free(block->array);
				block->array = NULL;
			}
		}

		struct Block *move = &control->blocks[*front];

		control->blocks[0] = *move;
		move->array        = NULL;
	}

	const size_t init_index = arr_cap / 2;

	*front = init_index;
	*back  = init_index - 1;

	control->blocks[0].nmemb = 0;
	control->block_count     = 1;
	*nmemb                   = 0;
}

void next_deque(Iter *iter)
{
	iter->data.deque.index++;
}

void prev_deque(Iter *iter)
{
	iter->data.deque.index--;
}

void *get_deque(const Iter iter)
{
	const Deque *deque = iter.data.deque.deque;
	const size_t index = iter.data.deque.index;

	return at_deque(deque, index);
}
