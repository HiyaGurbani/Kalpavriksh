#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define BLOCK_SIZE 512 
#define MAX_NUMBER_OF_BLOCKS 1024 

#define NAME_SIZE 50
#define MAX_BLOCKS_PER_FILE 1024

#define INPUT_SIZE 100

#define PATH_SIZE 100
#define TEXT_SIZE 100

#define CMD_MKDIR "mkdir "
#define CMD_RMDIR "rmdir "
#define CMD_CREATE "create "
#define CMD_DELETE "delete "
#define CMD_READ "read "
#define CMD_WRITE "write "
#define CMD_CD "cd "
#define CMD_CD_PARENT "cd.."
#define CMD_LS "ls"
#define CMD_DF "df"
#define CMD_PWD "pwd"
#define CMD_EXIT "exit"

typedef enum {
    RMDIR_SUCCESS,
    RMDIR_NOT_FOUND,
    RMDIR_NOT_EMPTY
} RmdirStatus;

typedef struct FreeBlock {
    int data;
    struct FreeBlock* prev;
    struct FreeBlock* next;
} FreeBlock;

typedef struct FileNode {
    char name[NAME_SIZE];
    bool isFile;
    int blockPointers[MAX_BLOCKS_PER_FILE];
    struct FileNode *next;
    struct FileNode *child;
    struct FileNode *parent;
} FileNode;

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

FileNode *createRootDirectory() {
    FileNode *root = createFileNode("/");
    root->isFile = false;
    
    return root;
}

