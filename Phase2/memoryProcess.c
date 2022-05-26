#include "headers.h"
#include "assert.h"

// Memory tree
struct Tree *memoryTree;

// Total memory size
const int memorySize = 1024;

// Exit flag (No more processes from process generator)
bool flag = true;


/**************************************** WaitList start ****************************************/

// Max size of the waitList
const int MAXSIZE = 10000;

// ActualSize of the waitList
int actualSize = 0;

// WaitQueue
struct ProcessStruct **waitList;


/*
 * INPUT       : process
 * OUTPUT      : NONE
 * RETURN      : VOID
 * DESCRIPTION : Adds the given process to the waitList
 * */
void addToWaitList(const struct ProcessStruct *const process) {
    // Make sure that the actual size not equals maxsize
    assert(actualSize != MAXSIZE);

    // Allocate the process
    struct ProcessStruct *newProcess = create_process(process->id, process->arrivalTime, process->priority,
                                                      process->runTime, process->running, process->startedBefore,
                                                      process->enterQueue, process->quitQueue, process->executionTime,
                                                      process->waitingTime, process->pid);
    newProcess->memsize = process->memsize;

    // Add to current position
    waitList[actualSize] = newProcess;

    // Increment the size
    actualSize++;
}

/*
 * INPUT       : process
 * OUTPUT      : Index of process
 * RETURN      : int
 * DESCRIPTION : find the index of the process in the waitList
 * */
int findProcess(const struct ProcessStruct *const process) {
    for (int i = 0; i < actualSize; i++) {
        if (waitList[i]->id == process->id)
            return i;
    }
    return -1;
}

/*
 * INPUT       : process
 * OUTPUT      : NONE
 * RETURN      : VOID
 * DESCRIPTION : Deletes the given process from waitList if exist
 * */
void removeFromWaiList(const struct ProcessStruct *const process) {
    // Get index of process
    int index = findProcess(process);

    // Process not found
    if (index == -1)return;

    // Shift processes
    for (int i = index; i < actualSize - 1; i++)
        waitList[i] = waitList[i + 1];

    // Decrement the actual size
    actualSize--;
}

void printWaitList() {
    printf("MAXSIZE = %d Actual Size = %d \n", MAXSIZE, actualSize);
    for (int i = 0; i < actualSize; i++) {
        printf("Process id = %d size = %d\n", waitList[i]->id, waitList[i]->memsize);
    }
}
/**************************************** WaitList end ****************************************/

/*
 * INPUT       : NONE
 * OUTPUT      : NONE
 * RETURN      : VOID
 * DESCRIPTION : This function loops over all processes in the wait list and try to allocate them
 * */


void tryAllocateProcesses() {
    for (int i = 0; i < actualSize; i++) {
        // Try to allocate process
        struct TNode *node = allocateMemory(memoryTree->root, waitList[i], waitList[i]->memsize);

        if (node != NULL) {
            // TODO: Testing
            printf("Process %d has been allocated in memory\n", waitList[i]->id);

            // Assign tree node to that of process struct
            waitList[i]->memoryNode = node;

            // TODO: Send process to scheduler [BESHOY]

            // remove process
            removeFromWaiList(waitList[i]);
        } else {
            // TODO: Testing
            printf("Process %d Cannot be allocated in memory\n", waitList[i]->id);
        }
    }
}


/**************************************** Memory Process Test Start ****************************************/
// Testing waitList operations
void testMemoryProcess() {
    for (int i = 0; i < 5; i++) {
        struct ProcessStruct *p = create_process(i, i, i, i, i, i, i, i, i, i, i);
        p->memsize = 100;
        addToWaitList(p);
        printf("Loop %d\n", i);
        printWaitList();
    }
    printf("After Loop\n");
    printWaitList();

    for (int i = 0; i < 5; i++) {
        struct ProcessStruct *p = create_process(i, i, i, i, i, i, i, i, i, i, i);
        p->memsize = 100;
        removeFromWaiList(p);
        printf("Loop %d\n", i);
        printWaitList();
    }

}

// Testing allocate process operations
void testAllocateProcess() {
    for (int i = 0; i < 5; i++) {
        struct ProcessStruct *p = create_process(i, i, i, i, i, i, i, i, i, i, i);
        p->memsize = 200;
        printf("Size = %d\n", p->memsize);
        addToWaitList(p);
        printf("Loop %d\n", i);
        printWaitList();
    }
    printf("After Loop\n");
    printWaitList();
    printf("\n\n\n\n");
    printTree(memoryTree->root);
    for (int i = 0; i < 5; i++) {
        printf("Loop %d\n", i);
        printWaitList();
        tryAllocateProcesses();
        printf("\n\n\n\n");
        printTree(memoryTree->root);
    }
}

/**************************************** Memory Process Test End   ****************************************/





int main() {
    // Allocate memory tree
    memoryTree = createMemoryTree(memorySize);

    // Allocate the waitList
    waitList = malloc(MAXSIZE * sizeof(struct ProcessStruct *));

    //testMemoryProcess();
    //testAllocateProcess();

    // Dummy loop
    while (flag || actualSize != 0) {
        tryAllocateProcesses();
    }

    return 0;
}