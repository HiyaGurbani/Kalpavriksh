#include "fcfs.h"

void getValidProcessName(char* name) {
    while (true) {
        if (!fgets(name, SIZE, stdin)) {
            printf("Invalid Input Error\n");
            continue;
        }

        name[strcspn(name, "\n")] = '\0';

        if (strlen(name) == 0) {
            printf("Name cannot be empty. Enter again: ");
        } else {
            break;
        }
    }
}

void getValidInteger(int *value) {
    while (scanf("%d", value) != 1)
    {
        printf("Error: Invalid Character! Enter Valid Integer: ");
        while(getchar() != '\n');
    }
}

void getValidPositiveInteger(int *value) {
    getValidInteger(value);
    while (*value <= 0) 
    {
        printf("Value must be > 0. Enter again: ");
        getValidInteger(value);
    }
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

ProcessControlBlock* getFromHashMap(HashMap* hashMap, int key) {
    if (!hashMap)
    {
        return NULL;
    }

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

void freeHashMap(HashMap* hashMap) {
    if (!hashMap)
    {
        return;
    }

    for (int index = 0; index < HASH_SIZE; index++)
    {
        HashNode* temp = hashMap->buckets[index];
        while (temp)
        {
            HashNode* toDelete = temp;
            temp = temp->next;
            free(toDelete->value);
            free(toDelete);
        }
    }
    
    free(hashMap);
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
            }
            else
            {
                prev->next = curr->next;
            }

            if (queue->rear == curr)
            {
                queue->rear = prev;
            }

            free(curr);
            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    return false;
}

bool isQueueEmpty(Queue* queue) {
    return !queue || !queue->front;
}

void freeQueue(Queue* queue) {
    if (!queue)
    {
        return;
    }

    while (queue->front)
    {
        dequeue(queue);
    }

    free(queue);
}

void moveToWaiting(ProcessControlBlock* pcb, int processId, Queue* waitingQueue) {
    pcb->state = WAITING;
    pcb->ioRemaining = pcb->ioDuration;
    pcb->ioStartedThisTick = true;
    enqueue(waitingQueue, processId);
}

void moveToTerminated(ProcessControlBlock* pcb, int processId, int clock, Queue* terminatedQueue) {
    pcb->turnAroundTime = clock;
    pcb->waitingTime = pcb->turnAroundTime - pcb->burstTime;
    pcb->state = TERMINATED;
    enqueue(terminatedQueue, processId);
}

void killProcess(ProcessControlBlock* pcb, int processId, int clock, Queue* terminatedQueue) {
    pcb->turnAroundTime = -1;
    pcb->waitingTime = -1;
    pcb->state = KILLED;
    enqueue(terminatedQueue, processId);
}

ExecuteResult executeProcessTick(ProcessControlBlock* pcb) {
    if (!pcb) 
    {
        return PROCESS_CONTINUE;
    }

    pcb->cpuExecuted++;
    pcb->remainingBurstTime--;

    if (pcb->ioStartTime >= 0 && pcb->cpuExecuted == pcb->ioStartTime && pcb->remainingBurstTime > 0)
    {
        return PROCESS_WAITING; 
    }    

    if (pcb->remainingBurstTime <= 0)
    {
        return PROCESS_TERMINATED; 
    }
        
    return PROCESS_CONTINUE; 
}

void handleExecutionResult(ExecuteResult result, ProcessControlBlock* pcb, int processId, int clock, Queue* waitingQueue, 
    Queue* terminatedQueue, int* runningProcessId, int* terminatedProcesses) {

    if (result == PROCESS_WAITING) 
    {
        moveToWaiting(pcb, processId, waitingQueue);
        *runningProcessId = -1;
    }

    else if (result == PROCESS_TERMINATED) 
    {
        moveToTerminated(pcb, processId, clock + 1, terminatedQueue);
        (*terminatedProcesses)++;
        *runningProcessId = -1;
    }
}

void handleIoCompletion(Queue* waitingQueue, Queue* readyQueue, Node* curr, Node* prev, ProcessControlBlock* pcb, int processId) {
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
    free(toRemove);
    pcb->state = READY;
    enqueue(readyQueue, processId);
}

void printResults(HashMap* hashMap) {
    printf("\n%-8s%-15s%-10s%-10s%-15s%-12s%-10s\n",
           "PID", "Name", "CPU", "IO", "Turnaround", "Waiting", "Status");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int index = 0; index < HASH_SIZE; index++) {
        HashNode* node = hashMap->buckets[index];
        while (node) 
        {
            ProcessControlBlock* pcb = node->value;
            const char* status = (pcb->state == KILLED) ? "KILLED" : "OK";
            
            printf("%-8d%-15s%-10d%-10d%-15d%-12d%-10s\n",
                   node->key, pcb->name, pcb->burstTime, pcb->ioDuration, pcb->turnAroundTime, pcb->waitingTime, status);
            
            node = node->next;
        }
    }
}

ProcessControlBlock* createProcess() {
    ProcessControlBlock* pcb = malloc(sizeof(ProcessControlBlock));
    if (!pcb)
    {
        printf("Memory Allocation Failed!\n");
        exit(1);
    }

    pcb->name[0] = '\0';
    pcb->burstTime = 0;
    pcb->remainingBurstTime = 0;
    pcb->ioStartTime = -1;
    pcb->ioDuration = 0;
    pcb->ioRemaining = 0;
    pcb->cpuExecuted = 0;
    pcb->state = READY;
    pcb->turnAroundTime = 0;
    pcb->waitingTime = 0;
    pcb->ioStartedThisTick = false;
    
    return pcb;
}