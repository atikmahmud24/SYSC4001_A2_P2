#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/sem.h>

#define SHM_KEY 0x1111
#define SEM_KEY 0x2222

typedef struct {
    int multiple;
    int counter;
} sharedvar_t;

void sem_op(int semid, short val) {
    struct sembuf op = {0, val, 0};
    semop(semid, &op, 1);
}
int main(void) {
    int shmid = shmget(SHM_KEY, sizeof(sharedvar_t), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        return 1;
    }

    sharedvar_t *data = (sharedvar_t *)shmat(shmid, NULL, 0);
    data->multiple = 3;
    data->counter = 0;
  
    int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    semctl(semid, 0, SETVAL, 1);

    pid_t pid = fork();
    if (pid < 0) { 
      perror("Fork failed, child process is not created"); 
      return 1;
    }

    else if (pid == 0) {
        // Process 2
        execl("./process2", "process2", NULL);
        /* While compiling, Part2_3_101318070_101256959_process2.c must be outputted to process2.c */
        perror("exec failed"); //Fail safe
        return 1;
    }
    else{
        while (1) {
            sem_op(semid, -1);
            data->counter++;
            sem_op(ssemid, +1);
            if (data->counter % data->multiple == 0)
                printf("Process 1 Counter %d is multiple of %d\n", data->counter, data->multiple);
            if (data->counter > 500) {
                printf("Process 1 Counter > 500, terminating.\n");
                kill(pid, SIGTERM);
                shmdt(data);
                shmctl(shmid, IPC_RMID, NULL);
                semctl(semid, 0, IPC_RMID);
                usleep(1000000);
            }
        }
    }
    return 0;
}
