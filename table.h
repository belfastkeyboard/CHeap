#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "internals/comp.h"


typedef struct Table Table;

Table *create_table(size_t k_size,
                    size_t v_size,
                    KComp compare);

void destroy_table(Table **table);


void insert_table(Table *table,
                  const void *key,
                  const void *value);


size_t count_table(const Table *table,
                   const void *key);

void *find_table(const Table *table,
                 const void *key);

bool contains_table(const Table *table,
                    const void *key);


void erase_table(Table *table,
                 const void *key);

void clear_table(Table *table);


bool empty_table(const Table *table);

size_t size_table(const Table *table);
