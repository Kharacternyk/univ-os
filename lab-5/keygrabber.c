#include <assert.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

#define PRINT(value, key)                                                      \
  if (value == KEY_##key)                                                      \
    assert(write(1, #key, 1));

int main(int argc, char **argv) {
  assert(argc > 1);
  int fd = open(argv[1], O_RDONLY);
  assert(fd > 0);

  struct input_event event;
  while (read(fd, &event, sizeof(struct input_event)) ==
         sizeof(struct input_event)) {
    if (event.type == EV_KEY && event.value == 1) {
      PRINT(event.code, A);
      PRINT(event.code, B);
      PRINT(event.code, C);
      PRINT(event.code, D);
      PRINT(event.code, E);
      PRINT(event.code, F);
      PRINT(event.code, G);
      PRINT(event.code, H);
      PRINT(event.code, I);
      PRINT(event.code, J);
      PRINT(event.code, K);
      PRINT(event.code, L);
      PRINT(event.code, M);
      PRINT(event.code, N);
      PRINT(event.code, O);
      PRINT(event.code, P);
      PRINT(event.code, Q);
      PRINT(event.code, R);
      PRINT(event.code, S);
      PRINT(event.code, T);
      PRINT(event.code, U);
      PRINT(event.code, V);
      PRINT(event.code, W);
      PRINT(event.code, X);
      PRINT(event.code, Y);
      PRINT(event.code, Z);
      if (event.code == KEY_SPACE)
        assert(write(1, " ", 1));
    }
  }
}
