#include "../../rbuff.h"
#include "../../internals/base.h"
#include "../../internals/cassert.h"
#include <memory.h>

typedef struct RingBuffer
{
	void  *array;
	size_t capacity;
	size_t size;
	size_t write;
	size_t read;
} RingBuffer;

RingBuffer *create_ringbuffer(const size_t capacity, const size_t size)
{
	RingBuffer *rbuff = memory_allocate_container(sizeof(RingBuffer));

	CHEAP_ASSERT(rbuff, "Failed to allocate memory.");

	rbuff->array    = malloc(capacity * size);
	rbuff->capacity = capacity;
	rbuff->size     = size;
	rbuff->read     = 0;
	rbuff->write    = 0;

	CHEAP_ASSERT(rbuff->array, "Failed to allocate memory.");

	return rbuff;
}

void destroy_ringbuffer(RingBuffer **rbuff)
{
	memory_free_container_generic((void **)rbuff, (*rbuff)->array);
}

void *read_ringbuffer(RingBuffer *rbuff)
{
	void *item = rbuff->array + rbuff->read;

	if (rbuff->read >= (rbuff->capacity - 1) * rbuff->size)
	{
		rbuff->read = 0;
	}
	else
	{
		rbuff->read += rbuff->size;
	}

	return item;
}

void write_ringbuffer(RingBuffer *rbuff, const void *item)
{
	memcpy(rbuff->array + rbuff->write, item, rbuff->size);

	if (rbuff->write >= (rbuff->capacity - 1) * rbuff->size)
	{
		rbuff->write = 0;
	}
	else
	{
		rbuff->write += rbuff->size;
	}
}

void clear_ringbuffer(RingBuffer *rbuff)
{
	rbuff->read  = 0;
	rbuff->write = 0;
}

bool empty_ringbuffer(const RingBuffer *rbuff)
{
	return rbuff->read == rbuff->write;
}

bool full_ringbuffer(const RingBuffer *rbuff)
{
	return (rbuff->write + rbuff->size) % (rbuff->capacity * rbuff->size) ==
	       rbuff->read;
}
