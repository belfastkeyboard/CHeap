# C Heap
Library for simplifying memory management in C.

- Enforced memory safety.
  - For performance reasons there are no memory safety checks in release builds
  (i.e. uses asserts for runtime safety checks)
- Reduced calls to malloc/free
  - Architecting memory to avoid malloc/free calls when possible results in improved performance 
  and less memory fragmentation
  - Fewer calls to malloc/free reduces opportunities for leaks, double free, or other incorrect handling
  of memory
  - This architectural philosophy comes with some trade-offs e.g. linked lists use bump allocation to 
  group memory into pages which tends to improve memory-caching (one of the frequent criticisms of the
  linked list) but results in increased memory overhead.  

## ALLOCATION LIBRARY
- Arena
- Bump Allocator

## CONTAINER LIBRARY

### Sequence containers
Sequence containers implement data structures which can be accessed sequentially.
- Array: dynamic contiguous array
- Deque: double-ended queue
- ForwardList: singly-linked list
- List: doubly-linked list
- Ring Buffer: fixed-size circular buffer

### Unordered associative containers
Unordered associative containers implement unsorted (hashed) data structures that can be quickly searched (O(1) average, O(n) worst-case complexity).
- Hash Set
- Hash Table

### Container adaptors
Container adaptors provide a different interface for sequential containers.
- PriorityQueue: adapts a container to provide priority queue
- Queue: adapts a container to provide queue (FIFO data structure)
- Stack: adapts a container to provide stack (LIFO data structure)

## STRINGS LIBRARY
- String

## RANGES LIBRARY
Very WIP. Ranges in C++ operate on iterators which currently do not exist in CHeap.
I do plan to add iterators at some point.
Currently, ranges exist only for adding multiple elements to arrays at once instead of one at a time.

## TODO:
- fixed-size array
- string builder
- iterators
- alternate arena type with a freeblock list?
- support for custom allocators
- thread safety?
- fix bugs in RB Tree
- add error handling to mimic C++ bad_alloc exception?
- stress test all types

## CONSIDER:
- C++ implementation of Priority Queue uses a binary heap which is faster than the current array/qsort implementation
- attempt to re-write create macro to accept hash table without having to write 'sizeof'
- separate linked list pages into array(node) + array(data) for even faster iteration?
- all the sequential containers have code duplication in the struct that could be reduced to the base.h file 
- consider creating spans on the stack instead
- re-hash or store hashes in buckets? (reduced memory overhead vs reduced time to hash)

