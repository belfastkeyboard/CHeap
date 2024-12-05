#include "../../internals/rbtree.h"
#include "../../internals/base.h"
#include "../../table.h"


typedef struct Table
{
    struct NodeAlloc *alloc;
    size_t k_size;
    size_t v_size;
    KComp k_comp;
    size_t nmemb;
    struct Node *head;
} Table;


Table *create_table(const size_t k_size,
                    const size_t v_size,
                    const KComp compare)
{
    Table *table = memory_allocate_container(sizeof(Table));

    table->alloc = create_node_allocator(sizeof(struct Node),
                                         k_size,
                                         v_size);

    table->k_size = k_size;
    table->v_size = v_size;
    table->k_comp = compare;
    table->nmemb = 0;
    table->head = NULL;

    return table;
}

void destroy_table(Table **table)
{
    destroy_node_allocator(&(*table)->alloc);

    memory_free_buffer((void**)table);
}


void insert_table(Table *table,
                  const void *key,
                  const void *value)
{
    insert_rbtree(table->alloc,
                  &table->head,
                  key,
                  value,
                  table->k_comp,
                  table->k_size,
                  table->v_size,
                  &table->nmemb);
}


size_t count_table(const Table *table,
                 const void *key)
{
    return (rbt_search_v(table->head,
                         key,
                         table->k_comp)) ? 1 :
                                           0;
}

void *find_table(const Table *table,
               const void *key)
{
    return rbt_search_v(table->head,
                        key,
                        table->k_comp);
}

bool contains_table(const Table *table,
                  const void *key)
{
    return rbt_search_v(table->head,
                        key,
                        table->k_comp) ? true :
                                         false;
}


void erase_table(Table *table,
               const void *key)
{
    delete_rbtree(table->alloc,
                  &table->head,
                  key,
                  table->k_comp,
                  &table->nmemb);
}

void clear_table(Table *table)
{
    clear_rbtree(table->alloc,
                 &table->nmemb);
}


bool empty_table(const Table *table)
{
    return generic_empty(table->nmemb);
}

size_t size_table(const Table *table)
{
    return generic_size(table->nmemb);
}
