cmake_minimum_required(VERSION 3.28)
project(CHeap C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native -funroll-loops -ffast-math")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -funroll-loops -ffast-math")

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.c")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/*.h")

add_library(
    CHeapLib STATIC
        ${SOURCES}
)

add_executable(
    CHeap main.c
        ${SOURCES}
)

target_link_libraries(CHeap PRIVATE CHeapLib)
