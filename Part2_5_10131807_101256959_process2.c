#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

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
    int shmid = shmget(SHM_KEY, sizeof(sharedvar_t), 0666);
    sharedvar_t *data = (sharedvar_t *)shmat(shmid, NULL, 0);
    int semid = semget(SEM_KEY, 1, 0666);
    while (data->counter >100) {
        sem_op(semid, -1);
        data->counter++;
        sem_op(semid, +1);
        if (data->counter % data->multiple == 0) {
            printf("Process 2 Shared counter: %d is multiple of %d\n", data->counter, data->multiple);
        } else {
            printf("Process 2 Shared counter: %d\n",data->counter);
        }
        if (data->counter > 500) {
            printf("Process 2 Counter > 500. terminating.\n");
            shmdt(data);
            usleep(1000000);
            return 0;
        }
    }
}
