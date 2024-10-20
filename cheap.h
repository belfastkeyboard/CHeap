#pragma once

#include "array.h"
#include "deque.h"
#include "list.h"
#include "flist.h"
#include "hash_set.h"
#include "pqueue.h"
#include "queue.h"
#include "stack.h"

// constructors / destructors
#define create(container_type, storage_type, ...) _Generic((container_type), \
    Array*:   create_array,                                                  \
    Deque*:   create_deque,                                                  \
    FList*:   create_forward_list,                                           \
    HashSet*: create_hash_set,                                               \
    List*:    create_list,                                                   \
    PQueue*:  create_pqueue,                                                 \
    Queue*:   create_queue,                                                  \
    Stack*:   create_stack                                                   \
)(sizeof(storage_type), ##__VA_ARGS__)
#define destroy(container) _Generic((container), \
    Array**:   destroy_array,                    \
    Deque**:   destroy_deque,                    \
    FList**:   destroy_forward_list,             \
    HashSet**: destroy_hash_set,                 \
    List**:    destroy_list,                     \
    PQueue**:  destroy_pqueue,                   \
    Queue**:   destroy_queue,                    \
    Stack**:   destroy_stack                     \
)(container)

// sequence containers
#define push_front(container, value) _Generic((container), \
    Deque*: push_front_deque,                              \
    FList*: push_front_forward_list,                       \
    List*:  push_front_list                                \
)(container, value)
#define push_back(container, value) _Generic((container), \
    Array*: push_back_array,                              \
    Deque*: push_back_deque,                              \
    List*:  push_back_list                                \
)(container, value)
#define insert(container, value, ...) _Generic((container), \
    Array*:   insert_array,                                 \
    Deque*:   insert_deque,                                 \
    HashSet*: insert_hash_set,                              \
    List*:    insert_list                                   \
)(container, value, ##__VA_ARGS__)
#define insert_after(container, value, index) _Generic((container), \
    FList*: insert_after_forward_list                               \
)(container, value, index)

#define pop_front(container) _Generic((container), \
    Deque*: pop_front_deque,                       \
    FList*: pop_front_forward_list,                \
    List*:  pop_front_list                         \
)(container)
#define pop_back(container) _Generic((container), \
    Array*: pop_back_array,                       \
    Deque*: pop_back_deque,                       \
    List*:  pop_back_list                         \
)(container)
#define erase(container, index_or_value) _Generic((container), \
    Array*:   erase_array,                            \
    Deque*:   erase_deque,                            \
    HashSet*: erase_hash_set                          \
)(container, index_or_value)
#define erase_after(container, index) _Generic((container), \
    FList*: erase_after_forward_list                               \
)(container, index)
#define clear(container) _Generic((container), \
    Array*:   clear_array,                     \
    Deque*:   clear_deque,                     \
    FList*:   clear_forward_list,              \
    HashSet*: clear_hash_set                   \
)(container)

#define at(type, container, index) *(type*) _Generic((container), \
    Array*: at_array                                              \
)(container, index)
#define front(type, container) *(type*) _Generic((container), \
    Array*:  front_array,                                     \
    Deque*:  front_deque,                                     \
    FList*:  front_forward_list,                              \
    List*:   front_list,                                      \
    PQueue*: front_pqueue,                                    \
    Queue*:  front_queue                                      \
)(container)
#define back(type, container) *(type*) _Generic((container), \
    Array*: back_array,                                      \
    Deque*: back_deque,                                      \
    Queue*: back_queue                                       \
)(container)

#define empty(container) _Generic((container), \
    Array*:   empty_array,                     \
    Deque*:   empty_deque,                     \
    FList*:   empty_forward_list,              \
    HashSet*: empty_hash_set,                  \
    List*:    empty_list,                      \
    PQueue*:  empty_pqueue,                    \
    Queue*:   empty_queue,                     \
    Stack*:   empty_stack                      \
)(container)
#define size(container) _Generic((container), \
    Array*:   size_array,                     \
    Deque*:   size_deque,                     \
    FList*:   size_forward_list,              \
    HashSet*: size_hash_set,                  \
    List*:    size_list,                      \
    PQueue*:  size_pqueue,                    \
    Queue*:   size_queue,                     \
    Stack*:   size_stack                      \
)(container)
#define capacity(container) _Generic((container), \
    Array*: capacity_array                        \
)(container)

#define count(container, value) _Generic((container), \
    HashSet*: count_hash_set                          \
)(container, value)
#define find(container, value) _Generic((container), \
    HashSet*: find_hash_set                          \
)(container, value)
#define contains(container, value) _Generic((container), \
    HashSet*: contains_hash_set                          \
)(container, value)

#define reserve(container, new_cap) _Generic((container), \
    Array*: reserve_array                                 \
)(container, new_cap)
#define shrink_to_fit(container) _Generic((container), \
    Array*: shrink_to_fit_array                        \
)(container)

// iterator insert

// associative containers

// unordered associative containers

// container adaptors
#define push(container, value) _Generic((container), \
    PQueue*: push_pqueue,                            \
    Queue*:  push_queue,                             \
    Stack*:  push_stack                              \
)(container, value)

#define pop(container) _Generic((container), \
    PQueue*: pop_pqueue,                     \
    Queue*:  pop_queue,                      \
    Stack*:  pop_stack                       \
)(container)

#define top(type, container) *(type*) _Generic((container), \
    Stack*: top_stack                                       \
)(container)

// extra
#define set_comparator(container, comparator) _Generic((container), \
    PQueue*: set_comparator_pqueue                                  \
)(container, comparator)
