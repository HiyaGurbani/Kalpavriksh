#include<stdio.h>
#include "header.h"

PlayerRole getRoleByString(const char* role) {
    if (strcmp(role, STR_ALL_ROUNDER) == 0)
    {
        return ALL_ROUNDER;
    }
    else if (strcmp(role, STR_BOWLER) == 0)
    {
        return BOWLER;
    }
    else if (strcmp(role, STR_BATSMAN) == 0)
    {
        return BATSMAN;
    }
}

void calculatePerformanceIndex(PlayerData* player) {
    if (player->role == BATSMAN)
    {
        player->performanceIndex = (player->battingAverage * player->strikeRate) / PERFORMANCE_DIVISION;
    }
    else if (player->role == BOWLER)
    {
        player->performanceIndex = (player->wickets * WICKET_POINTS) + (ECONOMY_BASE - player->economyRate);
    }
    else if (player->role == ALL_ROUNDER)
    {
        player->performanceIndex = 
            ((player->battingAverage * player->strikeRate) / PERFORMANCE_DIVISION) + (player->wickets * WICKET_POINTS);
    }
}

Team* searchTeamByName(Team* team, const char *teamName) {
    for (int index = 0; index < teamCount; index++)
    {
        if (strcmp(team[index].name, teamName) == 0)
        {
            return &team[index];
        }
    }
    return NULL;
}

PlayerData** getHeadByRole(Team *team, PlayerRole role) {
    if (role == BATSMAN)
    {
        return &(team->batsmanHead);
    }
    else if (role == BOWLER)
    {
        return &(team->bowlerHead);
    }
    else if (role == ALL_ROUNDER)
    {
        return &(team->allRounderHead);
    }
}

void updateAverageBattingStrikeRate(Team* team, PlayerData* player) {
    float totalBattingStrikeRate = team->averageBattingStrikeRate * team->battingPlayerCount;
    team->battingPlayerCount += 1;
    team->averageBattingStrikeRate = (totalBattingStrikeRate + player->strikeRate) / team->battingPlayerCount;
}

Team* searchTeamById(Team* team, int teamId) {
    int low = 0, high = teamCount-1;
    while (low <= high)
    {
        int mid = low + (high-low)/2;
        if (team[mid].id == teamId)
        {
            return &team[mid];
        }
        else if (team[mid].id < teamId)
        {
            low = mid + 1;
        }
        else 
        {
            high = mid - 1;
        }
    }
    return NULL;
}

void displayPlayerTableHeader() {
    printf("\n=========================================================================================\n");
    printf("%-5s %-15s %-12s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("=========================================================================================\n");
}

char *getStringByRole(PlayerRole role) {
    if (role == BATSMAN)
    {
        return STR_BATSMAN;
    }
    else if (role == BOWLER)
    {
        return STR_BOWLER;
    }
    else if (role == ALL_ROUNDER)
    {
        return STR_ALL_ROUNDER;
    }
}

void displayPlayerDetails(PlayerData* player) {
    char* role = getStringByRole(player->role);
    printf("%-5d %-15s %-12s %-8d %-8.1f %-8.1f %-8d %-8.1f %-12.2f\n",
    player->id,
    player->name,
    role,
    player->totalRuns,
    player->battingAverage,
    player->strikeRate,
    player->wickets,
    player->economyRate,
    player->performanceIndex
    );
}


void swap(Team* team1, Team* team2) {
    Team temp = *team1;
    *team1 = *team2;
    *team2 = temp;
}

int partitionIndex(Team* team, int low, int high) {
    Team pivot = team[high];
    int largerIndex = low - 1;

    for (int currentIndex = low; currentIndex < high; currentIndex++)
    {
        if (team[currentIndex].averageBattingStrikeRate > pivot.averageBattingStrikeRate)
        {
            largerIndex++;
            swap(&team[currentIndex], &team[largerIndex]);
        }
    }
    swap(&team[largerIndex + 1], &team[high]);
    return largerIndex + 1;
}

void quickSort(Team* team, int low, int high) {
    if (low < high)
    {
        int partition = partitionIndex(team, low, high);

        quickSort(team, low, partition - 1);
        quickSort(team, partition + 1, high);
    }
}

void initialiseHeap(Team* team, PlayerRole role, HeapNode* heap, int* heapSize) {
    for (int index = 0; index < teamCount; index++)
    {
        PlayerData** head = getHeadByRole(&team[index], role);
        if (*head != NULL) {
            heap[*heapSize].player = *head;
            heap[*heapSize].teamId = team[index].id; 
            (*heapSize)++;
        }
    }
}

void maxHeapify(HeapNode* heap, int size, int index) {
    int largest = index;
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    if (leftChild < size && heap[leftChild].player->performanceIndex > heap[largest].player->performanceIndex)
    {
        largest = leftChild;
    }

    if (rightChild < size && heap[rightChild].player->performanceIndex > heap[largest].player->performanceIndex)
    {
        largest = rightChild;
    }

    if (largest != index)
    {
        HeapNode temp = heap[index];
        heap[index] = heap[largest];
        heap[largest] = temp;

        maxHeapify(heap, size, largest);
    }
}

void buildMaxHeap(HeapNode* heap, int heapSize) {
    // Start start from the last non-leaf node
    for (int index = heapSize / 2 - 1; index >= 0; index--)
    {
        maxHeapify(heap, heapSize, index);
    }
}

HeapNode extractMax(HeapNode heap[], int *heapSize) {
    if (*heapSize <= 0) 
    {
        return (HeapNode){NULL, -1};
    }

    HeapNode root = heap[HEAP_ROOT_INDEX];

    heap[HEAP_ROOT_INDEX] = heap[*heapSize - 1];
    (*heapSize)--;

    maxHeapify(heap, *heapSize, HEAP_ROOT_INDEX);
    return root;
}