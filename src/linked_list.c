#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "scds/linked_list.h"

/**
 * @brief Create a new linked list.
 *
 * @return A pointer to the newly created linked list.
 */
linked_list_t* linked_list_new() {
  linked_list_t* list = malloc(sizeof(linked_list_t));

  linked_list_init(list);

  return list;
}

/**
 * @brief Frees an allocated linked list.
 * 
 * @param list The linked list to free.
 * @return 0 on success, -1 on failure.
 */
int linked_list_free(linked_list_t *list) {
  assert(list);

  linked_list_clear(list);

  free(list);

  return 0;

}

/**
 * @brief Initializes a linked list.
 * 
 * @param list The linked list to initialize.
 *
 * @return 0 on success, -1 on failure.
 */
int linked_list_init(linked_list_t *list) {
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;

  return 0;
}

/**
 * @brief Destroys a linked list.
 * 
 * @param list The linked list to destroy.
 *
 * @return 0 on success, -1 on failure.
 */
int linked_list_destroy(linked_list_t *list) {
  return 0;
}


/**
 * @brief Inserts data into a linked list.
 * 
 * @param list The linked list to insert into.
 * @param data The data to insert.
 *
 * @return 0 on success, -1 on failure.
 */
int linked_list_insert(linked_list_t *list, void *data) {
  node_t* node = NULL;
  
  if ((node = malloc(sizeof(node_t))) == NULL) {
    return -1;
  }

  node->data = data;

  if (list->size == 0) {
    list->head = node;
    list->tail = node;
  } else {
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
  }

  list->size++;

  return 0;
}

/**
 * @brief Removes data from a linked list.
 * 
 * @param list The linked list to remove from.
 * @param data The data to remove.
 *
 * @return 0 on success, -1 on failure.
 */
int linked_list_remove(linked_list_t *list, void *data) {
  return 0;
}

/**
 * @brief Gets the size of a linked list.
 * 
 * @param list The linked list to get the size of.
 *
 * @return The size of the linked list.
 */
int linked_list_size(linked_list_t *list) {
  return 0;
}

/**
 * @brief Clears a linked list.
 * 
 * @param list The linked list to clear.
 *
 * @return 0 on success, -1 on failure.
 */
int linked_list_clear(linked_list_t *list) {
  return 0;
}
