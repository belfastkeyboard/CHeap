#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "internals/comp.h"
#include "range.h"


typedef struct Set Set;

Set *create_set(size_t size,
                KComp compare);

void destroy_set(Set **set);


void insert_set(Set *set,
                const void *key);


size_t count_set(const Set *set,
                 const void *key);

void *find_set(const Set *set,
               const void *key);

bool contains_set(const Set *set,
                  const void *key);


void erase_set(Set *set,
               const void *key);

void clear_set(Set *set);


bool empty_set(const Set *set);

size_t size_set(const Set *set);
