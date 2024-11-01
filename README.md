# C Heap
Library for simplifying memory management in C
General idea behind the library is to reduce the number of malloc/free calls to the absolute minimum.
The trade-off of this design choice is that usually memory is traded for performance.
For example, some of the containers (linked list types) use bump allocators to group memory into related pools.
The lists perform better than traditional implementations but allocate more memory. 

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
- iterators?
- thread safety?
- fix bugs in RB Tree
- alternate arena type for containers that has a freeblock list?
- support for custom allocators 
- stress test all types

## CONSIDER:
- C++ implementation of Priority Queue uses a binary heap which is faster than the current array/qsort implementation
- attempt to re-write create macro to accept hash table without having to write 'sizeof'
- separate linked list pages into array(node) + array(data) for even faster iteration?
- all the sequential containers have code duplication in the struct that could be reduced to the base.h file

