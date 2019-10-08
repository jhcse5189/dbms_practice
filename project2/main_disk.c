#include "bpt_disk.h"

// MAIN

int main( int argc, char ** argv ) {

  int64_t key;
  /* Q. why not char value[120]? */
  char * value;
  char instruction;

  int table_id = open_table("databass");

  printf("> ");
  while (scanf("%c", &instruction) != EOF) {
    switch (instruction) {
      case 'd':
        scanf("%ld", &key);
        root = db_delete(key);
        break;
      case 'i':
        scanf("%d %s", &key, value);
        root = db_insert(key, value);
        break;
      case 'f':
        scanf("%ld", &key);
        db_find(key);
        break;
      case 'q':
        while (getchar() != (int)'\n');
        return EXIT_SUCCESS;
        break;
    }
    while (getchar() != (int)'\n');
    printf("> ");
  }
  printf("\n");
  return EXIT_SUCCESS;
}

/* fread();
 * - memory addr.
 * - number of bytes to read per block.
 * - number of blocks to read
 * - file variable
 */

 /*
 printf("sizeof(record): %ld\n", sizeof(record));
 printf("sizeof(node) must be 4096 Bytes: %ld\n", sizeof(node));
 printf("sizeof(header_page_t): %ld\n", sizeof(header_page_t));
 printf("sizeof(free_page_t): %ld\n\n", sizeof(free_page_t));

 root = (node*)malloc(sizeof(struct node));
 root->pairs.internal[0].key = 10;
 root->pairs.internal[0].pointer = 0x8000;
 root->pairs.internal[1].pointer = 0x1000;
 printf("%ld %lx | %ld %lx | %ld %lx\n",
         root->pairs.internal[0].key, &root->pairs.internal[0].key,
         root->pairs.internal[0].pointer, &root->pairs.internal[0].pointer,
         root->pairs.internal[1].pointer, &root->pairs.internal[1].pointer);

 root->pairs.records[0].key = 5;
 root->pairs.records[0].value[0] = 65;
 root->pairs.records[1].value[0] = 66;
 printf("%ld %lx | %lc %lx | %lc %lx\n",
         root->pairs.records[0].key, &root->pairs.records[0].key,
         root->pairs.records[0].value[0], root->pairs.records[0].value,
         root->pairs.records[1].value[0], root->pairs.records[1].value);
 printf("%ld %lx\n", root->pairs.internal[0].key, &root->pairs.internal[0].key);
 */
