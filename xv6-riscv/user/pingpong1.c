#include "kernel/types.h"
#include "user/user.h"

int main() {
  int p[2];
  char byte = 'P';

  if (pipe(p) < 0) {
    fprintf(2, "pingpong1: pipe failed\n");
    exit(1);
  }

  int pid = fork();
  if (pid < 0) {
    fprintf(2, "pingpong1: fork failed\n");
    close(p[0]);
    close(p[1]);
    exit(1);
  }

  if (pid == 0) {
    if (read(p[0], &byte, 1) != 1) {
      fprintf(2, "pingpong1: child failed to read from pipe\n");
      exit(1);
    }
    close(p[0]);

    printf("%d: received ping, %c\n", getpid(), byte);

    byte = 'R';
    if (write(p[1], &byte, 1) != 1) {
      fprintf(2, "pingpong1: child failed to write to pipe\n");
      exit(1);
    }
    close(p[1]);
    exit(0);
  }

  if (write(p[1], &byte, 1) != 1) {
    fprintf(2, "pingpong1: parent failed to write to pipe\n");
    exit(1);
  }
  close(p[1]);

  // Wait until the child has read 'P', written 'R', and exited. Without
  // this synchronization, the parent could read its own 'P' from the pipe.
  wait(0);

  if (read(p[0], &byte, 1) != 1) {
    fprintf(2, "pingpong1: parent failed to read from pipe\n");
    exit(1);
  }
  close(p[0]);

  printf("%d: received pong, %c\n", getpid(), byte);
  exit(0);
}
