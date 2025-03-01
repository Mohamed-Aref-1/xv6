#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(const char* dir, const char* name)
{
  int           fd;
  struct stat   st;
  struct dirent de;
  char          buf[512], *p;

  if ((fd = open(dir, 0)) < 0) {
    printf("find: cannot open %s\n", dir);
    return;
  }

  if (fstat(fd, &st) < 0) {
    printf("find: cannot stat %s\n", dir);
    close(fd);
    return;
  }

  if (st.type != T_DIR) {
    printf("find: %s is not a directory\n", dir);
    close(fd);
    return;
  }

  // 将目录路径复制到 buf 并准备构造完整路径
  strcpy(buf, dir);
  p    = buf + strlen(buf);
  *p++ = '/';

  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0) continue;

    // 跳过 "." 和 ".."
    if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

    // 构造完整的文件路径
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if (stat(buf, &st) < 0) {
      printf("find: cannot stat %s\n", buf);
      continue;
    }

    if (st.type == T_FILE && strcmp(de.name, name) == 0) {
      printf("%s\n", buf);
    } else if (st.type == T_DIR) {
      find(buf, name);  // 递归搜索子目录
    }
  }

  close(fd);
}

int main(int argc, char* argv[])
{
  if (argc != 3) {
    printf("Usage: find <directory> <filename>\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
