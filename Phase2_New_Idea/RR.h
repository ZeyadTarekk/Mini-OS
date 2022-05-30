#include <assert.h>
#include "limits.h"

void RR_run(struct ProcessStruct *const);

void print_RR_Info(const struct ProcessStruct *const);

void RR_stop(const struct ProcessStruct *const);

void RR_save_enter_queue_state(struct ProcessStruct *const);

void RR_save_exit_queue_state(struct ProcessStruct *const);

int RR_get_remaining_time(const struct ProcessStruct *const);

void RR_process_finish_handler(int);

struct ProcessStruct *running_process = NULL;

// for testing till getting as a parameter in function
//const int quantum = 3;
int noOfFinishedProcesses = 0;
//struct Queue *finishedProcessInQueue;


int RR_get_the_clock() {
    return *shmaddr;
}

void RR_run(struct ProcessStruct *const process) {

    if (process == NULL) {
        printf("\n****************************\nNULL Error in RR_run\n****************************\n");
        exit(0);
    }
    int pid = fork();

    // Handling forking error
    if (pid == -1) {
        perror("Error in forking");
        exit(-1);
    }

    // Child
    if (pid == 0) {

        printf("A process created with pid = %d \n", getpid());

        // Convert the remaining time to string
        char remaining_time_str[20];
        sprintf(remaining_time_str, "%d", RR_get_remaining_time(process));

        // Use execvp to replace the image of the child with that of the process file
        char *args[] = {"./process.out", remaining_time_str, NULL};
        int execlResult = execvp(args[0], args);

        // Handling execvp error
        if (execlResult == -1) {
            perror("Error in execl");
            exit(-1);
        }
    }

    process->pid = pid;
    print_process_info(process, 0);
    //Save it in the <running_process> pointer
    running_process = process;

    printf("\n=Running:");
    printf("\n Current time = %d\n", getClk());
//    print_RR_Info(process);
}

void print_RR_Info(const struct ProcessStruct *const process) {
    if (process == NULL) {
        printf("\n****************************\nNULL Error in __RR_print_process_info\n****************************\n");
        exit(0);
    }
    printf("\n*********************************************\n");
    printf("id                : %d\n", process->id);
    printf("arrivalTime       : %d\n", process->arrivalTime);
    printf("runtime           : %d\n", process->runTime);
    printf("running           : %d\n", process->running);
    printf("startedBefore     : %d\n", process->startedBefore);
    printf("enterQueue        : %d\n", process->enterQueue);
    printf("quitQueue         : %d\n", process->quitQueue);
    printf("waitingTime       : %d\n", process->waitingTime);
    printf("executionTime     : %d\n", process->executionTime);
    printf("pid               : %d\n", process->pid);
    printf("remaining time    : %d\n", RR_get_remaining_time(process));
    printf("*********************************************\n");
    fflush(stdout);

}

int RR_get_remaining_time(const struct ProcessStruct *const process) {

    //  indicates as a dummy process
    if (process == NULL) {
        return INT_MIN;
    }

    // remaining_time = run_time - execution_time
    int remainingRR = process->runTime - process->executionTime;


    // calling abort when remaining time < 0
    assert(remainingRR >= 0);
    return remainingRR;

}

void RR_save_exit_queue_state(struct ProcessStruct *const process_to_run) {
    if (process_to_run == NULL) {
        printf("\n****************************\nNULL Error in RR_save_exit_queue_state\n****************************\n");
        exit(0);
    }

    int current_time = getClk();
//    printf("\n=Left Queue:");
//    printf("\n Current time = %d\n", current_time);
//    print_RR_Info(process_to_run);

    //  Updating Values

    process_to_run->running = 1;
    process_to_run->quitQueue = current_time;
    process_to_run->waitingTime += current_time - process_to_run->enterQueue;
    printf("current time : %d Wait time : %d enter queue time: %d \n", current_time, process_to_run->waitingTime,
           process_to_run->enterQueue);
    assert(process_to_run->waitingTime >= 0);
}

void RR_save_enter_queue_state(struct ProcessStruct *const process_to_stop) {
    if (process_to_stop == NULL) {
        printf("\n****************************\nNULL Error in RR_save_enter_queue_state\n****************************\n");
        exit(0);
    }
    int current_time = getClk();
    printf("\n=Entered Queue:");
    printf("\n Current time = %d\n", current_time);
//    print_RR_Info(process_to_stop);

    process_to_stop->running = 0;
    process_to_stop->startedBefore = 1;
    process_to_stop->enterQueue = current_time;
    process_to_stop->executionTime += current_time - process_to_stop->quitQueue;
    assert(process_to_stop->executionTime >= 0);
}

