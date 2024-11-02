# C Heap
Library for simplifying memory management in C.

The idea behind the library is to reduce the number of malloc/free calls to the absolute minimum.
The trade-off of this design choice is that usually memory is traded for performance.
For example, some of the containers (linked list types) use bump allocators to group memory into related pools.
The lists perform better than traditional implementations but allocate more memory. 

- Arena
- Bump Allocator
- Ring Buffer
- Span
- String

## CONTAINER LIBRARY

### Sequence containers
Sequence containers implement data structures which can be accessed sequentially.
- Array: dynamic contiguous array
- Deque: double-ended queue
- ForwardList: singly-linked list
- List: doubly-linked list

### Unordered associative containers
Unordered associative containers implement unsorted (hashed) data structures that can be quickly searched (O(1) average, O(n) worst-case complexity).
- Hash Set
- Hash Table

### Container adaptors
Container adaptors provide a different interface for sequential containers.
- PriorityQueue: adapts a container to provide priority queue
- Queue: adapts a container to provide queue (FIFO data structure)
- Stack: adapts a container to provide stack (LIFO data structure)

## TODO:
- fixed-size array
- string builder
- alternate arena type with a freeblock list?
- re-hash or store hashes in buckets?
- support for custom allocators
- add error handling to mimic C++ bad_alloc exception?
- iterators?
- thread safety?
- fix bugs in RB Tree
- stress test all types

## CONSIDER:
- C++ implementation of Priority Queue uses a binary heap which is faster than the current array/qsort implementation
- attempt to re-write create macro to accept hash table without having to write 'sizeof'
- separate linked list pages into array(node) + array(data) for even faster iteration?
- all the sequential containers have code duplication in the struct that could be reduced to the base.h file

