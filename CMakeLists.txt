cmake_minimum_required(VERSION 3.28)
project(CHeap C)

set(CMAKE_C_STANDARD 23)

add_library(
    CHeap STATIC
        arena.c      arena.h
        array.c      array.h
        builder.c    builder.h
        pqueue.c     pqueue.h
        rbuff.c      rbuff.h
        stringview.c stringview.h
)
