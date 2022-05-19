
// creating an  empty memory
struct Memory *creatMemory(int start, int end) {
    struct Memory *newMem = (struct Memory *)malloc(sizeof(struct Memory));
    newMem->hasProcess = false;
    newMem->pid = -1;
    newMem->start = start;
    newMem->end = end;
    return newMem;
}

//Tree
struct TNode {
    struct Memory *data;
    struct TNode *leftChild, *rightChild, *parent;
};
struct Tree {
    struct TNode *root;
};

// creating new Tree
struct Tree *createMemoryTree(int size) {
    struct Tree *tree = (struct Tree *)malloc(sizeof(struct Tree));
    struct Memory *data = creatMemory(0, size - 1);
    tree->root = (struct TNode *)malloc(sizeof(struct TNode));
    tree->root->data = data;
    tree->root->leftChild = NULL;
    tree->root->rightChild = NULL;
    tree->root->parent = NULL;
    return tree;
}

// inserting  a new Node
struct TNode *InsertNodeT(struct TNode *root, struct Memory *addr) {
    struct TNode *newNode = (struct TNode *)malloc(sizeof(struct TNode));
    newNode->parent = root;
    newNode->data = addr;
    newNode->rightChild = NULL;
    newNode->leftChild = NULL;
    return newNode;
}

// insert Node left
struct TNode *insertLeft(struct TNode *root, struct Memory *m) {
    return root->leftChild = InsertNodeT(root, m);
}

// insert Node Right
struct TNode *insertRight(struct TNode *root, struct Memory *m) {
    return root->rightChild = InsertNodeT(root, m);
}

// check tree is empty
bool isEmptyTree(struct Tree *t) {
    return t->root == NULL;
}

bool printLevel(struct TNode *root, int level) {
    if (root == NULL)
        return false;
    if (level == 1) {
        printf("[%d-%d  %d  %d] ", root->data->start, root->data->end,
                                   root->data->hasProcess, root->data->pid);
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