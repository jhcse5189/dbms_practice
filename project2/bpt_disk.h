#ifndef __BPT_H__
#define __BPT_H__

// Uncomment the line below if you are compiling on Windows.
// #define WINDOWS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef WINDOWS
#define bool char
#define false 0
#define true 1
#endif

// Default order is 4. (Fixed order, Now.)
#define DEFAULT_ORDER 4

// return 0, if the program terminated successfully.
#define EXIT_SUCCESS 0

// TYPES.

/*
 *
 */

/* One record requires 128 Bytes
   ,therefore maximum 31 records per one data page
   can be contained. */

typedef struct record {
  int64_t key;
  char value[120];
} record;


/*
 * Type representing a node in the disk-based B+ tree.
 * This type is general enough to serve for both
 * the leaf and the internal node.
 * ...
 * sizeof (node pointer) is 8 bytes,
 * because it uses C's pointer size!
 */


/* Leaf and Internal Page */
typedef struct node {

  /* Page Header is different between leaf and internal page
     ,but it requires 128 Bytes
     except records / indexing key and page pointer area. */

  struct node * parent;
  int is_leaf;
  int num_keys;
  char reserved[104]; //112 - 8 = 104
  struct node * right_or_indexing;

  /* Leaf Page contains the key-value records
     ,otherwise Internal Page contains the key-(indexing)pointer pairs. */

  /* union, it means a single variable, so same memory location. */

  union { //union tag;
    struct {
        int64_t key;
        struct node * pointer;
    } internal[248];
    struct record records[31];
  } pairs; //union variable;

} node;


/* Header Page requires 24 Bytes except reserved space. */
typedef struct header_page_t {

  struct free_page_t * free;
  struct node * root;
  int num_pages;
  char reserved[4072];

} header_page_t;


/* Free Page requires 8 Bytes except not using space. */
typedef struct free_page_t {

  struct free_page_t * next;
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

// FUNCTION PROTOTYPES.

// Output and utility.

header_page_t * header_init( void );

int open_table(char * pathname);

void find_and_print(node * root, int64_t key);
record * find( node * root, int key );

// Insertion.

node * insert( node * root, int key, int value );

// Deletion.

node * delete(node * root, int key);

#endif /* __BPT_H__*/
