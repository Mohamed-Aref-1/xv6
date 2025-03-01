#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXARGS 10
#define MAXLEN 512

int main(int argc, char* argv[])
{
  if (argc < 2) {
    printf("Usage: xargs <command> [args...]\n");
    exit(1);
  }

  char  buf[MAXLEN];
  char* cmd[MAXARGS];
  int   n;

  // 设置命令和初始参数
  for (int i = 1; i < argc && i < MAXARGS - 1; i++) {
    cmd[i - 1] = argv[i];
  }

  while ((n = read(0, buf, sizeof(buf) - 1)) > 0) {
    buf[n] = 0;  // 确保以 '\0' 结尾

    // 将每一行的换行符替换为 '\0'
    char* p = buf;
    char* newline;
    while ((newline = strchr(p, '\n')) != 0) {
      *newline = 0;

      // 复制基本命令参数并添加输入行作为最后一个参数
      int i      = argc - 1;
      cmd[i]     = p;
      cmd[i + 1] = 0;

      if (fork() == 0) {
        exec(cmd[0], cmd);
        printf("xargs: exec %s failed\n", cmd[0]);
        exit(1);
      }

      // 等待子进程完成
      wait(0);

      // 继续处理下一行
      p = newline + 1;
    }
  }

  exit(0);
}
