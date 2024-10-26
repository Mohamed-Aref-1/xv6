#include <kernel/types.h>
#include <user/user.h>
#include <kernel/stat.h>

// This program uses two pipes to demonstrate inter-process communication (IPC) between a parent and a child process.
// The parent sends a "ping" message to the child, and the child responds with a "pong" message.
//
// p1[2] - Pipe from parent to child
// p2[2] - Pipe from child to parent

int main(int argc, char* argv[])
{
    int p1[2]; // Pipe for parent -> child communication
    int p2[2]; // Pipe for child -> parent communication
    pipe(p1);
    pipe(p2);
    char buf[1]; // Buffer to hold the byte for communication

    if (fork() == 0) {
        // Child process
        close(p1[1]); // Close write end of parent-to-child pipe
        close(p2[0]); // Close read end of child-to-parent pipe

        // Read message from parent
        read(p1[0], buf, 1);
        fprintf(1, "%d: received ping\n", getpid()); // Print "ping" received

        // Send response back to parent
        write(p2[1], buf, 1);

        // Close pipes and exit child process
        close(p1[0]);
        close(p2[1]);
        exit(0);

    } else {
        // Parent process
        close(p1[0]); // Close read end of parent-to-child pipe
        close(p2[1]); // Close write end of child-to-parent pipe

        buf[0] = 'A'; // Initialize buffer with any byte to send
        write(p1[1], buf, 1); // Send "ping" to child

        // Read response from child
        read(p2[0], buf, 1);
        fprintf(1, "%d: received pong\n", getpid()); // Print "pong" received

        // Close pipes and wait for child to complete
        close(p1[1]);
        close(p2[0]);
        wait(0); // Wait for child process to exit
    }

    exit(0);
}
