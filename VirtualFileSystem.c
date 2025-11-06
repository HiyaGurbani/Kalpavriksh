#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

//Virtual Disk
#define BLOCK_SIZE 5 //Column Number
#define NUMBER_OF_BLOCKS 5 //Row Number 

#define NAME_SIZE 100
#define MAX_BLOCKS_PER_FILE 5

#define INPUT_SIZE 100

typedef struct FreeBlock {
    int data;
    struct FreeBlock* prev;
    struct FreeBlock* next;
} FreeBlock;

//Files and Directory
typedef struct FileNode {
    char name[NAME_SIZE];
    bool isFile;
    int blockPointers[MAX_BLOCKS_PER_FILE];
    struct FileNode *next;
    struct FileNode *child;
} FileNode;

FreeBlock *createFreeBlockNode(int val) {
    FreeBlock *newNode = malloc(sizeof(FreeBlock));
    if (newNode == NULL)
    {
        printf("Memory Allocation Failed.");
        exit(1);
    }
    newNode->data = val;
    newNode->prev = NULL;
    newNode->next = NULL;
}

FileNode *createFileNode(char *name) {
    FileNode *newNode = malloc(sizeof(FileNode));
    if (newNode == NULL)
    {
        printf("Memory Allocation Failed.");
        exit(1);
    }
    strcpy(newNode->name, name);
    newNode->next = newNode; //Circular Linked List
    newNode->child = NULL;
}



int (*createVirtualDisk())[BLOCK_SIZE] {
    int (*virtualDisk)[BLOCK_SIZE] = malloc(NUMBER_OF_BLOCKS * sizeof(*virtualDisk));
    if (virtualDisk == NULL)
    {
        printf("Memory Allocation Failed.");
        exit(1);
    }
    return virtualDisk;
}

void createFreeBlocksStorage(FreeBlock **head, FreeBlock **tail) {
    FreeBlock *prev = NULL;
    for (int blockNumber = 0; blockNumber < NUMBER_OF_BLOCKS; blockNumber++)
    {
        FreeBlock *node = createFreeBlockNode(blockNumber);
        if (blockNumber == 0) {
            *head = node;
        }
        if (blockNumber == NUMBER_OF_BLOCKS - 1) {
            *tail = node;
        }
        node->prev = prev;
        if (prev) prev->next = node;
        prev = node;
    }
}

FileNode *createRootDirectory() {
    FileNode *root = createFileNode("/");
    root->isFile = false;

    return root;
}

int main() {
    //Initialisation
    int (*virtualDisk)[BLOCK_SIZE] = createVirtualDisk();

    FreeBlock *head = NULL, *tail = NULL;
    createFreeBlocksStorage(&head, &tail);

    FileNode *root = createRootDirectory();

    
    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    char input[INPUT_SIZE];
    while (strcmp(input, "exit") != 0)
    {
        printf("/> ");
        
        fgets(input, INPUT_SIZE, stdin);
        input[strcspn(input, "\n")] = '\0';

        printf("\n");
    }
}