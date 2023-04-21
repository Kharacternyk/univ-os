#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  assert(argc > 2);
  int fd = open(argv[1], O_RDONLY);
  assert(fd > 0);

  char *endptr = NULL;
  size_t address = strtol(argv[2], &endptr, 0);
  assert(!*endptr);

  assert(lseek(fd, address, SEEK_SET) >= 0);
  char byte;
  assert(read(fd, &byte, 1) == 1);
  assert(write(1, &byte, 1));
}
