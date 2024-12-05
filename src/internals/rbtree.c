#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "../../internals/nalloc.h"
#include "../../internals/rbtree.h"

#define BLACK 0
#define RED   1


static bool is_red(struct Node* node)
{
    return node &&
           node->colour == RED;
}


static struct Node *create_node(struct NodeAlloc *alloc,
                                const void *key,
                                const void *value,
                                const size_t k_size,
                                const size_t v_size)
{
    void *memory = alloc_node(alloc);

    struct Node *node = memory;

    node->key = memory + sizeof(struct Node);
    node->value = memory + sizeof(struct Node) + k_size;

    node->colour = RED;
    node->l = NULL;
    node->r = NULL;

    node->key = memcpy(node->key,
                       key,
                       k_size);

    node->value = memcpy(node->value,
                         value,
                         v_size);

    return node;
}


static struct Node *min(struct Node *node)
{
    while (node->l)
    {
        node = node->l;
    }

    return node;
}


static void swap_colours(struct Node *node)
{
    node->colour = !node->colour;

    node->l->colour = !node->l->colour;
    node->r->colour = !node->r->colour;
}


static struct Node *rotate_right(struct Node *node)
{
    assert(node &&
           is_red(node->l));

    struct Node *x = node->l;

    node->l = x->r;

    x->r = node;
    x->colour = node->colour;

    node->colour = RED;

    return x;
}

static struct Node *rotate_left(struct Node *node)
{
    assert(node &&
           is_red(node->r));

    struct Node *x = node->r;

    node->r = x->l;

    x->l = node;
    x->colour = node->colour;

    node->colour = RED;

    return x;
}

static struct Node *move_red_right(struct Node *node)
{
    swap_colours(node);

    if (is_red(node->l->l))
    {
        node = rotate_right(node);

        swap_colours(node);
    }

    return node;
}

static struct Node *move_red_left(struct Node *node)
{
    swap_colours(node);

    if (is_red(node->r->l))
    {
        node->r = rotate_right(node->r);

        node = rotate_left(node);

        swap_colours(node);
    }

    return node;
}


static struct Node *balance(struct Node *node)
{
    if (is_red(node->r) &&
        !is_red(node->l))
    {
        node = rotate_left(node);
    }

    if (is_red(node->l) &&
        is_red(node->l->l))
    {
        node = rotate_right(node);
    }

    if (is_red(node->l) &&
        is_red(node->r))
    {
        swap_colours(node);
    }

    return node;
}


static struct Node *rbt_delete_min(struct NodeAlloc *alloc,
                                   struct Node *node,
                                   size_t *nmemb)
{
    struct Node *result = NULL;

    if (node->l)
    {
        if (!is_red(node->l) &&
            !is_red(node->l->l))
        {
            node = move_red_left(node);
        }

        node->l = rbt_delete_min(alloc,
                                 node->l,
                                 nmemb);

        result = balance(node);
    }
    else
    {
        free_node(alloc,
                  node);

        (*nmemb)--;
    }

    return result;
}

static void delete_min(struct NodeAlloc *alloc,
                       struct Node *node,
                       size_t *nmemb)
{
    assert(node);

    if (!is_red(node->l) &&
        !is_red(node->r))
    {
        node->colour = RED;
    }

    node = rbt_delete_min(alloc,
                          node,
                          nmemb);

    if (node)
    {
        node->colour = BLACK;
    }
}


static struct Node *rbt_insert(struct NodeAlloc *alloc,
                               struct Node *node,
                               const void *key,
                               const void *value,
                               const KComp compare,
                               const size_t k_size,
                               const size_t v_size)
{
    struct Node *result = NULL;

    if (!node)
    {
        result = create_node(alloc,
                             key,
                             value,
                             k_size,
                             v_size);
    }
    else
    {
        int cmp = compare(key,
                     node->key);

        if (cmp < 0)
        {
            node->l = rbt_insert(alloc,
                                 node->l,
                                 key,
                                 value,
                                 compare,
                                 k_size,
                                 v_size);
        }
        else if (cmp > 0)
        {
            node->r = rbt_insert(alloc,
                                 node->r,
                                 key,
                                 value,
                                 compare,
                                 k_size,
                                 v_size);
        }
        else
        {
            node->key = memcpy(node->key,
                               key,
                               k_size); // TODO: overwrite value??
        }

        if (is_red(node->r) &&
            !is_red(node->l))
        {
            node = rotate_left(node);
        }


        if (is_red(node->l) &&
            is_red(node->l->l))
        {
            node = rotate_right(node);
        }

        if (is_red(node->l) &&
            is_red(node->r))
        {
            swap_colours(node);
        }

        result = node;
    }

    return result;
}


