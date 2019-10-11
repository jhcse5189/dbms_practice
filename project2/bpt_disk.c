/*
 * bpt_disk.c
 */
#define Version "0.4"
/*
 *
 * bpt: disk-based B+Tree Implementation
 * Copyright (C) 2019 Hyeonseo Jo     https://github.com/jhcse5189
 * All rights reserved.
 *
 * Author:    Hyeonseo Jo
 *        https://github.com/jhcse5189
 * Original Date:     5 Oct 2019
 * Last modified:     9 Oct 2019
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
pagenum_t file_alloc_page(void) {

    free_page_t * tmp_empty = (free_page_t*)malloc(sizeof(free_page_t));
    memset(tmp_empty, 0, PAGE_SIZE);

    if (get_header_free() == 0)
        file_free_page(get_header_num_pages());

    pagenum_t empty_page_num = get_header_free();
    pread(fd, tmp_empty, PAGE_SIZE, PAGE_SIZE * empty_page_num);
    set_header_free(tmp_empty->next);

    page_t * empty_page = (page_t*)malloc(sizeof(page_t));
    memset(empty_page, 0, PAGE_SIZE);
    pwrite(fd, empty_page, PAGE_SIZE, PAGE_SIZE * empty_page_num);

    inc_header_num_pages();

    free(tmp_empty);
    free(empty_page);
    return empty_page_num;
}

// Free an on-disk page to the free page list
void file_free_page(pagenum_t pagenum) {

    free_page_t * free_page = (free_page_t*)malloc(sizeof(page_t));
    memset(free_page, 0, PAGE_SIZE);

    free_page->next = get_header_free();

    pwrite(fd, free_page, PAGE_SIZE, PAGE_SIZE * pagenum);
    set_header_free(pagenum);
}

// Read an on-disk page into the in-memory page structure(dest)
void file_read_page(pagenum_t pagenum, page_t * dest) {
    pread(fd, dest, PAGE_SIZE, pagenum * PAGE_SIZE);
}

// Write an in-memory page(src) to the on-disk page
void file_write_page(pagenum_t pagenum, const page_t * src) {
        pwrite(fd, src, PAGE_SIZE, pagenum * PAGE_SIZE);
}


void print_tree(pagenum_t n) {

    int i;
    page_t * tmp_n = (page_t*)malloc(sizeof(page_t));
    file_read_page(n, tmp_n);

    if (n == 0) {
        printf("Empty tree.\n");
        return;
    }

    if (tmp_n->node.is_leaf == 1) {

        for (i = 0; i < tmp_n->node.num_keys; i++)
            printf("%ld ", tmp_n->node.records[i].key);
        printf("| ");

        if (tmp_n->node.parent == 0
             || get_node_parent_num_keys(tmp_n->node.parent) == i) printf("\n");

    } else {

        for (i = 0; i < tmp_n->node.num_keys; i++)
            printf("%ld ", tmp_n->node.internal[i].key);
        printf("| ");

        if (tmp_n->node.parent == 0
             || get_node_parent_num_keys(tmp_n->node.parent) + 1 == i) printf("\n");

        print_tree (tmp_n->node.right_or_indexing);
        for  (i = 0; i < tmp_n->node.num_keys; i++)
            print_tree(tmp_n->node.internal[i].pointer);
    }
    free(tmp_n);
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
    if (pread(fd, &header_page, PAGE_SIZE, HEADER_PAGE_OFFSET) == 0)
        init_header_page();

    return fd;
}



void db_exit(void) {

    close(fd);
    printf("bye\n");
}


// PLUS: use file manager API && ...
void init_header_page(void) {

    //page_t * header_page = (page_t*)malloc(sizeof(page_t));
    //memset(header_page, 0, PAGE_SIZE);

    header_page.num_pages = 1;
    pwrite(fd, &header_page, PAGE_SIZE, HEADER_PAGE_OFFSET);

    //??? delete...//
    free_page_list.next = 1;
    //free(header_page);
}

pagenum_t get_header_free(void) {
    pagenum_t free;
    pread(fd, &free, sizeof(pagenum_t), HEADER_PAGE_FREE_OFFSET);
    return free;
}

pagenum_t get_header_root(void) {
    pagenum_t root;
    pread(fd, &root, sizeof(pagenum_t), HEADER_PAGE_ROOT_OFFSET);
    return root;
}

int64_t get_header_num_pages(void) {
    int64_t num_pages;
    pread(fd, &num_pages, sizeof(int64_t), HEADER_PAGE_NUM_PAGES_OFFSET);
    return num_pages;
}

void set_header_free(pagenum_t free) {
    pwrite(fd, &free, sizeof(pagenum_t), HEADER_PAGE_FREE_OFFSET);
}
void set_header_root(pagenum_t root) {
    pwrite(fd, &root, sizeof(pagenum_t), HEADER_PAGE_ROOT_OFFSET);
}
void set_header_num_pages(int64_t num_pages) {
    pwrite(fd, &num_pages, sizeof(int64_t), HEADER_PAGE_NUM_PAGES_OFFSET);
}

void inc_header_num_pages(void) {
    int64_t num_pages = get_header_num_pages() + 1;
    pwrite(fd, &num_pages, sizeof(int64_t), HEADER_PAGE_NUM_PAGES_OFFSET);
}
void dec_header_free_pages(void) {
    pagenum_t free_pages = get_header_free() - 1;
    pwrite(fd, &free_pages, sizeof(pagenum_t), HEADER_PAGE_FREE_OFFSET);
}

pagenum_t get_node_parent_num_keys (pagenum_t p) {

    page_t * tmp_p = (page_t*)malloc(sizeof(page_t));
    file_read_page(p, tmp_p);
    return tmp_p->node.num_keys;
}


void find_and_print(int64_t key) {
    // record * r = find(key);
    // if (r == NULL)
    //     printf("Record not found under key %ld.\n", key);
    // else
    //     printf("Record at %lx -- key %ld, value %s.\n",
    //                     (unsigned long)r, key, r->value);
}

record * find(int key) {
    printf("(inside the find function...)\n");
    record * r = NULL;
    return r;
}

/* return proper leaf page position to the key.
 * Notice, when using it in deletion.
 */
