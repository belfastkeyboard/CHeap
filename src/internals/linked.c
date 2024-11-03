#include <malloc.h>
#include <memory.h>
#include "../../internals/linked.h"

#define INIT_LIST_COUNT 8

struct Page
{
    void *pool;
    size_t cursor;
    size_t size;

    struct Page *prev;
};

struct Node
{
    void *value;
    struct Node *next;
    struct Node *prev;
};


__attribute__((warn_unused_result))
static struct Page *create_page(const size_t size)
{
    struct Page *page = calloc(1,
                               sizeof(struct Page));

    page->pool = calloc(1,
                        size);

    page->size = size;

    return page;
}

__attribute__((warn_unused_result))
static struct Page *destroy_page(struct Page *page)
{
    struct Page *prev = page->prev;

    free(page->pool);
    free(page);

    return prev;
}

static void *alloc_page(struct Page **page,
                        const size_t size)
{
    struct Node *node = NULL;

    if ((*page)->cursor + size > (*page)->size)
    {
        struct Page *new = create_page((*page)->size * 2);
        new->prev = *page;
        *page = new;
    }

    node = (*page)->pool + (*page)->cursor;
    (*page)->cursor += size;

    return node;
}

static void clear_pages(struct Page **page)
{
    do {
        *page = destroy_page(*page);
    } while ((*page)->prev);

    (*page)->cursor = 0;
}


static struct Node *create_node(struct Page **page,
                                const size_t size,
                                const void *value)
{
    struct Node *node = alloc_page(page,
                                   sizeof(struct Node));

    node->value = alloc_page(page,
                             size);

    memcpy(node->value,
           value,
           size);

    return node;
}

struct Page *create_pages(size_t size)
{
    return create_page((sizeof(struct Node) + size) * INIT_LIST_COUNT);
}

void destroy_pages(struct Page **page)
{
    do {
        *page = destroy_page(*page);
    } while (*page);
}


static void create_node_singly_linked(struct Page **page,
                                      const size_t nmemb,
                                      const size_t size,
                                      const void *value,
                                      struct Node *position,
                                      struct Node **head,
                                      int prior)
{
    struct Node *node = create_node(page,
                                    size,
                                    value);

    if (prior || !nmemb)
    {
        *head = node;
        node->next = position;
    }
    else
    {
        node->next = position->next;
        position->next = node;
    }
}

static void erase_node_singly_linked(struct Node *position,
                                     struct Node **head,
                                     int prior)
{
    if (prior)
    {
        *head = position->next;
    }
    else
    {
        position->next = position->next->next;
    }
}


static void create_node_doubly_linked(struct Page **page,
                                      const size_t nmemb,
                                      const size_t size,
                                      const void *value,
                                      struct Node *position,
                                      struct Node **head,
                                      struct Node **tail)
{
    struct Node *node = create_node(page,
                                    size,
                                    value);

    if (!nmemb)
    {
        *head = node;
        *tail = node;
    }
    else if (!position)
    {
        (*tail)->next = node;
        node->prev = *tail;
        *tail = node;
    }
    else
    {
        if (position->prev)
        {
            position->prev->next = node;
            node->prev = position->prev;
        }
        else
        {
            *head = node;
        }

        position->prev = node;
        node->next = position;
    }
}

static void erase_node_doubly_linked(struct Node *position,
                                    struct Node **head,
                                    struct Node **tail)
{
    if (position->prev)
    {
        position->prev->next = position->next;
    }
    else
    {
        *head = position->next;
    }

    if(position->next)
    {
        position->next->prev = position->prev;
    }
    else
    {
        *tail = position->prev;
    }
}


static struct Node *seek_node(const size_t nmemb,
                              const size_t index,
                              struct Node *head,
                              struct Node *tail)
{
    struct Node *seek = NULL;

