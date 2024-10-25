Lab one note

dup() 复制一个已经有的文件描述符

`p[0]`标准输入 读取管道传递的数据
`p[1]`标准输出 把数据写入管道

lab: pipes
Your goal is to use pipe and fork to set up the pipeline.
The first process feeds the numbers 2 through 35 into the pipeline.
For each prime number, you will arrange to create one process that reads from its left neighbor over a pipe and writes to its right neighbor over another pipe.
Since xv6 has limited number of file descriptors and processes, the first process can stop at 35.

new_proc(int left[]) -> 2

```c
int main(int argc, char* argv[]) {
    int p[2];
    pipe(p);
    for (int i = 2; i <= MAX_NUM; i++) {
        new_proc(p)

    }

}
```

<++>

```c
int new_proc(int left[]) {
    int prime temp ;

}


```

Some hints:

Be careful to close file descriptors that a process doesn't need, because otherwise your program will run xv6 out of resources before the first process reaches 35.
Once the first process reaches 35, it should wait until the entire pipeline terminates, including all children, grandchildren, &c. Thus the main primes process should only exit after all the output has been printed, and after all the other primes processes have exited.
Hint: read returns zero when the write-side of a pipe is closed.
It's simplest to directly write 32-bit (4-byte) ints to the pipes, rather than using formatted ASCII I/O.
You should create the processes in the pipeline only as they are needed.
Add the program to UPROGS in Makefile.

O_RDONLY,
O_WRONLY,
O_RDWR,
O_CREATE,
O_TRUNC,
which instruct open to open the file for reading, or for writing,
or for both reading and writing, to create the file if it doesn’t exist,
and to truncate the file to zero length.

OS is too damn hard

But I believe i could made it
Let's go

iLet say if you worry about the future you will not make today sas as good day
