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