#include <stdio.h>
#include <unistd.h>

int main() {
  char t;
  char c;
  printf("%d\n%p\n", getpid(), &c);
  while (read(0, &t, 1) > 0)
    if (t != '\n')
      c = t;
}
