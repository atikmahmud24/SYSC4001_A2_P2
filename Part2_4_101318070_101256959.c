#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

#define SHM_KEY 0x1111

typedef struct {
    int multiple;
    int counter;
} sharedvar_t;

int main(void) {
    int shmid = shmget(SHM_KEY, sizeof(sharedvar_t), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        return 1;
    }

    sharedvar_t *data = (sharedvar_t *)shmat(shmid, NULL, 0);
    data->multiple = 3;
    data->counter = 0;

    pid_t pid = fork();
    if (pid < 0) { 
      perror("Fork failed, child process is not created"); 
      return 1;
    }

    if (pid == 0) {
        execl("./process2", "process2", NULL);
        perror("exec failed");
        return 1;
    }

    while (1) {
        data->counter++;
        if (data->counter % data->multiple == 0)
            printf("Process 1 Counter %d is multiple of %d\n", data->counter, data->multiple);
        if (data->counter > 500) {
            printf("Process 1 Counter > 500, terminating.\n");
            kill(pid, SIGTERM);
            shmdt(data);
            shmctl(shmid, IPC_RMID, NULL);
            usleep(1000000);
        }
    }
    return 0;
}
