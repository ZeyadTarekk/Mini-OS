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