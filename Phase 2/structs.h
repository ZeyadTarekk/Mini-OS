//Tree
struct TNode {
    struct Memory *data;
    struct TNode *leftChild, *rightChild, *parent;
};
struct Tree {
    struct TNode *root;
};


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


//Memory struct
struct Memory {
    // Start of memory [0 : 1023 - min limit ]
    // Min limit is initially defined with 8
    int start;

    // End of memory [0 + min limit : 1023]
    int end;

    // true  : Has a process allocated at this memory partition
    // false : Doesn't have a process allocated at this memory partition
    bool hasProcess;

    // This is the process id from the input file
    // This data member may be useless
    // Set it to -1 if hasProcess = false
    int pid;
};

//the struct of process
struct ProcessStruct
{
    //info from the file
    int id;
    int arrivalTime;
    int priority;
    int runTime;
    int memsize;

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

    // Pointer to the TNode allocated for this process
    struct TNode *memoryNode;

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