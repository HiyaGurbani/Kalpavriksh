#include<stdio.h>
#include<stdbool.h>

#include "vfs.h"

// Directory operations
bool createNodeInDirectory(FileNode* currentDirectory, char* nodeName, bool isFile) {
    FileNode *newNode = createFileNode(nodeName);
    if (isFile)
    {
        newNode->isFile = true;
    }
    else{
        newNode->isFile = false;
    }

    newNode->parent = currentDirectory;

    if (currentDirectory->child == NULL)
    {
        currentDirectory->child = newNode;
    }
    else 
    {
        if (isFileExists(currentDirectory->child, nodeName) != NULL)
        {
            free(newNode);
            newNode = NULL;
            return false;
        }

        FileNode *temp = currentDirectory->child;
        while(temp->next != currentDirectory->child)
        {
            temp = temp->next;
        }
        newNode->next = currentDirectory->child;
        temp->next = newNode;
    }

    if (newNode->isFile)
    {
        for (int index = 0; index < MAX_BLOCKS_PER_FILE; index++)
        {
            newNode->blockPointers[index] = -1;
        }
    }

    return true;
}

bool removeFileNode(FileNode* currentDirectory, char* nodeName, 
    FreeBlock** freeBlockHead, FreeBlock** freeBlockTail, bool isRmdirCall) {
    if (currentDirectory->child == NULL)
    {
        return false;
    }

    FileNode* previousNode = NULL, *currentNode = currentDirectory->child;

    do
    {
        if (strcmp(currentNode->name, nodeName) == 0)
        {
            if (!currentNode->isFile && currentNode->child)
            {
                printf("Directory not empty.\n");
                return false; 
            }

            if (currentNode->isFile)
            {
                freeFileBlocks(currentNode, freeBlockHead, freeBlockTail);
            }

            if (currentNode->next == currentNode)
            {
                currentDirectory->child = NULL;
            }
            else if (previousNode == NULL)
            {
                FileNode* last = currentDirectory->child;
                while (last->next != currentDirectory->child)
                {
                    last = last->next;
                }
                last->next = currentNode->next;
                currentDirectory->child = currentNode->next;
            }
            else
            {
                previousNode->next = currentNode->next;
            }

            free(currentNode);
            currentNode = NULL;
            return true;
        }
        previousNode = currentNode;
        currentNode = currentNode->next;
    } while (currentNode != currentDirectory->child);

    if (isRmdirCall)
    {
        printf("Directory '%s' not found.\n", nodeName);
    }
    return false;
}

bool removeDirectory(FileNode* currentDirectory, char* directoryName) {
    return removeFileNode(currentDirectory, directoryName, NULL, NULL, true);
}

bool deleteFile(FileNode* currentDirectory, char* fileName, FreeBlock** freeBlockHead, FreeBlock** freeBlockTail) {
    return removeFileNode(currentDirectory, fileName, freeBlockHead, freeBlockTail, false);
}


// Navigation Operations
bool changeDirectory(FileNode** currentDirectory, char *directoryName) {
    if ((*currentDirectory)->child == NULL)
    {
        return false;
    }

    FileNode *temp = (*currentDirectory)->child;
    do
    {
        if (strcmp(temp->name, directoryName) == 0 && !temp->isFile)
        {
            *currentDirectory = temp;
            return true;
        }
        temp = temp->next;
    } while (temp != (*currentDirectory)->child);
    
    return false;
}

bool cdParent(FileNode** currentDirectory) {
    if ((*currentDirectory)->parent == NULL)
    {
        return false;
    }

    *currentDirectory = (*currentDirectory)->parent;
    return true;
}

void listChildren(FileNode* currentDirectory) {
    if (currentDirectory->child == NULL)
    {
        printf("(empty)\n");
        return;
    }

    FileNode* temp = currentDirectory->child;

    do
    {
        if (temp->isFile)
        {
            printf("%s\n", temp->name);
        }
        else 
        {
            printf("%s/\n", temp->name);
        }
        temp = temp->next;
    } while (temp != currentDirectory->child);
}