void RR_process_finish_handler(int signum) {
    printf("Current clock is : %d \n", getClk());
    printf("Process %d has sent me a signal %d\nWhich means it has finished execution\n", running_process->pid, signum);
    noOfFinishedProcesses++;
    printf("Number of finished processes is %d \n", noOfFinishedProcesses);
    // Save enter queue state
    RR_save_enter_queue_state(running_process);

    // Log to file
    print_process_info(running_process, 3);
    // memory log file
    printMemoryDetails(running_process, 1);


    //deallocate from memory
    deAllocateMyMemory(running_process);

    // free process from memory
    free(running_process);

    // setting running_process to NULL
    running_process = NULL;


    // increment memory entered size
    checkMemoryFlag++;

    signal(SIGUSR2, RR_process_finish_handler);

}

void RR_stop(const struct ProcessStruct *const process_to_stop) {
    if (process_to_stop == NULL) {
        return;
    }
    printf("Signal Stop has been sent to the process %d\n", process_to_stop->pid);

    // Send signal to process (stop the process)
    int killResult = kill(process_to_stop->pid, SIGKILL);
    if (killResult == -1) {
        printf("Error in sending signal stop\n");
        exit(-1);
    }

    running_process = NULL;
}


void RR(int quantum, struct Queue *queue) {

    // Done: checking  the remaining time of each process
    printf("\n\n**************** Welcome To RR Algorithm ****************\n\n");

    signal(SIGUSR2, RR_process_finish_handler);

    int lastPid = -1;
    int current, slots;
    int prev = getClk();


    // flag is used to terminate the scheduler
    while (running_process != NULL || flag || isEmptyN(queue) == false || isEmptyN(waitQueue) == false) {


        // check if there is a process ready to allocate in memory
        if (checkMemoryFlag >= 1) {
            printf("=====================Before Allocation\"=====================\n\n");
            tryAllocateProcessesQueue(queue);
            printf("\"=====================After Allocation\"=====================\n\n");
            checkMemoryFlag--;
            printQ(0);
        }
        // check if there is no a process in the Queue or not
        if (isEmptyN(queue))
            continue;

        // get the turned process from the queue  before executing
        struct ProcessStruct *turnedProcess = peekN(queue);

        // update current time
        current = getClk();
        slots = current - prev;

        //check if there is no  running process
        if (running_process == NULL) {

            // dequeue the turned process and make it a running process
            deQueue(queue);

            // save the data of a process (PCB)
            RR_save_exit_queue_state(turnedProcess);

            // run RR algorithm
            RR_run(turnedProcess);

            //setting prev to current time
            prev = current;

        } else if (slots >= quantum && (slots % quantum) == 0 && turnedProcess->id != lastPid) {
//            printf("\n====================  Shifting occurs at time : %d  ====================\n", current);

            // exchange with next process
            lastPid = turnedProcess->id;

            //setting prev to current time
            prev = current;

            //Save running process before making it NULL in RR_stop
            struct ProcessStruct *premptedProcess = running_process;

            // calculate remaining
            int remainingTime = RR_get_remaining_time(premptedProcess) - (getClk() - premptedProcess->quitQueue);

            assert(remainingTime >= 0);
            if (remainingTime > 0) {

                // stopping the running process
                RR_stop(running_process);

                //  Save the state of the pr-empted process
                RR_save_enter_queue_state(premptedProcess);

                // Log to file
                print_process_info(premptedProcess, 1);

                //adding the prompted process to queue
                enQueue(queue, premptedProcess);

                // dequeue process from queue
                deQueue(queue);

                // saving the state of turned process in queue
                RR_save_exit_queue_state(turnedProcess);

                // entering  the next process in running state
                RR_run(turnedProcess);
            }

        }

    }
    printf("\n\n================================ RR Terminated at  time =%d================================\n\n",
           getClk());
}


// schedule the next process in the queue
// get next process to be scheduled from the process table and remove it from ready list
// intailize quantaum of new process
// send messege to the process with its remaining time
// get the remaining time of the currently running process


