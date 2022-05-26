/*
Code for chosen scheduling algorithm:
=====================================
HPF --> 1
SRTN --> 2
RR --> 3
*/
#include "headers.h"

// global variables
int msgq_id;
int memory_pGenerator_sem;
int processesNum;
int totalRunTime;
struct msgbuff message;


void clearResources(int);

void readFile(struct Queue *processQueue) {
    FILE *file = fopen("processes.txt", "r");

    int id, arrivaltime, runningtime, priority, memsize;
    processesNum = 0;
    totalRunTime = 0;
    //read the first line (commented one)
    char s[40];
    fgets(s, 40, file);

    //start reading the file
    while (fscanf(file, "%d\t%d\t%d\t%d\t%d", &id, &arrivaltime, &runningtime, &priority, &memsize) != EOF) {
        struct ProcessStruct *p = (struct ProcessStruct *) malloc(sizeof(struct ProcessStruct));
        p->id = id;
        p->arrivalTime = arrivaltime;
        p->priority = priority;
        p->runTime = runningtime;
        p->running = false;
        p->startedBefore = false;
        p->enterQueue = arrivaltime;
        p->quitQueue = 0;
        p->executionTime = 0;
        p->waitingTime = 0;
        p->pid = -1;
        p->memsize = memsize;
        p->memoryNode = NULL;

        processesNum++;
        totalRunTime += runningtime;
        enQueue(processQueue, p);
    }
    fclose(file);
}

int getSchedulingAlgorithm() {
    int algorithm;
    printf("Which scheduling algorithm do you want?\n");
    printf("=======================================\n");
    printf("[1] Non-preemptive Highest Priority First (HPF)\n");
    printf("[2] Shortest Remaining time Next (SRTN)\n");
    printf("[3] Round Robin (RR)\n");
    printf("=======================================\n");
    scanf("%d", &algorithm);
    while (algorithm > 3 || algorithm < 1) {
        printf("Enter a number from 1 to 3\n");
        scanf("%d", &algorithm);
    }

    return algorithm;
}

void intializeMessageQueue() {
    msgq_id = msgget(PROMEMSCH, 0666 | IPC_CREAT);

    if (msgq_id == -1) {
        perror("Error in create message queue");
        exit(-1);
    }
}

void initializeSemaphoreMemoryProcessG() {

    // Create a semaphore to synchronize the scheduler with process generator
    memory_pGenerator_sem = semget(SEMAMEMPRO, 1, 0666 | IPC_CREAT);

    // Check is semaphore id is -1
    if (memory_pGenerator_sem == -1) {
        perror("Error in creating semaphores");
        exit(-1);
    }

    // Initialize the semaphore
    union Semun semun;
    semun.val = 0;
    if (semctl(memory_pGenerator_sem, 0, SETVAL, semun) == -1) {
        perror("Error in internalizing memory_pGenerator_sem");
        exit(-1);
    }
}

void sendProcess(struct ProcessStruct *process) {
    //send the process to memory process using the message queue
    message.mtype = 7;
    message.process = *process;
    int send_val = msgsnd(msgq_id, &message, sizeof(message.process), !IPC_NOWAIT);

    printf("message sent from process generator: %d\n", message.process.id);
    if (send_val == -1)
        perror("Errror in send");
}

void sendStopProcess() {
    struct ProcessStruct *process = (struct ProcessStruct *) malloc(sizeof(struct ProcessStruct));
    process->id = -1;
    sendProcess(process);
}


int main(int argc, char *argv[]) {
    signal(SIGINT, clearResources);

    // Initialize the semaphore
    initializeSemaphoreMemoryProcessG();

    //intialize the message queue
    intializeMessageQueue();

    // TODO Initialization
    // 1. Read the input files.
    struct Queue *processQueue = createQueue();
    readFile(processQueue);

    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    int algorithm = getSchedulingAlgorithm();

    // 3. Initiate and create the scheduler and clock processes.
    int clkpid = fork();
    if (clkpid == 0) {
        //run the clk file
        char *args[] = {"./clk.out", NULL};
        execv(args[0], args);
    }

    int memorypid;
    int schedulerpid = fork();
    if (schedulerpid != 0)
        memorypid = fork();

    if (schedulerpid == 0) {
        //run the scheduler file
        int length = snprintf(NULL, 0, "%d", algorithm);
        char *algo = malloc(length + 1);
        snprintf(algo, length + 1, "%d", algorithm);

        length = snprintf(NULL, 0, "%d", processesNum);
        char *procNum = malloc(length + 1);
        snprintf(procNum, length + 1, "%d", processesNum);

        length = snprintf(NULL, 0, "%d", totalRunTime);
        char *totalRT = malloc(length + 1);
        snprintf(totalRT, length + 1, "%d", totalRunTime);

        length = snprintf(NULL, 0, "%d", memorypid);
        char *memID = malloc(length + 1);
        snprintf(memID, length + 1, "%d", memorypid);

        char *args[] = {"./scheduler.out", algo, procNum, totalRT, memID, NULL};
        execv(args[0], args);
    }

    if (memorypid == 0) {
        //run the memory file
        int length = snprintf(NULL, 0, "%d", schedulerpid);
        char *schedulerID = malloc(length + 1);
        snprintf(schedulerID, length + 1, "%d", schedulerpid);

        // TODO: change the .out file to final name
        char *args[] = {"./memorybeshoy.out", schedulerID, NULL};
        execv(args[0], args);
    }

    // 4. Use this function after creating the clock process to initialize clock
    initClk();


    // To get time use this
    int clk;

    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    while (!isEmptyN(processQueue)) {
        struct ProcessStruct *process = peekN(processQueue);

        //get the current time to check with the arrival time of the process
        clk = getClk();
        while (process->arrivalTime != clk)
            clk = getClk();

        printf("Process with id = %d arrived at %d\n", process->id, clk);

        //put the process in the message queue
        sendProcess(process);

        //send a signal to the schedular to be ready for the coming process
        kill(memorypid, SIGUSR1);

        // Down the semaphore to make sure that the scheduler has pushed the process to the queue
        down(memory_pGenerator_sem);

        //remove it from the queue
        deQueue(processQueue);
    }

    //send a process with id = -1
    //to inform the schedular that there is no other processes coming
    sendStopProcess();
    kill(memorypid, SIGUSR1);
    down(memory_pGenerator_sem);

    //wait for the memory to finish before clearing the clock resources
    waitpid(memorypid, NULL, 0);

    //wait for the schedular to finish before clearing the clock resources
    waitpid(schedulerpid, NULL, 0);

    printf("==================  Process generator terminated  ======================\n");

    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum) {
    // Clears all resources in case of interruption

    // Clear the message queue resources
    msgctl(msgq_id, IPC_RMID, (struct msqid_ds *) 0);

    // Clear the semaphores resources
    semctl(memory_pGenerator_sem, 0, IPC_RMID, (struct semid_ds *) 0);
}
