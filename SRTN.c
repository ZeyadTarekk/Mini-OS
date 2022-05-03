#include "headers.h"

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

void __SRTN_save_exit_queue_state(const struct ProcessStruct *process_to_stop) {
    /* TODO: Update
     *      1- running boolean
     *      2- startedBefore
     *      3- quitQueue
     *      4- waitingTime
     */
}

void SRTN() {
    while (/*TODO: There still processes in the Queue or there still processes will be received*/) {
        if (/*TODO: Check if the queue is [EMPTY]*/) {
            continue;
        }
        // TODO: Get the top of the Queue (DON'T POP)
        if (/*TODO: Check if there is [NO] a running process*/ /* __running_process == NULL */) {
            // TODO: Pop the first process in the queue
            // TODO: Save the state of the poped process    => Save state function  <__SRTN_save_exit_queue_state>
            // TODO: Run the process                        => Run process function
        } else if (/*TODO: Compare the remaining time of first process in ready Queue with that of the running one*/) {
            // IF THE FIRST PROCESS IN THE READY QUEUE HAS SMALLER REMAINING TIME

            // TODO: Stop the running process               => Signal
            // TODO: Save the state of the running process  => Save state function  <__SRTN_save_enter_queue_state>
            // TODO: Add the running process to the queue

            // TODO: Pop the first process in the queue
            // TODO: Save the state of the poped process    => Save state function  <__SRTN_save_exit_queue_state>
            // TODO: Run the other process                  => Run process function
        }
    }
}