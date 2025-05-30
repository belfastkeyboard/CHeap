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

## ALLOCATOR LIBRARY

Allocators provide an interface for safe memory management and lifetime control.
- Arena: dynamic-size variable-width allocator
- Bump allocator: fixed-size variable-width allocator
- Pool allocator: fixed-size fixed-width allocator

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
TODO: Re-work the string library.
See the implementation of [Simple Dynamic Strings](https://github.com/antirez/sds)

## RANGES LIBRARY
Very WIP. Ranges in C++ operate on iterators which currently do not exist in CHeap.
I do plan to add iterators at some point.
Currently, ranges exist only for adding multiple elements to arrays at once instead of one at a time.

## TODO:
- storing pointers
  - storing items as pointers is not intuitive
  - passing ```Item *item``` to ```push_vector(vector, item)``` will write ```*item``` into the vector
  - this gives the vector ownership of ```item```
  - it is possible to store pointers to ```Item*``` as well: ```push_vector(vector, &item)```
  - this will write the memory address of ```item``` into the vector, giving it a reference to but not ownership of ```item```
  - retrieval of ```item``` is counter-intuitive, rather than simply retrieve with ```Item *item2 = at_vector(vector, 0)```, you must cast the result
  - the cast to perform looks like this: ```Item *item2 = *(Item**)at_vector(vector, 0)```
  - this will correctly retrieve the element
  - it is also possible to retrieve a pointer to a pointer such as ```Item **item2 = at_vector(vector, 0)```
  - this is very annoying, and it is easy to make mistakes like this
- update generic macros
- range functions in cheap.h generics
- iterators
- stress test all types
- resize for deque/queue
- reduce malloc calls in src/internals/deq.c
- provide better implementation of push_range for deque type

## CONSIDER:
- attempt to re-write create macro to accept hash table without having to write 'sizeof'
- separate linked list pages into array(node) + array(data) for even faster iteration?
- all the sequential containers have code duplication in the struct that could be reduced to the base.h file 

## INFLUENCES

[Untangling Lifetimes: The Arena Allocator](https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator)
by Ryan Fleury.

[C++ Containers Library](https://en.cppreference.com/w/cpp/container)

[Java Red-Black Tree Implementation](https://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html)
by Robert Sedgewick and Kevin Wayne. 

_Algorithms in C_ by Robert Sedgewick. 1990.

[Simple Dynamic Strings](https://github.com/antirez/sds)
by Salvatore Sanfilippo
