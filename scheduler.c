#include "headers.h"

void getProcess(int);


int main(int argc, char * argv[])
{
    initClk();

    //add signal handler to get the processes from process_generator
    signal(SIGUSR1, getProcess);

    //intialize the message queue
    //intializeMessageQueue();

    //get the algorithm number
    int algorithm = atoi(argv[1]);
    printf("The algorithm chosen is: %d\n", algorithm);

    while (1)
    {
        
    }
    printf("out of while\n");
    //TODO implement the scheduler :)
    //upon termination release the clock resources.
    
    destroyClk(true);
}

void getProcess(int signum) {
    printf("i am ready to receive\n");

    //receive from the message queue and add to the ready queue
}