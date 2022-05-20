// Normal Queue
struct QNode {
    struct ProcessStruct* data;
    struct QNode* next;
};

struct Queue {
    struct QNode *front, *rear;
};


//Priority Queue
struct PQNode {
    struct ProcessStruct* data;
    // Lower values indicate higher priority
    int priority;
    struct PQNode* next;
};
struct PQueue {
    struct PQNode *head;
};


//the struct of process
struct ProcessStruct
{
    //info from the file
    int id;
    int arrivalTime;
    int priority;
    int runTime;

    bool running;

    // if equals false then create new process
    // else wake the process up
    bool startedBefore;

    // if (enterQueue == 0) enterQueue = arrivalTime
    // else enterQueue = clk;
    int enterQueue;

    // quitQueue = clk;
    int quitQueue;

    // executionTime = executionTime + (clk - quitQueue)
    int executionTime;

    // waitingTime = clk - enterQueue;
    int waitingTime;

    // The real id of forked process
    int pid;

};


/*
Messages Type:
==============
7 -> process from process_generator to schedular
*/
struct msgbuff
{
    long mtype;
    struct ProcessStruct process;
};