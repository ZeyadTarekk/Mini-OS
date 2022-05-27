#include "headers.h"
#include "assert.h"

//used to inform the memory that there is no other processes coming
int flag = 1;
int flagScheduler = 1;

// global variables
int memory_pGenerator_sem;
int memory_scheduler_sem;
int msgq_id;
struct msgbuff message;
int schedulerpid;

void sendProcess(struct ProcessStruct *);


// Memory tree
struct Tree *memoryTree;

// Total memory size
const int memorySize = 1024;


/**************************************** WaitList start ****************************************/

// Max size of the waitList
const int MAXSIZE = 10000;

// ActualSize of the waitList
int actualSize = 0;

// WaitQueue
struct ProcessStruct **waitList;


/*
 * INPUT       : process
 * OUTPUT      : NONE
 * RETURN      : VOID
 * DESCRIPTION : Adds the given process to the waitList
 * */
void addToWaitList(const struct ProcessStruct *const process) {
    // Make sure that the actual size not equals maxsize
    assert(actualSize != MAXSIZE);

    // Allocate the process
    struct ProcessStruct *newProcess = create_process(process->id, process->arrivalTime, process->priority,
                                                      process->runTime, process->running, process->startedBefore,
                                                      process->enterQueue, process->quitQueue, process->executionTime,
                                                      process->waitingTime, process->pid);
    newProcess->memsize = process->memsize;

    // Add to current position
    waitList[actualSize] = newProcess;

    // Increment the size
    actualSize++;
}

/*
 * INPUT       : process
 * OUTPUT      : Index of process
 * RETURN      : int
 * DESCRIPTION : find the index of the process in the waitList
 * */
int findProcess(const struct ProcessStruct *const process) {
    for (int i = 0; i < actualSize; i++) {
        if (waitList[i]->id == process->id)
            return i;
    }
    return -1;
}

/*
 * INPUT       : process
 * OUTPUT      : NONE
 * RETURN      : VOID
 * DESCRIPTION : Deletes the given process from waitList if exist
 * */
void removeFromWaiList(const struct ProcessStruct *const process) {
    // Get index of process
    int index = findProcess(process);

    // Process not found
    if (index == -1)return;

    // Shift processes
    for (int i = index; i < actualSize - 1; i++)
        waitList[i] = waitList[i + 1];

    // Decrement the actual size
    actualSize--;
}

void printWaitList() {
    printf("MAXSIZE = %d Actual Size = %d \n", MAXSIZE, actualSize);
    for (int i = 0; i < actualSize; i++) {
        printf("Process id = %d size = %d\n", waitList[i]->id, waitList[i]->memsize);
    }
}
/**************************************** WaitList end ****************************************/

/*
 * INPUT       : NONE
 * OUTPUT      : NONE
 * RETURN      : VOID
 * DESCRIPTION : This function loops over all processes in the wait list and try to allocate them
 * */


void tryAllocateProcesses() {
    for (int i = 0; i < actualSize; i++) {
        printf("Try to allocate process id = %d i = %d\n", waitList[i]->id, i);

        // Try to allocate process
        struct TNode *node = allocateMemory(memoryTree->root, waitList[i], waitList[i]->memsize);

        if (node != NULL) {
            // DONE: Testing
            printf("Process %d has been allocated in memory\n", waitList[i]->id);

            // Assign tree node to that of process struct
            waitList[i]->memoryNode = node;

            // TODO: Send process to scheduler [BESHOY]
            // TODO: put the process in the message queue
            sendProcess(waitList[i]);

            // TODO: send a signal to the schedular to be ready for the coming process
            kill(schedulerpid, SIGUSR1);

            printWaitList();

            // TODO: Down the semaphore to make sure that the scheduler has pushed the process to the queue
            down(memory_scheduler_sem);

            // remove process
            removeFromWaiList(waitList[i]);

            i--;
        } else {
            // TODO: Testing
            printf("Process %d Cannot be allocated in memory\n", waitList[i]->id);
        }
    }
    printf("After allocate\n");
}


