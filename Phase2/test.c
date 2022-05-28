#include "headers.h"


int main(int argc, char *argv[]) {

    struct Tree *memoryTree = createMemoryTree(1024);

    if (allocateMemory(memoryTree->root, 1, 500) == NULL) {
        printf("shit\n");
    }


    if (allocateMemory(memoryTree->root, 2, 510) == NULL) {
        printf("shit\n");
    }

    struct TNode* found1 = getTNodeByProcessID(memoryTree->root, 1);
    struct TNode* found2 = getTNodeByProcessID(memoryTree->root, 2);



    printTree(memoryTree->root);
    printf("\n");

    deAllocateMyMemory(found1);
    deAllocateMyMemory(found2);


    printTree(memoryTree->root);
    printf("\n");



    printf("allocating 3 processes again\n\n\n\n");

    if (allocateMemory(memoryTree->root, 1, 500) == NULL) {
        printf("shit\n");
    }


    if (allocateMemory(memoryTree->root, 2, 230) == NULL) {
        printf("shit\n");
    }

    if (allocateMemory(memoryTree->root, 3, 32) == NULL) {
        printf("shit\n");
    }

    printTree(memoryTree->root);
    printf("\n");
    

    deAllocateMyMemory(getTNodeByProcessID(memoryTree->root, 3));

    printTree(memoryTree->root);
    printf("\n");

    return 0;
}