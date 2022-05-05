#include "headers.h"
#include <assert.h>

void __SRTN_CLOCK();

void __SRTN_print_process_info(const struct ProcessStruct *const);

void __SRTN_run(const struct ProcessStruct *const);

void __SRTN_stop(const struct ProcessStruct *const);

void __SRTN_save_enter_queue_state(struct ProcessStruct *const);

void __SRTN_save_exit_queue_state(struct ProcessStruct *const);

int __SRTN_get_remaining_time(const struct ProcessStruct *const);

int __SRTN_compare_remaining_time(const struct ProcessStruct *const, const struct ProcessStruct *const);

struct ProcessStruct *__running_process = NULL;

int __SRTN_get_the_clock() {
    return *shmaddr;
}

void __SRTN_print_process_info(const struct ProcessStruct *const process) {
    printf("\n*********************************************\n");
    printf("id                : %d\n", process->id);
    printf("arrivalTime       : %d\n", process->arrivalTime);
    printf("priority          : %d\n", process->priority);
    printf("runtime           : %d\n", process->runTime);
    printf("running           : %d\n", process->running);
    printf("startedBefore     : %d\n", process->startedBefore);
    printf("enterQueue        : %d\n", process->enterQueue);
    printf("quitQueue         : %d\n", process->quitQueue);
    printf("waitingTime       : %d\n", process->waitingTime);
    printf("*********************************************\n");
}

void __SRTN_run(const struct ProcessStruct *const process_to_run) {
    printf("\n=Running:");
    printf("\n Current time = %d", __SRTN_get_the_clock());
    __SRTN_print_process_info(process_to_run);

    //DONE: Check boolean of <startedBefore>
    if (process_to_run->startedBefore == 1) {
        // TODO: Send a continue signal to the process using its pid in the struct (NOT EXIST YET)
        printf("Send a continue signal to the process\n");
    } else {
        // Create new process
        int pid = fork();

        // Handling forking error
        if (pid == -1) {
            perror("Error in execl");
            exit(-1);
        }

        // Child
        if (pid == 0) {
            printf("A process created with pid = %d \n", getpid());

            // Convert the remaining time to string
            char remaining_time_str[20];
            sprintf(remaining_time_str, "%d", __SRTN_get_remaining_time(process_to_run));

            // Use execvp to replace the image of the child with that of the process file
            char *args[] = {"./process.out", remaining_time_str, NULL};
            int execlResult = execvp(args[0], args);

            // Handling execvp error
            if (execlResult == -1) {
                perror("Error in execl");
                exit(-1);
            }
        }
    }
    // TODO: Save it in the <__running_process> pointer
}

void __SRTN_stop(const struct ProcessStruct *const process_to_stop) {
    printf("\n=Stopping:");
    printf("\n Current time = %d", __SRTN_get_the_clock());
    __SRTN_print_process_info(process_to_stop);

    // TODO: Send signal to process
    // TODO: Set the <__running_process> to NULL  __running_process = NULL;
}

// NOTE: If save functions have the same logic in all algorithms then there no need to implement it

void __SRTN_save_enter_queue_state(struct ProcessStruct *const process_to_stop) {
    /*
     * INPUT       : The process that has entered the queue
     * INPUT type  : const struct ProcessStruct *
     * OUTPUT      : void
     * NOTE        : This function saves the state of the process ON ENTERING the ready queue
     * */
    int current_time = __SRTN_get_the_clock();
    printf("\n=Entered Queue:");
    printf("\n Current time = %d", current_time);
    __SRTN_print_process_info(process_to_stop);

    /* DONE: Update
     *      1- running boolean
     *      2- startedBefore
     *      3- enterQueue
     *      4- executionTime
     */
    process_to_stop->running = 0;
    process_to_stop->startedBefore = 1;
    process_to_stop->enterQueue = current_time;
    process_to_stop->executionTime += current_time - process_to_stop->quitQueue;
    assert(process_to_stop->executionTime > 0);
}

