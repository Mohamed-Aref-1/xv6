#include <kernel/types.h>
#include <user/user.h>
#include <kernel/stat.h>
// #include <fcntl.h>

int main(int argc, char* argv[])
{
  int p[2];
  pipe(p);
  char* argvs[2];
  argvs[0] = "cat";
  argvs[1] = 0;

  if (fork() == 0) {
    close(0);
    dup(p[0]);
    close(p[0]);
    close(p[1]);
    exec("cat", argvs);
  } else {
    write(p[1], "hello world\n", 12);
    close(p[1]);
    close(p[0]);
  }
}
