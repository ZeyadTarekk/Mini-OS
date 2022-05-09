#include "headers.h"

/* Modify this file as needed*/
int remainingtime;

int main(int agrc, char *argv[]) {
    initClk();
    int previous_time, current_time;

    // DONE: it needs to get the remaining time from somewhere
    remainingtime = atoi(argv[1]);

    // DONE: Set the start time
    previous_time = getClk();

    while (remainingtime > 0) {

        // DONE: if the start time is less than the current time decrement the remaining time by one
        current_time = getClk();

        if (previous_time < current_time) {
            remainingtime--;
            previous_time = current_time;
            // TODO: TESTING (Comment)
            printf("Remaining time is %d\nPrevious time is %d \ncurrent time is %d\n", remainingtime, previous_time,
                   current_time);
        }

    }

    // DONE: When the remaining time is zero send a signal to the scheduler
     kill(getppid(), SIGUSR2);

    destroyClk(false);

    return 0;
}
