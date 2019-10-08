#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int open_table(char * pathname) {

  //fd means file descriptor.
  int fd = open("bin", O_RDWR | O_CREAT);
  int table_id;

  if (fd < 0) {
    printf("fd is -1 after open() func.\n");
    return -1;
  }

  if (write(fd, "output", 0x08) == -1) {
    write(2, "There was an error writing to testfile.txt\n", 43);
    return -1;
  }

  printf("successfully open and write to file::fd:%d\n", fd);
  table_id = 1;
  return table_id;
}

int main ( int argc, char ** argv ) {

  open_table("bin");
  printf("successfully terminate the main() func.\n");
  return 0;
}
