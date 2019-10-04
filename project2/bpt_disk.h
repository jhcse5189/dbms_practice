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

typedef struct record {
  int key;
  // *** value mush be implemented in string type!
  int value;
} record;


typedef struct node {
  void ** pointers;
  int * keys;
  struct node * parent;
  bool is_leaf;
  int num_keys;
  struct node * next;
} node;

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

void find_and_print(node * root, int key);
record * find( node * root, int key );

// Insertion.

node * insert( node * root, int key, int value );

// Deletion.

node * delete(node * root, int key);

#endif /* __BPT_H__*/
