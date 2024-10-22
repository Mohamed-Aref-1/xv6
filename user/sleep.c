#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    fprintf(2, "Error, You Should Put One and Only One argument\n");
  }
  int time = atoi(argv[1]);
  sleep(time);
}
