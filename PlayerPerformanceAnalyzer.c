#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "Players_Data.h"

#define NAME_SIZE 100
#define TEAM_NAME_SIZE 100

typedef enum {
    BATSMAN,
    BOWLER,
    ALL_ROUNDER
} PlayerRole;

typedef struct PlayerData {
    int id;
    char name[NAME_SIZE];
    char teamName[TEAM_NAME_SIZE]; 
    PlayerRole role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct PlayerData* next;
} PlayerData;

typedef struct Team {
    int id;
    char name[NAME_SIZE];
    int totalPlayers;
    int battingPlayerCount;
    float averageBattingStrikeRate;
    // PlayerData* playersHead; //Head of LL of players
    PlayerData* batsmanHead;
    PlayerData* bowlerHead;
    PlayerData* allRounderHead;
} Team;


typedef struct HeapNode {
    PlayerData* player;
    int teamId;
} HeapNode;

void calculatePerformanceIndex(PlayerData* player) {
    if (player->role == BATSMAN)
    {
        player->performanceIndex = (player->battingAverage * player->strikeRate) / 100;
    }
    else if (player->role == BOWLER)
    {
        player->performanceIndex = (player->wickets * 2) + (100 - player->economyRate);
    }
    else 
    {
        player->performanceIndex = 
            ((player->battingAverage * player->strikeRate) / 100) + (player->wickets * 2);
    }
}

