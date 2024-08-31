cmake_minimum_required(VERSION 3.28)
project(CHeap C)

set(CMAKE_C_STANDARD 23)

add_library(
    CHeap STATIC
        src/arena.c      arena.h
        src/array.c      array.h
        src/builder.c    builder.h
        src/deque.c      deque.h
        src/flist.c      flist.h
        src/hash_set.c   hash_set.h
        src/hash_table.c hash_table.h
        src/list.c       list.h
        src/pqueue.c     pqueue.h
        src/queue.c      queue.h
        src/rbuff.c      rbuff.h
        src/stack.c      stack.h
        src/stringview.c stringview.h
        src/span.c       span.h
        src/base.c       base.h
)

install(TARGETS CHeap
    EXPORT CHeapConfig
    ARCHIVE DESTINATION lib
)

install(EXPORT CHeapConfig
    DESTINATION lib/cmake/CHeap
)

install(DIRECTORY include/
    DESTINATION include
)

