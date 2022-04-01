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

#ifndef SCDS_HASH_TABLE_H
#define SCDS_HASH_TABLE_H

#include <stddef.h>

/**
 * Typedefs
**/
typedef struct hash_table_node hash_table_node_t;

/**
 * Structs
**/

/**
 * Represents a single node in a hash table bucket.
**/
typedef struct hash_table_node {
    char* key;
    void* value;
    hash_table_node_t* next;
} hash_table_node_t;

/**
 * Represents a bucket in a hash table using an internal linked list for conflicts.
**/
typedef struct hash_table_bucket {
    hash_table_node_t* head;
    hash_table_node_t* tail;
} hash_table_bucket_t;

/**
 * Represents a hash table with capacity buckets.
**/
typedef struct hash_table {
    hash_table_bucket_t** buckets;
    size_t capacity;
    size_t size;
} hash_table_t;

/**
 * Functions
**/
hash_table_t* hash_table_new(size_t capacity);
int hash_table_free(hash_table_t* table);

int hash_table_init(hash_table_t* table, size_t capacity);
int hash_table_destroy(hash_table_t* table);

int hash_table_set(hash_table_t* table, char* key, void* value);
void* hash_table_get(hash_table_t* table, char* key);
int hash_table_remove(hash_table_t* table, char* key);

#endif
