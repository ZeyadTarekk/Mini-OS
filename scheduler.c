//#include "headers.h"
#include "SRTN.h"

// global variables
int msgq_id;
int algorithm;
struct msgbuff message;
struct PQueue *priority_queue;
struct Queue *queue;

void getProcess(int);

void intializeMessageQueue() {
    key_t key_id = ftok("keyfile", PROSCH);
    msgq_id = msgget(key_id, 0666 | IPC_CREAT);

    if (msgq_id == -1) {
        perror("Error in create message queue");
        exit(-1);
    }
}

//for testing
bool flag = true;


int main(int argc, char *argv[]) {
    //TODO implement the scheduler :)
    initClk();
    //add signal handler to get the processes from process_generator
    signal(SIGUSR1, getProcess);

    //intialize the message queue
    intializeMessageQueue();


    //get the algorithm number
    algorithm = atoi(argv[1]);

    switch (algorithm) {
        case 1:
            // Allocate the priority queue
            priority_queue = createPriorityQueue();

            // TODO: Add your algorithm call here (HPF)

            break;
        case 2:
            // Allocate the priority queue
            priority_queue = createPriorityQueue();

            // Call the algorithm function
            SRTN(priority_queue);

            break;
        case 3:
            // Allocate the queue
            queue = createQueue();

            // TODO: Add your algorithm call here (RR)

            break;
    }

    //upon termination release the clock resources.
    destroyClk(false);

    return 0;
}

void add_to_SRTN_queue(struct ProcessStruct process) {
    // Push to the queue and the priority is the runTime (The remaining time at the beginning)
    push(priority_queue, &process, process.runTime);
}

void getProcess(int signum) {
    //receive from the message queue and add to the ready queue
    int rec_val = msgrcv(msgq_id, &message, sizeof(message), 7, !IPC_NOWAIT);

    if (rec_val == -1)
        perror("Error in receive");

    // For testing only
    __SRTN_print_process_info(&message.process);
    switch (algorithm) {
        case 1:
            // TODO: Add to [PRIORITY QUEUE] as HPF

            break;
        case 2:
            // DONE: Add to priority queue as SRTN
            add_to_SRTN_queue(message.process);
        case 3:
            // TODO: Add to [QUEUE] as RR

            break;
    }
}