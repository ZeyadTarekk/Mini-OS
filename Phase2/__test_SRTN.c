#include "SRTN.h"

struct ProcessStruct *__SRTN_TESTING_fill_process
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

int main() {
    struct ProcessStruct *p1 = __SRTN_TESTING_fill_process(1, 2, 3, 4, 0, 0, 0, 0, 0, 0, -1);
    struct ProcessStruct *p2 = __SRTN_TESTING_fill_process(2, 3, 4, 5, 0, 0, 0, 0, 0, 0, -1);
    __SRTN_print_process_info(p1);
    __SRTN_print_process_info(p2);
    struct PQueue *priority_queue = createPriorityQueue();
    push(priority_queue, p1, p1->runTime);
    push(priority_queue, p2, p2->runTime);
    SRTN(priority_queue);
    return 0;
}