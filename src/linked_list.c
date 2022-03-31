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
#include <stdio.h>

#include "scds/linked_list.h"

static node_t* node_at(linked_list_t* list, size_t index);
static int attach_node(linked_list_t* list, node_t* node);
static int detach_node(linked_list_t* list, node_t* node);
static int steal_node(linked_list_t* source, linked_list_t* dest, node_t* node);
static int quick_sort(linked_list_t* list, node_t* start, node_t* end, compare_func_t compare);
static node_t* partition(linked_list_t* list, node_t* start, node_t* end, node_t** new_start, node_t** new_end, compare_func_t compare);
static int insert_after(linked_list_t* list, node_t* after, node_t* node);

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
int linked_list_sort(linked_list_t *list, compare_func_t compare) {
  assert(list);
  assert(compare);

  if (list->size <= 1) {
    return 0;
  }

  if (quick_sort(list, list->head, list->tail, compare) == -1) {
    return -1;
  }

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
 * @brief Module internal function to attach a node to the end of a linked list.
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
 * @brief Module internal function to sort a linked list using quick sort.
 * 
 * @param list The linked list to sort.
 * @param start The start node of the partition of the linked list.
 * @param end The end node of the partition of the linked list.
 * @param compare The comparator to use.
 * @return 0 on success, -1 on failure.
 */
int quick_sort(linked_list_t* list, node_t* start, node_t* end, compare_func_t compare) {
  assert(list);
  assert(start);
  assert(end);
  assert(compare);
  
  node_t* new_start= NULL;
  node_t* new_end = NULL;

  node_t* pivot = partition(list, start, end, &new_start, &new_end, compare);
  
  if (pivot == NULL) {
    return -1;
  }

  if (pivot != new_start) {
    if (quick_sort(list, new_start, pivot->prev, compare) == -1) {
      return -1;
    }
  }

  if (pivot != new_end) {
    if (quick_sort(list, pivot->next, new_end, compare) == -1) {
      return -1;
    }
  }

  return 0;
}

/**
 * @brief Module internal function to partition a segment of a linked list during quick sort.
 * 
 * @param list The linked list to partition.
 * @param start The start of the segment to partition.
 * @param end The end of the segment to partition.
 * @param new_start Out parameter containing the new start of the segment.
 * @param new_end Out parameter containing the new end of the segment.
 * @param compare The compare function to use. 
 * @return The pivot node.
 */
node_t* partition(linked_list_t* list, node_t* start, node_t* end, node_t** new_start, node_t** new_end, compare_func_t compare) {
  assert(list);
  assert(start);
  assert(end);
  assert(compare);

  if (start == end) {
    *new_start = start;
    *new_end = start;

    return start;
  }

  node_t* pivot = end;
  node_t* current = start;

  while (current != pivot) {
    node_t* next = current->next;

    if (compare(current->data, pivot->data) > 0) {
      if (detach_node(list, current) == -1) {
        return NULL;
      }

      if (insert_after(list, pivot, current) == -1) {
        return NULL;
      }

      if (*new_end == NULL) {
        *new_end = current;
      }    
    } else {
      if (*new_start == NULL) {
        *new_start = current;
      }
    }

    current = next;
  }

  if (*new_start == NULL) {
    *new_start = pivot;
  }

  if (*new_end == NULL) {
    *new_end = pivot;
  }

  return pivot;
}

/**
 * @brief Module internal function to insert a node after a given node.
 * 
 * @param list The linked list to insert the node into.
 * @param after The node to insert after. 
 * @param node  The node to insert. 
 * @return 0 on success, -1 on failure.
 */ 
int insert_after(linked_list_t* list, node_t* after, node_t* node) {
  assert(list);
  assert(after);
  assert(node);
  assert(node->prev == NULL);
  assert(node->next == NULL);

  if (after->next == NULL) {
    return attach_node(list, node);
  }

  node->next = after->next;
  node->prev = after;

  after->next = node;
  node->next->prev = node;

  list->size++;

  return 0;
}
