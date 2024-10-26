#include "kernel/types.h"
#include "user/user.h"

#define MAX_NUM 280     // Define maximum number to check for primes
#define READ 0          // File descriptor for reading end of the pipe
#define WRITE 1         // File descriptor for writing end of the pipe

#define stdin 0         // Standard input file descriptor
#define stdout 1        // Standard output file descriptor
#define stderr 2        // Standard error file descriptor
#define INT_SIZE 4      // Size of an integer in bytes

// Function declaration to create a new process chain, used for filtering primes
int new_proc(int left[]) __attribute__((noreturn));

// Recursive function to create a process for each prime found
// This function reads numbers from the 'left' pipe, filters multiples of the current prime,
// and passes non-multiples to the next process through a new 'right' pipe
int new_proc(int left[])
{
    int prime, temp, pid;
    int right[2];

    // Close write end of the input pipe and read the first number (prime candidate)
    close(left[WRITE]);
    if (read(left[READ], &prime, INT_SIZE) <= 0) {
        close(left[READ]);
        exit(0); // Exit if no data is read
    }
    
    printf("prime %d\n", prime); // Print the prime number found

    // Create a new pipe to pass numbers to the next process in the chain
    pipe(right);

    // Fork a new process to handle further filtering
    if ((pid = fork()) < 0) {
        // Handle fork error
        close(right[WRITE]);
        close(right[READ]);
        close(left[READ]);
        fprintf(stderr, "primes: fork failed\n");
        exit(-1);
    } else if (pid > 0) {
        // Parent process: filter and pass non-divisible numbers to the next process
        close(right[READ]);
        while (read(left[READ], &temp, INT_SIZE) > 0) {
            if ((temp % prime) != 0) {
                write(right[WRITE], &temp, INT_SIZE); // Pass to next process if not divisible
            }
        }
        // Close pipes and wait for the child process to finish
        close(right[WRITE]);
        close(left[READ]);
        wait(0);
        exit(0);
    } else {
        // Child process: recursively call new_proc for the next prime filter
        new_proc(right);
        close(right[READ]);
        close(right[WRITE]);
        close(left[READ]);
        exit(0);
    }
}

// Main function initializes the pipe and starts the prime number filtering process
int main(int argc, char* argv[])
{
    int p[2], pid;
    pipe(p); // Initialize the initial pipe for prime number generation

    if ((pid = fork()) < 0) {
        // Handle fork error
        close(p[WRITE]);
        close(p[READ]);
        exit(1);
    } else if (pid > 0) {
        // Parent process: generate numbers from 2 to MAX_NUM and send to child process
        close(p[READ]);
        for (int i = 2; i <= MAX_NUM; i++) {
            write(p[WRITE], &i, INT_SIZE); // Write each number to the pipe
        }
        close(p[WRITE]);
        wait(0); // Wait for child process to complete
        exit(0);
    } else {
        // Child process: initiate the prime filtering process
        new_proc(p);
        close(p[READ]);
        close(p[WRITE]);
        exit(0);
    }
    return 0;
}
