#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(2, "Usage: sleep ticks...\n");
    exit(1);
  }

  int time = atoi(argv[1]);
  if (time > 0) {
    pause(time);
  } else {
    fprintf(2, "Error: Ticks must be greater than 0\n");
    exit(1);
  }
  exit(0);
};
