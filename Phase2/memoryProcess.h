#include "assert.h"

// Memory tree
struct Tree *memoryTree;

// Total memory size
const int memorySize = 1024;


struct PQueue *waitPriorityQueue;
struct Queue *waitQueue;

/*
 * INPUT       : process
 * OUTPUT      : NONE
 * RETURN      : VOID
 * DESCRIPTION : Adds the given process to the waitList
 * */
void addToWaitList(const struct ProcessStruct *const process) {
    // Make sure that the actual size not equals maxsize
    printf("Process add to wait list\n");
    // Allocate the process
    struct ProcessStruct *newProcess = create_process(process->id, process->arrivalTime, process->priority,
                                                      process->runTime, process->running, process->startedBefore,
                                                      process->enterQueue, process->quitQueue, process->executionTime,
                                                      process->waitingTime, process->pid);
    newProcess->memsize = process->memsize;

    switch (algorithm) {
        case 1:
            // DONE: Add to [PRIORITY QUEUE] as HPF
            push(waitPriorityQueue, newProcess, newProcess->priority);
            break;
        case 2:
            // DONE: Add to priority queue as SRTN
            push(waitPriorityQueue, newProcess, newProcess->runTime);
            break;
        case 3:
            // DONE: Add to [QUEUE] as RR
            enQueue(waitQueue, newProcess);
            break;
    }
//ToBeRemoved
    printf("end Process add to wait list\n");
}


void printWaitList() {
    struct PQNode *tempHead = waitPriorityQueue->head;
    while (tempHead) {
        printf("Process id = %d size = %d\n", tempHead->data->id, tempHead->data->memsize);
        tempHead = tempHead->next;
    }
}

void removeFromWaitPriorityQueue(struct PQNode *node) {
    struct PQNode *tempHead = waitPriorityQueue->head;
    if (tempHead == node) {
        waitPriorityQueue->head = waitPriorityQueue->head->next;
        tempHead->next = NULL;
        printf("Free head\n");
        free(tempHead);
        return;
    }
    while (tempHead->next != node) {
        tempHead = tempHead->next;
    }
    struct PQNode *nodeToFree = tempHead->next;
    printf("================================Process %d has been allocated in the memory===========================================\n", node->data->id);
    tempHead->next = nodeToFree->next;
    nodeToFree->next = NULL;
    free(nodeToFree);
    printWaitList();
}


void removeFromWaitQueue(struct QNode *node) {
    struct QNode *tempHead = waitQueue->front;
    if (tempHead == node) {
        if (waitQueue->front == waitQueue->rear) {
            waitQueue->front = NULL;
            waitQueue->rear = NULL;
            free(tempHead);
            return;
        }
        waitQueue->front = waitQueue->front->next;
        tempHead->next = NULL;
        printf("Free head\n");
        free(tempHead);
        return;

    }
    while (tempHead->next != node) {
        tempHead = tempHead->next;
    }
    struct QNode *nodeToFree = tempHead->next;
    printf("================================Process %d has been allocated in the memory===========================================\n", node->data->id);
    tempHead->next = nodeToFree->next;
    nodeToFree->next = NULL;
    free(nodeToFree);
    printWaitList();
}

void tryAllocateProcessesPriorityQueue(struct PQueue *priority_queue) {
    struct PQNode *tempHead = waitPriorityQueue->head;
    while (tempHead != NULL) {
        // Try to allocate process
        struct TNode *node = allocateMemory(memoryTree->root, tempHead->data, tempHead->data->memsize);
        if (node != NULL) {
            // TODO: Testing
            printf("Process %d has been allocated in memory\n", tempHead->data->id);

            // Assign tree node to that of process struct
            tempHead->data->memoryNode = node;
            printMemoryDetails(tempHead->data, 0);

            // TODO: MEMORY
            switch (algorithm) {
                case 1:
                    push(priority_queue, tempHead->data, tempHead->data->priority);
                    break;
                case 2:
                    push(priority_queue, tempHead->data, tempHead->data->runTime);
                    break;
                case 3:
                    perror("Error case 3 is not allowed here\n");
                    exit(-1);
            }
            struct PQNode *tempHeadToRemove = tempHead;
            tempHead = tempHead->next;
            removeFromWaitPriorityQueue(tempHeadToRemove);
            printf("Removed from memory\n");
        } else {
            printf("Process %d Cannot be allocated in memory\n", tempHead->data->id);
            tempHead = tempHead->next;
        }
    }
}


void tryAllocateProcessesQueue(struct Queue *queue) {
    struct QNode *tempHead = waitQueue->front;
    while (tempHead != NULL) {
        // Try to allocate process
        struct TNode *node = allocateMemory(memoryTree->root, tempHead->data, tempHead->data->memsize);
        if (node != NULL) {
            // TODO: Testing
            printf("Process %d has been allocated in memory\n", tempHead->data->id);

            // Assign tree node to that of process struct
            tempHead->data->memoryNode = node;
            printMemoryDetails(tempHead->data, 0);
            // TODO: MEMORY
            enQueue(queue, tempHead->data);
            struct QNode *tempHeadToRemove = tempHead;
            tempHead = tempHead->next;
            removeFromWaitQueue(tempHeadToRemove);
        } else {
            printf("==================== Failed to Allocate in Memory ====================\n");
            tempHead = tempHead->next;
        }
    }
}