pagenum_t find_leaf(int64_t key) {

    // TODO: prevent to be called when root doesn't exist yet.
    int i;
    pagenum_t c = get_header_root();
    page_t * root = (page_t*)malloc(sizeof(page_t));
    file_read_page(c, root);

    while (!root->node.is_leaf) {

        i = 0;
        while (i < root->node.num_keys) {
            if (key >= root->node.internal[i].key) i++;
            else break;
        }

        if (i == 0) c = root->node.right_or_indexing;
        else c = root->node.internal[i - 1].pointer;

        file_read_page(c, root);
    }

    free(root);
    return c;
}


char * db_find(int64_t key, char * ret_val) {

    if (get_header_root() == 0)
        return NULL;

    int i;
    pagenum_t c = find_leaf(key);
    page_t * tmp = (page_t*)malloc(sizeof(page_t));
    file_read_page(c, tmp);

    for (i = 0; i < tmp->node.num_keys; i++)
        if (tmp->node.records[i].key == key) break;
    if (i == tmp->node.num_keys)
        return NULL;
    else
        return tmp->node.records[i].value;
}


int cut(int length) {
    if (length % 2 == 0)
        return length / 2;
    else
        return length / 2 + 1;
}


// INSERTION


int insert_into_node_after_splitting(pagenum_t old_node, int left_index, int64_t key, pagenum_t right) {

    int split, i , j;
    pagenum_t new_node = file_alloc_page();
    int64_t k_prime;
    
    page_t * tmp_new = (page_t*)malloc(sizeof(page_t));
    memset(tmp_new, 0, PAGE_SIZE);

    page_t * tmp_old = (page_t*)malloc(sizeof(page_t));
    memset(tmp_old, 0, PAGE_SIZE);

    page_t * origin = (page_t*)malloc(sizeof(page_t));
    file_read_page(old_node, origin);

    //left index is 'insertion point' as
    //insert_into_leaf_after_splitting.

    for (i = 0, j = 0; i < origin->node.num_keys; i++, j++) {
        if (j == left_index) j++;
        tmp_old->node.internal[j].key = origin->node.internal[i].key;
        tmp_old->node.internal[j].pointer = origin->node.internal[i].pointer;
    }

    tmp_old->node.internal[left_index].key = key;
    tmp_old->node.internal[left_index].pointer = right;

    split = cut(order - 1);
    //if order is 4, split is 2.
    //if order is 5, slpit is 2.
    origin->node.num_keys = 0;
    for (i = 0; i < split - 1; i++) {
        origin->node.internal[i].key = tmp_old->node.internal[i].key;
        origin->node.internal[i].pointer = tmp_old->node.internal[i].pointer;
        origin->node.num_keys++;
    }
    //origin->node.
    k_prime = tmp_old->node.internal[split - 1].key;
    for (++i, j = 0; i < order; i++, j++) {
        tmp_new->node.internal[j].key = tmp_old->node.internal[i].key;
        tmp_new->node.internal[j].pointer = tmp_old->node.internal[i].pointer;
        tmp_new->node.num_keys++;
    }
    //in tmp_old, k_prime's pointer.
    tmp_new->node.right_or_indexing = tmp_old->node.internal[split - 1].pointer;
    tmp_new->node.parent = tmp_old->node.parent;

    file_write_page(new_node, tmp_new);
    file_write_page(old_node, origin);
    
    free(tmp_new);
    free(tmp_old);
    free(origin);

    return insert_into_parent(old_node, k_prime, new_node);
}


