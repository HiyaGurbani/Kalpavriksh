#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

//Virtual Disk
#define BLOCK_SIZE 16 //Column Number
#define MAX_NUMBER_OF_BLOCKS 10 //Row Number 

#define NAME_SIZE 50
#define MAX_BLOCKS_PER_FILE 5

#define INPUT_SIZE 100

#define PATH_SIZE 100
#define TEXT_SIZE 100

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

//Files and Directory
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
    newNode->parent = NULL;
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

void createFreeBlocksStorage(FreeBlock **head, FreeBlock **tail) {
    FreeBlock *prev = NULL;
    for (int blockNumber = 0; blockNumber < MAX_NUMBER_OF_BLOCKS; blockNumber++)
    {
        FreeBlock *node = createFreeBlockNode(blockNumber);
        if (blockNumber == 0) {
            *head = node;
        }
        if (blockNumber == MAX_NUMBER_OF_BLOCKS - 1) {
            *tail = node;
        }
        node->prev = prev;
        if (prev) prev->next = node;
        prev = node;
    }
}

int numberOfFreeBlocks(FreeBlock* head) {
    FreeBlock* temp = head;
    int size = 0;
    while(temp!=NULL) {
        size++;
        temp = temp->next;
    }
    return size;
}

FreeBlock* insertionAtEnd(FreeBlock* head, int value) {
    FreeBlock* newNode = createFreeBlockNode(value);

    if (head == NULL)
    {
        head = newNode;
        return head;
    }

    FreeBlock* temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newNode;
    newNode->prev = temp;

    return head;
}

FreeBlock* deletionAtHead(FreeBlock* head) {
    FreeBlock* toDelete = head;
    head = toDelete->next;
    head->prev = NULL;
    free(toDelete);
    toDelete = NULL;

    return head;
}

FreeBlock* deleteFreeBlocks(FreeBlock* head, int blocksToDelete, int* blockPointers) {
    while (blocksToDelete--) 
    {
        *blockPointers = head->data;
        blockPointers++;
        head = deletionAtHead(head);
    }
    return head;
}

FileNode *createRootDirectory() {
    FileNode *root = createFileNode("/");
    root->isFile = false;
    
    return root;
}