void initialiseVirtualFileSystem(char (**virtualDisk)[BLOCK_SIZE], FreeBlock** freeBlockHead, FreeBlock** freeBlockTail, FileNode** root) {
    *virtualDisk = createVirtualDisk();
    createFreeBlocksStorage(freeBlockHead, freeBlockTail);
    *root = createRootDirectory();
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

RmdirStatus removeDirectory(FileNode* currentDirectory, char* directoryName) {
    if (currentDirectory->child == NULL)
    {
        return RMDIR_NOT_FOUND;
    }

    FileNode* previousNode = NULL, *currentNode = currentDirectory->child;

    do
    {
        if (strcmp(currentNode->name, directoryName) == 0 && !currentNode->isFile)
        {
            if (currentNode->child)
            {
                return RMDIR_NOT_EMPTY; 
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
            return RMDIR_SUCCESS;
        }
        previousNode = currentNode;
        currentNode = currentNode->next;
    } while (currentNode != currentDirectory->child);

    return RMDIR_NOT_FOUND;
}

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

bool deleteFile(FileNode* currentDirectory, char* fileName, FreeBlock** freeBlockHead, FreeBlock** freeBlockTail) {
    FileNode* file  = isFileExists(currentDirectory->child, fileName);
    if (file == NULL || file->isFile == false)
    {
        return false;
    }

    freeFileBlocks(file, freeBlockHead, freeBlockTail);

    FileNode* previousNode = NULL, *currentNode = currentDirectory->child;
    do
    {
        if (strcmp(currentNode->name, fileName) == 0 && currentNode->isFile)
        {
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
    
    return false;
}

void displayDiskInformation(FreeBlock* freeBlockHead) {
    int freeBlocks = numberOfFreeBlocks(freeBlockHead);
    int usedBlocks = MAX_NUMBER_OF_BLOCKS - freeBlocks;

    printf("Total Blocks: %d\n", MAX_NUMBER_OF_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", ((float)usedBlocks / MAX_NUMBER_OF_BLOCKS) * 100);
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

void handleCommand(char *input, FileNode **currentDirectory, FreeBlock **freeBlockHead, FreeBlock **freeBlockTail, 
                    FileNode *root, char (*virtualDisk)[BLOCK_SIZE] ) 
{
    char path[PATH_SIZE] = "";

    if (strncmp(input, CMD_MKDIR, strlen(CMD_MKDIR)) == 0) 
    {
        char* directoryName = input + strlen(CMD_MKDIR);
        if (createNodeInDirectory(*currentDirectory, directoryName, false))
        {
            printf("Directory '%s' created successfully.\n", directoryName);
        }
        else
        {
            printf("Directory '%s' already exists.\n", directoryName);
        }
    }

    else if (strncmp(input, CMD_CD, strlen(CMD_CD)) == 0) 
    {
        char* directoryName = input + strlen(CMD_CD);
        if (changeDirectory(currentDirectory, directoryName)) 
        {
            getWorkingDirectory(*currentDirectory, path);
            printf("Moved to %s\n", path);
        } else 
        {
            printf("Directory '%s' not found.\n", directoryName);
        }
    }

    else if (strcmp(input, CMD_CD_PARENT) == 0) 
    {
        if (cdParent(currentDirectory)) 
        {
            getWorkingDirectory(*currentDirectory, path);
            printf("Moved to %s\n", path);
        } else 
        {
            printf("Already in root.\n");
        }
    }

    else if (strcmp(input, CMD_LS) == 0) 
    {
        listChildren(*currentDirectory);
    }

    else if (strncmp(input, CMD_RMDIR, strlen(CMD_RMDIR)) == 0) 
    {
        char* directoryName = input + strlen(CMD_RMDIR);
        RmdirStatus status = removeDirectory(*currentDirectory, directoryName);
        if (status == RMDIR_SUCCESS) 
        {
            printf("Directory removed successfully.\n");
        }
        else if (status == RMDIR_NOT_FOUND)
        {
            printf("Directory '%s' not found.\n", directoryName);
        }
        else
        {
            printf("Directory not empty.\n");
        }
    }

    else if (strcmp(input, CMD_PWD) == 0) 
    {
        getWorkingDirectory(*currentDirectory, path);
        printf("%s\n", path);
    }

    else if (strncmp(input, CMD_CREATE, strlen(CMD_CREATE)) == 0) 
    {
        char* fileName = input + strlen(CMD_CREATE);
        if (createNodeInDirectory(*currentDirectory, fileName, true)) 
        {
            printf("File '%s' created successfully.\n", fileName);
        }
        else
        {
            printf("File '%s' already exists.\n", fileName);
        }
    }

    else if (strncmp(input, CMD_WRITE, strlen(CMD_WRITE)) == 0) 
    {
        char* remainingInput = input + strlen(CMD_WRITE);
        char fileName[NAME_SIZE] = "";
        char text[TEXT_SIZE] = "";

        if (getDataFromString(remainingInput, fileName, text)) 
        {
            if (writeDataIntoFile(*currentDirectory, fileName, text, freeBlockHead, freeBlockTail, virtualDisk))
            {
                printf("Data written successfully (%lu bytes)\n", strlen(text));
            }
        } else 
        {
            printf("Invalid write command.\n");
        }
    }

    else if (strncmp(input, CMD_READ, strlen(CMD_READ)) == 0) 
    {
        char* fileName = input + strlen(CMD_READ);
        if (!readFile(*currentDirectory, fileName, virtualDisk))
        {
            printf("File not found.\n");
        }
    }

    else if (strncmp(input, CMD_DELETE, strlen(CMD_DELETE)) == 0) 
    {
        char* fileName = input + strlen(CMD_DELETE);
        if (deleteFile(*currentDirectory, fileName, freeBlockHead, freeBlockTail))
        {
            printf("File deleted successfully.\n");
        }
        else
        {
            printf("File not found.\n");
        }
    }

    else if (strcmp(input, CMD_DF) == 0) 
    {
        displayDiskInformation(*freeBlockHead);
    }

    else if (strcmp(input, CMD_EXIT) == 0) 
    {
        free(virtualDisk);
        freeFreeBlocks(freeBlockHead, freeBlockTail);
        freeFileNodes(root);
        printf("Memory released. Exiting program...\n");
        exit(0);
    }

    else 
    {
        printf("Invalid command.\n");
    }
}

int main() {
    char (*virtualDisk)[BLOCK_SIZE] = NULL;
    FreeBlock *freeBlockHead = NULL, *freeBlockTail = NULL;
    FileNode *root = NULL;

    initialiseVirtualFileSystem(&virtualDisk, &freeBlockHead, &freeBlockTail, &root);
    FileNode *currentDirectory = root;
    
    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    char input[INPUT_SIZE];
    while (true)
    {
        printf("%s> ", currentDirectory->name);
        
        fgets(input, INPUT_SIZE, stdin);
        input[strcspn(input, "\n")] = '\0';

        handleCommand(input, &currentDirectory, &freeBlockHead, &freeBlockTail, root, virtualDisk);
        printf("\n");
    }
}