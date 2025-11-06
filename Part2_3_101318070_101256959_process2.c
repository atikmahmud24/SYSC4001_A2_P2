#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int counter = 0;
    int cycle = 0;

    while (counter >= -500) {
        printf("Process 2 - Cycle number: %d", cycle);
        if (counter % 3 == 0) {
            printf(" - %d is a multiple of 3", counter);
        }
        printf("\n");
        counter--;
        cycle++;
        usleep(1000000);  // 1 second delay
    }
    printf("Process 2 terminated\n");
    return 0;
}