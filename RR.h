#include <assert.h>
#include "headers.h"
void print_RR_Info(const struct ProcessStruct *const);

void RR_run(struct ProcessStruct *const);

void RR_stop(const struct ProcessStruct *const);

void RR_save_enter_queue_state(struct ProcessStruct *const);

void RR_save_exit_queue_state(struct ProcessStruct *const);

int RR_get_remaining_time(const struct ProcessStruct *const);

int RR_compare_remaining_time(const struct ProcessStruct *const, const struct ProcessStruct *const);
int RR_get_Quantity_time(const int quanta)

void RR_process_finish_handler(int);

struct ProcessStruct *__running_process = NULL;

int RR_get_the_clock() {
    return *shmaddr;
}

// TODO:
// schedule the next process in the queue
// get next process to be scheduled from the process table and remove it from ready list
// intailize quantaum of new process
// send messege to the process with its remaining time
// get the remaining time of the currently running process


