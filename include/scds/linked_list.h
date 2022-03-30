#ifndef SCDS_LL_H
#define SCDS_LL_H

/**
 * Typedefs
 */
typedef struct node node_t;

/**
 * Structs
**/

/**
 * Represents a node in a linked list.
 */
typedef struct node {
    void *data;
    node_t *next;
    node_t *prev;
} node_t;

/**
 * Represents a linked list.
 */
typedef struct linked_list {
    node_t *head;
    node_t *tail;
    int size;
} linked_list_t;

/**
 * Functions
 */
linked_list_t* linked_list_new();
int linked_list_free(linked_list_t *list);
int linked_list_init(linked_list_t *list);
int linked_list_destroy(linked_list_t *list);
int linked_list_insert(linked_list_t *list, void *data);
int linked_list_remove(linked_list_t *list, void *data);
int linked_list_size(linked_list_t *list);
int linked_list_clear(linked_list_t *list);

#endif
