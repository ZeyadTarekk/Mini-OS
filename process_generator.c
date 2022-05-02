/*
Code for chosen scheduling algorithm:
=====================================
HPF --> 1
SRTN --> 2
RR --> 3
*/
#include "headers.h"

void clearResources(int);

void readFile(struct Queue* processQueue) {
    FILE * file = fopen("processes.txt", "r");

    int id, arrivaltime, runningtime, priority;
    //read the first line (commented one)
    char s[30];
    fgets(s, 30, file);

    //start reading the file
    while (fscanf(file, "%d\t%d\t%d\t%d", &id, &arrivaltime, &runningtime, &priority) != EOF)
    {
        struct ProcessStruct* p = (struct ProcessStruct*)malloc(sizeof(struct ProcessStruct));
        p->id = id;
        p->arrivalTime = arrivaltime;
        p->priority = priority;
        p->runTime = runningtime;
        p->running = false;
        p->startedBefore = false;
        p->enterQueue = 0;
        p->quitQueue = 0;
        p->executionTime = 0;
        p->waitingTime = 0;
        
        enQueue(processQueue, p);
    }
    fclose(file);
}

int main(int argc, char * argv[])
{
    // signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    struct Queue* processQueue = createQueue();
    readFile(processQueue);

    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock
    // initClk();
    // To get time use this
    // int x = getClk();
    // printf("current time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    // destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}
