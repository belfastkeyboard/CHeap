# C Heap
Library for all my memory-related functions 

- Arena
  - Static
  - Dynamic
- String Builder
- Hash Set
- Hash Table
- Ring Buffer
- Span
- String View

## CONTAINER LIBRARY

### SEQUENCE CONTAINERS
- Array: dynamic contiguous array
- Deque: double-ended queue
- ForwardList: singly-linked list
- List: doubly-linked list

### CONTAINER ADAPTORS
- PriorityQueue: adapts a container to provide priority queue
- Queue: adapts a container to provide queue (FIFO data structure)
- Stack: adapts a container to provide stack (LIFO data structure)

## TODO:
- add inline to helper functions
- add error handling to mimic C++ bad_alloc exception?
- re-hash or store hashes in buckets?
- pull arena out of lists and implement bespoke version (separation of concerns)
- iterators?
- thread safety?
- fix bugs in RB Tree
- alternate arena type for containers that has a freeblock list?
- support for custom allocators 

## CONSIDER:
- C++ implementation of Priority Queue uses a binary heap which is faster than the current array/qsort implementation
- attempt to re-write create macro
