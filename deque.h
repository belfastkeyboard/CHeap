#pragma once

#include <stdbool.h>
#include <stddef.h>

#define ALLOC __attribute__((warn_unused_result))

typedef struct DoubleEndedQueue DoubleEndedQueue, Deque;

ALLOC Deque *create_deque(size_t size);
void         destroy_deque(Deque **deque);

void push_front_deque(Deque *deque, const void *value);
void push_back_deque(Deque *deque, const void *value);
void insert_deque(Deque *deque, const void *value, size_t index);

void *at_deque(const Deque *deque, size_t index);
void *front_deque(const Deque *deque);
void *back_deque(const Deque *deque);

void   pop_front_deque(Deque *deque);
void   pop_back_deque(Deque *deque);
size_t erase_deque(Deque *deque, size_t index);
void   clear_deque(Deque *deque);

bool   empty_deque(const Deque *deque);
size_t size_deque(const Deque *deque);
