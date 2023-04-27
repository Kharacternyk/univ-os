#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>

struct counter {
  int value;
  bool is_synced;
  sem_t semaphore;
};

void *start(void *argument) {
  struct counter *counter = argument;

  for (int i = 0; i < 100000; ++i) {
    if (counter->is_synced) {
      sem_wait(&counter->semaphore);
    }

    ++counter->value;

    if (counter->is_synced) {
      sem_post(&counter->semaphore);
    }
  }

  return NULL;
}

int main(int argc, char **argv) {
  struct counter counter = {};

  if (argc > 1) {
    counter.is_synced = true;
    assert(sem_init(&counter.semaphore, 0, 1) == 0);
  }

  pthread_t threads[2];

  for (int i = 0; i < 2; ++i) {
    pthread_create(threads + i, NULL, start, &counter);
  }

  for (int i = 0; i < 2; ++i) {
    pthread_join(threads[i], NULL);
  }

  printf("%d\n", counter.value);
}
