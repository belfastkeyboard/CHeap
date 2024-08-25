cmake_minimum_required(VERSION 3.28)
project(CHeap C)

set(CMAKE_C_STANDARD 23)

#add_library(
#    CHeap STATIC library.c
#        arena.c arena.h
#        main.c
#
#)

add_executable(CHeap main.c arena.c)
