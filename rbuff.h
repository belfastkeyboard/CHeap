#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define ALLOC __attribute__((warn_unused_result))

typedef struct RingBuffer RingBuffer;

ALLOC RingBuffer *create_ringbuffer(size_t capacity, size_t size);
void destroy_ringbuffer(RingBuffer **rbuff);

void *read_ringbuffer(RingBuffer *rbuff);
void write_ringbuffer(RingBuffer *rbuff, const void *item);

void clear_ringbuffer(RingBuffer *rbuff);

bool empty_ringbuffer(const RingBuffer *rbuff);
bool full_ringbuffer(const RingBuffer *rbuff);
