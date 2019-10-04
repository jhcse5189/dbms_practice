/*
 * bpt_disk.c
 */
#define Version "0.1"
/*
 *
 * bpt: B+Tree Implementation
 * Copyright (C) 2019 Hyeonseo Jo   https://github.com/jhcse5189
 * All rights reserved.
 *
 * Author:  Hyeonseo Jo
 *    https://github.com/jhcse5189
 * Original Date:   5 October 2019
 * Last modified:   5 October 2019
 *
 * This Implementation demonstrates the disk-based B+ tree data structure
 * for educational purposes, includin binary file I/O, insertion, search, and deletion.
 *
 */
#include "bpt_disk.h"

// GLOBALS.

// FUNCTION DEFINITIONS.

// OUTPUT AND UTILITIES

void find_and_print( node * root, int key ) {
  record * r = find(root, key);
  if (r == NULL)
    printf("Record not found under key %d.\n", key);
  else
    printf("Record at %lx -- key %d, value %d.\n",
            (unsigned long)r, key, r->value);
}

record * find( node * root, int key ) {
  printf("(inside the find function...)\n");
  record * r = NULL;
  return r;
}

// INSERTION

/* Master insertion function.
 * Inserts a key and an associated value into
 * the B+ tree, causing the tree to be adjusted
 * however necessary to maintain the B+ tree
 * properties.
 */

node * insert( node * root, int key, int value ) {
  printf("(inside the master insert function...)\n");
  return root;
}

// DELETION

node * delete(node * root, int key) {
  printf("(inside the master delete function...)\n");
  return root;
}
