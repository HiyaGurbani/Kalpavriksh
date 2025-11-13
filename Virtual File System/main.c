#include<stdio.h>

#include "vfs.h"

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