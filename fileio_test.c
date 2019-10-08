#include <stdio.h>
#include <stdlib.h>


typedef struct record {
  int value;
} record;


int open_table( char * pathname );

int db_delete( int64_t key );

int db_insert( int64_t key, char * value);

char * db_find( int64_t key, char * ret_val);


int main( int argc, char ** argv ) {

    int64_t key;
    char * value;

    int table_id;
    char instruction;
    record r;

    table_id = open_table("junk");

    printf("> ");
    while (scanf("%c", &instruction) != EOF) {
      switch (instruction) {
        case 'd':
          scanf("%d", &key);
          db_delete(key);
          break;
        case 'i':
          open_table("junk");
          scanf("%d %s", &input, value);
          db_insert(key, value);
          break;
        case 'f':
          scanf("%d", &input);
          value = db_find(key);
          printf("%s\n", value);
          break;
        case 'q':
          while (getchar() != (int)'\n');
          printf("bye\n");
          return 0;
          break;
      }
      while (getchar() != (int)'\n');
      printf("> ");
    }
    printf("\n");
    return 0;
}

int open_table(char * pathname) {

  FILE * fp = fopen(pathname, "w+");
  return 1;
}

int db_delete( int64_t key ) {

  record r;
  return 1;
}

int db_insert( int64_t key, char * value) {

  record r;
  r.value = key;
  fwrite(&r, sizeof(record), 1);
  fclose(fp);

  return 1;
}

char * db_find( int64_t key, char * ret_val) {
  char * tmp;
  return tmp;
}

/*

    // create the file of 10 records
    fp = fopen("junk", "w");
    if (!fp)
      return 1;
    for (i = 1; i <= 11; i++) {
      r.value = i;
      fwrite(&r, sizeof(record), 1, fp);
    }
    fclose(fp);



    // read the 10 records
    fp = fopen("junk", "r");
    if (!fp)
      return 1;
    for (i = 1; i <= 11; i++) {
      fread(&r, sizeof(record), 1, fp);
      printf("%d ", r.value);
    }
    fclose(fp);
    printf("\n");

*/

/*
    //use fseek to read the 10 records in reverse order.

    fp = fopen("junk", "r");
    if (!fp)
      return 1;
    for (i = 10; i >= 0; i--) {
      fseek(fp, sizeof(record) * i, SEEK_SET);
      fread(&r, sizeof(record), 1, fp);
      printf("%d ", r.value);
    }
    fclose(fp);
    printf("\n");


    //use fseek to read the 10 record in reverse order

    fp = fopen("junk", "r");
    if (!fp)
      return 1;
    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < 5; i++) {
      fread(&r, sizeof(record), 1, fp);
      printf("%d ", r.value);
      fseek(fp, sizeof(record), SEEK_CUR);
    }
    fclose(fp);
    printf("\n");


    //use fseek to read 4th record, change it, and write it back

    fp = fopen("junk", "r+");
    if (!fp)
      return 1;
    fseek(fp, sizeof(record) * 3, SEEK_SET);
    fread(&r, sizeof(record), 1, fp);
    r.value = 100;

    fseek(fp, sizeof(record) * 3, SEEK_SET);
    fwrite(&r, sizeof(record), 1, fp);
    fclose(fp);
    printf("\n");


    //read the 10 records to insure 4th record was changed

    fp = fopen("junk", "r");
    if (!fp)
      return 1;
    for (i = 1; i <= 10; i++) {
      fread(&r, sizeof(record), 1, fp);
      printf("%d ", r.value);
    }
    fclose(fp);
    printf("\n");

*/
