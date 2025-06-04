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

#ifdef CHEAP_ITERATOR_AVAILABLE
/**
 * @brief Returns an iterator to the first element of the deque
 *
 * @param deque The Deque object
 * @return Iterator to the first element
 *
 * @warning Calling on an empty deque is undefined behaviour
 */
Iter begin_deque(const Deque *deque);

/**
 * @brief Returns an iterator to the end of the deque
 *
 * @param deque The Deque object
 * @return Sentinel iterator representing the end of the deque
 *
 * @warning Calling on an empty deque is undefined behaviour
 * @note End is a sentinel value and should not be dereferenced
 */
Iter end_deque(const Deque *deque);

/**
 * @brief Returns a reverse iterator to the last element of the deque
 *
 * @param deque The Deque object
 * @return Reverse iterator to the last element
 *
 * @warning Calling on an empty deque is undefined behaviour
 */
Iter rbegin_deque(const Deque *deque);

/**
 * @brief Returns an iterator to the reverse end of the deque
 *
 * @param deque The Deque object
 * @return Sentinel iterator representing the reverse end of the deque
 *
 * @warning Calling on an empty deque is undefined behaviour
 * @note End is a sentinel value and should not be dereferenced
 */
Iter rend_deque(const Deque *deque);
#endif

bool   empty_deque(const Deque *deque);
size_t size_deque(const Deque *deque);
