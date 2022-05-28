#include "headers.h"


int main(int argc, char *argv[]) {

    struct Tree *memoryTree = createMemoryTree(1024);

    
    struct ProcessStruct *process = (struct ProcessStruct *) malloc(sizeof(struct ProcessStruct));
    process->id = 5;


    if (allocateMemory(memoryTree->root, process, 512) == NULL) {
        printf("shit\n");
    }

    // printTree(memoryTree->root);
    // printf("\n");

    if (allocateMemory(memoryTree->root, process, 256) == NULL) {
        printf("shit\n");
    }

    // printTree(memoryTree->root);
    // printf("\n");

    if (allocateMemory(memoryTree->root, process, 32) == NULL) {
        printf("shit\n");
    }

    // printTree(memoryTree->root);
    // printf("\n");

    if (allocateMemory(memoryTree->root, process, 64) == NULL) {
        printf("shit\n");
    }




    printTree(memoryTree->root);

    
    return 0;
}