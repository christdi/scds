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

#include "scds/hash_table.h"

#define BASE_HASH_VALUE 3128
#define FIVE_BITS 5

static unsigned int calculate_hash_index(hash_table_t* table, const char *key, size_t len);
static int add_node_to_bucket(hash_table_t* table, hash_table_node_t* node);

hash_table_t* hash_table_new(size_t capacity) {
    hash_table_t* hash_table = malloc(sizeof(hash_table_t));

    hash_table_init(hash_table, capacity);

    return hash_table;
}

int hash_table_free(hash_table_t* table) {
    assert(table);

    hash_table_destroy(table);

    free(table);

    return 0;
}

int hash_table_init(hash_table_t* table, size_t capacity) {
    assert(table);

    table->capacity = capacity;

    table->buckets = malloc(sizeof(hash_table_bucket_t*) * capacity);

    return 0;
}

int hash_table_destroy(hash_table_t* table) {
    for (size_t i = 0; i < table->capacity; i++) {
        hash_table_bucket_t* bucket = table->buckets[i];
        hash_table_node_t* next = NULL;

        for (hash_table_node_t* node = bucket->head; node != NULL; node = next) {
            next = node->next;

            free(node->key);
            free(node);
        }
    }

    free(table->buckets);    

    return 0;
}

int hash_table_set(hash_table_t* table, char* key, void* value) {
    return 0;
}

void* hash_table_get(hash_table_t* table, char* key) {
    return 0;
}

int hash_table_remove(hash_table_t* table, char* key) {
    return 0;
}

unsigned int calculate_hash_index(hash_table_t* table, const char *key, size_t len) {
    assert(key);

    unsigned long hash = BASE_HASH_VALUE;
    unsigned char c = 0;
    size_t i = 0;

    for (i = 0; i < len; i++) {        
        c = key[i];

        hash = ((hash << FIVE_BITS) + hash) + c;
    }

    return hash % table->capacity;
}

/**
 * @brief Adds a node to a bucket list at the calculated index.
 *
 * @param table The hash table to add the node to.
 * @param node The node to add to the table.
 *
 * @return 0 on success, -1 on failure.
 */
int add_node_to_bucket(hash_table_t* table, hash_table_node_t* node) {
    size_t index = calculate_hash_index(table, node->key, strlen(node->key));

    hash_table_bucket_t* bucket = table->buckets[index];

    if (bucket == NULL) {
        bucket = malloc(sizeof(hash_table_bucket_t));
        bucket->head = node;
        bucket->tail = node;
        bucket->size = 1;

        table->buckets[index] = bucket;
    } else {
        bucket->tail->next = node;
        bucket->tail = node;
        bucket->size++;
    }

    return 0;
}