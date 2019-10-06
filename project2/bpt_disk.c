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


// Allocate an on-disk page from the free page list
pagenum_t file_alloc_page( void ) {
  pagenum_t pn = 1;
  return pn;
}

// Free an on-disk page to the free page list
void file_free_page(pagenum_t pagenum) {
  return;
}

// Read an on-disk page into the in-memory page structure(dest)
void file_read_page(pagenum_t pagenum, page_t * dest) {
  return;
}

// Write an in-memory page(src) to the on-disk page
void file_write_page(pagenum_t pagenum, const page_t * src) {
  return;
}



int open_table(char * pathname) {
  return 1;
}

int db_insert(int64_t key, char * value) {
  return 1;
}

char * db_find(int64_t key, char * ret_val) {
  char * c;
  return c;
}

int db_delete(int64_t key) {
  return 1;
}



header_page_t * header_init( void ) {

  header_page_t * tmp = (header_page_t*)malloc(sizeof(header_page_t));
  tmp->free = 0x1000;
  tmp->root = NULL;
  tmp->num_pages = 1;
  return tmp;
}


void find_and_print( page_t * root, int64_t key ) {
  record * r = find(root, key);
  if (r == NULL)
    printf("Record not found under key %ld.\n", key);
  else
    printf("Record at %lx -- key %ld, value %s.\n",
            (unsigned long)r, key, r->value);
}

record * find( page_t * root, int key ) {
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

page_t * insert( page_t * root, int key, int value ) {
  printf("(inside the master insert function...)\n");
  return root;
}

// DELETION

page_t * delete(page_t * root, int key) {
  printf("(inside the master delete function...)\n");
  return root;
}
