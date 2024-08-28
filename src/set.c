#include <malloc.h>
#include <memory.h>
#include "../set.h"

enum colour_t {
    BLACK,
    RED
};

struct Node
{
    struct Node *parent;
    struct Node *left;
    struct Node *right;
    enum colour_t colour;
    void *key;
};

typedef struct Set
{
    struct Node *root;

    size_t k_size;
    KeyComp k_comp;

    size_t nmemb;
} Set;

static struct Node *create_node(Set *set, struct Node *parent, void *key, int comp)
{
    struct Node *node = malloc(sizeof(struct Node));

    node->parent = parent;
    node->left = NULL;
    node->right = NULL;
    node->colour = RED;
    node->key = malloc(set->k_size);
    memcpy(node->key, key, set->k_size);

    if (parent)
    {
        if (comp > 0)
            parent->right = node;
        else
            parent->left = node;
    }

    return node;
}
static void destroy_node(struct Node *node)
{
    if (node->key)
        free(node->key);

    free(node);
}
static void destroy_node_r(struct Node *node)
{
    if (node->left)
        destroy_node_r(node->left);

    if (node->right)
        destroy_node_r(node->right);

    destroy_node(node);
}

Set *create_set(size_t key_size, KeyComp k_comp)
{
    Set *set = malloc(sizeof(Set));

    set->root = NULL;

    set->k_size = key_size;
    set->k_comp = k_comp;
    set->nmemb = 0;

    return set;
}
void destroy_set(Set **set)
{
    destroy_node_r((*set)->root);

    free(*set);
    *set = NULL;
}

static struct Node *minimum(struct Node *node)
{
    while (node->left)
        node = node->left;

    return node;
}
static void swap_colours(struct Node *y, struct Node *z)
{
    enum colour_t a = y->colour;
    enum colour_t b = z->colour;

    y->colour = b;
    z->colour = a;
}
static void left_rotate(struct Node **root, struct Node *x)
{
    struct Node *y = x->right;
    x->right = y->left;

    if (y->left)
        y->left->parent = x;

    y->parent = x->parent;

    if (!x->parent)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}
static void right_rotate(struct Node **root, struct Node *y)
{
    struct Node *x = y->left;
    y->left = x->right;

    if (x->right)
        x->right->parent = y;

    x->parent = y->parent;

    if (!y->parent)
        *root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;

    x->right = y;
    y->parent = x;
}
static void insertion_fix(struct Node **root, struct Node *node)

{
    while (node != *root && node->parent->colour == RED)
    {
        struct Node *p = node->parent;
        struct Node *gp = p->parent;

        if (p == gp->left)
        {
            struct Node *u = gp->right;

            if (u && u->colour == RED)
            {
                p->colour = BLACK;
                u->colour = BLACK;
                gp->colour = RED;
                node = gp;
            }
            else
            {
                if (node == p->right)
                {
                    node = p;
                    left_rotate(root, node);
                    // p = node->parent;
                }

                p->colour = BLACK;
                gp->colour = RED;
                right_rotate(root, gp);
            }
        }
        else
        {
            struct Node *u = gp->left;

            if (u && u->colour == RED)
            {
                p->colour = BLACK;
                u->colour = BLACK;
                gp->colour = RED;
                node = gp;
            }
            else
            {
                if (node == p->left)
                {
                    node = p;
                    right_rotate(root, node);
                    // p = node->parent;
                }

                p->colour = BLACK;
                gp->colour = RED;
                left_rotate(root, gp);
            }
        }
    }

    (*root)->colour = BLACK;
}

static struct Node *insert_node(Set *set, void *key)
{
    struct Node *node = NULL;

    if (!set->root)
    {
        set->root = create_node(set, NULL, key, 0);
        set->root->colour = BLACK;
        node = set->root;
    }
    else
    {
        struct Node *curr = set->root;
        struct Node *parent = NULL;
        int comp;

        while (curr)
        {
            parent = curr;
            comp = set->k_comp(key, curr->key);

            if (comp > 0)
                curr = curr->right;
            else if (comp < 0)
                curr = curr->left;
            else
                break;
        }

        if (!curr)
        {
            curr = create_node(set, parent, key, comp);

            insertion_fix(&set->root, curr);

            node = curr;
        }
    }

    return node;
}
static struct Node *find_node(Set *set, void *key)
{
    struct Node *found = set->root;

