#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum state {
  thinking = '.',
  hungry = '_',
  eating = '$',
};

struct philosopher {
  struct drand48_data random;
  sem_t *global_semaphore;
  sem_t own_semaphore;

  struct philosopher *left_neighbour;
  struct philosopher *right_neighbour;

  const enum state *state;
  void (*change_state)(enum state state, void *data);
  void *data;
};

void do_stuff(struct philosopher *philosopher) {
  long random;
  mrand48_r(&philosopher->random, &random);
  unsigned int seconds = random;
  seconds %= 4;
  sleep(seconds);
}

void philosopher_main(struct philosopher *philosopher) {
  for (;;) {
    do_stuff(philosopher);

    assert(sem_wait(philosopher->global_semaphore) == 0);

    if (*philosopher->left_neighbour->state != eating &&
        *philosopher->right_neighbour->state != eating) {
      philosopher->change_state(eating, philosopher->data);
    } else {
      philosopher->change_state(hungry, philosopher->data);
    }

    assert(sem_post(philosopher->global_semaphore) == 0);

    if (*philosopher->state == hungry) {
      assert(sem_wait(&philosopher->own_semaphore) == 0);
      assert(sem_wait(philosopher->global_semaphore) == 0);
      philosopher->change_state(eating, philosopher->data);
      assert(sem_post(philosopher->global_semaphore) == 0);
    }

    do_stuff(philosopher);

    assert(sem_wait(philosopher->global_semaphore) == 0);
    philosopher->change_state(thinking, philosopher->data);

    if (*philosopher->left_neighbour->state == hungry &&
        *philosopher->left_neighbour->left_neighbour->state != eating) {
      assert(sem_post(&philosopher->left_neighbour->own_semaphore) == 0);
    }
    if (*philosopher->right_neighbour->state == hungry &&
        *philosopher->right_neighbour->right_neighbour->state != eating) {
      assert(sem_post(&philosopher->right_neighbour->own_semaphore) == 0);
    }

    assert(sem_post(philosopher->global_semaphore) == 0);
  }
}

void *philosopher_main_wrapper(void *philosopher) {
  philosopher_main(philosopher);
  return NULL;
}

struct change_state_data {
  enum state *states;
  size_t n;
  size_t i;
};

void change_state(enum state state, struct change_state_data *data) {
  data->states[data->i] = state;

  for (size_t i = 0; i < data->n; ++i) {
    printf(" %c", data->states[i]);
  }

  printf("\n");
}

void change_state_wrapper(enum state state, void *data) {
  change_state(state, data);
}

int main() {
  size_t n;

  assert(scanf("%zd", &n) == 1);

  struct philosopher *philosophers = calloc(n, sizeof(struct philosopher));
  struct change_state_data *data = calloc(n, sizeof(struct change_state_data));
  enum state *states = calloc(n, sizeof(enum state));

  sem_t global_semaphore;
  assert(sem_init(&global_semaphore, 0, 1) == 0);

  for (size_t i = 0; i < n; ++i) {
    long seed;

    assert(scanf("%ld", &seed) == 1);
    srand48_r(seed, &philosophers[i].random);

    states[i] = thinking;

    data[i].i = i;
    data[i].n = n;
    data[i].states = states;

    philosophers[i].data = data + i;
    philosophers[i].change_state = change_state_wrapper;
    philosophers[i].global_semaphore = &global_semaphore;
    philosophers[i].left_neighbour =
        i ? philosophers + i - 1 : philosophers + n - 1;
    philosophers[i].right_neighbour =
        i == n - 1 ? philosophers : philosophers + i + 1;
    philosophers[i].state = states + i;

    assert(sem_init(&philosophers->own_semaphore, 0, 0) == 0);
  }

  pthread_t *threads = calloc(n, sizeof(pthread_t));

  for (size_t i = 0; i < n; ++i) {
    assert(pthread_create(threads + i, NULL, philosopher_main_wrapper,
                          philosophers + i) == 0);
  }

  for (size_t i = 0; i < n; ++i) {
    assert(pthread_join(threads[i], NULL) == 0);
  }

  pthread_exit(NULL);
}