int insert_into_node(pagenum_t n, int left_index, int64_t key, pagenum_t right) {

    int i;
    page_t * tmp_n = (page_t*)malloc(sizeof(page_t));
    file_read_page(n, tmp_n);

    for (i = tmp_n->node.num_keys; i > left_index; i--) {
        tmp_n->node.internal[i].key = tmp_n->node.internal[i - 1].key;
        tmp_n->node.internal[i].pointer = tmp_n->node.internal[i - 1].pointer;
    }
    tmp_n->node.internal[left_index].key = key;
    tmp_n->node.internal[left_index].pointer = right;
    tmp_n->node.num_keys++;

    file_write_page(n, tmp_n);

    free(tmp_n);
    return 0;
}

int get_left_index(pagenum_t parent, pagenum_t left) {

    printf("parent num is...%ld!\n", parent);

    page_t * tmp_parent = (page_t*)malloc(sizeof(page_t));
    file_read_page(parent, tmp_parent);

    printf("t_p->node.num_keys:%d\n", tmp_parent->node.num_keys);

    if (tmp_parent->node.right_or_indexing == left)
        return 0;

    int left_idx = 1;
    while (left_idx <= tmp_parent->node.num_keys &&
                        tmp_parent->node.internal[left_idx - 1].pointer != left)
        left_idx++;
    return left_idx;
}


int insert_into_new_root(pagenum_t left, int64_t key, pagenum_t right) {

    pagenum_t root;
    
    page_t * new_root = (page_t*)malloc(sizeof(page_t));
    memset(new_root, 0, PAGE_SIZE);

    page_t * tmp_left = (page_t*)malloc(sizeof(page_t));
    pread(fd, tmp_left, PAGE_SIZE, PAGE_SIZE * left);

    page_t * tmp_right = (page_t*)malloc(sizeof(page_t));
    pread(fd, tmp_right, PAGE_SIZE, PAGE_SIZE * right);

    new_root->node.parent = 0;
    new_root->node.is_leaf = 0;
    new_root->node.num_keys++;
    new_root->node.right_or_indexing = left;

    new_root->node.internal[0].key = key;
    new_root->node.internal[0].pointer = right;

    root = file_alloc_page();
    
    set_header_root(root);
    tmp_left->node.parent = root;
    tmp_right->node.parent = root;

    file_write_page(root, new_root);
    file_write_page(left, tmp_left);
    file_write_page(right, tmp_right);

    free(new_root);
    free(tmp_left);
    free(tmp_right);

    return 0;
}

