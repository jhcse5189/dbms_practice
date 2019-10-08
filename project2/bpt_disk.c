/*
 * bpt_disk.c
 */
#define Version "0.4"
/*
 *
 * bpt: disk-based B+Tree Implementation
 * Copyright (C) 2019 Hyeonseo Jo   https://github.com/jhcse5189
 * All rights reserved.
 *
 * Author:  Hyeonseo Jo
 *    https://github.com/jhcse5189
 * Original Date:   5 Oct 2019
 * Last modified:   9 Oct 2019
 *
 * This Implementation demonstrates the disk-based B+ tree data structure
 * for educational purposes, includin binary file I/O, insertion, search, and deletion.
 *
 */
#include "bpt_disk.h"

// GLOBALS.

// file descriptor.
int fd;

/* Header Page */
header_page_t header_page;

// FUNCTION DEFINITIONS.

// OUTPUT AND UTILITIES


// Allocate an on-disk page from the free page list
pagenum_t file_alloc_page( void ) {
  pagenum_t pn = 1;
  return pn;
}

// Free an on-disk page to the free page list
void file_free_page( pagenum_t pagenum ) {
  return;
}

// Read an on-disk page into the in-memory page structure(dest)
void file_read_page(pagenum_t pagenum, page_t * dest) {
  pread(fd, &dest, PAGE_SIZE, pagenum * PAGE_SIZE);
}

// Write an in-memory page(src) to the on-disk page
void file_write_page(pagenum_t pagenum, const page_t * src) {
    pwrite(fd, &src, PAGE_SIZE, pagenum * PAGE_SIZE);
}


// TODO: return unique table_id.
int open_table(char * pathname) {

  /* Q. Permission is required? */
  fd = open(pathname, O_RDWR | O_CREAT, 0666);

  atexit(db_exit);

  if (fd == -1) {
    perror("Fail to open table.");
    return fd;
  }

  // TODO: Header Page init "when the file doesn't have it".
  init_header_page();
  return fd;
}

void db_exit( void ) {

  close(fd);
  printf("bye\n");
}


// PLUS: use file manager API && ...
void init_header_page( void ) {

  int64_t num_pages;
  pread(fd, &num_pages, 1, 16);

  if (header_page.num_pages != 1) {

    header_page.free = 1;
    header_page.root = 0;
    header_page.num_pages = 1;

    pwrite(fd, &header_page, PAGE_SIZE, 0);
  }
}

// TODO: determine that it is really needed...
void set_header_free(pagenum_t n) { return; }
void set_header_num_pages(pagenum_t n) { return; }



void find_and_print( int64_t key ) {
  record * r = find(key);
  if (r == NULL)
    printf("Record not found under key %ld.\n", key);
  else
    printf("Record at %lx -- key %ld, value %s.\n",
            (unsigned long)r, key, r->value);
}

record * find( int key ) {
  printf("(inside the find function...)\n");
  record * r = NULL;
  return r;
}

char * db_find(int64_t key, char * ret_val) {
  char * c;
  return c;
}

// INSERTION

/* Master insertion function.
 * Inserts a key and an associated value into
 * the B+ tree, causing the tree to be adjusted
 * however necessary to maintain the B+ tree
 * properties.
 */

int db_insert(int64_t key, char * value) {

  return 0;
}

// DELETION

/* Master deletion function.
 *
 */

int db_delete(int64_t key) {
  return 1;
}
