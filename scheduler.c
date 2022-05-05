#include "headers.h"

// global variables
int msgq_id;
struct msgbuff message;

void getProcess(int);

void intializeMessageQueue() {
    key_t key_id = ftok("keyfile", PROSCH);
    msgq_id = msgget(key_id, 0666 | IPC_CREAT);

    if (msgq_id == -1)
    {
        perror("Error in create message queue");
        exit(-1);
    }
}

//for testing
bool flag = true;


int main(int argc, char * argv[])
{
    //TODO implement the scheduler :)
    initClk();

    //add signal handler to get the processes from process_generator
    signal(SIGUSR1, getProcess);

    //intialize the message queue
    intializeMessageQueue();


    //get the algorithm number
    int algorithm = atoi(argv[1]);

    while (flag)
    {
        
    }
    
    //upon termination release the clock resources.
    destroyClk(false);

    return 0;
}

void getProcess(int signum) {
    //receive from the message queue and add to the ready queue
    int rec_val = msgrcv(msgq_id, &message, sizeof(message), 7, !IPC_NOWAIT);

    if (rec_val == -1)
        perror("Error in receive");

    printf("Schedular received process with id = %d\n", message.process.id);

    if (message.process.id == 3)
    {
        flag = false;
    }
}