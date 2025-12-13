#ifndef FCFS_H
#define FCFS_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define SIZE 100
#define HASH_SIZE 11

typedef enum ProcessState {
    READY,
    RUNNING,
    WAITING,
    TERMINATED,
    KILLED
} ProcessState;

typedef enum ExecuteResult { 
    PROCESS_CONTINUE=0, 
    PROCESS_WAITING=1, 
    PROCESS_TERMINATED=2 
} ExecuteResult;

typedef struct ProcessControlBlock {
    char name[SIZE];
    int burstTime;
    int remainingBurstTime;
    int ioStartTime;
    int ioDuration;
    int ioRemaining;
    int cpuExecuted;
    ProcessState state; 
    int turnAroundTime;
    int waitingTime;
    bool ioStartedThisTick;
} ProcessControlBlock;

typedef struct HashNode {
    int key;
    ProcessControlBlock* value;
    struct HashNode* next;
} HashNode;

typedef struct HashMap {
    HashNode* buckets[HASH_SIZE];
} HashMap;

typedef struct Node {
    int processId;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

typedef struct KillEvent {
    int processId;
    int time;
} KillEvent;

//Core Functions
HashMap* createHashMap();
Queue* createQueue();
KillEvent* createKillEvents(int killCount);
void readProcessInput(HashMap* hashMap, Queue* readyQueue, int* totalProcesses);
void readKillEvents(KillEvent** kills, int* killCount);
void applyKillEvents(KillEvent* kills, int killCount, int clock, HashMap* hashMap, Queue* readyQueue, Queue* waitingQueue, Queue* terminatedQueue,
int* runningProcessId,  int* terminatedCount);
void fetchNextProcess(HashMap* hashMap, Queue* readyQueue, int* runningProcessId);
void executingRunningProcess(HashMap* hashMap, int* runningProcessId, int clock, Queue* waitingQueue, Queue* terminatedQueue, int* terminatedProcesses);
void updateWaitingProcesses(Queue* waitingQueue, Queue* readyQueue, HashMap* hashMap);
void fcfsScheduler(HashMap* hashMap, Queue* readyQueue, int totalProcesses, KillEvent* kills, int killCount);


//Helper Functions
void getValidProcessName(char* name);
void getValidInteger(int *value);
void getValidPositiveInteger(int *value);
HashNode* createHashNode(int key, ProcessControlBlock* pcb);
void insertIntoHashMap(HashMap* hashMap, int key, ProcessControlBlock* pcb);
ProcessControlBlock* getFromHashMap(HashMap* hashMap, int key);
void freeHashMap(HashMap* hashMap);
Node* createQueueNode(int processId);
void enqueue(Queue* queue, int processId);
int dequeue(Queue* queue);
bool removeFromQueue(Queue* queue, int processId);
bool isQueueEmpty(Queue* queue);
void freeQueue(Queue* queue) ;
void moveToWaiting(ProcessControlBlock* pcb, int processId, Queue* waitingQueue);
void moveToTerminated(ProcessControlBlock* pcb, int processId, int clock, Queue* terminatedQueue);
void killProcess(ProcessControlBlock* pcb, int processId, int clock, Queue* terminatedQueue);
ExecuteResult executeProcessTick(ProcessControlBlock* pcb);
void handleExecutionResult(ExecuteResult result, ProcessControlBlock* pcb, int processId, int clock, Queue* waitingQueue, 
    Queue* terminatedQueue, int* runningProcessId, int* terminatedProcesses);
void handleIoCompletion(Queue* waitingQueue, Queue* readyQueue, Node* curr, Node* prev, ProcessControlBlock* pcb, int processId);
void printResults(HashMap* hashMap);
ProcessControlBlock* createProcess();

#endif