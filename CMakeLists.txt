cmake_minimum_required(VERSION 3.28)
project(CHeap C)

set(CMAKE_C_STANDARD 23)


# uncomment to compile release build, which removes asserts and compiles with optimisation
set(BUILD Release)


set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.c")

if (BUILD STREQUAL "Release")
    message(STATUS "Release build: enabling optimizations and defining NDEBUG")

    add_compile_definitions(NDEBUG)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native -funroll-loops -ffast-math")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -funroll-loops -ffast-math")
endif()

add_library(
    CHeap STATIC
        ${SOURCES}
)

#add_executable(
#    CHeap main.c
#        ${SOURCES}
#        cheap.h
#)
