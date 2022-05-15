//TODO: Remove this flag after creating end shared memory for testing
#include "headers.h"

int flag = 1;

void print_process_info(const struct ProcessStruct *const, int);

#include "SRTN.h"

// global variables
int msgq_id;
int algorithm;
int scheduler_pGenerator_sem;
struct msgbuff message;
struct PQueue *priority_queue;
struct Queue *queue;

void getProcess(int);

void printQueue(int);

void create_scheduler_log();

struct ProcessStruct *create_process
        (int id, int arrivalTime, int priority, int runTime,
         bool running, bool startedBefore, int enterQueue,
         int quitQueue, int executionTime, int waitingTime, int pid) {
    struct ProcessStruct *process = (struct ProcessStruct *) malloc(sizeof(struct ProcessStruct));
    process->id = id;
    process->arrivalTime = arrivalTime;
    process->priority = priority;
    process->runTime = runTime;
    process->running = running;
    process->startedBefore = startedBefore;
    process->enterQueue = enterQueue;
    process->quitQueue = quitQueue;
    process->executionTime = executionTime;
    process->waitingTime = waitingTime;
    process->pid = pid;
    return process;
}

void intializeMessageQueue() {
    msgq_id = msgget(PROSCH, 0666 | IPC_CREAT);

    if (msgq_id == -1) {
        perror("Error in create message queue");
        exit(-1);
    }
}

void initializeSemaphore() {

    key_t semkey = ftok("sem.txt", 70);

    // Create a semaphore to synchronize the scheduler with process generator
    scheduler_pGenerator_sem = semget(semkey, 1, 0666 | IPC_CREAT);

    // Check is semaphore id is -1
    if (scheduler_pGenerator_sem == -1) {
        perror("Error in creating semaphores");
        exit(-1);
    }
    printf("Scheduler: Semaphore created with id: %d\n", scheduler_pGenerator_sem);
}

int main(int argc, char *argv[]) {
    // Create scheduler.log
    create_scheduler_log();

    //add signal handler to get the processes from process_generator
    signal(SIGUSR1, getProcess);
    signal(SIGTRAP, printQueue);


    //TODO implement the scheduler :)
    initClk();

    //Initialize the semaphore
    initializeSemaphore();

    //initialize the message queue
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

    printf("\n\n===================================scheduler Terminated at time = %d===================================\n\n",
           getClk());

    // TODO: Check its logic
    // Destroy your clock
    destroyClk(true);
    return 0;
}

void add_to_SRTN_queue(struct ProcessStruct process) {
    // Push to the queue and the priority is the runTime (The remaining time at the beginning)
    struct ProcessStruct *newProcess = create_process(process.id, process.arrivalTime, process.priority,
                                                      process.runTime, process.running,
                                                      process.startedBefore, process.enterQueue, process.quitQueue,
                                                      process.executionTime,
                                                      process.waitingTime, process.pid);
    push(priority_queue, newProcess, newProcess->runTime);
}

void getProcess(int signum) {
    //receive from the message queue and add to the ready queue
    int rec_val = msgrcv(msgq_id, &message, sizeof(message.process), 7, !IPC_NOWAIT);

    printf("message received: %d\n", message.process.id);
    fflush(stdout);
    if (rec_val == -1) {
        perror("Error in receive");
    }
    // For testing only
    //__SRTN_print_process_info(&message.process);
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

    // Process has been pushed to the queue
    // Up the semaphore to allow process generator to continue
    up(scheduler_pGenerator_sem);

    // message.process.id == <id of last process> => Last process is not valid
    // This is only for testing
    // Until we make a shared memory for terminating
    // Use the flag as a condition of your main loop
    //TODO: Remove this condition after creating end shared memory for testing
    if (message.process.id == -1) {
        flag = 0;
    }
}

void printQueue(int sigNum) {
    printf("I have recieved signal %d\n", sigNum);
    struct PQNode *start = priority_queue->head;
    while (start != NULL) {
        __SRTN_print_process_info(start->data);
        start = start->next;
    }
}

void create_scheduler_log() {
    /*
     * A function that creates an empty file with the beginning line for scheduler.log
     * */
    FILE *outputFile;
    outputFile = fopen("scheduler.log", "w");
    fprintf(outputFile, "%s", "#At time x process y state arr w total z remain y wait k\n");
    fclose(outputFile);
}

void print_process_info(const struct ProcessStruct *const process, int state) {
    /*
     * DESCRIPTION : A function that prints the scheduler action
     *
     * INPUT       : Process to print its info
     *               State of process
     *               0 : started
     *               1 : Stopped
     *               2 : Resumed
     *               3 : Finished
     *
     * OUTPUT      : void
     * */

    FILE *outputFile;
    int current_time = getClk();
    outputFile = fopen("scheduler.log", "a+");
    if (!outputFile) {
        perror("Unable to open file \"scheduler.log\" terminating ...\n");
        exit(-1);
    }
    fprintf(outputFile, "%s", "At time ");
    fprintf(outputFile, "%d ", current_time);
    fprintf(outputFile, "%s", "process ");
    fprintf(outputFile, "%d ", process->id);
    switch (state) {
        case 0:
            fprintf(outputFile, "%s", "started ");
            break;
        case 1:
            fprintf(outputFile, "%s", "stopped ");
            break;
        case 2:
            fprintf(outputFile, "%s", "resumed ");
            break;
        case 3:
            fprintf(outputFile, "%s", "finished ");
            break;
        default:
            perror("Wrong state terminating ...");
            exit(-1);
    }
    fprintf(outputFile, "%s", "arr ");
    fprintf(outputFile, "%d ", process->arrivalTime);
    fprintf(outputFile, "%s", "total ");
    fprintf(outputFile, "%d ", process->runTime);
    fprintf(outputFile, "%s", "remain ");
    fprintf(outputFile, "%d ", __SRTN_get_remaining_time(process));
    fprintf(outputFile, "%s", "wait ");
    fprintf(outputFile, "%d ", process->waitingTime);

    if (state == 3) {
        fprintf(outputFile, "%s", "TA ");
        fprintf(outputFile, "%d ", current_time - process->arrivalTime);
        fprintf(outputFile, "%s", "WTA ");
        // TODO: Divide by the total number of processes
        fprintf(outputFile, "%d ", (current_time - process->arrivalTime));
    }
    fprintf(outputFile, "%s", "\n");
    fclose(outputFile);
}