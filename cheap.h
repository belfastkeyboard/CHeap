#pragma once

#include "vector.h"
#include "deque.h"
#include "list.h"
#include "flist.h"
#include "hash_set.h"
#include "hash_table.h"
#include "pqueue.h"
#include "queue.h"
#include "stack.h"

// constructors / destructors

// TODO: possibly use macro to avoid having to call create on hash table with sizeof for value type
#define create(container_type, storage_type, ...) _Generic((container_type), \
    Vector*:    create_vector,                                               \
    Deque*:     create_deque,                                                \
    FList*:     create_forward_list,                                         \
    HashSet*:   create_hash_set,                                             \
    HashTable*: create_hash_table,                                           \
    List*:      create_list,                                                 \
    PQueue*:    create_pqueue,                                               \
    Queue*:     create_queue,                                                \
    Stack*:     create_stack                                                 \
)(sizeof(storage_type), ##__VA_ARGS__)

#define destroy(container) _Generic((container), \
    Vector**:    destroy_vector,                 \
    Deque**:     destroy_deque,                  \
    FList**:     destroy_forward_list,           \
    HashSet**:   destroy_hash_set,               \
    HashTable**: destroy_hash_table,             \
    List**:      destroy_list,                   \
    PQueue**:    destroy_pqueue,                 \
    Queue**:     destroy_queue,                  \
    Stack**:     destroy_stack                   \
)(container)


// sequence containers
#define push_front(container, value) _Generic((container), \
    Deque*: push_front_deque,                              \
    FList*: push_front_forward_list,                       \
    List*:  push_front_list                                \
)(container, value)

#define push_back(container, value) _Generic((container), \
    Vector*: push_back_vector,                            \
    Deque*:  push_back_deque,                             \
    List*:   push_back_list                               \
)(container, value)

#define insert(container, value, ...) _Generic((container), \
    Vector*:    insert_vector,                              \
    Deque*:     insert_deque,                               \
    HashSet*:   insert_hash_set,                            \
    HashTable*: insert_hash_table,                          \
    List*:      insert_list                                 \
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
    Vector*: pop_back_vector,                     \
    Deque*:  pop_back_deque,                      \
    List*:   pop_back_list                        \
)(container)

#define erase(container, index_or_value) _Generic((container), \
    Vector*:    erase_vector,                                  \
    Deque*:     erase_deque,                                   \
    HashSet*:   erase_hash_set,                                \
    HashTable*: erase_hash_table,                              \
    List*:      erase_list                                     \
)(container, index_or_value)

#define erase_after(container, index) _Generic((container), \
    FList*: erase_after_forward_list                        \
)(container, index)

#define clear(container) _Generic((container), \
    Vector*:    clear_vector,                  \
    Deque*:     clear_deque,                   \
    FList*:     clear_forward_list,            \
    HashSet*:   clear_hash_set,                \
    HashTable*: clear_hash_table,              \
    List*:      clear_list                     \
)(container)

#define at(type, container, index) _Generic((container), \
    Vector*: at_vector                                   \
)(container, index)

#define front(type, container) _Generic((container), \
    Vector*: front_vector,                           \
    Deque*:  front_deque,                            \
    FList*:  front_forward_list,                     \
    List*:   front_list,                             \
    Queue*:  front_queue                             \
)(container)

#define back(type, container) _Generic((container), \
    Vector*: back_vector,                           \
    Deque*:  back_deque,                            \
    List*:   back_list,                             \
    Queue*:  back_queue                             \
)(container)


#define empty(container) _Generic((container), \
    Vector*:    empty_vector,                  \
    Deque*:     empty_deque,                   \
    FList*:     empty_forward_list,            \
    HashSet*:   empty_hash_set,                \
    HashTable*: empty_hash_table,              \
    List*:      empty_list,                    \
    PQueue*:    empty_pqueue,                  \
    Queue*:     empty_queue,                   \
    Stack*:     empty_stack                    \
)(container)

#define size(container) _Generic((container), \
    Vector*:    size_vector,                  \
    Deque*:     size_deque,                   \
    FList*:     size_forward_list,            \
    HashSet*:   size_hash_set,                \
    HashTable*: size_hash_table,              \
    List*:      size_list,                    \
    PQueue*:    size_pqueue,                  \
    Queue*:     size_queue,                   \
    Stack*:     size_stack                    \
)(container)

#define capacity(container) _Generic((container), \
    Vector*: capacity_vector                      \
)(container)


#define reserve(container, new_cap) _Generic((container), \
    Vector*: reserve_vector                               \
)(container, new_cap)

#define shrink_to_fit(container) _Generic((container), \
    Vector*: shrink_to_fit_vector                      \
)(container)

// iterator insert

// associative containers

// unordered associative containers
#define count(container, value) _Generic((container), \
    HashSet*:   count_hash_set                        \
    HashTable*: count_hash_table                      \
)(container, value)

#define find(container, value) _Generic((container), \
    HashSet*:   find_hash_set,                       \
    HashTable*: find_hash_table                      \
)(container, value)

#define contains(container, value) _Generic((container), \
    HashSet*:   contains_hash_set                        \
    HashTable*: contains_hash_table                      \
)(container, value)


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
    Stack*:  top_stack                                      \
    PQueue*: top_pqueue,                                    \
)(container)


// extra
#define set_comparator(container, comparator) _Generic((container), \
    PQueue*: set_comparator_pqueue                                  \
)(container, comparator)
