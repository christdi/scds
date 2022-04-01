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

#ifndef SCDS_LINKED_LIST_H
#define SCDS_LINKED_LIST_H

#include <stdbool.h>

/**
 * Typedefs
 */
typedef struct linked_list_node linked_list_node_t;
typedef int (*compare_func_t)(const void *, const void *);

/**
 * Structs
**/

/**
 * Represents a node in a linked list.
 */
typedef struct linked_list_node {
    void *data;
    linked_list_node_t *next;
    linked_list_node_t *prev;
} linked_list_node_t;

/**
 * Represents a linked list.
 */
typedef struct linked_list {
    linked_list_node_t *head;
    linked_list_node_t *tail;
    size_t size;
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
int linked_list_remove_if(linked_list_t *list, void* data, bool (*predicate)(void *, void*));
int linked_list_steal_if(linked_list_t *list, linked_list_t* dest, void* data, bool (*predicate)(void *, void*));
int linked_list_steal(linked_list_t* source, linked_list_t* dest, size_t index);
int linked_list_sort(linked_list_t *list, compare_func_t compare);
int linked_list_clear(linked_list_t *list);

#endif