void __SRTN_save_exit_queue_state(struct ProcessStruct *const process_to_run) {
    /*
     * INPUT       : The process that has left the queue
     * INPUT type  : const struct ProcessStruct *
     * OUTPUT      : void
     * NOTE        : This function saves the state of the process ON LEAVING the ready queue
     * */
    int current_time = __SRTN_get_the_clock();
    printf("\n=Left Queue:");
    printf("\n Current time = %d", current_time);
    __SRTN_print_process_info(process_to_run);
    /* DONE : Update
     *      1- running boolean
     *      2- startedBefore
     *      3- quitQueue
     *      4- waitingTime
     */
    process_to_run->running = 1;
    process_to_run->quitQueue = current_time;
    process_to_run->waitingTime += current_time - process_to_run->enterQueue;
    assert(process_to_run->waitingTime > 0);
}

int __SRTN_get_remaining_time(const struct ProcessStruct *const process) {
    /*
     * INPUT       : The process to calculate its remaining time
     * INPUT type  : const struct ProcessStruct *const
     * OUTPUT      : Remaining time of the process
     * Return type : int
     * */

    // remaining_time = run_time - execution_time
    int process_remaining_time = process->runTime - process->executionTime;

    // Assert that the remaining time is greater than ZERO
    assert(process_remaining_time > 0);

    return process_remaining_time;
}

int
__SRTN_compare_remaining_time(const struct ProcessStruct *const process1, const struct ProcessStruct *const process2) {
    /*
     * INPUT       : Two processes to compare there remaining time
     *               process1 type: const struct ProcessStruct *const
     *               process2 type: const struct ProcessStruct *const
     * OUTPUT      : 1 if the remaining time of the first process is less than that of second process else returns 0
     * Return type : int
     * */

    int process1_remaining_time = __SRTN_get_remaining_time(process1);
    int process2_remaining_time = __SRTN_get_remaining_time(process2);

    return process1_remaining_time < process2_remaining_time;
}

// TODO: remove this parameter after testing

void SRTN(struct PQueue *priority_queue) {
    // Initiate the clock
    initClk();

    // DONE: Allocate the Queue
    // TODO: Uncomment this line after testing
    // struct PQueue *priority_queue = createPriorityQueue();

    while (1/*TODO: There still processes in the Queue or there still processes will be received*/) {

        // DONE: Check if the queue is empty
        if (isEmpty(priority_queue)) {
            // TODO: Comment these two line after testing
            printf("Queue is empty\n");
            break;
            continue;
        }

        // DONE: Get the top of the Queue (DON'T POP)
        struct ProcessStruct *top_queue = peek(priority_queue);

        // DONE: Check if there is [NO] a running process
        if (__running_process == NULL) {

            // DONE: Pop the first process in the queue
            pop(priority_queue);

            // DONE: Save the state of the poped process    => Save state function  <__SRTN_save_exit_queue_state>
            __SRTN_save_exit_queue_state(top_queue);

            // DONE: Run the process                        => Run process function
            __SRTN_run(top_queue);

        } else if (__SRTN_compare_remaining_time(top_queue, __running_process)) {
            // IF THE FIRST PROCESS IN THE READY QUEUE HAS SMALLER REMAINING TIME

            /**** STEPS TO STOP THE RUNNING PROCESS ****/
            // DONE: Stop the running process               => Signal
            __SRTN_stop(__running_process);

            // DONE: Save the state of the running process  => Save state function  <__SRTN_save_enter_queue_state>
            __SRTN_save_enter_queue_state(__running_process);

            // DONE: Add the running process to the queue
            push(priority_queue, __running_process, __SRTN_get_remaining_time(__running_process));


            /**** STEPS TO RUN THE TOP PROCESS ****/
            // DONE: Pop the first process in the queue
            pop(priority_queue);

            // DONE: Save the state of the poped process    => Save state function  <__SRTN_save_exit_queue_state>
            __SRTN_save_exit_queue_state(top_queue);

            // DONE: Run the other process                  => Run process function
            __SRTN_run(top_queue);
        }
    }
}