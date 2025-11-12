#include<stdio.h>
#include<stdbool.h>

#include "vfs.h"

void handleMkdir(char* input, FileNode** currentDirectory) {
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

void handleCd(char* input, FileNode** currentDirectory, char* path) {
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

void handleCdParent(FileNode** currentDirectory, char* path) {
    if (cdParent(currentDirectory)) 
    {
        getWorkingDirectory(*currentDirectory, path);
        printf("Moved to %s\n", path);
    } else 
    {
        printf("Already in root.\n");
    }
}

void handleRmdir(char* input, FileNode** currentDirectory) {
    char* directoryName = input + strlen(CMD_RMDIR);

    if (removeDirectory(*currentDirectory, directoryName)) 
    {
        printf("Directory removed successfully.\n");
    }
}

void handleCreate(char* input, FileNode** currentDirectory) {
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

void handleWrite(char* input, FileNode** currentDirectory, FreeBlock **freeBlockHead, 
    FreeBlock **freeBlockTail, char (*virtualDisk)[BLOCK_SIZE]) {
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

void handleRead(char* input, FileNode** currentDirectory, char (*virtualDisk)[BLOCK_SIZE]) {
    char* fileName = input + strlen(CMD_READ);
    if (!readFile(*currentDirectory, fileName, virtualDisk))
    {
        printf("File not found.\n");
    }    
}

void handleDelete(char* input, FileNode** currentDirectory, 
    FreeBlock **freeBlockHead, FreeBlock **freeBlockTail) {
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

void handleExit(FreeBlock **freeBlockHead, FreeBlock **freeBlockTail, 
    FileNode *root, char (*virtualDisk)[BLOCK_SIZE]) {
    free(virtualDisk);
    freeFreeBlocks(freeBlockHead, freeBlockTail);
    freeFileNodes(root);
    printf("Memory released. Exiting program...\n");
    exit(0);
}

void handleCommand(char *input, FileNode **currentDirectory, FreeBlock **freeBlockHead, 
    FreeBlock **freeBlockTail, FileNode *root, char (*virtualDisk)[BLOCK_SIZE]) {
    char path[PATH_SIZE] = "";

    if (strncmp(input, CMD_MKDIR, strlen(CMD_MKDIR)) == 0) 
    {
        handleMkdir(input, currentDirectory);
    }

    else if (strncmp(input, CMD_CD, strlen(CMD_CD)) == 0) 
    {
        handleCd(input, currentDirectory, path);
    }

    else if (strcmp(input, CMD_CD_PARENT) == 0) 
    {
        handleCdParent(currentDirectory, path);
    }

    else if (strcmp(input, CMD_LS) == 0) 
    {
        listChildren(*currentDirectory);
    }

    else if (strncmp(input, CMD_RMDIR, strlen(CMD_RMDIR)) == 0) 
    {
        handleRmdir(input, currentDirectory);
    }

    else if (strcmp(input, CMD_PWD) == 0) 
    {
        getWorkingDirectory(*currentDirectory, path);
        printf("%s\n", path);
    }

    else if (strncmp(input, CMD_CREATE, strlen(CMD_CREATE)) == 0) 
    {
        handleCreate(input, currentDirectory);
    }

    else if (strncmp(input, CMD_WRITE, strlen(CMD_WRITE)) == 0) 
    {
        handleWrite(input, currentDirectory, freeBlockHead, freeBlockTail, virtualDisk);
    }

    else if (strncmp(input, CMD_READ, strlen(CMD_READ)) == 0) 
    {
        handleRead(input, currentDirectory, virtualDisk);
    }

    else if (strncmp(input, CMD_DELETE, strlen(CMD_DELETE)) == 0) 
    {
        handleDelete(input, currentDirectory, freeBlockHead, freeBlockTail);
    }

    else if (strcmp(input, CMD_DF) == 0) 
    {
        displayDiskInformation(*freeBlockHead);
    }

    else if (strcmp(input, CMD_EXIT) == 0) 
    {
        handleExit(freeBlockHead, freeBlockTail, root, virtualDisk);
    }

    else 
    {
        printf("Invalid command.\n");
    }
}
