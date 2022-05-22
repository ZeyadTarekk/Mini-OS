/*
The total memory size is 1024 bytes.
Each process size is less than or equal 256 bytes.
*/

#include "Tree.h"
//#include "structs.h"

// Function used to get the first power of two that can fit the size
// EX: [97 -> 128]   [250 -> 256]   [64 -> 64]   [65 -> 128]
int getPowerOfTwo(int size) {
    int max = 1024;
    while ((max / 2) >= size)
        max /= 2;
    return max;
}

// Function used to split the current memory part into two other
// parts with size = currentSize / 2
// EX:  [1024 --> 512 512]    [512 --> 256 256]
void splitMemory(struct TNode *root) {
    int start = root->data->start;
    int end = root->data->end;
    int currentSize = end - start + 1;

    //create two other Memory struct (1024 --> 512  512)
    int firstEnd = start + (currentSize / 2) - 1;
    struct Memory *memLeft = creatMemory(start, firstEnd);
    struct Memory *memRight = creatMemory(firstEnd + 1, end);

    insertLeft(root, memLeft);
    insertRight(root, memRight);
}

// Function used to allocate memory for the sent process with the perfect size
// Return NULL if there is no available memory at this moment
struct TNode *allocateMemory(struct TNode *root, struct ProcessStruct *process, int powerOfTwo) {

    // check if the root has no process and size is good --> then assign and return
    if ((root->data->hasProcess == true && (root->data->end - root->data->start + 1) == powerOfTwo)
        || (root->data->hasProcess == true && root->data->pid != -1)) {
        // perfect size for the process but it is taken by other process
        // or this part is already taken by a process and can't be divided
        return NULL;
    } else if (root->data->hasProcess == false && (root->data->end - root->data->start + 1) == powerOfTwo) {
        // found the perfect part in the memory to allocate the process in
        root->data->hasProcess = true;
        root->data->pid = process->id;
        return root;
    } else {
        //check if it has no process then we need to split it into two parts
        if (root->data->hasProcess == false) {
            root->data->hasProcess = true;
            splitMemory(root);
            return allocateMemory(root->leftChild, process, powerOfTwo);
        } else {
            //root have a running process
            //need to check the left branch first then check the right one
            if (root->leftChild == NULL)
                splitMemory(root);

            struct TNode *leftBranch = allocateMemory(root->leftChild, process, powerOfTwo);
            if (leftBranch == NULL)
                return allocateMemory(root->rightChild, process, powerOfTwo);
            else return leftBranch;
        }
    }
}

void deAllocateMemory(struct TNode *memoryNodeToFree) {
//    Check if the node is null
//    TODO: make the parent of the root = NULL
    if (memoryNodeToFree == NULL)
        return;
    int mySize;
    mySize = memoryNodeToFree->data->end - memoryNodeToFree->data->start + 1;
    if (mySize == 1024)
        return;
//    printf("My address is %d\n",&memoryNodeToFree);
//    first save the parent in a pointer
    struct TNode *myParent = memoryNodeToFree->parent;
//    then free my memory process
//    TODO I need the id not the pid to print it
    printf("deallocating memory from process with PID %d from: %d to: %d\n", memoryNodeToFree->data->pid,
           memoryNodeToFree->data->start, memoryNodeToFree->data->end);
    int oldPid = memoryNodeToFree->data->pid;
    memoryNodeToFree->data->hasProcess = false;
    memoryNodeToFree->data->pid = -1;
//    get my sibling
    struct TNode *mySibling;
    if (myParent->leftChild == memoryNodeToFree) {
        mySibling = myParent->rightChild;
//        printf("RIGHT\n");
    } else {
        mySibling = myParent->leftChild;
//        printf("LEFT\n");
    }
    if (mySibling->data->hasProcess == false) {
        printf("My sibling is free, my pid: %d\n", oldPid);
//        free(memoryNodeToFree);
//        free(mySibling);
        deAllocateMemory(myParent);
    }


}