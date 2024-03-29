#ifndef __BPT_H__
#define __BPT_H__

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

// Default order is 4.
#define DEFAULT_ORDER 4
// return 0, if the program terminated successfully.
#define EXIT_SUCCESS 0
// One page size is 4096 Bytes.
#define PAGE_SIZE 4096
#define HEADER_PAGE_OFFSET 0
// Queue for just print tree.
#define QUEUE_SIZE 1000



// TYPES.



// means page number, not offset.
typedef uint64_t pagenum_t;



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



typedef struct node_t {
    /* Page Header is different between leaf and internal page
         ,but it requires 128 Bytes
         except records / indexing key and page pointer area. */

    pagenum_t parent; //because, it doesn't need union with free_page_t.
    int is_leaf;
    int num_keys;
    char reserved[104];
    pagenum_t right_or_indexing;

    /* Leaf Page contains the key-value records
         ,otherwise Internal Page contains the key-(indexing)pointer pairs. */

    /* union, it means a single variable, so using same memory location. */

    union {
        struct {
            int64_t key;
            pagenum_t pointer;
        } internal[248];
        struct {
            record records[31];
        };
    };

} node_t;



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



typedef union page_t {

    header_page_t header_page;
    free_page_t free_page;
    node_t node;

} page_t;



typedef struct Queue Queue;
struct Queue {
    int front, rear;
    int size;
    pagenum_t * pages;
};



// GLOBALS.

/* The order determines the maximum and minimum
 * number of entries (keys and pointers) in any
 * node. Every node has at most order - 1 keys and
 * and can have (roughly speaking) half that number
 * because it uses delayed merger when all keys in
 * the (leaf or internal) page have deleted.
 * ...
 */

/* order of tree */
extern int order;

/* file descriptor. */
extern int fd;

/* queue */
extern Queue * queue;



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



void init_queue(void);
int isEmpty(void);
int isFull(void);
void enqueue(pagenum_t n);
pagenum_t dequeue(void);
void print_tree();



int open_table( char * pathname );
void init_header_page( page_t * tmp_h );
void db_exit( void );

pagenum_t get_header_free ( void );
pagenum_t get_header_root( void );
int64_t get_header_num_pages( void );

void set_header_free( pagenum_t free );
void set_header_root( pagenum_t root );
void set_header_num_pages( int64_t num_pages );

void inc_header_num_pages( void );
void dec_header_free_pages( void );

pagenum_t get_node_parent_page(pagenum_t n);
pagenum_t get_node_parent_num_keys( pagenum_t p );

void set_node_parent( pagenum_t n, pagenum_t p );

record * find( int key );
pagenum_t find_leaf( int64_t key );
char * db_find( int64_t key, char * ret_val );

int cut(int length);



// Insertion.




int insert_into_node( pagenum_t n, int left_index, int64_t key, pagenum_t right );
int insert_into_node_after_splitting( pagenum_t parent, int left_index, int64_t key, pagenum_t right );

int insert_into_new_root( pagenum_t left, int64_t key, pagenum_t right );
int insert_into_parent( pagenum_t leaf, int64_t new_key, pagenum_t new_leaf );
int get_left_index( pagenum_t parent, pagenum_t left );

int insert_into_leaf( pagenum_t leaf, int64_t key, char * value );
int insert_into_leaf_after_splitting( pagenum_t leaf, int64_t key, char * value );
int start_new_tree( int64_t key, char * value );

int db_insert( int64_t key, char * value );

// Deletion.

int get_neighbor_index(pagenum_t n);
void remove_entry_from_node( pagenum_t n, int64_t key );
pagenum_t adjust_root( pagenum_t root );
int delete_entry( pagenum_t n, int64_t key );

int coalesce_nodes(pagenum_t n, pagenum_t neighbor, int neighbor_index, int64_t k_prime);
int redistribute_nodes(pagenum_t n, pagenum_t neighbor, int neighbor_index, int k_prime_index, int64_t k_prime);

int db_delete( int64_t key );

#endif /* __BPT_H__*/