void workingDirectory(FileNode* currentDirectory, char *path) {
    if (currentDirectory->parent == NULL) 
    {
        return;
    }
    
    workingDirectory(currentDirectory->parent, path);
    strcat(path, "/");
    strcat(path, currentDirectory->name);
}

void getWorkingDirectory(FileNode* currentDirectory, char *path) {
    workingDirectory(currentDirectory, path);
    if (strcmp(path, "") == 0)
    {
        strcpy(path, "/");
    }
}

// File read/write Operations
bool isFileWritable(FileNode* currentDirectory, FreeBlock** freeBlockHead, FreeBlock**freeBlockTail, char* fileName, char* text) {
    FileNode* file = isFileExists(currentDirectory->child, fileName);
    if (file == NULL )
    {
        printf("File '%s' does not exist.\n", fileName);
        return false; 
    }

    if (file->isFile == false)
    {
        printf("'%s' is a directory, not a file.\n", fileName);
        return false;
    }

    freeFileBlocks(file, freeBlockHead, freeBlockTail);

    int neededBlocks = (strlen(text) + BLOCK_SIZE - 1) / BLOCK_SIZE; 
    int freeBlockSize = numberOfFreeBlocks(*freeBlockHead);

    if (freeBlockSize < neededBlocks)
    {
        printf("Error: Insufficient memory. No free blocks available on the virtual disk.\n");
        return false; 
    }

    return true;
}

void writeTextToBlocks(FileNode* file, int neededBlocks, char* text, char (*virtualDisk)[BLOCK_SIZE]) {
    int textIndex = 0;
    for (int index = 0; index < neededBlocks; index++) 
    {
        int blockNumber = file->blockPointers[index];
        if (blockNumber == -1)
        {
            break;
        }

        int byteNumber = 0;
        while (byteNumber < BLOCK_SIZE - 1 && text[textIndex] != '\0') 
        {
            virtualDisk[blockNumber][byteNumber++] = text[textIndex++];
        }
        virtualDisk[blockNumber][byteNumber] = '\0';
    }
}

bool writeDataIntoFile(FileNode* currentDirectory, char* fileName, char* text, FreeBlock** freeBlockHead, FreeBlock** freeBlockTail, char (*virtualDisk)[BLOCK_SIZE]) {
    if (!isFileWritable(currentDirectory, freeBlockHead, freeBlockTail, fileName, text))
    {
        return false;
    }

    FileNode* file = isFileExists(currentDirectory->child, fileName);
    int neededBlocks = (strlen(text) + BLOCK_SIZE - 1) / BLOCK_SIZE;
    allocateBlocksToFile(freeBlockHead, freeBlockTail, neededBlocks, file->blockPointers);

    writeTextToBlocks(file, neededBlocks, text, virtualDisk);

    return true;
}

bool readFile(FileNode* currentDirectory, char* fileName, char (*virtualDisk)[BLOCK_SIZE]) {
    FileNode* file = isFileExists(currentDirectory->child, fileName);
    if (file == NULL || file->isFile == false)
    {
        return false;
    }

    for (int index = 0; index < MAX_BLOCKS_PER_FILE; index++) 
    {
        int blockIndex = file->blockPointers[index];
        if (blockIndex == -1)
        {
            break;
        }
        printf("%s", virtualDisk[blockIndex]);
    }

    printf("\n");
    return true;
}

void displayDiskInformation(FreeBlock* freeBlockHead) {
    int freeBlocks = numberOfFreeBlocks(freeBlockHead);
    int usedBlocks = MAX_NUMBER_OF_BLOCKS - freeBlocks;

    printf("Total Blocks: %d\n", MAX_NUMBER_OF_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", ((float)usedBlocks / MAX_NUMBER_OF_BLOCKS) * 100);
}

void initialiseVirtualFileSystem(char (**virtualDisk)[BLOCK_SIZE], FreeBlock** freeBlockHead, FreeBlock** freeBlockTail, FileNode** root) {
    *virtualDisk = createVirtualDisk();
    createFreeBlocksStorage(freeBlockHead, freeBlockTail);
    *root = createRootDirectory();
}