    while (found)
    {
        int comp = set->k_comp(key, found->key);

        if (comp > 0)
            found = found->right;
        else if (comp < 0)
            found = found->left;
        else
            break;
    }

    return found;
}
static struct Node *rebalance_for_erase(struct Node **root, struct Node *z)
{
    // the below implementation is lifted from GCC with only slight modification

    struct Node *x, *y, *x_parent;

    y = z;

    if (!y->left)
        x = y->right;
    else if (!y->right)
        x =  y->left;
    else
    {
        y = y->right;
        y = minimum(y);
        x = y->right;
    }

    if (y != z)
    {
        z->left->parent = y;
        y->left = z->left;

        if (y != z->right)
        {
            x_parent = y->parent;

            if (x)
                x->parent = y->parent;

            y->parent->left = x;
            y->right = z->right;
            z->right->parent = y;
        }
        else
            x_parent = y;

        if (*root == z)
            *root = y;
        else if (z->parent->left == z)
            z->parent->left = y;
        else
            z->parent->right = y;

        y->parent = z->parent;
        swap_colours(y, z);
        y = z;
    }
    else // y == z
    {
        x_parent = y->parent;

        if (x)
            x->parent = x->parent;

        if (*root == z)
            *root = x;
        else if (z->parent->left == z)
            z->parent->left = x;
        else
            z->parent->right = x;
    }

    if (y->colour != RED)
    {
        while (x != *root && (!x || x->colour == BLACK))
        {
            if (x == x_parent->left)
            {
                struct Node *w = x_parent->right;

                if (w->colour == RED)
                {
                    w->colour = BLACK;
                    x_parent->colour = RED;
                    left_rotate(root, x_parent);
                    w = x_parent->right;
                }
                if ((!w->left || w->left->colour == BLACK) && (!w->right || w->right->colour == BLACK))
                {
                    w->colour = RED;
                    x = x_parent;
                    x_parent = x_parent->parent;
                }
                else
                {
                    if (!w->right || w->right->colour == BLACK)
                    {
                        w->left->colour = BLACK;
                        w->colour = RED;
                        right_rotate(root, w);
                        w = x_parent->right;
                    }

                    w->colour = x_parent->colour;
                    x_parent->colour = BLACK;

                    if (w->right)
                        w->right->colour = BLACK;

                    left_rotate(root, x_parent);
                    break;
                }
            }
            else
            {
                struct Node *w = x_parent->left;

                if (w->colour == RED)
                {
                    w->colour = BLACK;
                    x_parent->colour = RED;
                    right_rotate(root, x_parent);
                    w = x_parent->left;
                }
                if ((!w->right || w->right->colour == BLACK) && (!w->left || w->left->colour == BLACK))
                {
                    w->colour = RED;
                    x = x_parent;
                    x_parent = x_parent->parent;
                }
                else
                {
                    if (!w->left || w->left->colour == BLACK)
                    {
                        w->right->colour = BLACK;
                        w->colour = RED;
                        left_rotate(root, w);
                        w = x_parent->left;
                    }

                    w->colour = x_parent->colour;
                    x_parent->colour = BLACK;

                    if (w->left)
                        w->left->colour = BLACK;

                    right_rotate(root, x_parent);
                    break;
                }
            }
        }

        if (x)
            x->colour = BLACK;
    }

    return y;
}

void insert(Set *set, void *key)
{
    if (insert_node(set, key))
        set->nmemb++;
}

size_t count(Set *set, void *key)
{
    return (find_node(set, key)) ? 1 : 0;
}
void *find(Set *set, void *key)
{
    void *result = NULL;

    struct Node *node = NULL;
    if ((node = find_node(set, key)))
        result = node->key;

    return result;
}
bool contains(Set *set, void *key)
{
    return (find_node(set, key)) ? true : false;
}

void erase(Set *set, void *key)
{
    struct Node *z;

    if ((z = find_node(set, key)))
    {
        z = rebalance_for_erase(&set->root, z);
        destroy_node(z);
        set->nmemb--;
    }
}
void clear(Set *set)
{
    destroy_node_r(set->root);
    set->nmemb = 0;
    set->root = NULL;
}

bool empty(Set *set)
{
    return set->nmemb == 0;
}
size_t size(Set *set)
{
    return set->nmemb;
}