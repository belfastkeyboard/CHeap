#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct RingBuffer RingBuffer;


__attribute__((warn_unused_result))
RingBuffer *create_ringbuffer(size_t capacity,
                              size_t size);

void destroy_ringbuffer(RingBuffer **rbuff);


// call !empty() before read() to ensure no value is read twice
void *read_ringbuffer(RingBuffer *rbuff);

// call !full() before write() to ensure no data is overwritten before being read
void write_ringbuffer(RingBuffer *rbuff,
           const void *item);

void clear_ringbuffer(RingBuffer *rbuff);


bool empty_ringbuffer(const RingBuffer *rbuff);

bool full_ringbuffer(const RingBuffer *rbuff);