    if (index < nmemb)
    {
        if (tail && index > nmemb / 2)
        {
            seek = tail;

            for (size_t i = nmemb - 1; i > index; i--)
            {
                seek = seek->prev;
            }
        }
        else
        {
            seek = head;

            for (size_t i = 0; i < index; i++)
            {
                seek = seek->next;
            }
        }
    }

    return seek;
}


void generic_push_front_singly_linked(struct Page **page,
                                      size_t *nmemb,
                                      size_t size,
                                      struct Node **head,
                                      const void *value)
{
    generic_insert_singly_linked(page,
                                 nmemb,
                                 size,
                                 head,
                                 value,
                                 0,
                                 1);
}

size_t generic_insert_singly_linked(struct Page **page,
                                    size_t *nmemb,
                                    const size_t size,
                                    struct Node **head,
                                    const void *value,
                                    size_t index,
                                    int prior)
{
    struct Node *seek = seek_node(*nmemb,
                                  index,
                                  *head,
                                  NULL);

    create_node_singly_linked(page,
                              *nmemb,
                              size,
                              value,
                              seek,
                              head,
                              prior);
    (*nmemb)++;

    return index;
}


void generic_push_front_doubly_linked(struct Page **page,
                                      size_t *nmemb,
                                      size_t size,
                                      struct Node **head,
                                      struct Node **tail,
                                      const void *value)
{
    generic_insert_doubly_linked(page,
                                 nmemb,
                                 size,
                                 head,
                                 tail,
                                 value,
                                 0);
}

void generic_push_back_doubly_linked(struct Page **page,
                                     size_t *nmemb,
                                     size_t size,
                                     struct Node **head,
                                     struct Node **tail,
                                     const void *value)
{
    generic_insert_doubly_linked(page,
                                 nmemb,
                                 size,
                                 head,
                                 tail,
                                 value,
                                 *nmemb);
}

size_t generic_insert_doubly_linked(struct Page **page,
                                    size_t *nmemb,
                                    const size_t size,
                                    struct Node **head,
                                    struct Node **tail,
                                    const void *value,
                                    size_t index)
{
    struct Node *seek = seek_node(*nmemb,
                                  index,
                                  *head,
                                  *tail);

    create_node_doubly_linked(page,
                              *nmemb,
                              size,
                              value,
                              seek,
                              head,
                              tail);

    (*nmemb)++;

    return index;
}


void generic_pop_front_singly_linked(size_t *nmemb,
                                     struct Node **head)
{
    generic_erase_singly_linked(nmemb,
                                0,
                                head,
                                1);
}

size_t generic_erase_singly_linked(size_t *nmemb,
                                   size_t index,
                                   struct Node **head,
                                   int prior)
{
    struct Node *seek = seek_node(*nmemb,
                                  index,
                                  *head,
                                  NULL);

    erase_node_singly_linked(seek,
                             head,
                             prior);

    (*nmemb)--;

    return index;
}


void generic_pop_front_doubly_linked(size_t *nmemb,
                                     struct Node **head,
                                     struct Node **tail)
{
    generic_erase_doubly_linked(nmemb,
                                0,
                                head,
                                tail);
}

void generic_pop_back_doubly_linked(size_t *nmemb,
                                    struct Node **head,
                                    struct Node **tail)
{
    generic_erase_doubly_linked(nmemb,
                                *nmemb - 1,
                                head,
                                tail);
}

size_t generic_erase_doubly_linked(size_t *nmemb,
                                   size_t index,
                                   struct Node **head,
                                   struct Node **tail)
{
    struct Node *seek = seek_node(*nmemb,
                                  index,
                                  *head,
                                  *tail);

    erase_node_doubly_linked(seek,
                             head,
                             tail);

    (*nmemb)--;

    return index;
}

void generic_clear_linked(struct Page **page,
                          struct Node **head,
                          struct Node **tail,
                          size_t *nmemb)
{
    clear_pages(page);

    *head = NULL;

    if (tail)
    {
        *tail = NULL;
    }

    *nmemb = 0;
}


void *generic_access_linked(struct Node *node)
{
    return node->value;
}
