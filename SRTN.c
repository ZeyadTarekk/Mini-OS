#include "headers.h"
#include <assert.h>

struct ProcessStruct *__running_process = NULL;

void __SRTN_run(const struct ProcessStruct *const process_to_run) {
    if (/*TODO: Check boolean of <startedBefore>*/) {
        // TODO: Send a continue signal to the process
    } else {
        // TODO: Fork a process
        // TODO: EXCL the forked process with process file
    }
    // TODO: Save it in the <__running_process> pointer
}

void __SRTN_stop(const struct ProcessStruct *const process_to_stop) {
    // TODO: Send signal to process
    // TODO: Set the <__running_process> to NULL  __running_process = NULL;
}

// NOTE: If save functions have the same logic in all algorithms then there no need to implement it

void __SRTN_save_enter_queue_state(const struct ProcessStruct *process_to_stop) {
    /* TODO: Update
     *      1- running boolean
     *      2- startedBefore
     *      3- enterQueue
     *      4- executionTime
     */
}

void __SRTN_save_exit_queue_state(const struct ProcessStruct *process_to_run) {
    /* TODO: Update
     *      1- running boolean
     *      2- startedBefore
     *      3- quitQueue
     *      4- waitingTime
     */
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

void SRTN() {
    // DONE: Allocate the Queue
    struct PQueue *priority_queue = createPriorityQueue();

    while (1/*TODO: There still processes in the Queue or there still processes will be received*/) {
        // DONE: Check if the queue is empty
        if (isEmpty(priority_queue)) {
            continue;
        }

        // DONE: Get the top of the Queue (DON'T POP)
        struct ProcessStruct *top_queue = peek(priority_queue);

        // DONE: Check if there is [NO] a running process
        if (__running_process == NULL) {

            // DONE: Pop the first process in the queue (**Redundant assignment**)
            top_queue = pop(priority_queue);

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
            // DONE: Pop the first process in the queue (**Redundant assignment**)
            top_queue = pop(priority_queue);

            // DONE: Save the state of the poped process    => Save state function  <__SRTN_save_exit_queue_state>
            __SRTN_save_exit_queue_state(top_queue);

            // DONE: Run the other process                  => Run process function
            __SRTN_run(top_queue);
        }
    }
}