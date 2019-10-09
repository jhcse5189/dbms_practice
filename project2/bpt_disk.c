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
  file_free_page( header_page.free );
  return header_page.free;
}

// Free an on-disk page to the free page list
void file_free_page( pagenum_t pagenum ) {
  header_page.free = pagenum + 1;
  free_page_t * tmp_free = (free_page_t*)malloc(sizeof(free_page_t));
  file_write_page( header_page.free, (page_t*)tmp_free );
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

  fd = open(pathname, O_RDWR | O_CREAT, 0666);
  atexit(db_exit);

  if (fd == -1) {
    perror("open");
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

  if ( get_header_num_pages() != 1 ) {

    header_page.free = 1;
    header_page.root = 0;
    header_page.num_pages = 1;

    //free_page_t * tmp_free = (free_page_t*)malloc(sizeof(free_page_t));
    //tmp_free->next = header_page.free + 1;

    pwrite(fd, &header_page, PAGE_SIZE, HEADER_PAGE_OFFSET);
    //pwrite(fd, tmp_free, PAGE_SIZE, PAGE_SIZE * header_page.free);
  }
}

// TODO: determine that it is really needed...

pagenum_t get_header_root( void ) {
  pagenum_t root;
  pread(fd, &root, sizeof(pagenum_t), HEADER_PAGE_ROOT_OFFSET);
  return root;
}

int64_t get_header_num_pages( void ) {
  int64_t num_pages;
  pread(fd, &num_pages, sizeof(int64_t), HEADER_PAGE_NUM_PAGES_OFFSET);
  return num_pages;
}

void set_header_free(pagenum_t n) { return; }
void set_header_root( pagenum_t root ) {
  pwrite(fd, &root, sizeof(pagenum_t), HEADER_PAGE_ROOT_OFFSET);
}
void set_header_num_pages( int64_t n ) {
  return;
}



void find_and_print( int64_t key ) {
  // record * r = find(key);
  // if (r == NULL)
  //   printf("Record not found under key %ld.\n", key);
  // else
  //   printf("Record at %lx -- key %ld, value %s.\n",
  //           (unsigned long)r, key, r->value);
}

record * find( int key ) {
  printf("(inside the find function...)\n");
  record * r = NULL;
  return r;
}

char * db_find(int64_t key, char * ret_val) {

  char * value = ret_val;

  if (header_page.root == 0)
    return value = NULL;

  value = "not null";
  return value;
}

// INSERTION

/* Master insertion function.
 * Inserts a key and an associated value into
 * the B+ tree, causing the tree to be adjusted
 * however necessary to maintain the B+ tree
 * properties.
 */
int db_insert(int64_t key, char * value) {

  //0. already has the pair...?
  if (db_find(key, value) != NULL)
    return -1; //yes. you can't insert. //ret_val is "not null", currentlly.
  //nope. you can insert.


  //1. find the right place to insert.
  //2. make a record to insert has the key-value pair.



  //3-1. root page dosen't exist yet.
  //   -> make root and write.

  // f_read_p of header page.
  // read header_page's root num -> 0.
  if ( get_header_root() == 0 ) {

    page_t * root = (page_t*)malloc(sizeof(page_t));

    root->node.is_leaf = 1;
    ++root->node.num_keys;
    root->node.pairs.records[0].key = key;

    pwrite(fd, &key, sizeof(pagenum_t), PAGE_SIZE * 1 + 128);
    pwrite(fd, value, strlen(value), PAGE_SIZE * 1 + 128 + 8);
    root->node.pairs.records[0].value[0] = value;
    file_write_page(file_alloc_page(), root);

    set_header_root(1);
    free(root);
  }

  // f_alloc_p to
  //    calc. pagenum_t by header page's free
  //    alloc free page.
  // (... issue.) header page isn't included to page_t type...

  // get a free page and,
  // make a leaf page (as root) and then,
  // write using f_write_p.


  //3-2. root page already exists.
  //     -> leaf page has space for a record.
  //  OR -> leaf page must be split first.



  return 0;
}

// DELETION

/* Master deletion function.
 *
 */
int db_delete(int64_t key) {
  return 1;
}
