#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();  // Creates a new process

    if (pid < 0) {
        perror("Fork failed, child process not created.");
        return 1;
    } 
    else if (pid == 0) {
        // Process 2
        execl("./process2", "process2", NULL);
        /* While compiling, Part2_3_101318070_101256959_process2.c must be outputted to process2.c */
        perror("exec failed"); // Fail-safe
        return 1;
    } 
    else {
        // Process 1
        int counter = 0;
        int cycle = 0;

        while (1) {
            printf("Process 1 - Cycle number: %d", cycle);
            if (counter % 3 == 0) {
                printf(" - %d is a multiple of 3", counter);
            }
            printf("\n");
            counter++;
            cycle++;
            usleep(1000000);  // 1 second delay
        }
    }

    return 0;
}