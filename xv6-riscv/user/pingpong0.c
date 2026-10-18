#include "kernel/types.h"
#include "user/user.h"

int main() {
  // Declare two pipes
  // p2c - a parent to a child communication
  // c2p - a child to a parent communication
  int p2c[2];
  int c2p[2];

  // Call the pipe() system call to create p2c and c2p
  if (pipe(p2c) < 0) {
    fprintf(2, "pingpong0: failed to create parent-to-child pipe\n");
    exit(1);
  }
  if (pipe(c2p) < 0) {
    fprintf(2, "pingpong0: failed to create child-to-parent pipe\n");
    close(p2c[0]);
    close(p2c[1]);
    exit(1);
  }

  // Declare one byte for sending to the child process
  char our_byte = 'P';

  // Fork a new process
  int pid = fork();
  if (pid > 0) {
    close(p2c[0]);
    close(c2p[1]);

    // 1. Send a byte to the child process.
    if (write(p2c[1], &our_byte, 1) != 1) {
      fprintf(2, "pingpong0: parent failed to write to pipe\n");
      exit(1);
    }
    close(p2c[1]);

    // 5. Receive the modified byte.
    if (read(c2p[0], &our_byte, 1) != 1) {
      fprintf(2, "pingpong0: parent failed to read from pipe\n");
      exit(1);
    }
    close(c2p[0]);

    printf("%d: received pong, %c\n", getpid(), our_byte);
    wait(0);
  }
  else if (pid == 0) {
    close(p2c[1]);
    close(c2p[0]);

    // 2. Read a byte from the parent-to-child pipe.
    if (read(p2c[0], &our_byte, 1) != 1) {
      fprintf(2, "pingpong0: child failed to read from pipe\n");
      exit(1);
    }
    close(p2c[0]);

    printf("%d: received ping, %c\n", getpid(), our_byte);

    // 3. Modify the byte to letter 'R'.
    our_byte = 'R';

    // 4. Send the byte back over the child-to-parent pipe.
    if (write(c2p[1], &our_byte, 1) != 1) {
      fprintf(2, "pingpong0: child failed to write to pipe\n");
      exit(1);
    }
    close(c2p[1]);
  }
  else {
    fprintf(2, "pingpong0: fork failed\n");
    close(p2c[0]);
    close(p2c[1]);
    close(c2p[0]);
    close(c2p[1]);
    exit(1);
  }

  exit(0);
}
