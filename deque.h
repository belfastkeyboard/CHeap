#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct DoubleEndedQueue DoubleEndedQueue, Deque;

__attribute__((warn_unused_result))
Deque *create_deque(size_t size);
void destroy_deque(Deque **deque);

void push_front_deque(Deque *deque, void *value);
void push_back_deque(Deque *deque, void *value);
void insert_deque(Deque *deque, void *value, size_t index);

void *front_deque(Deque *deque);
void *back_deque(Deque *deque);

void pop_front_deque(Deque *deque);
void pop_back_deque(Deque *deque);
size_t erase_deque(Deque *deque, size_t index);
void clear_deque(Deque *deque);

bool empty_deque(Deque *deque);
size_t size_deque(Deque *deque);
