//
// Created by zeyad on 22/05/22.
//
#include "headers.h"


int main() {
    struct Tree *memoryTree = createMemoryTree(1024);
    struct ProcessStruct *process1 = (struct ProcessStruct *) malloc(sizeof(struct ProcessStruct));
    process1->id = 1;
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
    process2->memoryNode = allocateMemory(memoryTree->root, process2, 256);
    process3->memoryNode = allocateMemory(memoryTree->root, process3, 256);
    process4->memoryNode = allocateMemory(memoryTree->root, process4, 128);
    process5->memoryNode = allocateMemory(memoryTree->root, process5, 128);
    printTree(memoryTree->root);
//    deAllocateMemory(memory4);
    deAllocateMyMemory(process4);
    printTree(memoryTree->root);
//    deAllocateMemory(memory5);
    deAllocateMyMemory(process5);

    printTree(memoryTree->root);

//    deAllocateMemory(memory3);
    printTree(memoryTree->root);
//    deAllocateMemory(memory2);
    deAllocateMyMemory(process2);

    printTree(memoryTree->root);


//    struct TNode *testNode = memoryTree->root->rightChild;
//    deAllocateMemory(testNode);

}