#include "header.h"

void createTeam(Team* currentTeam, unsigned int teamId, const char* teamName) {
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
        if (newPlayer->performanceIndex >= (*head)->performanceIndex)
        {
            newPlayer->next = *head;
            *head = newPlayer;
        }
        else
        {
            PlayerData* temp = *head;
            while (temp->next != NULL && temp->next->performanceIndex >= newPlayer->performanceIndex)
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
    if (newPlayer->role == INVALID_ROLE)
    {
        free(newPlayer);
        return false;
    }
    newPlayer->totalRuns = player.totalRuns;
    newPlayer->battingAverage = player.battingAverage;
    newPlayer->strikeRate = player.strikeRate;
    newPlayer->wickets = player.wickets;
    newPlayer->economyRate = player.economyRate;
    if (!calculatePerformanceIndex(newPlayer))
    {
        printf("Error: Invalid role\n");
        free(newPlayer);
        return false;
    }

    Team *currentTeam = searchTeamByName(team, player.team);
    if (!currentTeam)
    {
        free(newPlayer);
        return false;    
    }
    addPlayerToTeam(currentTeam, newPlayer);

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

PlayerData* createNewPlayer(Team* team) {
    PlayerData* player = malloc(sizeof(PlayerData));
    if (player == NULL)
    {
        return NULL;
    }

    printf("\nEnter Player Details: \n");

    printf("Player ID: ");
    getValidPlayerId(team, &player->id);

    printf("Name: ");
    getValidPlayerName(player->name);

    unsigned int roleId = 0;
    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    getValidRoleId(&roleId);
    player->role = (PlayerRole)roleId;

    printf("Total Runs: ");
    getValidInteger(&player->totalRuns);
   
    printf("Batting Average: ");
    getValidFloat(&player->battingAverage);

    printf("Strike Rate: ");
    getValidFloat(&player->strikeRate);

    printf("Wickets: ");
    getValidInteger(&player->wickets);

    printf("Economy Rate: ");
    getValidFloat(&player->economyRate);

    if (!calculatePerformanceIndex(player))
    {
        free(player);
        printf("Error: Invalid role\n");
        return NULL;
    }

    return player;
}

bool addNewPlayerToTeam(Team* team, unsigned int teamId) {
    Team* currentTeam = searchTeamById(team, teamId);
    if (currentTeam == NULL)
    {
        return false;
    }
    if (currentTeam->totalPlayers == MAX_PLAYERS_IN_TEAM)
    {
        return false;
    }

    PlayerData* newPlayer = createNewPlayer(team);
    if (newPlayer == NULL)
    {
        return false;
    }
    
    addPlayerToTeam(currentTeam, newPlayer);
    playerCount++;

    return true;
}

bool displayTeamData(Team* team, unsigned int teamId) {
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

    printf("\nTotal Players: %u\n", currentTeam->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", currentTeam->averageBattingStrikeRate);

    return true;
}

bool displaySortedTeams(Team* team) {
    if (teamCount <= 0)
    {
        return false;
    }

    Team* temp = malloc(teamCount * sizeof(Team));

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
        printf("%-10u %-20s %-12.2f %-10u\n", temp[index].id, temp[index].name, temp[index].averageBattingStrikeRate, temp[index].totalPlayers);
    }

    free(team);
    return true;
}

bool getTopKPlayers(Team* team, unsigned int teamId, PlayerRole role, unsigned int topCount) {
    Team* currentTeam = searchTeamById(team, teamId);
    if (currentTeam == NULL) 
    {
        return false;
    }

    printf("\nPlayers of Team %s\n", currentTeam->name);
    displayPlayerTableHeader();

    PlayerData** head = getHeadByRole(currentTeam, role);
    PlayerData* temp = *head; 

    while (temp && topCount--)
    {
        displayPlayerDetails(temp);
        temp = temp->next;     
    }

    return true;
}

bool displaySortedPlayers(Team* team, PlayerRole role) {
    int heapSize = 0;
    HeapNode heap[teamCount];

    if (!initialiseHeap(team, role, heap, &heapSize))
    {
        return false;
    }

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

    return true;
}

bool displaySortedPlayersOfRole(Team* team, PlayerRole role) {
    char* player = getStringByRole(role);
    printf("%s of all teams: \n", player);

    displayPlayerTableHeader();

    if (!displaySortedPlayers(team, role))
    {
        return false;
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
    free(team);
}

