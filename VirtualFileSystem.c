#include<stdio.h>
#include<stdlib.h>

//Virtual Disk
#define BLOCK_SIZE 5 //Column Number
#define NUMBER_OF_BLOCKS 5 //Row Number 

typedef struct FreeBlock {
    int data;
    FreeBlock* prev;
    FreeBlock* next;
} FreeBlock;

FreeBlock *createNode(int val) {
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

void createFreeBlocksStorage(FreeBlock **head, FreeBlock **tail) {
    FreeBlock *prev = NULL;
    FreeBlock *next = NULL;
    for (int i = 0; i < NUMBER_OF_BLOCKS; i++)
    {
        FreeBlock *node = createNode(i);
        if (i == 0) {
            *head = node;
        }
        if (i == NUMBER_OF_BLOCKS - 1) {
            *tail = node;
        }
        node->prev = prev;
        if (prev) prev->next = node;
        prev = node;
    }
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

int main() {
    int (*virtualDisk)[BLOCK_SIZE] = createVirtualDisk();

    FreeBlock *head = NULL, *tail = NULL;
    createFreeBlocksStorage(&head, &tail);
}