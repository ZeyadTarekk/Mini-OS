#include <stdio.h>
#include <stdlib.h>

typedef short bool;
#define true 1
#define false 0

struct TNode {
    int data;
    struct TNode *leftChild, *rightChild, *parent;
};
struct Tree {
    struct TNode *root;
};
//// for testing
//struct Memory {
//    int address; // segment
//};
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

// inserting new Tree
struct Tree *newTree() {
    struct Tree *tree = (struct Tree *) malloc(sizeof(struct Tree));
    tree->root = NULL;
    return tree;
}

// inserting  a new Node
struct TNode *InsertNodeT(struct TNode *root, int addr) {
    struct TNode *newNode = (struct TNode *) malloc(sizeof(struct TNode));
    newNode->parent = root;
    newNode->data = addr;
    newNode->rightChild = NULL;
    newNode->leftChild = NULL;
    return newNode;
}

// insert Node left
struct TNode *insertLeft(struct TNode *root, struct Memory *m) {
    return root->leftChild = InsertNodeT(root, m->end - m->start);
}

// insert Node Right
struct TNode *insertRight(struct TNode *root, struct Memory *m) {
    return root->rightChild = InsertNodeT(root, m->end - m->start);
}

// check tree is empty
bool isEmptyTree(struct Tree *t) {
    if (t->root == NULL)
        return true;
    return false;
}

bool printLevel(struct TNode *root, int level) {
    if (root == NULL)
        return false;
    if (level == 1) {
        printf("%d ", root->data);
        return true;
    }
    bool left = printLevel(root->leftChild, level - 1);
    bool right = printLevel(root->rightChild, level - 1);
    return left || right;
}

// printing for testing
void printTree(struct TNode *root) {
    int level = 1;
    while (printLevel(root, level)) {
        printf("\n");
        level++;
    }
}

// creating an  empty memory
struct Memory *creatMemory() {
    struct Memory *newMem = (struct Memory *) malloc(sizeof(struct Memory));
//    newMem->end = 16;
//    newMem->start = 8;
    newMem->hasProcess = false;
    newMem->pid = -1;
    return newMem;
}

// for testing
int main() {

    struct Tree *temp = newTree();
    struct Memory *mem = creatMemory();
    struct TNode *root = InsertNodeT(temp->root, 1023); ///parent

    // left , right
    mem->start = 0;
    mem->end = 511;
    struct TNode *rr1 = insertRight(root, mem);
    mem->start = 512;
    mem->end = 1023;
    struct TNode *ll1 = insertLeft(root, mem);



    //right left
    mem->end = 767;
    mem->start = 512;
    insertRight(rr1, mem);

    // right right
    mem->end = 1023;
    mem->start = 768;
    insertLeft(rr1, mem);

    // left left
    mem->start = 0;
    mem->end = 255;
    insertLeft(ll1, mem);

    // left right
    mem->start = 256;
    mem->end = 511;
    insertRight(ll1, mem);

    printTree(root);
    printf("####################################\n");
    printf("Root:%d\n", root->data);
    printf("Right:%d\n", root->rightChild->data);
    printf("Left:%d\n", root->leftChild->data);
    printf("Right-Right:%d\n", root->rightChild->rightChild->data);
    printf("Right-Left:%d\n", root->rightChild->leftChild->data);
    printf("Left-Right:%d\n", root->leftChild->rightChild->data);
    printf("Left-Left:%d\n", root->leftChild->leftChild->data);

    return 0;
}


