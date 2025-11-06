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
        int counter = 0;
        while (1) {
            printf("Process 2 [PID=%d]: counter = %d\n", getpid(), counter++);
            usleep(1000000);  // 1 second delay
        }
    } 
    else {
        int counter = 0;
        while (1) {
            printf("Process 1 [PID=%d]: counter = %d\n", getpid(), counter++);
            usleep(1000000);  // 1 second delay
        }
    }

    return 0;
}