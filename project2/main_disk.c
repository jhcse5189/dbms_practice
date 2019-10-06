#include "bpt_disk.h"

// MAIN

/* fread();
 * - memory addr.
 * - number of bytes to read per block.
 * - number of blocks to read
 * - file variable
 */

/* writes and then reads 10 arbitrary records
   from the file "junk". */

int main( int argc, char ** argv ) {

  char * filepath;
  FILE * fp;
  page_t * root;
  int input, valueOfKey;
  char instruction;

  root = NULL;

  header_page_t * hp = header_init();
  fp = fopen("databass", "w+");
  fwrite(hp, sizeof(*hp), 1, fp);


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

  printf("> ");
  while (scanf("%c", &instruction) != EOF) {
    switch (instruction) {
      case 'd':
        scanf("%d", &input);
        root = delete(root, input);
        printf("key: %d has transferred via 'delete' function.\n", input);
        break;
      case 'i':
        scanf("%d %d", &input, &valueOfKey);
        root = insert(root, input, valueOfKey);
        printf("key: %d, value: %d have transferred via 'insert' function.\n", input, valueOfKey);
        break;
      case 'f':
        scanf("%d", &input);
        find_and_print(root, input);
        break;
      case 'o':
        scanf("%s", filepath);
        fp = fopen(filepath, "w+");
        printf("o: successfully opened '%s' file and table_id is %d.\n",
                filepath, open_table(filepath));
        break;
      case 'q':
        while (getchar() != (int)'\n');
        printf("bye\n");
        return EXIT_SUCCESS;
        break;
    }
    while (getchar() != (int)'\n');
    printf("> ");
  }
  printf("\n");
  fclose(fp);

  return EXIT_SUCCESS;
}
