#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 100
#define HASH_SIZE 11

typedef struct ProcessControlBlock {
    char name[SIZE];
    int burstTime;
    int ioStartTime;
    int ioDuration;
    char state[100];
} ProcessControlBlock;

typedef struct HashNode {
    int key;
    ProcessControlBlock* value;
    struct HashNode* next;
} HashNode;

typedef struct HashMap {
    HashNode* buckets[HASH_SIZE];
} HashMap;

HashMap* initialiseHashMap() {
    HashMap* map = malloc(sizeof(HashMap));
    if (!map)
    {
        printf("Memory Allocation Failed!\n");
        exit(1);
    }

    for (int index = 0; index < HASH_SIZE; index++)
    {
        map->buckets[index] = NULL;
    }

    return map;
}

HashNode* createHashNode(int key, ProcessControlBlock* pcb) {
    HashNode* newNode = malloc(sizeof(HashNode));
    if (!newNode)
    {
        printf("Memory Allocation Failed!\n");
        exit(1);
    }

    newNode->key = key;
    newNode->value = pcb;

    return newNode;
}

typedef struct Node {
    int processId;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

Queue* initialiseQueue() {
    Queue* queue = malloc(sizeof(Queue));
    if (!queue) 
    {
        printf("Memory Allocation Failed!\n");
        exit(1);
    }

    queue->front = queue->rear = NULL;

    return queue;
}

int hash(int key) {
    return (key % HASH_SIZE + HASH_SIZE) % HASH_SIZE;
}

void insertIntoHashMap(HashMap* hashMap, int key, ProcessControlBlock* pcb) {
    if (!hashMap || !pcb)
    {
        return;
    }

    int index = hash(key);

    HashNode* newNode = createHashNode(key, pcb);
    newNode->next = hashMap->buckets[index];
    hashMap->buckets[index] = newNode;
}

Node* createQueueNode(int processId) {
    Node* newNode = malloc(sizeof(Node));
    if (!newNode)
    {
        printf("Memory Allocation Failed!\n");
        exit(1);
    }

    newNode->processId = processId;
    newNode->next = NULL;

    return newNode;
}

void enqueue(Queue* queue, int processId) {
    if (!queue)
    {
        return;
    }

    Node* newNode = createQueueNode(processId);
    if (!queue->front)
    {
        queue->front = queue->rear = newNode;
        return;
    }

    queue->rear->next = newNode;
    newNode = queue->rear;
}

int main() {
    int numberOfProcess = 0;
    printf("\nEnter number of processes: ");
    scanf("%d", &numberOfProcess);

    HashMap* hashMap = initialiseHashMap();

    Queue* readyQueue = initialiseQueue();

    for (int index = 0; index < numberOfProcess; index++)
    {
        printf("\nEnter details of Process %d: \n", index + 1);

        int processId;
        ProcessControlBlock* pcb = malloc(sizeof(ProcessControlBlock));

        printf("Enter Process Name: \n");
        while (getchar() != '\n');
        fgets(pcb->name, sizeof(pcb->name), stdin);
        pcb->name[strcspn(pcb->name, "\n")] = '\0';

        printf("Enter Process ID: \n");
        scanf("%d", &processId);

        printf("Enter Process Burst Time: \n");
        scanf("%d", pcb->burstTime);

        printf("Enter Process IO Start Time: \n");
        scanf("%d", pcb->ioStartTime);

        printf("Enter Process IO Duration: \n");
        scanf("%d", pcb->ioDuration);

        strcpy(pcb->state, "READY");

        insertIntoHashMap(hashMap, processId, pcb);
        enqueue(readyQueue, processId);
    }
}