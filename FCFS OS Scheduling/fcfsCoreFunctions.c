#include "fcfs.h"

HashMap* createHashMap() {
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

Queue* createQueue() {
    Queue* queue = malloc(sizeof(Queue));
    if (!queue) 
    {
        printf("Memory Allocation Failed!\n");
        exit(1);
    }

    queue->front = queue->rear = NULL;

    return queue;
}

KillEvent* createKillEvents(int killCount) {
    KillEvent* kills = malloc(killCount * sizeof(KillEvent));
    if (!kills)
    {
        printf("Memory Allocation Failed\n");
        exit(1);
    }

    for (int index = 0; index < killCount; index++)
    {
        kills[index].processId = -1;
        kills[index].time = -1;
    }

    return kills;
}

void readProcessInput(HashMap* hashMap, Queue* readyQueue, int* totalProcesses) {
    printf("Enter number of processes: "); 
    getValidInteger(totalProcesses);

    for (int index = 0; index < *totalProcesses; index++) 
    {
        printf("\nEnter details of Process %d:\n", index + 1);

        int processId;
        ProcessControlBlock* pcb = createProcess();

        printf("Process Name: ");
        while (getchar() != '\n');
        fgets(pcb->name, sizeof(pcb->name), stdin);
        pcb->name[strcspn(pcb->name, "\n")] = '\0';

        printf("Process ID: "); 
        getValidPositiveInteger(&processId);

        printf("Burst Time: "); 
        getValidPositiveInteger(&pcb->burstTime);
        pcb->remBurstTime = pcb->burstTime;
        
        printf("IO Start Time (-1 for none): "); 
        getValidInteger(&pcb->ioStartTime);

        printf("IO Duration (0 for none): "); 
        getValidInteger(&pcb->ioDuration);


        if (pcb->ioStartTime < 0 || pcb->ioDuration <= 0) 
        {
            pcb->ioStartTime = -1; 
            pcb->ioDuration = 0;
        }

        insertIntoHashMap(hashMap, processId, pcb);
        enqueue(readyQueue, processId);
    }
}

void readKillEvents(KillEvent** kills, int* killCount) {
    printf("\nEnter number of KILL events: ");
    getValidInteger(killCount);

    if (*killCount > 0)
    {
        *kills = createKillEvents(*killCount);

        for (int index = 0; index < *killCount; index++)
        {
            printf("KILL event %d: \n", index + 1);
            
            printf("Process ID: ");
            getValidPositiveInteger(&(*kills)[index].processId);

            printf("Time: ");
            getValidInteger(&(*kills)[index].time);
        }
    }
}

void applyKillEvents(KillEvent* kills, int killCount, int clock, HashMap* hashMap, Queue* readyQueue, Queue* waitingQueue, Queue* terminatedQueue,
int* runningProcessId,  int* terminatedCount) {
    for (int index = 0; index < killCount; index++)
    {
        if (kills[index].time != clock)
        {
            continue;
        }

        int processId = kills[index].processId;
        ProcessControlBlock* pcb = getFromHashMap(hashMap, processId);
        if (!pcb || pcb->state == TERMINATED || pcb->state == KILLED) {
            continue;
        }

        if (*runningProcessId == processId)
        {
            killProcess(pcb, processId, clock, terminatedQueue);
            (*terminatedCount)++;
            *runningProcessId = -1;
            continue;
        }

        if (removeFromQueue(readyQueue, processId) || removeFromQueue(waitingQueue, processId))
        {
            killProcess(pcb, processId, clock, terminatedQueue);
            (*terminatedCount)++;
        }

    }
}

void fetchNextProcess(HashMap* hashMap, Queue* readyQueue, int* runningProcessId) {
    if (*runningProcessId == -1 && !isQueueEmpty(readyQueue)) 
    {
        *runningProcessId = dequeue(readyQueue);
        ProcessControlBlock* pcb = getFromHashMap(hashMap, *runningProcessId);
        if (pcb) 
        {
            pcb->state = RUNNING;
        }
    }
}

void executingRunningProcess(HashMap* hashMap, int* runningProcessId, int clock, Queue* waitingQueue, Queue* terminatedQueue, int* terminatedProcesses) {
    if (*runningProcessId == -1)
    {
        return;
    }

    ProcessControlBlock* pcb = getFromHashMap(hashMap, *runningProcessId);
    if (!pcb) 
    {
        return;
    }

    ExecuteResult result = executeProcessTick(pcb);

    handleExecutionResult(result, pcb, *runningProcessId, clock, waitingQueue, terminatedQueue, runningProcessId, terminatedProcesses);
}

void updateWaitingProcesses(Queue* waitingQueue, Queue* readyQueue, HashMap* hashMap) {
    Node* curr = waitingQueue->front;
    Node* prev = NULL;

    while (curr)
    {
        int processId = curr->processId;
        ProcessControlBlock* pcb = getFromHashMap(hashMap, processId);

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
            handleIoCompletion(waitingQueue, readyQueue, curr, prev, pcb, processId);

            if (!prev)
            {
                curr = waitingQueue->front;
            }
            else
            {
                curr = prev->next;
            }
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
}

void fcfsScheduler(HashMap* hashMap, Queue* readyQueue, int totalProcesses, KillEvent* kills, int killCount) {
    Queue* waitingQueue = createQueue();
    Queue* terminatedQueue = createQueue();

    int clock = 0;
    int runningProcessId = -1;
    int terminatedProcesses = 0;

    while (terminatedProcesses < totalProcesses)
    {
        applyKillEvents(kills, killCount, clock, hashMap, readyQueue, waitingQueue, terminatedQueue, &runningProcessId, &terminatedProcesses);

        fetchNextProcess(hashMap, readyQueue, &runningProcessId);

        executingRunningProcess(hashMap, &runningProcessId, clock, waitingQueue, terminatedQueue, &terminatedProcesses);

        updateWaitingProcesses(waitingQueue, readyQueue, hashMap);

        clock++;
    }

    printResults(hashMap);

    freeQueue(waitingQueue);
    freeQueue(terminatedQueue);
}