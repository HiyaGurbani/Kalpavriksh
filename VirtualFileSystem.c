#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

//Virtual Disk
#define BLOCK_SIZE 16 //Column Number
#define NUMBER_OF_BLOCKS 5 //Row Number 

#define NAME_SIZE 50
#define MAX_BLOCKS_PER_FILE 5

#define INPUT_SIZE 100

#define PATH_SIZE 100

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
    newNode->parent = NULL;
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

// ---------------------------------Operations------------------------------------------------
bool isUniqueName(FileNode* node, char *name) {
    FileNode* temp = node;
    do
    {
        if (strcmp(temp->name, name) == 0)
        {
            return false;
        }
        temp = temp->next;
    } while (temp != node);
    
    return true;
}

bool makeDirectory(FileNode *cwd, char *directoryName) {
    FileNode *newDirectory = createFileNode(directoryName);
    newDirectory->isFile = false;
    newDirectory->parent = cwd;
    if (cwd->child == NULL)
    {
        cwd->child = newDirectory;
    }
    else 
    {
        if (!isUniqueName(cwd->child, directoryName))
        {
            free(newDirectory);
            newDirectory = NULL;
            printf("Directory Name %s already exists.\n", directoryName);
            return false;
        }

        FileNode *temp = cwd->child;
        while (temp->next != cwd->child)
        {
            temp = temp->next;
        }
        newDirectory->next = cwd->child;
        temp->next = newDirectory;
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
        if (strcmp(curr->name, directoryName) == 0)
        {
            if (curr->child)
            {
                return RMDIR_NOT_EMPTY; //Child Exists
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
        if (strcmp(temp->name, directoryName) == 0 )
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

int main() {
    //Initialisation
    int (*virtualDisk)[BLOCK_SIZE] = createVirtualDisk();

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
            if (makeDirectory(cwd, input + 6))
            {
                printf("Directory \"%s\" created successfully.\n", input + 6);
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

        else {
            printf("Invalid Command. Enter Again!\n");
        }

        printf("\n");
    }
}