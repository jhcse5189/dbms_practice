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

/* order of tree */
int order = DEFAULT_ORDER;

// file descriptor.
int fd;

/* Free Page List */
free_page_t free_page_list;

/* Header Page */
header_page_t header_page;

// FUNCTION DEFINITIONS.

// OUTPUT AND UTILITIES


// Allocate an on-disk page from the free page list
pagenum_t file_alloc_page( void ) {

  if ( get_header_free() == 0 )
    file_free_page( get_header_num_pages() );

  pagenum_t empty_page_num = get_header_free();
  set_header_free(  );

  
  pread(fd, &, PAGE_SIZE, PAGE_SIZE * empty_page_num);

  page_t * empty_page = (page_t*)malloc(sizeof(page_t));
  memset(empty_page, 0, PAGE_SIZE);
  pwrite(fd, empty_page, PAGE_SIZE, PAGE_SIZE * empty_page_num);

  inc_header_num_pages();

  return empty_page_num;
}

// Free an on-disk page to the free page list
void file_free_page( pagenum_t pagenum ) {

  free_page_t * free_page = (free_page_t*)malloc(sizeof(page_t));
  memset(free_page, 0, PAGE_SIZE);

  free_page->next = get_header_free();

  pwrite(fd, free_page, PAGE_SIZE, PAGE_SIZE * pagenum);
  set_header_free( pagenum );
}

// Read an on-disk page into the in-memory page structure(dest)
void file_read_page(pagenum_t pagenum, page_t * dest) {
  pread(fd, dest, PAGE_SIZE, pagenum * PAGE_SIZE);
}

// Write an in-memory page(src) to the on-disk page
void file_write_page(pagenum_t pagenum, const page_t * src) {
    pwrite(fd, src, PAGE_SIZE, pagenum * PAGE_SIZE);
}




// TODO: init. just once!!!
int open_table(char * pathname) {

  fd = open(pathname, O_RDWR | O_CREAT, 0666);
  atexit(db_exit);

  if (fd == -1) {
    perror("open");
    return fd;
  }

  // TODO: Header Page init "when the file doesn't have it".
  if ( pread(fd, &header_page, PAGE_SIZE, HEADER_PAGE_OFFSET) == 0 )
    init_header_page();

  return fd;
}



void db_exit( void ) {

  close(fd);
  printf("bye\n");
}


// PLUS: use file manager API && ...
void init_header_page( void ) {

  //page_t * header_page = (page_t*)malloc(sizeof(page_t));
  //memset(header_page, 0, PAGE_SIZE);

  header_page.num_pages = 1;
  pwrite(fd, &header_page, PAGE_SIZE, HEADER_PAGE_OFFSET);

  //??? delete...//
  free_page_list.next = 1;
  //free(header_page);
}

pagenum_t get_header_free( void ) {
  pagenum_t free;
  pread(fd, &free, sizeof(pagenum_t), HEADER_PAGE_FREE_OFFSET);
  return free;
}

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

void set_header_free(pagenum_t free) {
  pwrite(fd, &free, sizeof(pagenum_t), HEADER_PAGE_FREE_OFFSET);
}
void set_header_root( pagenum_t root ) {
  pwrite(fd, &root, sizeof(pagenum_t), HEADER_PAGE_ROOT_OFFSET);
}
void set_header_num_pages( int64_t num_pages ) {
  pwrite(fd, &num_pages, sizeof(int64_t), HEADER_PAGE_NUM_PAGES_OFFSET);
}

