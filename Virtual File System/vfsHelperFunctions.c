#include<stdio.h>
#include<stdbool.h>

#include "vfs.h"

// Disk Management
FreeBlock *createFreeBlockNode(int value) {
    FreeBlock *newNode = malloc(sizeof(FreeBlock));
    if (newNode == NULL)
    {
        printf("Memory Allocation Failed.");
        exit(1);
    }
    newNode->data = value;
    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}

int numberOfFreeBlocks(FreeBlock* freeBlockHead) {
    FreeBlock* temp = freeBlockHead;
    int size = 0;
    while(temp != NULL) {
        size++;
        temp = temp->next;
    }
    return size;
}

void insertionAtEnd(FreeBlock** freeBlockHead, FreeBlock** freeBlockTail, int value) {
    FreeBlock* newNode = createFreeBlockNode(value);

    if (*freeBlockHead == NULL)
    {
        *freeBlockHead = *freeBlockTail = newNode;
        return;
    }

    (*freeBlockTail)->next = newNode;
    newNode->prev = *freeBlockTail;
    *freeBlockTail = newNode;
}

void deletionAtfreeBlockHead(FreeBlock** freeBlockHead, FreeBlock** freeBlockTail) {
    if (*freeBlockHead == NULL)
    {
        return;
    }

    FreeBlock* toDelete = *freeBlockHead;
    *freeBlockHead = toDelete->next;

    if (*freeBlockHead != NULL)
    {
        (*freeBlockHead)->prev = NULL;
    }
    else
    {
        *freeBlockTail = NULL;
    }

    free(toDelete);
    toDelete = NULL;
}

void allocateBlocksToFile(FreeBlock** freeBlockHead, FreeBlock** freeBlockTail, int neededBlocks, int* blockPointers) {
    while (neededBlocks-- && *freeBlockHead != NULL) 
    {
        *blockPointers = (*freeBlockHead)->data;
        blockPointers++;
        deletionAtfreeBlockHead(freeBlockHead, freeBlockTail);
    }
}

char (*createVirtualDisk())[BLOCK_SIZE] {
    char (*virtualDisk)[BLOCK_SIZE] = malloc(MAX_NUMBER_OF_BLOCKS * sizeof(*virtualDisk));
    if (virtualDisk == NULL)
    {
        printf("Memory Allocation Failed.");
        exit(1);
    }
    return virtualDisk;
}

void createFreeBlocksStorage(FreeBlock **freeBlockHead, FreeBlock **freeBlockTail) {
    FreeBlock *prev = NULL;
    for (int blockNumber = 0; blockNumber < MAX_NUMBER_OF_BLOCKS; blockNumber++)
    {
        FreeBlock *node = createFreeBlockNode(blockNumber);
        if (blockNumber == 0) 
        {
            *freeBlockHead = node;
        }
        if (blockNumber == MAX_NUMBER_OF_BLOCKS - 1) 
        {
            *freeBlockTail = node;
        }
        node->prev = prev;
        if (prev)
        {
            prev->next = node;
        }
        prev = node;
    }
}

void freeFreeBlocks(FreeBlock** freeBlockHead, FreeBlock** freeBlockTail) {
    FreeBlock* current = *freeBlockHead;
    while (current != NULL)
    {
        FreeBlock* next = current->next;
        free(current);
        current = next;
    }
    *freeBlockTail = NULL;
    return;
}

// File/Directory node helpers
FileNode *createFileNode(char *name) {
    FileNode *newNode = malloc(sizeof(FileNode));
    if (newNode == NULL)
    {
        printf("Memory Allocation Failed.");
        exit(1);
    }
    strcpy(newNode->name, name);
    newNode->next = newNode; 
    newNode->child = NULL;
    newNode->parent = NULL;

    return newNode;
}

FileNode *createRootDirectory() {
    FileNode *root = createFileNode("/");
    root->isFile = false;
    
    return root;
}

FileNode* isFileExists(FileNode* node, char *name) {
    if (node == NULL)
    {
        return NULL;
    }

    FileNode* temp = node;
    do
    {
        if (strcmp(temp->name, name) == 0)
        {
            return temp;
        }
        temp = temp->next;
    } while (temp != node);
    
    return NULL;
}

void freeFileBlocks(FileNode* file, FreeBlock** freeBlockHead, FreeBlock** freeBlockTail) {
    if (file->blockPointers[0] != -1)
    {
        for (int index = 0; index < MAX_BLOCKS_PER_FILE; index++)
        {
            int blockIndex = file->blockPointers[index];
            if (blockIndex == -1)
            {
                break;
            }
            insertionAtEnd(freeBlockHead, freeBlockTail, blockIndex);
            file->blockPointers[index] = -1;
        }
    }
}

void freeFileNodes(FileNode* node) {
    if (node == NULL)
    {
        return;
    }

    FileNode* start = node;
    FileNode* current = node;

    do 
    {
        FileNode* nextNode = current->next;
        if (current->child)
        {
            freeFileNodes(current->child);
        }

        free(current);
        current = nextNode;
    } while(current != NULL && current != start);
}

// Utility parsers
bool getDataFromString(char* input, char* fileName, char* text) {
    if (strchr(input, '"') == NULL)
    {
        return false;
    }
    char *temp = input;
    int index = 0;
    while (*temp != '"')
    {
        fileName[index++] = *temp;
        temp++;
    }
    fileName[--index] = '\0';
    
    index = 0;

    if (strchr(++temp, '"') == NULL || strlen(temp) == 0)
    {
        return false;
    }
    while(*temp != '"')
    {
        text[index++] = *temp;
        temp++;
    }
    text[index] = '\0';

    if (strcmp(fileName, "") == 0 || strcmp(text, "") == 0)
    {
        return false;
    }

    return true;
}