/**************************************** Memory Process Test Start ****************************************/
// Testing waitList operations
void testMemoryProcess() {
    for (int i = 0; i < 5; i++) {
        struct ProcessStruct *p = create_process(i, i, i, i, i, i, i, i, i, i, i);
        p->memsize = 100;
        addToWaitList(p);
        printf("Loop %d\n", i);
        printWaitList();
    }
    printf("After Loop\n");
    printWaitList();

    for (int i = 0; i < 5; i++) {
        struct ProcessStruct *p = create_process(i, i, i, i, i, i, i, i, i, i, i);
        p->memsize = 100;
        removeFromWaiList(p);
        printf("Loop %d\n", i);
        printWaitList();
    }

}

// Testing allocate process operations
void testAllocateProcess() {
    for (int i = 0; i < 5; i++) {
        struct ProcessStruct *p = create_process(i, i, i, i, i, i, i, i, i, i, i);
        p->memsize = 200;
        printf("Size = %d\n", p->memsize);
        addToWaitList(p);
        printf("Loop %d\n", i);
        printWaitList();
    }
    printf("After Loop\n");
    printWaitList();
    printf("\n\n\n\n");
    printTree(memoryTree->root);
    for (int i = 0; i < 5; i++) {
        printf("Loop %d\n", i);
        printWaitList();
        tryAllocateProcesses();
        printf("\n\n\n\n");
        printTree(memoryTree->root);
    }
}

/**************************************** Memory Process Test End   ****************************************/


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

void schedulerExitHandler(int);

int main(int argc, char *argv[]) {

    initClk();

    //add signal handler to get the processes from process_generator
    signal(SIGUSR1, getProcess);

    //add signal handler to get the process from the scheduler to deallocate
    signal(SIGUSR2, deallocateHandler);

    signal(SIGTSTP, schedulerExitHandler);

    // Initialize the semaphore
    initializeSemaphoreMemoryProcessG();
    initializeSemaphoreMemoryScheduler();

    //intialize the message queue
    intializeMessageQueue();

    //get the scheduler process id
    schedulerpid = atoi(argv[1]);

    // Intialize the waiting queue

    // Allocate memory tree
    memoryTree = createMemoryTree(memorySize);

    // Allocate the waitList
    waitList = malloc(MAXSIZE * sizeof(struct ProcessStruct *));


    // Dummy loop
    while (flag || actualSize != 0) {
    }

    //send a process with id = -1
    //to inform the schedular that there is no other processes coming
    sendStopProcess();
    kill(schedulerpid, SIGUSR1);
    down(memory_scheduler_sem);

    while (flagScheduler) {}

    printf("================= Memory process exit =================\n");
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
    if (message.process.id != -1) {
        addToWaitList(&message.process);
    }

    // Try to allocate processes
    tryAllocateProcesses();

    // Up the semaphore to allow process generator to continue
    up(memory_pGenerator_sem);

    //check if that process was the terminating one (id = -1)
    if (message.process.id == -1) {
        printf("Change the flag\n");
        flag = 0;
    }
}

void schedulerExitHandler(int sigNum) {
    flagScheduler = 0;
}

void deallocateHandler(int signum) {

    //receive from the message queue the process to deallocate
    int rec_val = msgrcv(msgq_id, &message, sizeof(message.process), 9, !IPC_NOWAIT);

    printf("message received in memory process from scheduler to deallocate: %d\n", message.process.id);
    fflush(stdout);
    if (rec_val == -1) {
        perror("Error in receive");
    }

    printf("Try to deallocate\n");
    // DONE: deallocate the memory of the received process
    //message.process.memoryNode
    deAllocateMyMemory(&message.process);

    printf("After deallocate\n");
    // Try to allocate processes
    tryAllocateProcesses();
}