// ---------------------------------Operations------------------------------------------------
FileNode* isUniqueName(FileNode* node, char *name) {
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

bool createNodeInDirectory(FileNode* cwd, char* nodeName, bool isFile) {
    FileNode *newNode = createFileNode(nodeName);
    if (isFile)
    {
        newNode->isFile = true;
    }
    else{
        newNode->isFile = false;
    }

    newNode->parent = cwd;

    if (cwd->child == NULL)
    {
        cwd->child = newNode;
    }
    else 
    {
        if (isUniqueName(cwd->child, nodeName) != NULL)
        {
            free(newNode);
            newNode = NULL;
            return false;
        }

        FileNode *temp = cwd->child;
        while(temp->next != cwd->child)
        {
            temp = temp->next;
        }
        newNode->next = cwd->child;
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

RmdirStatus removeDirectory(FileNode* cwd, char* directoryName) {
    if (cwd->child == NULL)
    {
        return RMDIR_NOT_FOUND;
    }

    FileNode* prev = NULL, *curr = cwd->child;

    do
    {
        if (strcmp(curr->name, directoryName) == 0 && !curr->isFile)
        {
            if (curr->child)
            {
                return RMDIR_NOT_EMPTY; 
            }
            if (curr->next == curr)
            {
                cwd->child = NULL;
            }
            else if (prev == NULL)
            {
                FileNode* last = cwd->child;
                while (last->next != cwd->child)
                {
                    last = last->next;
                }
                last->next = curr->next;
                cwd->child = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            free(curr);
            curr = NULL;
            return RMDIR_SUCCESS;
        }
        prev = curr;
        curr = curr->next;
    } while (curr != cwd->child);

    return RMDIR_NOT_FOUND;
}

bool changeDirectory(FileNode** cwd, char *directoryName) {
    if ((*cwd)->child == NULL)
    {
        return false;
    }

    FileNode *temp = (*cwd)->child;
    do
    {
        if (strcmp(temp->name, directoryName) == 0 && !temp->isFile)
        {
            *cwd = temp;
            return true;
        }
        temp = temp->next;
    } while (temp != (*cwd)->child);
    
    return false;
}

bool cdParent(FileNode** cwd) {
    if ((*cwd)->parent == NULL)
    {
        return false;
    }

    *cwd = (*cwd)->parent;
    return true;
}

void listChildren(FileNode* cwd) {
    if (cwd->child == NULL)
    {
        printf("(empty)\n");
        return;
    }

    FileNode* temp = cwd->child;

    do
    {
        printf("%s\n", temp->name);
        temp = temp->next;
    } while (temp != cwd->child);
}

void workingDirectory(FileNode* cwd, char *path) {
    if (cwd->parent == NULL) {
        return;
    }
    
    workingDirectory(cwd->parent, path);
    strcat(path, "/");
    strcat(path, cwd->name);
}

void getWorkingDirectory(FileNode* cwd, char *path) {
    workingDirectory(cwd, path);
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

FreeBlock* freeFileBlocks(FileNode* file, FreeBlock* head) {
    if (file->blockPointers[0] != -1)
    {
        for (int i = 0; i < MAX_BLOCKS_PER_FILE; i++)
        {
            int blockIndex = file->blockPointers[i];
            if (blockIndex == -1)
            {
                break;
            }
            head = insertionAtEnd(head, blockIndex);
            file->blockPointers[i] = -1;
        }
    }
    return head;
}

bool writeDataIntoFile(FileNode* cwd, char* fileName, char* text, FreeBlock** head, char (*virtualDisk)[BLOCK_SIZE]) {
    FileNode* file = isUniqueName(cwd->child, fileName);
    if (file == NULL )
    {
        printf("Error: File '%s' does not exist.\n", fileName);
        return false; 
    }

    if (file->isFile == false)
    {
        printf("Error: '%s' is a directory, not a file.\n", fileName);
        return false;
    }

    *head = freeFileBlocks(file, *head);

    int neededBlocks = (strlen(text) + BLOCK_SIZE - 1) / BLOCK_SIZE; //Ceil Value
    int freeBlockSize = numberOfFreeBlocks(*head);

    if (freeBlockSize < neededBlocks)
    {
        printf("Error: Insufficient memory. No free blocks available on the virtual disk.\n");
        return false; 
    }

    *head = deleteFreeBlocks(*head, neededBlocks, file->blockPointers);

    int textIndex = 0;
    for (int index = 0; index < neededBlocks; index++) {
        int blockNumber = file->blockPointers[index];

        int byteNumber = 0;
        while (byteNumber < BLOCK_SIZE && text[textIndex] != '\0') {
            virtualDisk[blockNumber][byteNumber++] = text[textIndex++];
        }
        virtualDisk[blockNumber][byteNumber] = '\0';
    }

    for (int i = 0; i < MAX_BLOCKS_PER_FILE; i++) {
        printf("%d ", file->blockPointers[i]);
    }
    
    return true;
}

bool readFile(FileNode* cwd, char* fileName, char (*virtualDisk)[BLOCK_SIZE]) {
    FileNode* file = isUniqueName(cwd->child, fileName);
    if (file == NULL || file->isFile == false)
    {
        return false;
    }

    for (int i = 0; i < MAX_BLOCKS_PER_FILE; i++) {
        int blockIndex = file->blockPointers[i];

        if (blockIndex == -1)
        {
            break;
        }

        printf("%s", virtualDisk[blockIndex]);
    }

    printf("\n");
    return true;
}

bool deleteFile(FileNode* cwd, char* fileName, FreeBlock** head) {
    FileNode* file  = isUniqueName(cwd->child, fileName);
    if (file == NULL || file->isFile == false)
    {
        return false;
    }

    *head = freeFileBlocks(file, *head);

    //Delete the file
    FileNode* prev = NULL, *curr = cwd->child;
    do
    {
        if (strcmp(curr->name, fileName) == 0 && curr->isFile)
        {
            if (curr->next == curr)
            {
                cwd->child = NULL;
            }
            else if (prev == NULL)
            {
                FileNode* last = cwd->child;
                while (last->next != cwd->child) 
                {
                    last = last->next;
                }
                last->next = curr->next;
                cwd->child = curr->next;
            }
            else 
            {
                prev->next = curr->next;
            }
            free(curr);
            curr = NULL;
            return true;
        }
        prev = curr;
        curr = curr->next;
    } while (curr != cwd->child);
    
    return false;
}

void displayDiskInformation(FreeBlock* head) {
    int freeBlocks = numberOfFreeBlocks(head);
    int usedBlocks = MAX_NUMBER_OF_BLOCKS - freeBlocks;

    printf("Total Blocks: %d\n", MAX_NUMBER_OF_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", ((float)usedBlocks / MAX_NUMBER_OF_BLOCKS) * 100);
}

int main() {
    //Initialisation
    char (*virtualDisk)[BLOCK_SIZE] = createVirtualDisk();

    FreeBlock *head = NULL, *tail = NULL;
    createFreeBlocksStorage(&head, &tail);

    FileNode *root = createRootDirectory();
    FileNode *cwd = root;
    
    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    char input[INPUT_SIZE];
    while (strcmp(input, "exit") != 0)
    {
        printf("%s> ", cwd->name);
        
        fgets(input, INPUT_SIZE, stdin);
        input[strcspn(input, "\n")] = '\0';

        char path[PATH_SIZE] = "";

        if (strncmp(input, "mkdir ", 6) == 0) 
        {
            if (createNodeInDirectory(cwd, input + 6, false))
            {
                printf("Directory '%s' created successfully.\n", input + 6);
            }
            else
            {
                printf("Directory Name '%s' already exists.\n", input + 6);
            }
        }

        else if (strncmp(input, "cd ", 3) == 0)
        {
            if (changeDirectory(&cwd, input + 3))
            {
                getWorkingDirectory(cwd, path);
                printf("Moved to %s\n", path);
            }
            else 
            {
                printf("Directory '%s' does not exist under '%s'.\n", input + 3, cwd->name);
            }
        }

        else if (strncmp(input, "cd..", 4) == 0)
        {
            if (cdParent(&cwd))
            {
                getWorkingDirectory(cwd, path);
                printf("Moved to %s\n", path);
            }
            else 
            {
                printf("Already in the root directory.\n");
            }
        }

        else if (strncmp(input, "ls", 2) == 0)
        {
            listChildren(cwd);
        }

        else if (strncmp(input, "rmdir ", 6) == 0)
        {
            RmdirStatus status = removeDirectory(cwd, input + 6);
            if (status == RMDIR_SUCCESS)
            {
                printf("Directory removed successfuly.\n");
            }
            else if (status == RMDIR_NOT_FOUND)
            {
                printf("Directory '%s' does not exist under '%s'.\n", input + 6, cwd->name);
            }
            else 
            {
                printf("Failed to remove '%s': directory is not empty.\n", input + 6);
            }
        }

        else if (strncmp(input, "pwd", 3) == 0)
        {
            getWorkingDirectory(cwd, path);
            printf("%s\n", path);
        }

        else if (strncmp(input, "create ", 7) == 0)
        {
            if (createNodeInDirectory(cwd, input + 7, true))
            {
                printf("File '%s' created successfully.\n", input + 7);
            }
            else
            {
                printf("File Name '%s' already exists.\n", input + 7);
            }
        }

        else if (strncmp(input, "write ", 6) == 0)
        {
            char fileName[NAME_SIZE] = "";
            char text[TEXT_SIZE] = "";
            if (getDataFromString(input + 6, fileName, text)) {
                if (writeDataIntoFile(cwd, fileName, text, &head, virtualDisk))
                {
                    printf("Data written successfully (size=%d bytes)\n", strlen(text));
                }
            } 
            else
            {
                printf("Error: Invalid Command Format.\n");
            }
        }

        else if (strncmp(input, "read ", 5) == 0)
        {
            if (!readFile(cwd, input + 5, virtualDisk))
            {
                printf("File not found.\n");
            }
        }

        else if (strncmp(input, "delete ", 7) == 0)
        {
            if (!deleteFile(cwd, input + 7, &head))
            {
                printf("File Not Found.\n");
            }
            else 
            {
                printf("File deleted successfully.\n");
            }
        }

        else if (strncmp(input, "df", 2) == 0)
        {
            displayDiskInformation(head);
        }

        else {
            printf("Invalid Command. Enter Again!\n");
        }

        printf("\n");
    }
}