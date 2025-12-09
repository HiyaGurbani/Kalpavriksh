#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 100
#define HASH_SIZE 11

typedef struct ProcessControlBlock {
    char name[SIZE];
    int burstTime;
    int remBurstTime;
    int ioStartTime;
    int ioDuration;
    int ioRemaining;
    int cpuExpected;
    char state[100]; //Ready, Running, Waiting, Terminated, Killed
    int turnAroundTime;
    int waitingTime;
    bool isKilled;
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
    newNode->next = NULL;

    return newNode;
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

ProcessControlBlock* getValueByKey(HashMap* hashMap, int key) {
    int index = hash(key);

    HashNode* temp = hashMap->buckets[index];

    while(temp) 
    {
        if (temp->key == key)
        {
            return temp->value;
        }
        temp = temp->next;
    }

    return NULL;
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
    queue->rear = newNode;
}

int dequeue(Queue* queue) {
    if (!queue || !queue->front)
    {
        printf("Empty Queue\n");
        return -1;
    }

    Node* toDelete  = queue->front;
    int processId = toDelete->processId;

    if (queue->front == queue->rear) 
    {
        queue->rear = NULL;
    }
    queue->front = queue->front->next;
    free(toDelete);

    return processId;
}

bool removeFromQueue(Queue* queue, int processId) {
    if (!queue || !queue->front)
    {
        return false;
    }

    Node* curr = queue->front, *prev = NULL;

    while (curr) {
        if (curr->processId == processId)
        {
            if (!prev)
            {
                queue->front = curr->next;
                if (queue->rear == curr) 
                {
                    queue->rear = NULL;
                }
            }
            else
            {
                prev->next = curr->next;
                if (queue->rear == curr)
                {
                    queue->rear = prev;
                }
            }

            free(curr);
            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    return false;
}

bool isInQueue(Queue* queue, int processId) {
    Node* curr = queue->front;
    while (curr)
    {
        if (curr->processId == processId)
        {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

void reduceIoDuration(Queue* waitingQueue, Queue* readyQueue, HashMap* hashMap) {
    Node* curr = waitingQueue->front;
    Node* prev = NULL;

    while (curr)
    {
        int processId = curr->processId;
        ProcessControlBlock* pcb = getValueByKey(hashMap, processId);
        if (!pcb)
        {
            prev = curr;
            curr = curr->next;
            continue;
        }

        if (pcb->ioStartedThisTick)
        {
            pcb->ioStartedThisTick = false;
            prev = curr;
            curr = curr->next;
            continue;
        }

        pcb->ioRemaining--;

        if (pcb->ioRemaining <= 0)
        {
            Node* toRemove = curr;

            if (!prev)
            {
                waitingQueue->front = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }

            if (waitingQueue->rear == curr)
            {
                waitingQueue->rear = prev;
            }

            curr = curr->next;
            toRemove->next = NULL;
            free(toRemove);

            strcpy(pcb->state, "Ready");
            enqueue(readyQueue, processId);
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
}


void fcfs(HashMap* hashMap, Queue* readyQueue, int numberOfProcess) {
    Queue* waitingQueue = initialiseQueue();
    Queue* terminatedQueue = initialiseQueue();

    int clock = 0;
    int completedProcess = 0;

    while(completedProcess < numberOfProcess) {
        int processId = dequeue(readyQueue);
        ProcessControlBlock* pcb = getValueByKey(hashMap, processId);

        pcb->remBurstTime--;
        
        if (pcb->ioStartTime == clock)
        {
            enqueue(waitingQueue, processId);
        }

        reduceIODuration(waitingQueue, readyQueue, hashMap);


        if (pcb->remBurstTime == 0)
        {
            enqueue(terminatedQueue, processId);
            pcb->turnAroundTime = clock + 1;
            pcb->waitingTime = pcb->turnAroundTime - pcb->burstTime;
            completedProcess++;
        }


        clock++;
    }
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
        pcb->waitingTime = 0;
        pcb->completionTime = 0;
        pcb->turnAroundTime = 0;

        printf("Enter Process ID: \n");
        scanf("%d", &processId);

        printf("Enter Process Burst Time: \n");
        scanf("%d", &pcb->burstTime);
        pcb->remBurstTime = pcb->burstTime;

        printf("Enter Process IO Start Time: \n");
        scanf("%d", &pcb->ioStartTime);

        printf("Enter Process IO Duration: \n");
        scanf("%d", &pcb->ioDuration);

        strcpy(pcb->state, "READY");

        insertIntoHashMap(hashMap, processId, pcb);
        enqueue(readyQueue, processId);
    }

    fcfs(hashMap, readyQueue, numberOfProcess);
}