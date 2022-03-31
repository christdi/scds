# SCDS

SCDS (Simple C Data Structures) is a library providing data structures for use in C.

## Linked List

    #include <scds/linked_list.h>

The SCDS Linked List is a doubly linked list and uses void pointers as the mechanism by which it stores data.

* Use `linked_list_new()` and `linked_list_free()` method to have the library allocate memory on the head

* Use `linked_list_init` and `linked_list_destroy` if allocating your own memory or if the list is on the stack.

* Sorting is implemented via quick sort.

* This data structure is not thread safe.
