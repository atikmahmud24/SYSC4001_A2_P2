#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 0x1111

typedef struct {
    int multiple;
    int counter;
} sharedvar_t;

int main(void) {
    int shmid = shmget(SHM_KEY, sizeof(sharedvar_t), 0666);
    sharedvar_t *data = (sharedvar_t *)shmat(shmid, NULL, 0);

    while (data->counter <= 100)
        usleep(100000);

    while (1) {
        data->counter++;
        if (data->counter % data->multiple == 0) {
            printf("Process 2 Shared counter %d is multiple of %d\n", data->counter, data->multiple);
        } else {
            printf("Process 2 Shared counter %d\n",data->counter);
        if (data->counter > 500) {
            printf("Process 2 Counter > 500. terminating.\n");
            shmdt(data);
            usleep(1000000);
            return 0;
        }
    }
}
