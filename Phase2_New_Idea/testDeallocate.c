//ToBeRemoved
//
// Created by zeyad on 22/05/22.
//
#include "headers.h"
//#include "scheduler.c"


void createMemoryLog() {
    /*
     * A function that creates an empty file with the beginning line for memory.log
     * */
    FILE *outputFile;
    outputFile = fopen("memory.log", "w");
    fprintf(outputFile, "%s", "#At time x allocated y bytes for process z from i t o j\n");
    fclose(outputFile);
}

void printMemoryDetails(const struct ProcessStruct *const process, int state) {
/*
 * State 0 => allocation
 * State 1 => deallocation
 */
    FILE *outputFile;
    int currentTime = 1;
    outputFile = fopen("memory.log", "a");
    if (!outputFile) {
        perror("Unable to open file \"memory.log\" terminating ...\n");
        exit(-1);
    }

//    At time 3 allocated 200 bytes for process 2 from 256 t o 383
//    At time 6 freed 200 bytes from process 2 from 256 t o 383
    if (state == 0)
        fprintf(outputFile, "At time %d allocated %d bytes for process %d from %d to %d\n", currentTime,
                process->memsize, process->id, process->memoryNode->data->start, process->memoryNode->data->end);
    else if (state == 1)
        fprintf(outputFile, "At time %d freed %d bytes for process %d from %d to %d\n", currentTime,
                process->memsize, process->id, process->memoryNode->data->start, process->memoryNode->data->end);

    fclose(outputFile);

}


int main() {
    createMemoryLog();
    struct Tree *memoryTree = createMemoryTree(1024);
    struct ProcessStruct *process1 = (struct ProcessStruct *) malloc(sizeof(struct ProcessStruct));
    process1->id = 1;
    process1->memsize = 200;
    struct ProcessStruct *process2 = (struct ProcessStruct *) malloc(sizeof(struct ProcessStruct));
    process2->id = 2;
    struct ProcessStruct *process3 = (struct ProcessStruct *) malloc(sizeof(struct ProcessStruct));
    process3->id = 3;
    struct ProcessStruct *process4 = (struct ProcessStruct *) malloc(sizeof(struct ProcessStruct));
    process4->id = 4;
    struct ProcessStruct *process5 = (struct ProcessStruct *) malloc(sizeof(struct ProcessStruct));
    process4->id = 5;
    struct ProcessStruct *process6 = (struct ProcessStruct *) malloc(sizeof(struct ProcessStruct));
    process4->id = 6;
//    struct TNode* memory1 = allocateMemory(memoryTree->root, process1, 512);
    process2->memsize = 200;
    process2->memoryNode = allocateMemory(memoryTree->root, process2, 256);
    printMemoryDetails(process2,0);
    process3->memsize = 200;
    process3->memoryNode = allocateMemory(memoryTree->root, process3, 256);
    printMemoryDetails(process3,0);
    process4->memsize = 100;

    process4->memoryNode = allocateMemory(memoryTree->root, process4, 128);
    printMemoryDetails(process4,0);
    process5->memsize = 100;

    process5->memoryNode = allocateMemory(memoryTree->root, process5, 128);
    printMemoryDetails(process5,0);

    printTree(memoryTree->root);
//    deAllocateMemory(memory4);
    deAllocateMyMemory(process4);
    printMemoryDetails(process4,1);

    printTree(memoryTree->root);
//    deAllocateMemory(memory5);
    deAllocateMyMemory(process5);
    printMemoryDetails(process5,1);


    printTree(memoryTree->root);

//    deAllocateMemory(memory3);
//    deAllocateMyMemory(process3);

    printTree(memoryTree->root);
//    deAllocateMemory(memory2);
    deAllocateMyMemory(process2);
    printMemoryDetails(process2,1);


    printTree(memoryTree->root);


//    struct TNode *testNode = memoryTree->root->rightChild;
//    deAllocateMemory(testNode);

}