static struct Node *rbt_delete(struct NodeAlloc *alloc,
                               struct Node *node,
                               const void *key,
                               const KComp compare,
                               size_t *nmemb)
{
    if (!node)
    {
        return node;
    }

    int res = compare(key,
                 node->key);

    if (res < 0)
    {
        if (!is_red(node->l) &&
            (node->l &&
            !is_red(node->l->l)))
        {
            node = move_red_left(node);
        }

        node->l = rbt_delete(alloc,
                             node->l,
                             key,
                             compare,
                             nmemb);
    }
    else
    {
        if (is_red(node->l))
        {
            node = rotate_right(node);
        }

        if (res == 0 &&
            !node->r)
        {
            free_node(alloc,
                      node);

            (*nmemb)--;

            return NULL;
        }

        if (!is_red(node->r) &&
            (node->r &&
            !is_red(node->r->l)))
        {
            node = move_red_right(node);
        }

        res = compare(key,
                 node->key);

        if (res == 0)
        {
            struct Node *x = min(node->r);

            node->key = x->key;

            node->r = rbt_delete_min(alloc,
                                     node->r,
                                     nmemb);
        }
        else
        {
            node->r = rbt_delete(alloc,
                                 node->r,
                                 key,
                                 compare,
                                 nmemb);
        }
    }

    return balance(node);
}

static void print_r(const struct Node *node)
{
    if (node->l)
    {
        print_r(node->l);
    }

    printf("%p ", node->key);

    if (node->r)
    {
        print_r(node->r);
    }
}


static struct Node *rbt_search(struct Node *head,
                               const void *key,
                               const KComp compare)
{
    struct Node *result = NULL;
    struct Node *node = head;

    while (node && !result)
    {
        int res = compare(key,
                     node->key);

        if (res > 0)
        {
            node = node->r;
        }
        else if (res < 0)
        {
            node = node->l;
        }
        else
        {
            result = node;
        }
    }

    return result;
}

void *rbt_search_k(struct Node *head,
                   const void *key,
                   const KComp compare)
{
    void *result = NULL;

    struct Node *node = rbt_search(head,
                                   key,
                                   compare);

    if (node)
    {
        result = node->key;
    }

    return result;
}

void *rbt_search_v(struct Node *head,
                   const void *key,
                   const KComp compare)
{
    void *result = NULL;

    struct Node *node = rbt_search(head,
                                   key,
                                   compare);

    if (node)
    {
        result = node->value;
    }

    return result;
}


void print(const struct Node *head)
{
    print_r(head);
    puts("");
}

void insert_rbtree(struct NodeAlloc *alloc,
                   struct Node **head,
                   const void *key,
                   const void *value,
                   const KComp compare,
                   const size_t k_size,
                   const size_t v_size,
                   size_t *nmemb)
{
    assert(key);

    *head = rbt_insert(alloc,
                       *head,
                       key,
                       value,
                       compare,
                       k_size,
                       v_size);

    (*head)->colour = BLACK;

    (*nmemb)++;
}


void delete_rbtree(struct NodeAlloc *alloc,
                   struct Node **head,
                   const void *key,
                   const KComp compare,
                   size_t *nmemb)
{
    assert(*head);

    if (!is_red((*head)->l) &&
        !is_red((*head)->r))
    {
        (*head)->colour = RED;
    }

    *head = rbt_delete(alloc,
                       *head,
                       key,
                       compare,
                       nmemb);

    if (*head)
    {
        (*head)->colour = BLACK;
    }
}


void clear(struct NodeAlloc *alloc)
{
    clear_nodes(alloc);
}
