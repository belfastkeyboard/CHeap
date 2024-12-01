#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include "rbtree.h"

#define BLACK 0
#define RED   1

struct Node
{
    int colour;
    int key;
    struct Node *l;
    struct Node *r;
};

struct Node *head = NULL;

static int sort_max(int a,
                    int b)
{
    return ((a > b) - (a < b));
}


static bool is_red(struct Node* node)
{
    return node &&
           node->colour == RED;
}

static bool rbt_empty(void)
{
    return !head;
}

static struct Node *create_node(int colour,
                                int key)
{
    struct Node *node = malloc(sizeof(struct Node));

    *node = (struct Node){
        .colour = colour,
        .key = key,
        .l = NULL,
        .r = NULL
    };

    return node;
}


static struct Node *min(struct Node *node)
{
    struct Node *result = node;

    if (node->l)
    {
        result = min(node->l);
    }

    return result;
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
    if (is_red(node->r) && !is_red(node->l))
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


static struct Node *rbt_delete_min(struct Node *node)
{
    if (!node->l)
    {
        return NULL;
    }

    if (!is_red(node->l) &&
        !is_red(node->l->l))
    {
        node = move_red_left(node);
    }

    node->l = rbt_delete_min(node->l);

    return balance(node);
}

static void delete_min(struct Node *node)
{
    assert(!rbt_empty());

    if (!is_red(node->l) &&
        !is_red(node->r))
    {
        node->colour = RED;
    }

    node = rbt_delete_min(node);

    if (!rbt_empty())
    {
        node->colour = BLACK;
    }
}


static struct Node *rbt_insert(struct Node *node, int key)
{
    struct Node *result = NULL;

    if (!node)
    {
        result = create_node(RED, key);
    }
    else
    {
        int cmp = sort_max(key, node->key);

        if (cmp < 0)
        {
            node->l = rbt_insert(node->l, key);
        }
        else if (cmp > 0)
        {
            node->r = rbt_insert(node->r, key);
        }
        else
        {
            node->key = key;
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

static struct Node *rbt_delete(struct Node *node,
                               int key)
{
    if (!node)
    {
        return node;
    }

    int res = sort_max(key, node->key);

    if (res < 0)
    {
        if (!is_red(node->l) &&
            (node->l &&
            !is_red(node->l->l)))
        {
            node = move_red_left(node);
        }

        node->l = rbt_delete(node->l,
                             key);
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
            return NULL;
        }

        if (!is_red(node->r) &&
            (node->r &&
            !is_red(node->r->l)))
        {
            node = move_red_right(node);
        }

        res = sort_max(key, node->key);

        if (res == 0)
        {
            struct Node *x = min(node->r);
            node->key = x->key;
            node->r = rbt_delete_min(node->r);
        }
        else
        {
            node->r = rbt_delete(node->r,
                                 key);
        }
    }

    return balance(node);
}

static void print_r(struct Node *node)
{
    if (node->l)
    {
        print_r(node->l);
    }

    printf("%d ", node->key);

    if (node->r)
    {
        print_r(node->r);
    }
}

void print(void)
{
    print_r(head);
    puts("");
}

void insert(int key)
{
    head = rbt_insert(head, key);
    head->colour = BLACK;
}

void delete(int key)
{
    if (!is_red(head->l) &&
        !is_red(head->r))
    {
        head->colour = RED;
    }

    head = rbt_delete(head,
                      key);

    if (head)
    {
        head->colour = BLACK;
    }
}
