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

static int sort_max(int a, int b)
{
    return ((a < b) - (a > b));
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

static struct Node *create_node(int colour, int key)
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

static void swap_colours(struct Node *node)
{
    node->colour = !node->colour;

    node->l->colour = !node->l->colour;
    node->r->colour = !node->r->colour;
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

        if (cmp > 0)
        {
            node->l = rbt_insert(node->l, key);
        }
        else if (cmp < 0)
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
}

void insert(int key)
{
    head = rbt_insert(head, key);
    head->colour = BLACK;
}
