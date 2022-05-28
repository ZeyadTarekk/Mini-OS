#include "assert.h"

// Memory tree
struct Tree *memoryTree;

// Total memory size
const int memorySize = 1024;

/**************************************** WaitList start ****************************************/

// Max size of the waitList
//const int MAXSIZE = 10000;

// ActualSize of the waitList
//int actualSize = 0;

// WaitQueue
//struct ProcessStruct **waitList;

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
//    assert(actualSize != MAXSIZE);
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
    printf("end Process add to wait list\n");
}

/*
 * INPUT       : process
 * OUTPUT      : Index of process
 * RETURN      : int
 * DESCRIPTION : find the index of the process in the waitList
 * */
//int findProcess(const struct ProcessStruct *const process) {
//    for (int i = 0; i < actualSize; i++) {
//        if (waitList[i]->id == process->id)
//            return i;
//    }
//    return -1;
//}

/*
 * INPUT       : process
 * OUTPUT      : NONE
 * RETURN      : VOID
 * DESCRIPTION : Deletes the given process from waitList if exist
 * */
//void removeFromWaiList(const struct ProcessStruct *const process) {
//    // Get index of process
//    int index = findProcess(process);
//
//    // Process not found
//    if (index == -1)return;
//
//    // Shift processes
//    for (int i = index; i < actualSize - 1; i++)
//        waitList[i] = waitList[i + 1];
//
//    // Decrement the actual size
//    actualSize--;
//}

void printWaitList() {
    struct PQNode *tempHead = waitPriorityQueue->head;
    while (tempHead) {
        printf("Process id = %d size = %d\n", tempHead->data->id, tempHead->data->memsize);
        tempHead = tempHead->next;
    }
}

void removeFromWaitPriorityQueue(struct PQNode *node) {
    printf("Reeeeeeeeeeeeeecived Process %d has been allocated in memory\n", node->data->id);

    struct PQNode *tempHead = waitPriorityQueue->head;
    if (tempHead == node) {
        waitPriorityQueue->head = waitPriorityQueue->head->next;
        tempHead->next = NULL;
        printf("Freeeeeeeeeeeeeeeeeeeeeeeeeeeeeee head\n");
        free(tempHead);
        return;
    }
    while (tempHead->next != node) {
        tempHead = tempHead->next;
    }
    struct PQNode *nodeToFree = tempHead->next;
    printf("================================Process %d has been Deallocated From memory\n", 1);
    tempHead->next = nodeToFree->next;
    nodeToFree->next = NULL;
    free(nodeToFree);
    printWaitList();
}

void removeFromWaitQueue(struct QNode *node) {
    printf("Reeeeeeeeeeeeeecived Process %d has been allocated in memory\n", node->data->id);

    struct QNode *tempHead = waitQueue->front;
    if (tempHead == node) {
        waitQueue->front = waitQueue->front->next;
        tempHead->next = NULL;
        printf("Freeeeeeeeeeeeeeeeeeeeeeeeeeeeeee head\n");
        free(tempHead);
        return;
    }
    while (tempHead->next != node) {
        tempHead = tempHead->next;
    }
    struct QNode *nodeToFree = tempHead->next;
    printf("================================Process %d has been Deallocated From memory\n", 1);
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
            printf("REEEEEEEEEEEEEEEEEEEEEEEmoved from memory\n");
        } else {
            printf("CCCCCCCCCCCCCCCCCCANOT Process %d Cannot be allocated in memory\n", tempHead->data->id);
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

            // TODO: MEMORY
            enQueue(queue, tempHead->data);
            struct QNode *tempHeadToRemove = tempHead;
            tempHead = tempHead->next;
            removeFromWaitQueue(tempHeadToRemove);
        } else {
            tempHead = tempHead->next;
        }
    }
}
/**************************************** WaitList end ****************************************/

/*
 * INPUT       : NONE
 * OUTPUT      : NONE
 * RETURN      : VOID
 * DESCRIPTION : This function loops over all processes in the wait list and try to allocate them
 * */

// queueType 1 for SRTN , 2 for HPF and 3 for RR
//void tryAllocateProcesses(struct PQueue *priority_queue, struct Queue *queue) {
//    for (int i = 0; i < actualSize; i++) {
//        // Try to allocate process
//        struct TNode *node = allocateMemory(memoryTree->root, waitList[i], waitList[i]->memsize);
//
//        if (node != NULL) {
//            // TODO: Testing
//            printf("Process %d has been allocated in memory\n", waitList[i]->id);
//
//            // Assign tree node to that of process struct
//            waitList[i]->memoryNode = node;
//
//            // TODO: MEMORY
//            switch (algorithm) {
//                case 1:
//                    push(priority_queue, waitList[i], waitList[i]->priority);
//                    break;
//                case 2:
//                    push(priority_queue, waitList[i], waitList[i]->runTime);
//                    break;
//                case 3:
//                    enQueue(queue, waitList[i]);
//                    break;
//            }
//
//            // remove process
//            removeFromWaiList(waitList[i]);
//        } else {
//            // TODO: Testing
//            printf("Process %d Cannot be allocated in memory\n", waitList[i]->id);
//        }
//    }
//}


/**************************************** Memory Process Test Start ****************************************/
// Testing waitList operations
//void testMemoryProcess() {
//    for (int i = 0; i < 5; i++) {
//        struct ProcessStruct *p = create_process(i, i, i, i, i, i, i, i, i, i, i);
//        p->memsize = 100;
//        addToWaitList(p);
//        printf("Loop %d\n", i);
//        printWaitList();
//    }
//    printf("After Loop\n");
//    printWaitList();
//
//    for (int i = 0; i < 5; i++) {
//        struct ProcessStruct *p = create_process(i, i, i, i, i, i, i, i, i, i, i);
//        p->memsize = 100;
//        removeFromWaiList(p);
//        printf("Loop %d\n", i);
//        printWaitList();
//    }
//
//}

// Testing allocate process operations
//void testAllocateProcess() {
//    for (int i = 0; i < 5; i++) {
//        struct ProcessStruct *p = create_process(i, i, i, i, i, i, i, i, i, i, i);
//        p->memsize = 200;
//        printf("Size = %d\n", p->memsize);
//        addToWaitList(p);
//        printf("Loop %d\n", i);
//        printWaitList();
//    }
//    printf("After Loop\n");
//    printWaitList();
//    printf("\n\n\n\n");
//    printTree(memoryTree->root);
//    for (int i = 0; i < 5; i++) {
//        printf("Loop %d\n", i);
//        printWaitList();
//        //tryAllocateProcesses();
//        printf("\n\n\n\n");
//        printTree(memoryTree->root);
//    }
//}

/**************************************** Memory Process Test End   ****************************************/





//int main() {
//    // Allocate memory tree
//    memoryTree = createMemoryTree(memorySize);
//
//    // Allocate the waitList
//    waitList = malloc(MAXSIZE * sizeof(struct ProcessStruct *));
//
//    //testMemoryProcess();
//    //testAllocateProcess();
//
//    // Dummy loop
//    while (flag || actualSize != 0) {
//        tryAllocateProcesses();
//    }
//
//    return 0;
//}