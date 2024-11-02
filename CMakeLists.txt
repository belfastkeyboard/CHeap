cmake_minimum_required(VERSION 3.28)
project(CHeap C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native -funroll-loops -ffast-math")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -funroll-loops -ffast-math")

add_library(
    CHeapLib STATIC
        src/containers/arena.c      arena.h
        src/containers/array.c      array.h
        src/containers/deque.c      deque.h
        src/containers/flist.c      flist.h
        src/containers/hash_set.c   hash_set.h
        src/containers/hash_table.c hash_table.h
        src/containers/list.c       list.h
        src/containers/pqueue.c     pqueue.h
        src/containers/queue.c      queue.h
        src/containers/rbuff.c      rbuff.h
        src/containers/stack.c      stack.h
        src/containers/span.c       span.h

        src/internals/base.c        internals/base.h
        src/internals/mpool.c       internals/mpool.h
        src/internals/linked.c      internals/linked.h
        src/internals/hash.c        internals/hash.h
        src/containers/bump.c       bump.h
        src/containers/string_t.c   string_t.h
)

add_executable(
    CHeap main.c cheap.h
        debug_malloc.c            debug_malloc.h
        src/containers/bump.c     bump.h
        src/containers/string_t.c string_t.h
)

target_link_libraries(CHeap PRIVATE CHeapLib)