int insert_into_parent(pagenum_t left, int64_t key, pagenum_t right) {

    printf("in insert_into_parent...left is %ld.\n", left);

    page_t * tmp_left = (page_t*)malloc(sizeof(page_t)); 
    file_read_page(left, tmp_left);

    int left_index;
    pagenum_t parent = tmp_left->node.parent;

    /* Case 1: new root. */

    if (parent == 0)
        return insert_into_new_root(left, key, right);


    /* Case 2: insert new_key from laaf or internal to existing parent page. */

    page_t * tmp_parent = (page_t*)malloc(sizeof(page_t));
    pread(fd, tmp_parent, PAGE_SIZE, PAGE_SIZE * tmp_left->node.parent);

    left_index = get_left_index(parent, left);

    free(tmp_left);

    /* 2-1 (simple.): the new_key fits into the order. */

    if (tmp_parent->node.num_keys < order - 1) {
        free(tmp_parent);
        return insert_into_node(parent, left_index, key, right);
    }

    /* 2-2. (hard.): split the parent node */

    return insert_into_node_after_splitting(parent, left_index, key, right);
}

int insert_into_leaf(pagenum_t leaf, int64_t key, char * value) {

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

    file_write_page(leaf, tmp);
    free(tmp);
    return 0;
}

/* Inserts a new key-value record
 * into the leaf page so as to exceed
 * the tree's order, causing the leaf page to be split
 * in helf.
 */
int insert_into_leaf_after_splitting(pagenum_t leaf, int64_t key, char * value) {

    int ip, split, i, j;
    pagenum_t new_leaf = file_alloc_page();
    int64_t new_key;

    page_t * tmp_new = (page_t*)malloc(sizeof(page_t));
    memset(tmp_new, 0, PAGE_SIZE);
    tmp_new->node.is_leaf = 1;

    page_t * tmp_leaf = (page_t*)malloc(sizeof(page_t));
    memset(tmp_leaf, 0, PAGE_SIZE);

    page_t * origin = (page_t*)malloc(sizeof(page_t));
    file_read_page(leaf, origin);


    ip = 0;
    while (ip < order - 1 && origin->node.records[ip].key < key)
        ip++;

    for (i = 0, j = 0; i < origin->node.num_keys; i++, j++) {
        if (j == ip) j++;
        tmp_leaf->node.records[j].key = origin->node.records[i].key;
        strcpy(tmp_leaf->node.records[j].value, origin->node.records[i].value);
    }

    tmp_leaf->node.records[ip].key = key;
    strcpy(tmp_leaf->node.records[ip].value, value);
    
    origin->node.num_keys = 0;

    split = cut(order - 1);
    //if order is 4, split is 2.
    //if order is 5, split is 3;

    for (i = 0; i < split; i++) {
        origin->node.records[i].key = tmp_leaf->node.records[i].key;
        strncpy(origin->node.records[i].value, "", 120);
        strcpy(origin->node.records[i].value, tmp_leaf->node.records[i].value);
        origin->node.num_keys++;
    }

    for (i = split, j = 0; i < order; i++, j++) {
        tmp_new->node.records[j].key = tmp_leaf->node.records[i].key;
        strcpy(tmp_new->node.records[j].value, tmp_leaf->node.records[i].value);
        tmp_new->node.num_keys++;
    }
    
    tmp_new->node.right_or_indexing = origin->node.right_or_indexing;
    origin->node.right_or_indexing = new_leaf;

    for (i = origin->node.num_keys; i < order - 1; i++) {
        origin->node.records[i].key = 0;
        strncpy(origin->node.records[i].value, "", 120);
    }
    // for (i = tmp_new->node.num_keys; i < order - 1; i++) {

    // }

    tmp_new->node.parent = origin->node.parent;

    file_write_page(new_leaf, tmp_new);
    file_write_page(leaf, origin);

    new_key = tmp_new->node.records[0].key;

    free(tmp_new);
    free(tmp_leaf);
    free(origin);

    return insert_into_parent(leaf, new_key, new_leaf);
}

