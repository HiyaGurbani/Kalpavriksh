#include "fcfs.h"

int main() {
    HashMap* hashMap = createHashMap();
    Queue* readyQueue = createQueue();

    int totalProcesses = 0;
    readProcessInput(hashMap, readyQueue, &totalProcesses);

    KillEvent* kills = NULL;
    int killCount = 0;
    readKillEvents(&kills, &killCount);

    fcfsScheduler(hashMap, readyQueue, totalProcesses, kills, killCount);

    if (kills)
    {
        free(kills);
    }

    freeQueue(readyQueue);
    freeHashMap(hashMap);

    return 0;
}