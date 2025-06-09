# C Heap

This fork exposes only the files necessary to use the strings library.

Arenas, vectors, String, ConstString, and StringView are exposed.
The implementation is simplified to reduce the complexity of the build.

## Strings Library

Provides a String object and associated interface for working with strings. 

The interface automatically allocates memory, and arenas may be substituted as
allocators for simplifying memory management. 

Supports operations such as string concatenation, duplication, pattern 
substitution, case transformation, slicing, stripping, splitting and joining.