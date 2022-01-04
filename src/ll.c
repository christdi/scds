#include <assert.h>
#include <stdlib.h>

#include "scds/ll.h"

/**
 * Creates a new singly linked list.
 *
 * It is the responsibility of the caller to free the allocated memory.
 *
 * Returns a pointer to the newly allocated linked list.
**/
ll_t* new_ll_t() {
  ll_t* ll = calloc(1, sizeof *ll);

  return ll;
}

/**
 * Frees an allocated linked list.
**/
void free_ll_t(ll_t* ll) {
  assert(ll);

  if (ll) {
    free(ll);
  }
}
