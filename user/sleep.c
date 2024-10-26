#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    // Check if the correct number of arguments is provided
    if (argc != 2) {
        fprintf(2, "Error: You must provide exactly one argument for the sleep time.\n");
        exit(1);  // Exit with an error code
    }

    // Convert the argument to an integer (sleep duration in seconds)
    int time = atoi(argv[1]);
    if (time < 0) {
        fprintf(2, "Error: Time must be a non-negative integer.\n");
        exit(1);
    }

    // Call sleep with the specified duration
    sleep(time);
    
    // Exit successfully
    exit(0);
}
