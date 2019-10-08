#ifndef __BPT_H__
#define __BPT_H__

// Uncomment the line below if you are compiling on Windows.
// #define WINDOWS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef WINDOWS
#define bool char
#define false 0
#define true 1
#endif

#include <unistd.h>
#include <fcntl.h>

// Default order is 4. (Fixed order, Now.)
#define DEFAULT_ORDER 4
// return 0, if the program terminated successfully.
#define EXIT_SUCCESS 0
// One page size is 4096 Bytes.
#define PAGE_SIZE 4096


//means page number, not offset.
typedef uint64_t pagenum_t;


// TYPES.

/* One record requires 128 Bytes.
   Therefore maximum 31 records per one data page
   can be contained. */

typedef struct record {
  int64_t key;
  char value[120];
} record;



/*
 * Type representing a node in the disk-based B+ tree.
 * This type is general enough to serve for both
 * the leaf and the internal node.
 */

/* Leaf and Internal Page */
typedef struct page_t {

  /* Page Header is different between leaf and internal page
     ,but it requires 128 Bytes
     except records / indexing key and page pointer area. */

  struct page_t * parent;
  int is_leaf;
  int num_keys;
  char reserved[104]; //112 - 8 = 104
  struct page_t * right_or_indexing;

  /* Leaf Page contains the key-value records
     ,otherwise Internal Page contains the key-(indexing)pointer pairs. */

  /* union, it means a single variable, so same memory location. */

  union { //union tag;
    struct {
        int64_t key;
        struct page_t * pointer;
    } internal[248];
    struct record records[31];
  } pairs; //union variable;

} page_t;



/* Header Page requires 24 Bytes except reserved space. */
typedef struct header_page_t {

  pagenum_t free;
  pagenum_t root;
  int64_t num_pages;
  char reserved[4072];

} header_page_t;



/* Free Page requires 8 Bytes except not using space. */
typedef struct free_page_t {

  pagenum_t next;
  char reserved[4088];

} free_page_t;



// GLOBALS.

/* The order determines the maximum and minimum
 * number of entries (keys and pointers) in any
 * node. Every node has at most order - 1 keys and
 * and can have (roughly speaking) half that number
 * because it uses delayed merger when all keys in
 * the (leaf or internal) page have deleted.
 * ...
 */

extern int order;

/* file descriptor. */

extern int fd;

/* Header Page */

extern header_page_t header_page;

// FUNCTION PROTOTYPES.

// Output and utility.



/* file space manager (same as disk space manager) API */

// Allocate an on-disk page from the free page list
pagenum_t file_alloc_page( void );

// Free an on-disk page to the free page list
void file_free_page(pagenum_t pagenum);

// Read an on-disk page into the in-memory page structure(dest)
void file_read_page(pagenum_t pagenum, page_t * dest);

// Write an in-memory page(src) to the on-disk page
void file_write_page(pagenum_t pagenum, const page_t * src);

void set_header_free(pagenum_t);

void set_header_num_pages(pagenum_t);


int open_table( char * pathname );
int db_insert( int64_t key, char * value );
char * db_find( int64_t key, char * ret_val );
int db_delete( int64_t key );
void db_exit( void );



void init_header_page( void );

void find_and_print(page_t * root, int64_t key);
record * find( page_t * root, int key );

// Insertion.

page_t * insert( page_t * root, int key, char * value );

// Deletion.

page_t * delete(page_t * root, int key);

#endif /* __BPT_H__*/
