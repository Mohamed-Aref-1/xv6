#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    int p[2];
    char buf[20];
    
    // 创建管道
    pipe(p);

    // 同时进行读和写操作（错误）
    write(p[1], "hello", 6); // 写入数据
    read(p[0], buf, 6);      // 从同一管道读取

    // 如果同时进行读和写，可能会出现数据错乱或阻塞
    write(p[1], "world", 6); // 再次写入
    read(p[0], buf, 6);      // 再次读取

    printf("Read from pipe: %s\n", buf);

    return 0;
}
