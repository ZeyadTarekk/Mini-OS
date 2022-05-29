#include "headers.h"

//used to inform the scheduler that there is no other processes coming
int flag = 1;

void print_process_info(const struct ProcessStruct *const, int);

#include "SRTN.h"
#include "RR.h"
#include "HPF.h"

// global variables
int msgq_id;
int algorithm;
int processesNum;
int scheduler_pGenerator_sem;

//variables for scheduler.perf file
int totalRunTime;
int totalWaitingTime = 0;
float sumWTA = 0.0;
float meanWTA;
float sumWTASq = 0.0;
float STD;

struct msgbuff message;
struct PQueue *priority_queue;
struct Queue *queue;

void getProcess(int);
//ToBeRemoved
void printQueue(int);

void create_scheduler_log();
void create_scheduler_perf();

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

    // Create a semaphore to synchronize the scheduler with process generator
    scheduler_pGenerator_sem = semget(SEMA, 1, 0666 | IPC_CREAT);

    // Check is semaphore id is -1
    if (scheduler_pGenerator_sem == -1) {
        perror("Error in creating semaphores");
        exit(-1);
    }
}

int main(int argc, char *argv[]) {
    // Create scheduler.log
    create_scheduler_log();

    //add signal handler to get the processes from process_generator
    signal(SIGUSR1, getProcess);

//ToBeRemoved
    //for testing
    signal(SIGTRAP, printQueue);


    //TODO implement the scheduler :)
    initClk();

    //Initialize the semaphore
    initializeSemaphore();

    //Initialize the message queue
    intializeMessageQueue();


    //get the algorithm number
    algorithm = atoi(argv[1]);

    //get the total number of the processes
    processesNum = atoi(argv[2]);

    //get the total run time of all processes
    totalRunTime = atoi(argv[3]);

    switch (algorithm) {
        case 1:
            // Allocate the priority queue
            priority_queue = createPriorityQueue();
            // Call the algorithm function
            HPF(priority_queue);

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
            // Call the algorithm function
            RR(2, queue);
            break;
    }

    printf("\n\n===================================scheduler Terminated at time = %d===================================\n\n",
            getClk());

    create_scheduler_perf();
    
    // Destroy your clock
    destroyClk(false);
    return 0;
}

void add_to_SRTN_queue(struct ProcessStruct process) {
    if (process.id != -1)
    {
        // Push to the queue and the priority is the runTime (The remaining time at the beginning)
        struct ProcessStruct *newProcess = create_process(process.id, process.arrivalTime, process.priority,
                                                          process.runTime, process.running,
                                                          process.startedBefore, process.enterQueue, process.quitQueue,
                                                          process.executionTime,
                                                          process.waitingTime, process.pid);
        push(priority_queue, newProcess, newProcess->runTime);
    }
}

void add_to_HPF_queue(struct ProcessStruct process) {
    if (process.id != -1) {
        // Push to the queue and the priority is the priority (The priority of the process)
        struct ProcessStruct *newProcess = create_process(process.id, process.arrivalTime, process.priority,
                                                          process.runTime, process.running,
                                                          process.startedBefore, process.enterQueue, process.quitQueue,
                                                          process.executionTime,
                                                          process.waitingTime, process.pid);
        push(priority_queue, newProcess, newProcess->priority);
    }
}

void add_to_RR_queue(struct ProcessStruct process) {
    if (process.id != -1) {
        // enqueue (The remaining time at the beginning)
        struct ProcessStruct *newProcess = create_process(process.id, process.arrivalTime, process.priority,
                                                          process.runTime, process.running,
                                                          process.startedBefore, process.enterQueue, process.quitQueue,
                                                          process.executionTime,
                                                          process.waitingTime, process.pid);
        enQueue(queue, newProcess);
    }
}

void getProcess(int signum) {
    //receive from the message queue and add to the ready queue
    int rec_val = msgrcv(msgq_id, &message, sizeof(message.process), 7, !IPC_NOWAIT);

    printf("message received: %d\n", message.process.id);
    fflush(stdout);
    if (rec_val == -1) {
        perror("Error in receive");
    }

    switch (algorithm) {
        case 1:
            // TODO: Add to [PRIORITY QUEUE] as HPF
            add_to_HPF_queue(message.process);
            break;
        case 2:
            // DONE: Add to priority queue as SRTN
            add_to_SRTN_queue(message.process);
            break;
        case 3:
            // DONE: Add to [QUEUE] as RR
            add_to_RR_queue(message.process);
            break;
    }

    // Process has been pushed to the queue
    // Up the semaphore to allow process generator to continue
    up(scheduler_pGenerator_sem);

    //check if that process was the terminating one (id = -1)
    if (message.process.id == -1) {
        flag = 0;
    }
}
//ToBeRemoved
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

void create_scheduler_perf() {
    /*
     * A function that creates "scheduler.perf"
     * and fill it with some statistics
     * */
    FILE *outputFile;
    outputFile = fopen("scheduler.perf", "w");

    //CPU utilization
    fprintf(outputFile, "%s", "CPU utilization = ");

    fprintf(outputFile, "%d%%\n", (totalRunTime * 100 / getClk()));

    //Avg WTA
    fprintf(outputFile, "%s", "Avg WTA = ");
    fprintf(outputFile, "%.2f\n", (sumWTA / processesNum));

    //Avg Waiting
    fprintf(outputFile, "%s", "Avg Waiting = ");
    fprintf(outputFile, "%.2f\n", ((float)totalWaitingTime / processesNum));

    //Std WTA
    meanWTA = sumWTA / processesNum;
    float x = ((sumWTA) - (processesNum * pow(meanWTA, 2)));
    STD = sqrt(abs(x) / processesNum);

    fprintf(outputFile, "%s", "Std WTA = ");
    fprintf(outputFile, "%.2f\n", STD);

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
    //save this value for scheduler.perf file
    totalWaitingTime += process->waitingTime;

    if (state == 3) {
        int TA = current_time - process->arrivalTime;
        float WTA;
        if (process->runTime == 0)
            WTA = 0;
        else WTA = (float) TA / process->runTime;
        //save this values for scheduler.perf file
        sumWTA += WTA;
        sumWTASq += pow(WTA, 2);

        fprintf(outputFile, "%s", "TA ");
        fprintf(outputFile, "%d ", TA);
        fprintf(outputFile, "%s", "WTA ");
        fprintf(outputFile, "%.2f ", WTA);
    }
    fprintf(outputFile, "%s", "\n");
    fclose(outputFile);
}