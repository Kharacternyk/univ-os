#include <assert.h>
#include <pthread.h>
#include <stdio.h>

void *start(void *argument) {
  printf("%s\n", (char *)argument);
  return NULL;
}

int main(int argc, char **argv) {
  char string[] = {0, 0};
  for (char i = '0'; i <= '9'; ++i) {
    string[0] = i;
    pthread_t thread;
    pthread_create(&thread, NULL, start, string);
    pthread_detach(thread);
  }
  pthread_exit(NULL);
}
