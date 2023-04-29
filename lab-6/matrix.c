#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct argset {
  long *A;
  long *B;
  size_t i;
  size_t j;
  size_t m;
  size_t k;
  long *result;
};

static size_t get_shift(size_t i, size_t j, size_t width) {
  return i * width + j;
}

void multiply(struct argset *argset) {
  *argset->result = 0;
  for (size_t e = 0; e < argset->m; ++e) {
    *argset->result += argset->A[get_shift(argset->i, e, argset->m)] *
                       argset->B[get_shift(e, argset->j, argset->k)];
  }
  printf("A[%zd, *] * B[*, %zd] = %ld\n", argset->i, argset->j,
         *argset->result);
};

void *start(void *argset) {
  multiply(argset);
  return NULL;
}

int main(int argc, char **argv) {
  size_t n;
  size_t m;
  size_t k;
  long seed;

  assert(scanf("%zd %zd %zd %ld", &n, &m, &k, &seed) == 4);

  srand48(seed);

  long *A = calloc(n * m, sizeof(long));
  long *B = calloc(m * k, sizeof(long));
  long *C = calloc(n * k, sizeof(long));

  printf("A:\n");
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m; ++j) {
      long value = mrand48();
      A[get_shift(i, j, m)] = value;
      printf(" %ld", value);
    }
    printf("\n");
  }

  printf("B:\n");
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < k; ++j) {
      long value = mrand48();
      B[get_shift(i, j, k)] = value;
      printf(" %ld", value);
    }
    printf("\n");
  }

  struct argset *argsets = calloc(n * k, sizeof(struct argset));
  pthread_t *threads = calloc(n * k, sizeof(pthread_t));

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < k; ++j) {
      size_t shift = get_shift(i, j, k);
      argsets[shift].A = A;
      argsets[shift].B = B;
      argsets[shift].i = i;
      argsets[shift].j = j;
      argsets[shift].m = m;
      argsets[shift].k = k;
      argsets[shift].result = C + shift;

      assert(pthread_create(threads + shift, NULL, start, argsets + shift) ==
             0);
    }
  }

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < k; ++j) {
      assert(pthread_join(threads[get_shift(i, j, k)], NULL) == 0);
    }
  }

  printf("C:\n");
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < k; ++j) {
      printf(" %ld", C[get_shift(i, j, k)]);
    }
    printf("\n");
  }
}
