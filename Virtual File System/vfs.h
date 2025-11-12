#ifndef VFS_H
#define VFS_H

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

//Disk Management Functions
FreeBlock* createFreeBlockNode(int value);
void createFreeBlocksStorage(FreeBlock **freeBlockHead, FreeBlock **freeBlockTail);
int numberOfFreeBlocks(FreeBlock* freeBlockHead);
void insertionAtEnd(FreeBlock** freeBlockHead, FreeBlock** freeBlockTail, int value);
void deletionAtfreeBlockHead(FreeBlock** freeBlockHead, FreeBlock** freeBlockTail);
void allocateBlocksToFile(FreeBlock** freeBlockHead, FreeBlock** freeBlockTail, int neededBlocks, int* blockPointers);
char (*createVirtualDisk())[BLOCK_SIZE];
void freeFreeBlocks(FreeBlock** freeBlockHead, FreeBlock** freeBlockTail);

// File & Directory Nodes
FileNode* createFileNode(char *name);
FileNode* createRootDirectory();
FileNode* isFileExists(FileNode* node, char *name);
bool createNodeInDirectory(FileNode* currentDirectory, char* nodeName, bool isFile);
bool removeFileNode(FileNode* currentDirectory, char* nodeName, FreeBlock** freeBlockHead, FreeBlock** freeBlockTail, bool isRmdirCall);
bool removeDirectory(FileNode* currentDirectory, char* directoryName);
bool deleteFile(FileNode* currentDirectory, char* fileName, FreeBlock** freeBlockHead, FreeBlock** freeBlockTail);
void freeFileBlocks(FileNode* file, FreeBlock** freeBlockHead, FreeBlock** freeBlockTail);
void freeFileNodes(FileNode* node);

// Operations
bool changeDirectory(FileNode** currentDirectory, char *directoryName);
bool cdParent(FileNode** currentDirectory);
void listChildren(FileNode* currentDirectory);
void workingDirectory(FileNode* currentDirectory, char *path);
void getWorkingDirectory(FileNode* currentDirectory, char *path);
bool getDataFromString(char* input, char* fileName, char* text);
bool isFileWritable(FileNode* currentDirectory, FreeBlock** freeBlockHead, FreeBlock**freeBlockTail, char* fileName, char* text);
void writeTextToBlocks(FileNode* file, int neededBlocks, char* text, char (*virtualDisk)[BLOCK_SIZE]);
bool writeDataIntoFile(FileNode* currentDirectory, char* fileName, char* text, FreeBlock** freeBlockHead, FreeBlock** freeBlockTail, char (*virtualDisk)[BLOCK_SIZE]);
bool readFile(FileNode* currentDirectory, char* fileName, char (*virtualDisk)[BLOCK_SIZE]);
void displayDiskInformation(FreeBlock* freeBlockHead);

// Initialization
void initialiseVirtualFileSystem(char (**virtualDisk)[BLOCK_SIZE], FreeBlock** freeBlockHead, FreeBlock** freeBlockTail, FileNode** root);

// Command Handlers
void handleCommand(char *input, FileNode **currentDirectory, FreeBlock **freeBlockHead, 
    FreeBlock **freeBlockTail, FileNode *root, char (*virtualDisk)[BLOCK_SIZE]);
void handleMkdir(char* input, FileNode** currentDirectory);
void handleCd(char* input, FileNode** currentDirectory, char* path);
void handleCdParent(FileNode** currentDirectory, char* path);
void handleRmdir(char* input, FileNode** currentDirectory);
void handleCreate(char* input, FileNode** currentDirectory);
void handleWrite(char* input, FileNode** currentDirectory, FreeBlock **freeBlockHead, 
    FreeBlock **freeBlockTail, char (*virtualDisk)[BLOCK_SIZE]);
void handleRead(char* input, FileNode** currentDirectory, char (*virtualDisk)[BLOCK_SIZE]);
void handleDelete(char* input, FileNode** currentDirectory, 
    FreeBlock **freeBlockHead, FreeBlock **freeBlockTail);
void handleExit(FreeBlock **freeBlockHead, FreeBlock **freeBlockTail, 
    FileNode *root, char (*virtualDisk)[BLOCK_SIZE]);

#endif