void inc_header_num_pages( void ) {
  int64_t num_pages = get_header_num_pages() + 1;
  pwrite(fd, &num_pages, sizeof(int64_t), HEADER_PAGE_NUM_PAGES_OFFSET);
}
void dec_header_free_pages( void ) {
  pagenum_t free_pages = get_header_free() - 1;
  pwrite(fd, &free_pages, sizeof(pagenum_t), HEADER_PAGE_FREE_OFFSET);
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

pagenum_t find_leaf( int64_t key ) {

  // TODO: prevent to be called when root doesn't exist yet.
  int i = 0;
  pagenum_t c = get_header_root();
  page_t * tmp = (page_t*)malloc(sizeof(page_t));
  pread(fd, tmp, PAGE_SIZE, PAGE_SIZE * c);

  while (!tmp->node.is_leaf) {

    i = 0;
    while (i < tmp->node.num_keys) {
      if (key >= tmp->node.internal[i].key) i++;
      else break;
    }

    if (i == 0) c = tmp->node.right_or_indexing;
    else c = tmp->node.internal[i - 1].pointer;

    pread(fd, &tmp, PAGE_SIZE, PAGE_SIZE * c);
  }

  free(tmp);
  return c;
}


char * db_find(int64_t key, char * ret_val) {

  if (get_header_root() == 0)
    return NULL;

  int i;
  pagenum_t c = find_leaf( key );
  page_t * tmp = (page_t*)malloc(sizeof(page_t));
  file_read_page( c, tmp );

  for (i = 0; i < tmp->node.num_keys; i++)
    if (tmp->node.records[i].key == key) break;
  if (i == tmp->node.num_keys)
    return NULL;
  else
    return tmp->node.records[i].value;
}




// INSERTION

int insert_into_parent( pagenum_t leaf, int64_t new_key, pagenum_t new_leaf ) {

  printf("in insert_into_parent...\n");
  return 0;
}

int insert_into_leaf( pagenum_t leaf, int64_t key, char * value ) {

  int i, ip;
  page_t * tmp = (page_t*)malloc(sizeof(page_t));
  pread(fd, tmp, PAGE_SIZE, PAGE_SIZE * leaf);

  ip = 0;
  while (ip < tmp->node.num_keys && tmp->node.records[ip].key < key)
    ip++;

  for (i = tmp->node.num_keys; i > ip; i--) {
    tmp->node.records[i].key = tmp->node.records[i - 1].key;
    strcpy(tmp->node.records[i].value, tmp->node.records[i - 1].value);
  }

  tmp->node.records[ip].key = key;
  strcpy(tmp->node.records[ip].value, value);
  ++tmp->node.num_keys;

  file_write_page( leaf, tmp );
  free(tmp);
  return 0;
}

/* Inserts a new key-value record
 * into the leaf page so as to exceed
 * the tree's order, causing the leaf page to be split
 * in helf.
 */
int insert_into_leaf_after_splitting( pagenum_t leaf, int64_t key, char * value ) {

  pagenum_t new_leaf;
  int64_t new_key;

  file_alloc_page()

  printf("Ohooo~shit~ i must implement split~ bb...\n");


  new_key = new_leaf(page_t type)->node.records[0].key;

  return insert_into_parent( leaf, new_key, new_leaf );
}

int start_new_tree( int64_t key, char * value ) {

  page_t * root = (page_t*)malloc(sizeof(page_t));
  memset(root, 0, PAGE_SIZE);

  root->node.is_leaf = 1;
  ++root->node.num_keys;
  root->node.records[0].key = key;
  strcpy(root->node.records[0].value, value);

  //before insert the root, header page setting.
  set_header_free(1);
  file_free_page( get_header_free() );


  file_write_page( file_alloc_page(), root );
  set_header_root(1);

  free(root);
  return 0;
}

/* Master insertion function.
 * Inserts a key and an associated value into
 * the B+ tree, causing the tree to be adjusted
 * however necessary to maintain the B+ tree
 * properties.
 */
int db_insert(int64_t key, char * value) {

  pagenum_t leaf;

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
    printf("%ld\n", get_header_root());
    return start_new_tree( key, value );
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

  printf("case:root already...\n");

  leaf = find_leaf( key );
  page_t * tmp = (page_t*)malloc(sizeof(page_t));
  pread(fd, tmp, PAGE_SIZE, PAGE_SIZE * leaf);

  if ( tmp->node.num_keys < order - 1 ) {
    return insert_into_leaf( leaf, key, value );
  }

  return insert_into_leaf_after_splitting( leaf, key, value );
}

// DELETION

/* Master deletion function.
 *
 */
int db_delete(int64_t key) {
  return 1;
}
