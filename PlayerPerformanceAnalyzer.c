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

//We actually don't need all those things except performanceIndex
typedef struct PlayerData {
    int id;
    char name[NAME_SIZE];
    char teamName[TEAM_NAME_SIZE]; //It can be a struct type of team too 
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
    float averageBattingStrikeRate; //consider strike rate of batters and all-rounders only
    PlayerData* playersHead; //Head of LL of players
} Team;

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
    currentTeam->averageBattingStrikeRate = 0;
    currentTeam->playersHead = NULL;
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

bool getTeamByName(Team* teams, const char *teamName, Team* team) {
    Team foundTeam;
    for (int index = 0; index < teamCount; index++)
    {
        if (strcmp(teams[index].name, teamName) == 0)
        {
            *team = teams[index];
            return true;
        }
    }
    return false;
}

void addPlayerToTeam(Team* team, PlayerData* newPlayer) {
    if (team->playersHead == NULL)
    {
        team->playersHead = newPlayer;
        return;
    }

    PlayerData* temp = team->playersHead;
    while  (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newPlayer;
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

    Team team;
    getTeamByName(teams, player.team, &team);
    if (getTeamByName(teams, player.team, &team))
    {
        addPlayerToTeam(&team, newPlayer);
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
    if (choice == 1)
    {
        player->role = BATSMAN;
    }
    else if (choice == 2)
    {
        player->role = BOWLER;
    }
    else if (choice == 3)
    {
        player->role = ALL_ROUNDER;
    }

    printf("Total Runs: ");
    scanf("%d", player->totalRuns);
   
    printf("Batting Average: ");
    scanf("%f", player->battingAverage);

    printf("Strike Rate: ");
    scanf("%f", player->strikeRate);

    printf("Wickets: ");
    scanf("%d", player->wickets);

    printf("Economy Rate: ");
    scanf("%f", player->economyRate);

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
    
    printf("==============================================================================\n");
    printf("                      ICC ODI Player Performance Analyzer                      \n");
    printf("==============================================================================\n");
    printf("1. Add Player to Team\n");
    printf("2. Display Players of a Specific Team\n");
    printf("3. Display Teams by Average Batting Strike Rate\n");
    printf("4. Display Top K Players of a Specific Team by Role\n");
    printf("5. Display All Players of Specific Role Across All Teams by Performance Index\n");
    printf("6. Exit\n");
    printf("==============================================================================\n");
    
    int choice;

    while(true)
    {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        switch(choice) 
        {
            case 1: 
                {
                    int teamId;
                    printf("Enter Team ID to add player: ");
                    scanf("%d", &teamId);
                    if (addNewPlayerToTeam(team, teamId))
                    {
                        printf("Player Added Successfully to Team %s", team[teamId].name);
                    }
                    else 
                    {
                        printf("Team Not Found.");
                    }
                }
        }
    }

}
