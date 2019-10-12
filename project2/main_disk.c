#include "bpt_disk.h"

// MAIN

int main(int argc, char ** argv) {

    int64_t key;
    // TODO: at Windows, change to char * value and run.;
    char value[120];
    char instruction;

    int table_id = open_table("databass");

    printf("> ");
    while (scanf("%c", &instruction) != EOF) {
        switch (instruction) {
            case 'd':
                scanf("%ld", &key);
                db_delete(key);
                print_tree();
                break;
            case 'i':
                scanf("%ld %s", &key, value);
                db_insert(key, value);
                print_tree();
                break;
            case 'f':
                scanf("%ld", &key);
                db_find(key, value);
                break;
            case 'q':
                while (getchar() != (int)'\n');
                return EXIT_SUCCESS;
                break;
            case 't':
                print_tree();
                break;
        }
        while (getchar() != (int)'\n');
        printf("> ");
    }
    printf("\n");
    return EXIT_SUCCESS;
}