int start_new_tree(int64_t key, char * value) {

    page_t * root = (page_t*)malloc(sizeof(page_t));
    memset(root, 0, PAGE_SIZE);

    root->node.is_leaf = 1;
    root->node.num_keys++;
    root->node.records[0].key = key;
    strcpy(root->node.records[0].value, value);

    file_write_page(file_alloc_page(), root);
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
    //     -> make root and write.

    // f_read_p of header page.
    // read header_page's root num -> 0.
    if (get_header_root() == 0)
        return start_new_tree(key, value);

    // f_alloc_p to
    //        calc. pagenum_t by header page's free
    //        alloc free page.
    // (... issue.) header page isn't included to page_t type...

    // get a free page and,
    // make a leaf page (as root) and then,
    // write using f_write_p.

    //3-2. root page already exists.
    //         -> leaf page has space for a record.
    //    OR -> leaf page must be split first.

    leaf = find_leaf(key);
    page_t * tmp = (page_t*)malloc(sizeof(page_t));
    pread(fd, tmp, PAGE_SIZE, PAGE_SIZE * leaf);

    if (tmp->node.num_keys < order - 1) {
        return insert_into_leaf(leaf, key, value);
    }

    return insert_into_leaf_after_splitting(leaf, key, value);
}

// DELETION

pagenum_t remove_entry_from_node(pagenum_t leaf_n, int64_t key) {

    int i;
    
    page_t * tmp_n = (page_t*)malloc(sizeof(page_t));
    file_read_page(leaf_n, tmp_n);

    i = 0;
    while (tmp_n->node.records[i].key != key)
        i++;
    for ( ++i; i < tmp_n->node.num_keys; i++ ) {
        tmp_n->node.records[i].key 
        tmp_n->node.
    }


    tmp_n->node.
}

pagenum_t adjust_root(pagenum_t root) {

}

int delete_entry(pagenum_t n, int64_t key) {

    //ddoururuk...
    int min_keys;

    // Remove key - value pair from the node.

    n = remove_entry_from_node(n, key);

    /* Case 1: deletion from the root. */

    if (n == get_header_root())
        return adjust_root(get_header_root());

    /* Case 2: deletion from a node below the root. */



    // DEADLINE -- DEADLINE -- DEADLINE -- DEADLINE -- DEADLINE



    //??? guji?
    /* EX. order = 4,
     * leaf      --> 
     * internal  --> 
     */
    min_keys = tmp_n->node.is_leaf ? cut(order - 1) : cut(order) - 1;

    /* 2-1 (simple.): node stays at or above minimum. */
    
    if (tmp_n->node.num_keys >= min_keys)
        return 0;
    
    /* 2-2 (hard.): node falls below minimum,
     * so coalescence or redistribution is needed.
     */

    print_tree("oh, shit~ let's merge~!\n");

    return 0;
}

/* Master deletion function.
 *
 */
int db_delete(int64_t key) {

    pagenum_t key_leaf;
    char key_value[120];

    strcpy(key_value, db_find(key, NULL));
    key_leaf = find_leaf(key);

    if (key_value != NULL && key_leaf != 0) {
        printf("key_leaf:%ld\n", key_leaf);
        return delete_entry(key_leaf, key);
    }
    return -1;
}
