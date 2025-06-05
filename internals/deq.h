#pragma once

#include "../range.h"
#include <stddef.h>

struct Block
{
	void  *array;
	size_t nmemb;
};

struct ControlArray
{
	struct Block *blocks;
	size_t        block_count;
	size_t        capacity;
};

size_t minimum_array_size(size_t size);

struct ControlArray create_control_array(size_t arr_cap, size_t size);

void destroy_control_array(struct ControlArray *control, size_t front);

void deque_push_front(struct ControlArray *control,
                      size_t              *front,
                      size_t              *back,
                      size_t               arr_cap,
                      const void          *value,
                      size_t               size,
                      size_t              *nmemb);

void deque_push_back(struct ControlArray *control,
                     size_t              *front,
                     size_t              *back,
                     size_t               arr_cap,
                     const void          *value,
                     size_t               size,
                     size_t              *nmemb);

void deque_insert(struct ControlArray *control,
                  size_t              *front,
                  size_t              *back,
                  const void          *value,
                  size_t               arr_cap,
                  size_t               size,
                  size_t              *nmemb,
                  size_t               index);

void deque_push_back_range(struct ControlArray *control,
                           size_t              *front,
                           size_t              *back,
                           size_t               arr_cap,
                           Range                range,
                           size_t               size,
                           size_t              *nmemb);

void deque_push_front_range(struct ControlArray *control,
                            size_t              *front,
                            size_t              *back,
                            size_t               arr_cap,
                            Range                range,
                            size_t               size,
                            size_t              *nmemb);

void deque_insert_range(struct ControlArray *control,
                        size_t              *front,
                        size_t              *back,
                        Range                range,
                        size_t               arr_cap,
                        size_t               size,
                        size_t              *nmemb,
                        size_t               index);

void *deque_at(const struct ControlArray *control,
               size_t                     front,
               size_t                     arr_cap,
               size_t                     nmemb,
               size_t                     size,
               size_t                     index);

void *deque_front(const struct ControlArray *control,
                  size_t                     front,
                  size_t                     arr_cap,
                  size_t                     size,
                  size_t                     nmemb);

void *deque_back(const struct ControlArray *control,
                 size_t                     back,
                 size_t                     size,
                 size_t                     nmemb);

void deque_pop_front(struct ControlArray *control,
                     size_t              *front,
                     size_t              *back,
                     size_t              *nmemb);

void deque_pop_back(struct ControlArray *control,
                    size_t              *front,
                    size_t              *back,
                    size_t              *nmemb,
                    size_t               arr_cap);

size_t deque_erase(struct ControlArray *control,
                   size_t              *front,
                   size_t              *back,
                   size_t              *nmemb,
                   size_t               arr_cap,
                   size_t               index,
                   size_t               size);

void deque_clear(struct ControlArray *control,
                 size_t              *front,
                 size_t              *back,
                 size_t              *nmemb,
                 size_t               arr_cap);
