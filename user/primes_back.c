#include "kernel/types.h"
#include "user/user.h"

#define MAX_NUM 280

#define stdin 0
#define stdout 1
#define stderr 2

#define READ 0
#define WRITE 1
#define INT_SIZE 4

void sieveProc(int* left) __attribute__((noreturn));

void sieveProc(int* left)
{
  close(left[WRITE]);
  int prime, temp, pid, right[2];

  if (read(left[READ], &prime, INT_SIZE) == 0) {
    close(left[READ]);
    exit(0);
  }
  fprintf(stdout, "prime %d\n", prime);
  pipe(right);

  if ((pid = fork()) < 0) {
    fprintf(stderr, "failed to fork");
    close(right[READ]);
    close(right[WRITE]);
    close(left[READ]);
    exit(1);
  } else if (pid > 0) {
    close(right[READ]);
    while (read(left[READ], &temp, INT_SIZE) > 0) {
      if (temp % prime != 0) {
        write(right[WRITE], &temp, INT_SIZE);
      }
    }
    close(left[READ]);    // 确保关闭左侧读取端
    close(right[WRITE]);  // 关闭右侧写入端
    wait(0);
    exit(0);
  } else {  // child process
    sieveProc(right);
    exit(0);
  }
}

int main(int argc, char* argv[])
{
  int pid, fd_pair[2];
  pipe(fd_pair);
  if ((pid = fork()) < 0) {
    fprintf(stderr, "primes: fork failed\n");
    close(fd_pair[WRITE]);
    close(fd_pair[READ]);
    exit(-1);
  } else if (pid > 0) {
    close(fd_pair[READ]);
    for (int i = 2; i <= MAX_NUM; i++) {
      write(fd_pair[WRITE], &i, INT_SIZE);
    }
    close(fd_pair[WRITE]);
    wait(0);
    exit(0);
  } else {
    sieveProc(fd_pair);
    close(fd_pair[WRITE]);
    close(fd_pair[READ]);
    exit(0);
  }
}
