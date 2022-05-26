#include "headers.h"

//used to inform the memory that there is no other processes coming
int flag = 1;

// global variables
int memory_pGenerator_sem;
int memory_scheduler_sem;
int msgq_id;
struct msgbuff message;
int schedulerpid;



void intializeMessageQueue() {
    msgq_id = msgget(PROMEMSCH, 0666 | IPC_CREAT);

    if (msgq_id == -1) {
        perror("Error in create message queue");
        exit(-1);
    }
}

// Intialize the semaphore between the scheduler and the memory process
void initializeSemaphoreMemoryScheduler() {

    // Create a semaphore to synchronize the scheduler with process generator
    memory_scheduler_sem = semget(SEMAMEMSCH, 1, 0666 | IPC_CREAT);

    // Check is semaphore id is -1
    if (memory_scheduler_sem == -1) {
        perror("Error in creating semaphores");
        exit(-1);
    }

    // Initialize the semaphore
    union Semun semun;
    semun.val = 0;
    if (semctl(memory_scheduler_sem, 0, SETVAL, semun) == -1) {
        perror("Error in internalizing memory_scheduler_sem");
        exit(-1);
    }
}

// Intialize the semaphore between the process generator and the memory process
void initializeSemaphoreMemoryProcessG() {

    // Create a semaphore to synchronize the scheduler with process generator
    memory_pGenerator_sem = semget(SEMAMEMPRO, 1, 0666 | IPC_CREAT);

    // Check is semaphore id is -1
    if (memory_pGenerator_sem == -1) {
        perror("Error in creating semaphores");
        exit(-1);
    }
}

// Send the allocated process from the memory to the scheduler to run
void sendProcess(struct ProcessStruct *process) {
    //send the process to schedular using the message queue
    message.mtype = 8;
    message.process = *process;
    int send_val = msgsnd(msgq_id, &message, sizeof(message.process), !IPC_NOWAIT);

    printf("message sent from memory process: %d\n", message.process.id);
    if (send_val == -1)
        perror("Errror in send");
}

// Send the stoping process to the scheduler to change its flag
void sendStopProcess() {
    struct ProcessStruct *process = (struct ProcessStruct *) malloc(sizeof(struct ProcessStruct));
    process->id = -1;
    sendProcess(process);
}


void getProcess(int);
void deallocateHandler(int);


int main(int argc, char *argv[]) {

    initClk();

    //add signal handler to get the processes from process_generator
    signal(SIGUSR1, getProcess);

    //add signal handler to get the process from the scheduler to deallocate
    signal(SIGUSR2, deallocateHandler);

    // Initialize the semaphore
    initializeSemaphoreMemoryProcessG();
    initializeSemaphoreMemoryScheduler();

    //intialize the message queue
    intializeMessageQueue();

    //get the scheduler process id
    schedulerpid = atoi(argv[1]);

    // Intialize the waiting queue


    while (/*condtion*/flag) {
        
        // TODO: Logic here


        //////////////////////////////////////////////////////
        // If you managed to allocate memory for this process
        //////////////////////////////////////////////////////

        // TODO: put the process in the message queue
        // sendProcess(/*process*/);

        // TODO: send a signal to the schedular to be ready for the coming process
        // kill(schedulerpid, SIGUSR1);

        // TODO: Down the semaphore to make sure that the scheduler has pushed the process to the queue
        // down(memory_scheduler_sem);

        // TODO: remove the process from the queue

        //////////////////////////////////////////////////////

    }

    //send a process with id = -1
    //to inform the schedular that there is no other processes coming
    sendStopProcess();
    kill(schedulerpid, SIGUSR1);
    printf("calling down(memory_scheduler_sem)\n");
    down(memory_scheduler_sem);

    // Clear clock resources
    destroyClk(false);
}

void getProcess(int signum) {
    //receive from the message queue and add to the waiting queue
    int rec_val = msgrcv(msgq_id, &message, sizeof(message.process), 7, !IPC_NOWAIT);

    printf("message received in memory process: %d\n", message.process.id);
    fflush(stdout);
    if (rec_val == -1) {
        perror("Error in receive");
    }





    // TODO: add the process to the waiting queue
    if (message.process.id != -1)
    {
        printf("size of process: %d\n", message.process.memsize);
    }
    

    //for testing 
    struct ProcessStruct *process = &message.process;
    sendProcess(process);

    // TODO: send a signal to the schedular to be ready for the coming process
    kill(schedulerpid, SIGUSR1);

    // TODO: Down the semaphore to make sure that the scheduler has pushed the process to the queue
    printf("calling down(memory_scheduler_sem)\n");
    down(memory_scheduler_sem);







    // Up the semaphore to allow process generator to continue
    printf("calling up(memory_pGenerator_sem)\n");
    up(memory_pGenerator_sem);

    //check if that process was the terminating one (id = -1)
    if (message.process.id == -1) {
        printf("Change the flag\n");
        flag = 0;
    }
}


void deallocateHandler(int signum) {

    //receive from the message queue the process to deallocate
    int rec_val = msgrcv(msgq_id, &message, sizeof(message.process), 9, !IPC_NOWAIT);

    printf("message received in memory process from scheduler to deallocate: %d\n", message.process.id);
    fflush(stdout);
    if (rec_val == -1) {
        perror("Error in receive");
    }


    // TODO: deallocate the memory of the received process
    //message.process.memoryNode
}