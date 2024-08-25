cmake_minimum_required(VERSION 3.28)
project(CHeap C)

set(CMAKE_C_STANDARD 23)

add_library(
    CHeap STATIC
        src/arena.c      arena.h
        src/array.c      array.h
        src/builder.c    builder.h
        src/deque.c      deque.h
        src/pqueue.c     pqueue.h
        src/queue.c      queue.h
        src/rbuff.c      rbuff.h
        src/stringview.c stringview.h
)
