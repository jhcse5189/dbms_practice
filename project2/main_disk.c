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
  node * root;
  int input, valueOfKey;
  char instruction;

  root = NULL;

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
        printf("o: successfully opened '%s' file.\n", filepath);
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
