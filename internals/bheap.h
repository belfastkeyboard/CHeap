#pragma once

#include "../pqueue.h"

void bubble_up(void *array,
               size_t i,
               size_t size,
               CompareFunc fnc);


void bubble_down(void *array,
                 size_t nmemb,
                 size_t size,
                 CompareFunc fnc,
                 size_t i);


void make_heap(void *array,
               size_t nmemb,
               size_t size,
               CompareFunc fnc);


void pop_front_binary_heap(void *array,
                           size_t *nmemb,
                           size_t size);
