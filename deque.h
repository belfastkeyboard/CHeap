#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct DoubleEndedQueue Deque;

__attribute__((warn_unused_result))
Deque *create_deque(size_t size);
void destroy_deque(Deque *deque);

void push_front(Deque *deque, void* item);
void push_back(Deque *deque, void* item);
void insert(Deque *deque, void *item, size_t index);

void *front(Deque *deque);
void *back(Deque *deque);

void pop_front(Deque *deque);
void pop_back(Deque *deque);
size_t erase(Deque *deque, size_t index);
void clear(Deque *deque);

bool empty(Deque *deque);
size_t size(Deque *deque);