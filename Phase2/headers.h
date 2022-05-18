#include <stdio.h>      //if you don't use scanf/printf change this include
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

typedef short bool;
#define true 1
#define false 0

//keys for ftok function
#define SHKEY 300
#define PROSCH 65

///==============================
//don't mess with this variable//
int * shmaddr;                 //
//===============================



int getClk()
{
    return *shmaddr;
}


/*
 * All process call this function at the beginning to establish communication between them and the clock module.
 * Again, remember that the clock is only emulation!
*/
void initClk()
{
    int shmid = shmget(SHKEY, 4, 0444);
    while ((int)shmid == -1)
    {
        //Make sure that the clock exists
        printf("Wait! The clock not initialized yet!\n");
        sleep(1);
        shmid = shmget(SHKEY, 4, 0444);
    }
    shmaddr = (int *) shmat(shmid, (void *)0, 0);
}


/*
 * All process call this function at the end to release the communication
 * resources between them and the clock module.
 * Again, Remember that the clock is only emulation!
 * Input: terminateAll: a flag to indicate whether that this is the end of simulation.
 *                      It terminates the whole system and releases resources.
*/

void destroyClk(bool terminateAll)
{
    shmdt(shmaddr);
    if (terminateAll)
    {
        killpg(getpgrp(), SIGINT);
    }
}

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
    // char mtext[70];
    struct ProcessStruct process;
};


// Normal Queue
struct QNode {
    struct ProcessStruct* data;
    struct QNode* next;
};

struct Queue {
    struct QNode *front, *rear;
};

// Function to create an empty queue
struct Queue* createQueue()
{
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

// Function to create a new linked list node.
struct QNode* newNNode(struct ProcessStruct* d)
{
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->data = d;
    temp->next = NULL;
    return temp;
}

// The function to add a key k to q
void enQueue(struct Queue* q, struct ProcessStruct* d)
{
    // Create a new LL node
    struct QNode* temp = newNNode(d);

    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
}

// Return the value at head
struct ProcessStruct* peekN(struct Queue* q)
{
    return q->front->data;
}

// Function to remove a key from given queue q
void deQueue(struct Queue* q)
{
    // If queue is empty, return.
    if (q->front == NULL)
        return;

    // Store previous front and move front one node ahead
    struct QNode* temp = q->front;

    q->front = q->front->next;

    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
        q->rear = NULL;

    free(temp);
}

// Function to check is list is empty
bool isEmptyN(struct Queue* q)
{
    return q->front == NULL;
}



/*
HOW TO USE:
===========

struct ProcessStruct* p1 = (struct ProcessStruct*)malloc(sizeof(struct ProcessStruct));

*create the queue and insert elements in it
===========================================
struct Queue* q = createQueue();
enQueue(q, p1);

*check if the queue is empty
============================
isEmptyN(q)

*get the first element in the queue
===================================
struct ProcessStruct* p = peekN(q);

*** don't forget to call deQueue(q) after finish playing with (p)

*/


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
// Function to create an empty queue
struct PQueue* createPriorityQueue()
{
    struct PQueue* q = (struct PQueue*)malloc(sizeof(struct PQueue));
    q->head = NULL;
    return q;
}

// Function to create a new node
struct PQNode* newNode(struct ProcessStruct* d, int p)
{
    struct PQNode* temp = (struct PQNode*)malloc(sizeof(struct PQNode));
    temp->data = d;
    temp->priority = p;
    temp->next = NULL;
    return temp;
}

// Return the value at head
struct ProcessStruct* peek(struct PQueue* q)
{
    return q->head->data;
}

// Removes the element with the
// highest priority form the list
void pop(struct PQueue* q)
{
    struct PQNode* temp = q->head;
    q->head = q->head->next;
    free(temp);
}

// Function to push according to priority
void push(struct PQueue* q, struct ProcessStruct* d, int p)
{
    struct PQNode* start = q->head;
    // Create new Node
    struct PQNode* temp = newNode(d, p);

    if (start == NULL) {
        q->head = temp;
    } else {
        // Special Case: The head of list has
        // lesser priority than new node. So
        // insert newnode before head node
        // and change head node.
        if (q->head->priority > p)
        {
            // Insert New Node before head
            temp->next = q->head;
            q->head = temp;
        }
        else
        {
            // Traverse the list and find a
            // position to insert new node
            while (start->next != NULL && start->next->priority <= p)
            {
                start = start->next;
            }
            // Either at the ends of the list
            // or at required position
            temp->next = start->next;
            start->next = temp;
        }
    }
}

// Function to check is list is empty
bool isEmpty(struct PQueue* q)
{
    return q->head == NULL;
}

/*
HOW TO USE:
===========

struct ProcessStruct* p1 = (struct ProcessStruct*)malloc(sizeof(struct ProcessStruct));

*create the queue and insert elements in it
===========================================
struct PQueue* q = createPriorityQueue();
push(q, p1, 1);
            priority

*check if the queue is empty
============================
isEmpty(q)

*get the first element in the queue
===================================
struct ProcessStruct* p = peek(q);

*** don't forget to call pop(q) after finish playing with (p)

*/

// Semaphores

/* arg for semctl system calls. */
union Semun
{
    int val;               /* value for SETVAL */
    struct semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
    ushort *array;         /* array for GETALL & SETALL */
    struct seminfo *__buf; /* buffer for IPC_INFO */
    void *__pad;
};

void down(int sem)
{
    struct sembuf p_op;

    p_op.sem_num = 0;
    p_op.sem_op = -1;
    p_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &p_op, 1) == -1)
    {
        perror("Error in down()");
        exit(-1);
    }
}

void up(int sem)
{
    struct sembuf v_op;

    v_op.sem_num = 0;
    v_op.sem_op = 1;
    v_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &v_op, 1) == -1)
    {
        perror("Error in up()");
        exit(-1);
    }
}