void createTeam(Team* currentTeam, int teamId, const char* teamName) {
    currentTeam->id = teamId;
    strcpy(currentTeam->name, teamName);
    currentTeam->totalPlayers = 0;
    currentTeam->battingPlayerCount = 0;
    currentTeam->averageBattingStrikeRate = 0;
    // currentTeam->playersHead = NULL;
    currentTeam->batsmanHead = NULL;
    currentTeam->bowlerHead = NULL;
    currentTeam->allRounderHead = NULL;
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

Team* initialiseTeams() {
    Team* team = malloc(teamCount * sizeof(Team));
    for (int index = 0; index < teamCount; index++) 
    {
        createTeam(&team[index], index+1, teams[index]);
    }
    return team;
}

bool searchTeamByName(Team* teams, const char *teamName, int* teamIndex) {
    Team foundTeam;
    for (int index = 0; index < teamCount; index++)
    {
        if (strcmp(teams[index].name, teamName) == 0)
        {
            *teamIndex = index;
            return true;
        }
    }
    return false;
}

void updateAverageBattingStrikeRate(Team* team, PlayerData* player) {
    float totalBattingStrikeRate = team->averageBattingStrikeRate * team->battingPlayerCount;
    team->battingPlayerCount += 1;
    team->averageBattingStrikeRate = (totalBattingStrikeRate + player->strikeRate) / team->battingPlayerCount;
}

void addPlayerToTeam(Team* team, PlayerData* newPlayer) {
    newPlayer->next = NULL;
    strcpy(newPlayer->teamName, team->name);

    PlayerData **head;
    if (newPlayer->role == BATSMAN)
    {
        head = &(team->batsmanHead);
    }
    else if (newPlayer->role == BOWLER)
    {
        head = &(team->bowlerHead);
    }
    else 
    {
        head = &(team->allRounderHead);
    }
    
    if (*head == NULL)
    {
        *head = newPlayer;
    }
    else 
    {
        if (newPlayer->performanceIndex > (*head)->performanceIndex)
        {
            newPlayer->next = *head;
            *head = newPlayer;
        }
        else
        {
            PlayerData* temp = *head;
            while (temp->next != NULL && temp->next->performanceIndex > newPlayer->performanceIndex)
            {
                temp = temp->next;
            }
            newPlayer->next = temp->next;
            temp->next = newPlayer;
        }
    }

    team->totalPlayers += 1;
    if (newPlayer->role != BOWLER)
    {
        updateAverageBattingStrikeRate(team, newPlayer);
    }
}

bool createPlayer(Team* teams, Player player) {
    PlayerData* newPlayer = malloc(sizeof(PlayerData));
    if (newPlayer == NULL)
    {
        return false;
    }

    newPlayer->id = player.id;
    strcpy(newPlayer->name, player.name);
    if (strcmp(player.role, "All-rounder") == 0)
    {
        newPlayer->role = ALL_ROUNDER;
    }
    else if (strcmp(player.role, "Bowler") == 0)
    {
        newPlayer->role = BOWLER;
    }
    else 
    {
        newPlayer->role = BATSMAN;
    }
    newPlayer->totalRuns = player.totalRuns;
    newPlayer->battingAverage = player.battingAverage;
    newPlayer->strikeRate = player.strikeRate;
    newPlayer->wickets = player.wickets;
    newPlayer->economyRate = player.economyRate;
    calculatePerformanceIndex(newPlayer);

    int index;

    if (searchTeamByName(teams, player.team, &index))
    {
        addPlayerToTeam(&teams[index], newPlayer);
    }
    else
    {
        return false;
    }

    return true;
}

bool initialisePlayers(Team* teams) {
    for (int index = 0; index < playerCount; index++)
    {
        if (!createPlayer(teams, players[index]))
        {
            return false;
        }
    }
    return true;
}

PlayerRole getPlayerRoleByChoice(int choice) {
    if (choice == 1)
    {
        return BATSMAN;
    }
    else if (choice == 2)
    {
        return BOWLER;
    }
    else if (choice == 3)
    {
        return ALL_ROUNDER;
    }
}

PlayerData* createNewPlayer() {
    PlayerData* player = malloc(sizeof(PlayerData));
    if (player == NULL)
    {
        return NULL;
    }

    printf("\nEnter Player Details: \n");
    printf("Player ID: ");
    scanf("%d", &player->id);

    while (getchar() != '\n');
    printf("Name: ");
    fgets(player->name, NAME_SIZE, stdin);
    player->name[strcspn(player->name, "\n")] = '\0';

    int choice = 0;
    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &choice);
    player->role = getPlayerRoleByChoice(choice);

    printf("Total Runs: ");
    scanf("%d", &player->totalRuns);
   
    printf("Batting Average: ");
    scanf("%f", &player->battingAverage);

    printf("Strike Rate: ");
    scanf("%f", &player->strikeRate);

    printf("Wickets: ");
    scanf("%d", &player->wickets);

    printf("Economy Rate: ");
    scanf("%f", &player->economyRate);

    calculatePerformanceIndex(player);

    return player;
}


bool addNewPlayerToTeam(Team* team, int teamId) {
    Team* currentTeam = searchTeamById(team, teamId);
    if (currentTeam == NULL)
    {
        return false;
    }

    PlayerData* newPlayer = createNewPlayer();
    if (newPlayer == NULL)
    {
        return false;
    }
    addPlayerToTeam(currentTeam, newPlayer);
    
    return true;
}

char *getRole(PlayerRole role) {
    if (role == BATSMAN)
    {
        return "Batsman";
    }
    else if (role == BOWLER)
    {
        return "Bowler";
    }
    else if (role == ALL_ROUNDER)
    {
        return "All-Rounder";
    }
}

