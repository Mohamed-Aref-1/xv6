#include <kernel/types.h>
#include <user/user.h>
#include <kernel/stat.h>
// 2 pipes
// p1[2] parent -> child
// p2[2] child -> parent
//
// parent -> byte -> child
// child: print "<pid>: received ping"
// parent: write the byte from child print "<pid>: received pong"
// wait()
// exit()
//
int main(int argc, char* argv[])
{
  int p1[2];
  int p2[2];
  pipe(p1);
  pipe(p2);
  char buf[1];
  if (fork() == 0) {
    close(p1[1]);
    close(p2[0]);
    read(p1[0], buf, 1);
    fprintf(1, "%d: received ping\n", getpid());
    write(p2[1], buf, 1);
    close(p1[0]);
    close(p2[1]);
    exit(0);
  } else {
    close(p1[0]);
    close(p2[1]);
    buf[0] = 'A';
    write(p1[1], buf, 1);
    read(p2[0], buf, 1);
    fprintf(1, "%d: received pong\n", getpid());
    close(p1[1]);
    close(p2[0]);
    wait(0);
  }
  exit(0);
}
