#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct Table Table;
typedef int (*KeyComp)(const void *a, const void *b);
typedef bool (*ValueComp)(const void *a, const void *b);

__attribute__((warn_unused_result))
Table *create_hash_table(size_t key_size, size_t value_size, KeyComp kc, ValueComp vc);
void destroy_hash_table(Table **table);

void insert_hash_table(Table *table, void *key, void *value);

size_t count_hash_table(Table *table, void *key);
void *find_hash_table(Table *table, void *key);
bool contains_hash_table(Table *table, void *key);

void erase_hash_table(Table *table, void *key);
void clear_hash_table(Table *table);

bool empty_hash_table(Table *table);
size_t size_hash_table(Table *table);