bool displayTeamData(Team* team, int teamId) {
    Team* currentTeam = searchTeamById(team, teamId);
    if (currentTeam == NULL)
    {
        return false;
    }

    printf("\nPlayers of Team %s\n", currentTeam->name);
    printf("=========================================================================================\n");
    printf("%-5s %-15s %-12s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("=========================================================================================\n");

    PlayerData* lists[3] = {
        currentTeam->batsmanHead,
        currentTeam->bowlerHead,
        currentTeam->allRounderHead
    };
    
    for (int index = 0; index < 3; index++)
    {
        PlayerData* temp = lists[index];
        while (temp)
        {
            char* role = getRole(temp->role);
            printf("%-5d %-15s %-12s %-8d %-8.1f %-8.1f %-8d %-8.1f %-12.2f\n",
            temp->id,
            temp->name,
            role,
            temp->totalRuns,
            temp->battingAverage,
            temp->strikeRate,
            temp->wickets,
            temp->economyRate,
            temp->performanceIndex
            );

            temp = temp->next;
        }
    }

    printf("\nTotal Players: %d\n", currentTeam->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", currentTeam->averageBattingStrikeRate);

    return true;
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

void displaySortedTeams(Team* team) {
    Team temp[teamCount];
    for (int index = 0; index < teamCount; index++)
    {
        temp[index] = team[index];
    }
    quickSort(temp, 0, teamCount - 1);

    printf("\n=========================================================================================\n");
    printf("%-10s %-20s %-12s %-10s\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("=========================================================================================\n");

    for (int index = 0; index < teamCount; index++)
    {
        printf("%-10d %-20s %-12.2f %-10d\n", temp[index].id, temp[index].name, temp[index].averageBattingStrikeRate, temp[index].totalPlayers);
    }
}

bool getTopKPlayers(Team* team, int teamId, PlayerRole role, int k) {
    Team* currentTeam = searchTeamById(team, teamId);
    if (currentTeam == NULL) 
    {
        return false;
    }

    printf("\nPlayers of Team %s\n", currentTeam->name);
    printf("=========================================================================================\n");
    printf("%-5s %-15s %-12s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("=========================================================================================\n");

    PlayerData* temp; 
    if (role == BATSMAN)
    {
        temp = team->batsmanHead;
    }
    else if (role == BOWLER)
    {
        temp = team->bowlerHead;
    }
    else
    {
        temp = team->allRounderHead;
    }

    while (temp && k--)
    {
        char* role = getRole(temp->role);
        printf("%-5d %-15s %-12s %-8d %-8.1f %-8.1f %-8d %-8.1f %-12.2f\n",
            temp->id,
            temp->name,
            role,
            temp->totalRuns,
            temp->battingAverage,
            temp->strikeRate,
            temp->wickets,
            temp->economyRate,
            temp->performanceIndex
        );

        temp = temp->next;     
    }

    return true;
}



void initialiseHeap(Team* team, PlayerRole role, HeapNode* heap, int* heapSize) {
    for (int index = 0; index < teamCount; index++)
    {
        PlayerData* head = NULL;
        if (role == BATSMAN)
        {
            head = team[index].batsmanHead;
        }
        else if (role == BOWLER)
        {
            head = team[index].bowlerHead;
        }
        else if (role == ALL_ROUNDER)
        {
            head = team[index].allRounderHead;
        }

        if (head != NULL) {
            heap[*heapSize].player = head;
            heap[*heapSize].teamId = team[index].id; 
            (*heapSize)++;
        }
    }
}

void maxHeapify(HeapNode* heap, int size, int i) {
    int largest = i;
    int leftChild = 2 * i + 1;
    int rightChild = 2 * i + 2;

    if (leftChild < size && heap[leftChild].player->performanceIndex > heap[largest].player->performanceIndex)
    {
        largest = leftChild;
    }

    if (rightChild < size && heap[rightChild].player->performanceIndex > heap[largest].player->performanceIndex)
    {
        largest = rightChild;
    }

    if (largest != i)
    {
        HeapNode temp = heap[i];
        heap[i] = heap[largest];
        heap[largest] = temp;

        maxHeapify(heap, size, largest);
    }
}

void buildMaxHeap(HeapNode* heap, int size) {
    // Start start from the last non-leaf node
    for (int index = size / 2 - 1; index >= 0; index--)
    {
        maxHeapify(heap, size, index);
    }
}

HeapNode extractMax(HeapNode heap[], int *size) {
    if (*size <= 0) 
    {
        return (HeapNode){NULL, -1};
    }

    HeapNode root = heap[0];

    heap[0] = heap[*size - 1];
    (*size)--;

    maxHeapify(heap, *size, 0);
    return root;
}

void displayPlayersByRoleUsingHeap(Team* team, PlayerRole role) {
    int heapSize = 0;
    HeapNode heap[teamCount];

    initialiseHeap(team, role, heap, &heapSize);

    buildMaxHeap(heap, heapSize);

    while (heapSize > 0)
    {
        HeapNode top = extractMax(heap, &heapSize);

        char* role = getRole(top.player->role);
        printf("%-5d %-15s %-12s %-8d %-8.1f %-8.1f %-8d %-8.1f %-12.2f\n",
            top.player->id,
            top.player->name,
            role,
            top.player->totalRuns,
            top.player->battingAverage,
            top.player->strikeRate,
            top.player->wickets,
            top.player->economyRate,
            top.player->performanceIndex
        );

        if (top.player->next != NULL)
        {
            heap[heapSize].player = heap[0].player;
            heap[heapSize].teamId = heap[0].teamId;

            heap[0].player = top.player->next;
            heap[0].teamId = top.teamId;
            heapSize++;

            maxHeapify(heap, heapSize, 0);
        }
    }
}

void displayPlayersOfSpecificRole(Team* team, PlayerRole role) {
    if (role == BATSMAN) 
    {
        printf("Batsmen of all teams: \n");
    }
    else if (role == BOWLER) 
    {
        printf("Bowlers of all teams: ");
    }
    else 
    {
        printf("All-rounders of all teams: ");
    }

    printf("=========================================================================================\n");
    printf("%-5s %-15s %-12s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("=========================================================================================\n");

    displayPlayersByRoleUsingHeap(team, role);

}

int main() {
    Team* team = initialiseTeams();
    if (team == NULL)
    {
        printf("Initialisation of teams Failed!");
        return 0;
    }
    if (!initialisePlayers(team))
    {
        printf("Initialisation of players Failed!");
        return 0;
    }
    
    int choice;

    while(true)
    {
        printf("\n=========================================================================================\n");
        printf("                             ICC ODI Player Performance Analyzer                         \n");
        printf("=========================================================================================\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display All Players of Specific Role Across All Teams by Performance Index\n");
        printf("6. Exit\n");
        printf("=========================================================================================\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        int teamId;
        switch(choice) 
        {
            case 1: 
                    printf("Enter Team ID to add player: ");
                    scanf("%d", &teamId);
                    if (addNewPlayerToTeam(team, teamId))
                    {
                        printf("\nPlayer Added Successfully to Team %s!\n", team[teamId-1].name);
                    }
                    else 
                    {
                        printf("Team Not Found.\n");
                    }
                    break;

            case 2:
                    printf("Enter Team ID: ");
                    scanf("%d", &teamId);
                    if (!displayTeamData(team, teamId))
                    {
                        printf("Team Not Found.\n");
                    }
                    break;

            case 3:
                    printf("Teams Sorted by Average Batting Strike Rate");
                    displaySortedTeams(team);
                    break;
            
            case 4:
                    printf("Enter Team ID: ");
                    scanf("%d", &teamId);

                    int choice;
                    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-Rounder): ");
                    scanf("%d", &choice);
                    PlayerRole role = getPlayerRoleByChoice(choice);

                    int k;
                    printf("Enter number of players: ");
                    scanf("%d", &k);

                    if (!getTopKPlayers(team, teamId, role, k))
                    {
                        printf("Team Not Found.\n");
                    }
                    break;
            
            case 5:
                    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-Rounder): ");
                    scanf("%d", &choice);
                    role = getPlayerRoleByChoice(choice);

                    displayPlayersOfSpecificRole(team, role);
                    break;
            
            case 6:
                    return 0;
            
            default:
                    printf("Invalid Choice.");
            
        }
    }

}
