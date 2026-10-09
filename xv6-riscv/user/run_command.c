#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

void execute_line(char** args) {

  // Use fork() to create a child process. Determine if the process
  // is a parent or a child, and run exec() if it is a child, and
  // run wait() if it is a parent.
  int f = fork();
  if (f < 0) {
    fprintf(2, "run_command: fork failed\n");
    return;
  } else if (f == 0) {
    exec(args[0], args);
    fprintf(2, "run_command: exec failed for %s\n", args[0]);
    exit(1);
  } else {
    wait(0);
  }
}


int main(int argc, char *argv[]) {

  // Two pointers: to a command and to a null terminator.
  char *args[2] = {0};
  // Fixed-size storage for reading in a line of input, and a pointer
  // to a particular character in this storage.
  char buf[512], *b;
  // Point b to the first character in the line.
  b = buf;
  args[0] = buf;

  // Read characters from the standard input. When we find '\n' that
  // means one line has terminated - execute the command.
  for (;;) {
    b = buf;

    fprintf(2, "Please enter command and press enter (type 'q' to exit):\n");
    while (read(0, b, 1) > 0) {
      // Check the current character in b. If it is not special, move the pointer
      // b to the next character and continue reading. If the command string has
      // been fully entered ('\n' is found in b), execute the command by calling
      // execute_line().
      if (*b == '\n') {
        *b = '\0';

        if (strcmp(buf, "q") == 0) {
          exit(0);
        } else if (buf[0] != '\0') {
          execute_line(args);
        }
        break;
      }

      if (b < buf + sizeof(buf) - 1)
        b++;
    }
  }
}
