#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct Table Table;
typedef int (*KeyComp)(const void *a, const void *b);
typedef bool (*ValueComp)(const void *a, const void *b);

__attribute__((warn_unused_result))
Table *create_table(size_t key_size, size_t value_size, KeyComp kc, ValueComp vc);
void destroy_table(Table **table);

void insert(Table *table, void *key, void *value);

size_t count(Table *table, void *key);
void *find(Table *table, void *key);
bool contains(Table *table, void *key);

void erase(Table *table, void *key);
void clear(Table *table);

bool empty(Table *table);
size_t size(Table *table);
