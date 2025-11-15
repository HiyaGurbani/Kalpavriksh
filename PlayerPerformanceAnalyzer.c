#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "Players_Data.h"

#define NAME_SIZE 100
#define TEAM_NAME_SIZE 100
#define STR_BOWLER "Bowler"
#define STR_BATSMAN "Batsman"
#define STR_ALL_ROUNDER "All-rounder"
#define PERFORMANCE_DIVISION 100
#define WICKET_POINTS 2
#define ECONOMY_BASE 100 
#define ROLE_COUNT 3
#define HEAP_ROOT_INDEX 0

typedef enum {
    BATSMAN = 1,
    BOWLER = 2,
    ALL_ROUNDER = 3
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
    PlayerData* batsmanHead;
    PlayerData* bowlerHead;
    PlayerData* allRounderHead;
} Team;


typedef struct HeapNode {
    PlayerData* player;
    int teamId;
} HeapNode;

void createTeam(Team* currentTeam, int teamId, const char* teamName) {
    currentTeam->id = teamId;
    strcpy(currentTeam->name, teamName);
    currentTeam->totalPlayers = 0;
    currentTeam->battingPlayerCount = 0;
    currentTeam->averageBattingStrikeRate = 0;
    currentTeam->batsmanHead = NULL;
    currentTeam->bowlerHead = NULL;
    currentTeam->allRounderHead = NULL;
}

Team* initialiseTeams() {
    Team* team = malloc(teamCount * sizeof(Team));
    if (team == NULL)
    {
        return NULL;
    }

    for (int index = 0; index < teamCount; index++) 
    {
        createTeam(&team[index], index+1, teams[index]);
    }
    return team;
}


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
    else 
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

void addPlayerToTeam(Team* team, PlayerData* newPlayer) {
    newPlayer->next = NULL;
    strcpy(newPlayer->teamName, team->name);

    PlayerData **head = getHeadByRole(team, newPlayer->role);
    
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

bool createPlayer(Team* team, Player player) {
    PlayerData* newPlayer = malloc(sizeof(PlayerData));
    if (newPlayer == NULL)
    {
        return false;
    }

    newPlayer->id = player.id;
    strcpy(newPlayer->name, player.name);
    newPlayer->role = getRoleByString(player.role);
    newPlayer->totalRuns = player.totalRuns;
    newPlayer->battingAverage = player.battingAverage;
    newPlayer->strikeRate = player.strikeRate;
    newPlayer->wickets = player.wickets;
    newPlayer->economyRate = player.economyRate;
    calculatePerformanceIndex(newPlayer);

    Team *currentTeam = searchTeamByName(team, player.team);
    if (currentTeam != NULL)
    {
        addPlayerToTeam(currentTeam, newPlayer);
    }
    else
    {
        return false;
    }

    return true;
}

bool initialisePlayers(Team* team) {
    for (int index = 0; index < playerCount; index++)
    {
        if (!createPlayer(team, players[index]))
        {
            return false;
        }
    }
    return true;
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
    player->role = (PlayerRole)choice;

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
    
    addPlayerToTeam(currentTeam, newPlayer);
    playerCount++;

    return true;
}

void displayPlayerTableHeader() {
    printf("=========================================================================================\n");
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

bool displayTeamData(Team* team, int teamId) {
    Team* currentTeam = searchTeamById(team, teamId);
    if (currentTeam == NULL)
    {
        return false;
    }

    printf("\nPlayers of Team %s\n", currentTeam->name);
    displayPlayerTableHeader();

    PlayerData* lists[3] = {
        currentTeam->batsmanHead,
        currentTeam->bowlerHead,
        currentTeam->allRounderHead
    };
    
    for (int index = 0; index < ROLE_COUNT; index++)
    {
        PlayerData* temp = lists[index];
        while (temp)
        {
            displayPlayerDetails(temp);
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

bool displaySortedTeams(Team* team) {
    if (teamCount <= 0)
    {
        return false;
    }

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

    return true;
}


bool getTopKPlayers(Team* team, int teamId, PlayerRole role, int k) {
    Team* currentTeam = searchTeamById(team, teamId);
    if (currentTeam == NULL) 
    {
        return false;
    }

    printf("\nPlayers of Team %s\n", currentTeam->name);
    displayPlayerTableHeader();

    PlayerData** head = getHeadByRole(team, role);
    PlayerData* temp = *head; 

    while (temp && k--)
    {
        displayPlayerDetails(temp);
        temp = temp->next;     
    }

    return true;
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

void displaySortedPlayers(Team* team, PlayerRole role) {
    int heapSize = 0;
    HeapNode heap[teamCount];

    initialiseHeap(team, role, heap, &heapSize);

    buildMaxHeap(heap, heapSize);

    while (heapSize > 0)
    {
        HeapNode top = extractMax(heap, &heapSize);

        displayPlayerDetails(top.player);

        if (top.player->next != NULL)
        {
            heap[heapSize].player = heap[HEAP_ROOT_INDEX].player;
            heap[heapSize].teamId = heap[HEAP_ROOT_INDEX].teamId;

            heap[HEAP_ROOT_INDEX].player = top.player->next;
            heap[HEAP_ROOT_INDEX].teamId = top.teamId;
            heapSize++;

            maxHeapify(heap, heapSize, HEAP_ROOT_INDEX);
        }
    }
}

void displaySortedPlayersOfRole(Team* team, PlayerRole role) {
    char* player = getStringByRole(role);
    printf("%s of all teams: \n", player);

    displayPlayerTableHeader();

    displaySortedPlayers(team, role);
}


void displayMenu() {
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
}

void handleAddPlayer(Team* team, int teamId) {
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
}

void handleTopKPlayers(Team* team, int teamId) {
    printf("Enter Team ID: ");
    scanf("%d", &teamId);

    int choice;
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-Rounder): ");
    scanf("%d", &choice);
    PlayerRole role = (PlayerRole)choice;

    int k;
    printf("Enter number of players: ");
    scanf("%d", &k);

    if (!getTopKPlayers(team, teamId, role, k))
    {
        printf("Team Not Found.\n");                
    }                                
}

void freePlayers(PlayerData* head) {
    while (head != NULL)
    {
        PlayerData* temp = head;
        head = head->next;
        free(temp);
    }
}

void freeAllTeams(Team* team) {
    for (int index = 0; index < teamCount; index++) {
        freePlayers(team[index].batsmanHead);
        freePlayers(team[index].bowlerHead);
        freePlayers(team[index].allRounderHead);
    }
    free(teams);
}

void handleChoice(Team* team) {
    int choice;
    
    int teamId;
    switch(choice) 
    {
        case 1: 
            handleAddPlayer(team, teamId);
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
            if (!displaySortedTeams(team))
            {
                printf("No teams exist.");
            }
            break;
            
        case 4:
            handleTopKPlayers(team, teamId);
            break;
        
        case 5:
            printf("Enter Role (1-Batsman, 2-Bowler, 3-All-Rounder): ");
            scanf("%d", &choice);
            PlayerRole role = (PlayerRole)choice;
            displaySortedPlayersOfRole(team, role);
            break;
        
        case 6:
            freeAllTeams(team);
            printf("Memory released. Exiting program...\n");
            exit(0);
        
        default:
            printf("Invalid Choice."); 
    }  
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

    while(true)
    {
        displayMenu();
        handleChoice(team);
    }
}
