#include <assert.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <unistd.h>

int f(int x) {
  sleep(10);
  return x;
}

int g(int x) { return x; }

void child_main(int fd, int (*compute)(int)) {
  int x;
  assert(read(fd, &x, sizeof x) == sizeof x);
  x = compute(x);
  assert(write(fd, &x, sizeof x) == sizeof x);
}

int main() {
  int f_fds[2];
  assert(socketpair(AF_UNIX, SOCK_STREAM, 0, f_fds) == 0);

  int g_fds[2];
  assert(socketpair(AF_UNIX, SOCK_STREAM, 0, g_fds) == 0);

  pid_t f_pid = fork();
  assert(f_pid >= 0);
  if (f_pid == 0) {
    child_main(f_fds[1], f);
    return 0;
  }

  pid_t g_pid = fork();
  assert(g_pid >= 0);
  if (g_pid == 0) {
    child_main(g_fds[1], g);
    return 0;
  }

  int x;
  printf("x: ");
  if (scanf("%d", &x) != 1) {
    return 1;
  }

  write(f_fds[0], &x, sizeof x);
  write(g_fds[0], &x, sizeof x);
  struct pollfd pollfds[] = {
      {
          .fd = f_fds[0],
          .events = POLLIN,
      },
      {
          .fd = g_fds[0],
          .events = POLLIN,
      },
  };

  int results[2];
  int got_results = 0;
  int timeout = 1000;

  while (got_results < 2) {
    int status = poll(pollfds, 2, timeout);
    if (status > 0) {
      for (int i = 0; i < 2; ++i) {
        if (pollfds[i].revents & POLLIN) {
          read(pollfds[i].fd, results + i, sizeof results[i]);
          ++got_results;
          if (results[i] == 0) {
            got_results = 2;
          }
        }
      }
    } else {
      printf("How many seconds to wait? Enter a number, or ^D to stop: ");
      int seconds = 0;
      if (scanf("%d", &seconds) == EOF) {
        printf("\n");
        break;
      }
      timeout = seconds * 1000;
    }
  }

  kill(f_pid, SIGKILL);
  kill(g_pid, SIGKILL);

  if (got_results == 2) {
    printf("%d\n", results[0] * results[1]);
  }
}
