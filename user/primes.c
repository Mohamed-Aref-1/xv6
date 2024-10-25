#include "kernel/types.h"
#include "user/user.h"

#define MAX_NUM 280
#define READ 0
#define WRITE 1

#define stdin 0
#define stdout 1
#define stderr 2
#define INT_SIZE 4
int new_proc(int left[]) __attribute__((noreturn));
int new_proc(int left[])
{
  int prime, temp, pid;
  int right[2];
  close(left[WRITE]);
  if (read(left[READ], &prime, INT_SIZE) <= 0) {
    close(left[READ]);
    exit(0);
  }
  printf("prime %d\n", prime);

  pipe(right);

  if ((pid = fork()) < 0) {
    close(right[WRITE]);
    close(right[READ]);
    close(left[READ]);
    fprintf(stderr, "primes: fork failed\n");
    exit(-1);
  } else if (pid > 0) {
    close(right[READ]);
    while (read(left[READ], &temp, INT_SIZE) > 0) {
      if ((temp % prime) == 0) continue;
      write(right[WRITE], &temp, INT_SIZE);
    }
    close(right[WRITE]);
    close(left[READ]);
    wait(0);
    exit(0);
  } else {
    new_proc(right);
    close(right[READ]);
    close(right[WRITE]);
    close(left[READ]);
    exit(0);
  }
}

int main(int argc, char* argv[])
{
  int p[2], pid;
  pipe(p);
  if ((pid = fork()) < 0) {
    close(p[WRITE]);
    close(p[READ]);
    exit(1);
  } else if (pid > 0) {
    close(p[READ]);
    for (int i = 2; i <= MAX_NUM; i++) {
      write(p[WRITE], &i, INT_SIZE);
    }
    close(p[WRITE]);
    wait(0);
    exit(0);
  } else {
    new_proc(p);
    close(p[READ]);
    close(p[WRITE]);
    exit(0);
  }
  return 0;
}
