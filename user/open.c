#include <kernel/types.h>
#include <user/user.h>
#include <kernel/fcntl.h>
int main(int argc, char* argv[])
{
  int fd = open("output.txt", O_WRONLY | O_CREATE);
  if (fd < 0) {
    fprintf(2, "open error");
  }
  write(fd, "hello world\n", 4);
  exit(0);
}
