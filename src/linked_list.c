/*
MIT License

Copyright (c) 2022 Christopher Irvine

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "scds/linked_list.h"

static node_t* node_at(linked_list_t* list, size_t index);
static int attach_node(linked_list_t* list, node_t* node);
static int detach_node(linked_list_t* list, node_t* node);
static int steal_node(linked_list_t* source, linked_list_t* dest, node_t* node);
static int swap_nodes(linked_list_t* list, node_t* a, node_t* b);

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
  assert(list);

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
  assert(list);

  linked_list_clear(list);

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
  assert(list);
  assert(data);

  node_t* node = NULL;
  
  if ((node = malloc(sizeof(node_t))) == NULL) {
    return -1;
  }

  node->data = data;

  if (attach_node(list, node) == -1) {
    free(node);

    return -1;
  }

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
  assert(list);
  assert(data);

  for (node_t* node = list->head; node != NULL; node = node->next) {
    if (node->data == data) {
      if (detach_node(list, node) == -1) {
        return -1;
      }

      free(node);

      return 0;
    }
  }

  return -1;
}

/**
 * @brief Removes nodes from a linked list that match a given predicate.
 * 
 * @param list The linked list to remove from.
 * @param data Contextual data for the predicate.
 * @param predicate The predicate to use.
 * @return 0 on success, -1 on failure.
 */
int linked_list_remove_if(linked_list_t *list, void* data, bool (*predicate)(void *, void*)) {
  assert(list);
  assert(predicate);

  node_t* node = list->head;

  while (node != NULL) {
    if (!predicate(node->data, data)) {
      node = node->next;
      
      continue;
    }

    node_t* next = node->next;

    if (detach_node(list, node) == -1) {
      return -1;
    }

    free(node);

    node = next;
  }

  return 0;
}

/**
 * @brief Transfer nodes from one linked list to another when the predicate is true.
 * 
 * @param list The linked list to remove from.
 * @param dest The linked list to transfer to.
 * @param data Contextual data for the predicate.
 * @param predicate The predicate to use.
 * @return 0 on success, -1 on failure.
 */
int linked_list_steal_if(linked_list_t *list, linked_list_t* dest, void* data, bool (*predicate)(void *, void*)) {
  assert(list);
  assert(dest);
  assert(predicate);

  node_t* node = list->head;

  while (node != NULL) {
    if (!predicate(node->data, data)) {
      node = node->next;
      
      continue;
    }

    node_t* next = node->next;

    if (steal_node(list, dest, node) == -1) {
      return -1;
    }

    node = next;
  }

  return 0;
}

/**
 * @brief Transfer the node at the given index to another linked list.
 * 
 * @param source The linked list to remove from.
 * @param dest The linked list to transfer to.
 * @param index The index of the node to transfer.
 * @return 0 on success, -1 on failure.
 */
int linked_list_steal(linked_list_t* source, linked_list_t* dest, size_t index) {
  assert(source);
  assert(dest);

  node_t* node = NULL;

  if ((node = node_at(source, index)) == NULL) {
    return -1;
  }

  if (steal_node(source, dest, node) == -1) {
    return -1;
  }

  return 0;
}

/**
 * @brief Sorts nodes in a linked list by a given comparator.
 * 
 * @param list The linked list to sort.
 * @param compare The comparator to use.
 * @return 0 on success, -1 on failure.
 */
int linked_list_sort(linked_list_t *list, int (*compare)(const void *, const void *)) {
  assert(list);
  assert(compare);
}

/**
 * @brief Gets the size of a linked list.
 * 
 * @param list The linked list to get the size of.
 *
 * @return The size of the linked list.
 */
int linked_list_size(linked_list_t *list) {
  assert(list);

  return list->size;
}

/**
 * @brief Clears a linked list.
 * 
 * @param list The linked list to clear.
 *
 * @return 0 on success, -1 on failure.
 */
int linked_list_clear(linked_list_t *list) {
  assert(list);

  node_t* node = list->head;

  while (node != NULL) {
    node_t* next = node->next;
    free(node);
    node = next;
  }

  list->head = NULL;
  list->tail = NULL;
  list->size = 0;

  return 0;
}

/**
 * @brief Gets the data from a linked list at a given index.
 * 
 * @param list The linked list to get the data from.
 * @param index The index to get the data from.
 * @return The data at the given index.
 */
void* linked_list_at(linked_list_t *list, size_t index) {
  assert(list);
  assert(index < list->size);

  node_t* node = list->head;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return node->data;
}

/**
 * @brief Module internal function to get a node at a given index.
 * 
 * @param list The linked list to get the node from.
 * @param index The index to get the node from.
 * @return The node at the given index.
 */
node_t* node_at(linked_list_t* list, size_t index) {
  assert(list);
  assert(index < list->size);

  node_t* node = list->head;

  for (size_t i = 0; i < index; i++) {
    node = node->next;
  }

  return node;
}

/**
 * @brief Module internal function to attach a node to a linked list.
 * 
 * @param list The linked list to attach the node to.
 * @param node The node to attach.
 * @return 0 on success, -1 on failure.
 */
int attach_node(linked_list_t* list, node_t* node) {
  assert(list);
  assert(node);

  if (list->size == 0) {
    list->head = node;
    list->tail = node;

    node->prev = NULL;
  } else {
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
  }

  node->next = NULL;

  list->size++;

  return 0;
}

/**
 * @brief Module internal function to detach a node from a linked list.
 * 
 * @param list The linked list to detach the node from.
 * @param node The node to detach.
 * @return 0 on success, -1 on failure.
 */
int detach_node(linked_list_t* list, node_t* node) {
  assert(list);
  assert(node);

  if (node->prev != NULL) {
    node->prev->next = node->next;
  } else {
    list->head = node->next;
  }

  if (node->next != NULL) {
    node->next->prev = node->prev;
  } else {
    list->tail = node->prev;
  }

  node->prev = NULL;
  node->next = NULL;

  list->size--;

  return 0;
}

/**
 * @brief Module internal function to move a node to a different linked list.
 * 
 * @param source The source linked list.
 * @param dest The destination linked list.
 * @param node The node to move.
 * @return 0 on success, -1 on failure.
 */
int steal_node(linked_list_t* source, linked_list_t* dest, node_t* node) {
  assert(source);
  assert(dest);

  if (detach_node(source, node) == -1) {
    return -1;
  }

  if (attach_node(dest, node) == -1) {
    return -1;
  }

  return 0;
}

/**
 * @brief Module internal function to swap two nodes.
 * 
 * @param list The linked list to swap the nodes in.
 * @param a The first node to swap.
 * @param b The second node to swap.
 * @return 0 on success, -1 on failure.
 */
int swap_nodes(linked_list_t* list, node_t* a, node_t* b) {
  assert(list);
  assert(a);
  assert(b);

  if (a == b) {
    return 0;
  }

  if (a->prev != NULL) {
    a->prev->next = b;
  } else {
    list->head = b;
  }

  if (b->next != NULL) {
    b->next->prev = a;
  } else {
    list->tail = a;
  }

  if (a->next != NULL) {
    a->next->prev = b;
  }

  if (b->prev != NULL) {
    b->prev->next = a;
  }

  node_t* tmp = a->next;
  a->next = b->next;
  b->next = tmp;

  tmp = a->prev;
  a->prev = b->prev;
  b->prev = tmp;

  return 0;
}
