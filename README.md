# C Heap
Library for simplifying memory management in C.

- Enforced memory safety
  - For performance reasons there are no memory safety checks in release builds
  (i.e. uses asserts for runtime safety checks)
- Reduced calls to malloc/free
  - Architecting memory to avoid malloc/free calls when possible results in improved performance 
  and less memory fragmentation
  - Fewer calls to malloc/free reduces opportunities for leaks, double free, or other incorrect handling
  of memory
  - This architectural philosophy comes with some trade-offs e.g. linked lists use bump allocation to 
  group memory into pages which tends to improve caching (one of the frequent criticisms of the
  linked list) but results in increased memory overhead.  

## MANAGEMENT CONTEXT
Open and close a CHeap memory management context.
All CHeap allocations make use of a custom allocator rather than standard library memory management.
All memory is freed upon context closure, simplifying memory management and reducing opportunities for
memory leakage.

## ALLOCATION LIBRARY
Allocators provide an interface for safe memory management and lifetime control.
- Arena
- Bump allocator

## CONTAINER LIBRARY

### Sequence containers
Sequence containers implement data structures which can be accessed sequentially.
- Array: fixed-sized contiguous array
- Vector: dynamic contiguous array
- Deque: double-ended queue
- Forward List: singly-linked list
- List: doubly-linked list
- Ring Buffer: fixed-capacity circular buffer

#### Associative containers
Associative containers implement sorted data structures that can be quickly searched (O(log n) complexity).
- Set: collection of unique keys, sorted by keys
- Table: collection of key-value pairs, sorted by keys, keys are unique

### Unordered associative containers
Unordered associative containers implement unsorted (hashed) data structures that can be quickly searched (O(1) average, O(n) worst-case complexity).
- Hash Set: collection of unique keys, hashed by keys
- Hash Table: collection of key-value pairs, hashed by keys, keys are unique

### Container adaptors
Container adaptors provide a different interface for sequential containers.
- Priority Queue: adapts a container to provide priority queue
- Queue: adapts a container to provide queue (FIFO data structure)
- Stack: adapts a container to provide stack (LIFO data structure)

## STRINGS LIBRARY
Strings implement an interface for resizable character arrays.
A number of functions for manipulating strings are provided.

Only char type strings supported.

Strings own their character data which allows for writing strings to memory and retrieving them with char data intact.
As a result strings can be stored inside other containers, without ownership the char data may go out of scope and 
invalidate the string's data before its retrieval from the container.

^--- is this true ?

## RANGES LIBRARY
Very WIP. Ranges in C++ operate on iterators which currently do not exist in CHeap.
I do plan to add iterators at some point.
Currently, ranges exist only for adding multiple elements to arrays at once instead of one at a time.

## TODO:
- implement a deque type for the queue container
  - benefits: faster pushing, popping, slightly slower access. good trade-off
- graphs?
- update generic macros
- range functions in cheap.h generics
- iterators
- enforce alignment in CHeap allocator
- stress test all types
- resize for deque/queue
- reduce malloc calls in src/internals/deq.c
- provide better implementation of push_range for deque type

## CONSIDER:
- alternate arena type with a freeblock list?
- thread safety?
- attempt to re-write create macro to accept hash table without having to write 'sizeof'
- separate linked list pages into array(node) + array(data) for even faster iteration?
- all the sequential containers have code duplication in the struct that could be reduced to the base.h file 
- re-hash or store hashes in buckets? (reduced memory overhead vs reduced time to hash)

## INFLUENCES

[Untangling Lifetimes: The Arena Allocator](https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator)
by Ryan Fleury.

[C++ Containers Library](https://en.cppreference.com/w/cpp/container)

[Java Red-Black Tree Implementation](https://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html)
by Robert Sedgewick and Kevin Wayne. 

Algorithms in C by Robert Sedgewick. 1990.

## WATCH LATER

### GRAPHS

- [8 Data Structures Every Programmer Should Know](https://www.youtube.com/watch?v=gxdQiBkidWk)
by Forrest Knight

- [3 Types of Algorithms Every Programmer Needs to Know](https://www.youtube.com/watch?v=Uym4-KhP3Lc)
by Forrest Knight

- [7 Design Patterns EVERY Developer Should Know](https://www.youtube.com/watch?v=BJatgOiiht4)
by Forrest Knight

### ALLOCATOR TYPES

- [Arena Allocator, Stack Allocator, Free-list Allocator, Pool Allocator Visualized](https://www.youtube.com/watch?v=Cdger2-hlt4)
by Code with Arian
