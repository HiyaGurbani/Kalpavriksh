#ifndef HEADER_H
#define HEADER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "Players_Data.h"

#define NAME_SIZE 50
#define TEAM_NAME_SIZE 100
#define MIN_TEAM_COUNT 1
#define MIN_PLAYER_ID 1
#define MAX_PLAYER_ID 1000
#define MAX_PLAYERS_IN_TEAM 50
#define STR_BOWLER "Bowler"
#define STR_BATSMAN "Batsman"
#define STR_ALL_ROUNDER "All-rounder"
#define STR_EMPTY ""
#define PERFORMANCE_DIVISION 100
#define WICKET_POINTS 2
#define ECONOMY_BASE 100 
#define ROLE_COUNT 3
#define HEAP_ROOT_INDEX 0

typedef enum {
    BATSMAN = 1,
    BOWLER = 2,
    ALL_ROUNDER = 3,
    INVALID_ROLE = -1
} PlayerRole;

typedef struct PlayerData {
    unsigned int id;
    char name[NAME_SIZE];
    char teamName[TEAM_NAME_SIZE]; 
    PlayerRole role;
    unsigned int totalRuns;
    float battingAverage;
    float strikeRate;
    unsigned int wickets;
    float economyRate;
    float performanceIndex;
    struct PlayerData* next;
} PlayerData;

typedef struct Team {
    unsigned int id;
    char name[NAME_SIZE];
    unsigned int totalPlayers;
    unsigned int battingPlayerCount;
    float averageBattingStrikeRate;
    PlayerData* batsmanHead;
    PlayerData* bowlerHead;
    PlayerData* allRounderHead;
} Team;

typedef struct HeapNode {
    PlayerData* player;
    unsigned int teamId;
} HeapNode;

//Command Handlers
void displayMenu();
void handleAddPlayer(Team* team);
void handleTopKPlayers(Team* team);
void handleChoice(Team* team);

//Core Functions
void createTeam(Team* currentTeam, unsigned int teamId, const char* teamName);
Team* initialiseTeams();
void addPlayerToTeam(Team* team, PlayerData* newPlayer);
bool createPlayer(Team* team, Player player);
bool initialisePlayers(Team* team);
PlayerData* createNewPlayer(Team* team);
bool addNewPlayerToTeam(Team* team, unsigned int teamId);
bool displayTeamData(Team* team, unsigned int teamId);
bool displaySortedTeams(Team* team);
bool getTopKPlayers(Team* team, unsigned int teamId, PlayerRole role, unsigned int topCount);
void displaySortedPlayers(Team* team, PlayerRole role);
void displaySortedPlayersOfRole(Team* team, PlayerRole role);
void freePlayers(PlayerData* head);
void freeAllTeams(Team* team);

//Helper Functions
void getValidInteger(unsigned int* value);
void getValidFloat(float* value);
void getValidTeamId(unsigned int *value);
void getValidRoleId(unsigned int *value);
bool isUniquePlayerId(Team* team, unsigned int id);
void getValidPlayerId(Team* team, unsigned int *value);
void getValidPlayerName(char* name);
PlayerRole getRoleByString(const char* role);
bool calculatePerformanceIndex(PlayerData* player);
Team* searchTeamByName(Team* team, const char *teamName);
PlayerData** getHeadByRole(Team *team, PlayerRole role);
void updateAverageBattingStrikeRate(Team* team, PlayerData* player);
Team* searchTeamById(Team* team, unsigned int teamId);
bool containsId(PlayerData* head, unsigned int id);
void displayPlayerTableHeader();
char *getStringByRole(PlayerRole role);
void displayPlayerDetails(PlayerData* player);
void swap(Team* team1, Team* team2);
int partitionIndex(Team* team, int low, int high);
void quickSort(Team* team, int low, int high);
void initialiseHeap(Team* team, PlayerRole role, HeapNode* heap, int* heapSize);
void maxHeapify(HeapNode* heap, int size, int index);
void buildMaxHeap(HeapNode* heap, int heapSize);
HeapNode extractMax(HeapNode heap[], int *heapSize);

#endif